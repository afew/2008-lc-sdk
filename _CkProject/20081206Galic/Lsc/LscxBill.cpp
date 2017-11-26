// Implementation of the LscxBill class.
//
////////////////////////////////////////////////////////////////////////////////


#include <windows.h>
#include <d3dx9.h>

#include <Lua/lua.h>
#include <Lua/lualib.h>
#include <Lua/lauxlib.h>

#include <Lc/LcEuclid.h>
#include <Lc/ILcM3d.h>
#include <Lc/ILcDev.h>

#include <Lc/ILcxCam.h>
#include <Lc/ILcxtObj.h>

#include <Lc/ILcLsc.h>
#include "LscxBill.h"

#ifndef SAFE_DELETE
#	define SAFE_DELETE(p)	{	if(p){delete (p);p = NULL;	}}
#endif



namespace	LcLua
{

// For Container
////////////////////////////////////////////////////////////////////////////////

LscxBill::ILcCont::~ILcCont()
{
	if(!vLscH.empty())
	{
		INT iSize = vLscH.size();

		for(INT i=0; i<iSize; ++i)
		{
			ILcxtObj* pDst = (ILcxtObj*)(vLscH[i]->pIns);
			delete pDst;

			delete vLscH[i];
		}

		vLscH.clear();
	}
}


INT LscxBill::ILcCont::Alloc(INT nCnt)
{
	INT			hr	=-1;
	ILcxtObj*	pObj= NULL;

	hr = LcxtObj_Create("Billboard", &pObj, LscxBill::m_pDevD3D, (void*)nCnt);

	if(FAILED(hr))
		return -1;

	ILcLsc::LscH* pLH = new	ILcLsc::LscH;


	pLH->pIns	= pObj;
	hr = vLscH.size();
	vLscH.push_back(pLH);

	return hr;
}


INT LscxBill::ILcCont::ReAlloc(INT nKey, INT nCnt)
{
	INT			hr	=-1;
	ILcxtObj*	pObj = NULL;


	// Key를 통해서 해당 인덱스의 텍스춰 포인터를 찾는다.
	ILcLsc::LscH* p = LscxBill::m_pContAuto->Find(nKey);
	
	// 있다면 이전 Object를 해제하고, 새로운 Object를 할당한다.
	// 없으면 메모리를 할당한다.
	hr = LcxtObj_Create("PC", &pObj, LscxBill::m_pDevD3D, (void*)nCnt);

	if(FAILED(hr))
		return -1;


	if(p && p->pIns)
	{
		ILcxtObj*	pTmp = (ILcxtObj*)p->pIns;
		SAFE_DELETE(	pTmp	);

		p->pIns = pObj;
	}
	
	else
		nKey = LscxBill::m_pContAuto->Alloc(nCnt);

	return nKey;
}


INT LscxBill::ILcCont::Release(INT nKey)
{
	INT iSize = vLscH.size();

	if(iSize<=nKey || nKey<0)
		return -1;

	ILcLsc::LscH** pLa  = vLscH.begin() + nKey;
	ILcxtObj*  pDst = (ILcxtObj*)( (*pLa)->pIns);

	if(pDst)
		delete pDst;

	vLscH.erase( pLa);

	return vLscH.size();
}

ILcLsc::LscH* LscxBill::ILcCont::Find(INT nKey)
{
	INT iSize = vLscH.size();

	if(iSize<=nKey || nKey<0)
		return NULL;

	return vLscH[nKey];
}



////////////////////////////////////////////////////////////////////////////////
// Script

ILcDev*				LscxBill::m_pDevSys	= NULL;
void*				LscxBill::m_pDevD3D	= NULL;

LscxBill::ILcCont*	LscxBill::m_pContAuto	= NULL;
LscxBill::ILcCont*	LscxBill::m_pContStat	= NULL;


INT LscxBill::Create(ILcDev* pDev)
{
	LscxBill::m_pDevSys	= pDev;
	LscxBill::m_pDevD3D	= LcDev_GetD3Device(&m_pDevSys);

	if(NULL == LscxBill::m_pDevD3D)
		return -1;

	return 0;
}



static const luaL_reg regLscxBill[] =
{
	{"New"		,	LscxBill::Lsc_Alloc	},
	{"ReAlloc"	,	LscxBill::Lsc_ReAlloc	},
	{"Release"	,	LscxBill::Lsc_Release	},
	{"FrameMove",	LscxBill::Lsc_FrameMove	},
	{"Render"	,	LscxBill::Lsc_Render	},

	{"GetBillNum"	,	LscxBill::Lsc_GetBillNum	},
	{"Lock"			,	LscxBill::Lsc_Lock			},
	{"Unlock"		,	LscxBill::Lsc_Unlock		},

	{"SetDimension"	,	LscxBill::Lsc_SetDimension		},
	{"SetPos"		,	LscxBill::Lsc_SetPos			},
	{"SetDiffuse"	,	LscxBill::Lsc_SetDiffuse		},
	{"GetUV"		,	LscxBill::Lsc_SetUV				},
	{"SetTexPointer",	LscxBill::Lsc_SetTexPointer		},
	{"GetPos"		,	LscxBill::Lsc_GetPos			},

	{"SetupCamera"	,	LscxBill::Lsc_SetupCamera		},
	{"SetupCameraPointer",	LscxBill::Lsc_SetupCameraPointer	},

	{NULL, NULL},
};


INT LscxBill::OpenLib(lua_State *pL)
{
	luaL_openlib(pL, "Lxbill", regLscxBill, 0);
	return 1;
}


// X-File
INT	LscxBill::Lsc_Alloc(lua_State *pL)
{
	INT		nKey =-1;
	INT		nCnt = 0;

	INT		n  = lua_gettop(pL);
	
	nCnt = (INT)lua_tonumber(pL, 1);

	nKey	= LscxBill::m_pContAuto->Alloc(nCnt);
	
	// Key를 돌려 준다.
	lua_pushnumber(pL, nKey);
	return 1;
}




// X-File
INT	LscxBill::Lsc_ReAlloc(lua_State *pL)
{
	INT		nKey =-1;
	INT		nCnt = 0;

	INT		n	 = lua_gettop(pL);
	
	nKey = (INT)lua_tonumber(pL, 1);
	nCnt = (INT)lua_tonumber(pL, 2);

	nKey	= LscxBill::m_pContAuto->Alloc(nCnt);
	
	// Key를 돌려 준다.
	lua_pushnumber(pL, nKey);
	return 1;
}




INT LscxBill::Lsc_Release(lua_State *pL)
{
	INT		nKey =-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	INT iSize = LscxBill::m_pContAuto->Release(nKey);
	
	// 남아 있는 사이즈를 돌려준다.
	lua_pushnumber(pL, iSize);
	return 1;
}


INT LscxBill::Lsc_FrameMove(lua_State *pL)
{
	INT		hr = -1;
	INT		nKey=-1;

	ILcLsc::LscH* p	= NULL;

	INT		n = lua_gettop(pL);

	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey= (INT)lua_tonumber(pL, 1);

	p	= LscxBill::m_pContAuto->Find(nKey);

	if(NULL ==p || NULL ==p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}


	
	ILcxtObj*	pObj = (ILcxtObj*)p->pIns;

	hr = pObj->FrameMove();
	
	lua_pushnumber(pL, hr);
	return 1;
}




INT LscxBill::Lsc_Render(lua_State *pL)
{
	INT		hr = -1;
	INT		nKey=-1;

	ILcLsc::LscH* p	= NULL;

	INT		n = lua_gettop(pL);

	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey= (INT)lua_tonumber(pL, 1);
	p	= LscxBill::m_pContAuto->Find(nKey);

	if(NULL ==p || NULL ==p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}


	ILcxtObj*	pObj = (ILcxtObj*)p->pIns;
	pObj->Render();	
	
	lua_pushnumber(pL, 0);

	return 1;
}




INT	LscxBill::Lsc_GetBillNum(lua_State *pL)
{
	INT		hr = -1;
	INT		nKey=-1;
	INT		iNum=0;

	ILcLsc::LscH* p	= NULL;

	INT		n = lua_gettop(pL);

	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey= (INT)lua_tonumber(pL, 1);

	p	= LscxBill::m_pContAuto->Find(nKey);

	if(NULL ==p || NULL ==p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}


	ILcxtObj*	pObj = (ILcxtObj*)p->pIns;

	
	
	
	
	//입력은 인덱스, 출력은 인덱스에 해당하는 버텍스 수.
	// 인덱스를 넣으면 이 변수에 버텍수를 저장한다.

	hr = pObj->Query("Get Number", &iNum);

	if(FAILED(hr))
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	lua_pushnumber(pL, iNum);
	return 1;
}


INT	LscxBill::Lsc_Lock(lua_State *pL)
{
	INT		hr = -1;
	INT		nKey=-1;

	ILcLsc::LscH* p	= NULL;

	INT		n = lua_gettop(pL);

	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey= (INT)lua_tonumber(pL, 1);

	p	= LscxBill::m_pContAuto->Find(nKey);

	if(NULL ==p || NULL ==p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}


	ILcxtObj*	pObj = (ILcxtObj*)p->pIns;

	
	//입력은 인덱스, 출력은 인덱스에 해당하는 버텍스 수.
	// 인덱스를 넣으면 이 변수에 버텍수를 저장한다.
	hr = pObj->Query("Lock", NULL);

	if(FAILED(hr))
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	lua_pushnumber(pL, 0);
	return 1;
}


INT	LscxBill::Lsc_Unlock(lua_State *pL)
{
	INT		hr = -1;
	INT		nKey=-1;

	ILcLsc::LscH* p	= NULL;

	INT		n = lua_gettop(pL);

	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey= (INT)lua_tonumber(pL, 1);

	p	= LscxBill::m_pContAuto->Find(nKey);

	if(NULL ==p || NULL ==p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}


	ILcxtObj*	pObj = (ILcxtObj*)p->pIns;

	
	//입력은 인덱스, 출력은 인덱스에 해당하는 버텍스 수.
	// 인덱스를 넣으면 이 변수에 버텍수를 저장한다.
	hr = pObj->Query("Unlock", NULL);

	if(FAILED(hr))
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	lua_pushnumber(pL, 0);
	return 1;
}



INT	LscxBill::Lsc_SetDimension(lua_State *pL)
{
	INT		hr = -1;
	INT		nKey=-1;
	INT		nIdx=-1;

	ILcLsc::LscH* p	= NULL;

	INT		n = lua_gettop(pL);

	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey	= (INT)lua_tonumber(pL, 1);
	nIdx	= (INT)lua_tonumber(pL, 2);

	
	FLOAT	xy[4]={0};

	xy[0]	= (FLOAT)lua_tonumber(pL, 0 + 3);
	xy[1]	= (FLOAT)lua_tonumber(pL, 1 + 3);
	xy[2]	= (FLOAT)lua_tonumber(pL, 2 + 3);
	xy[3]	= (FLOAT)lua_tonumber(pL, 3 + 3);

	p	= LscxBill::m_pContAuto->Find(nKey);

	if(NULL ==p || NULL ==p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	ILcxtObj*	pObj = (ILcxtObj*)p->pIns;

	struct _T
	{
		INT		n;
		FLOAT*	p;
	} t={nIdx, xy};

	hr = pObj->Query("Set Dimension", &t);
	lua_pushnumber(pL, 0);
	return 1;
}



INT	LscxBill::Lsc_SetPos(lua_State *pL)
{
	INT		hr = -1;
	INT		nKey=-1;
	INT		nIdx=-1;

	ILcLsc::LscH* p	= NULL;

	INT		n = lua_gettop(pL);

	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey	= (INT)lua_tonumber(pL, 1);
	nIdx	= (INT)lua_tonumber(pL, 2);

	
	FLOAT	pos[4]={0};

	pos[0]	= (FLOAT)lua_tonumber(pL, 0 + 3);
	pos[1]	= (FLOAT)lua_tonumber(pL, 1 + 3);
	pos[2]	= (FLOAT)lua_tonumber(pL, 2 + 3);

	p	= LscxBill::m_pContAuto->Find(nKey);

	if(NULL ==p || NULL ==p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	ILcxtObj*	pObj = (ILcxtObj*)p->pIns;

	struct _T
	{
		INT		n;
		FLOAT*	p;
	} t={nIdx, pos};

	hr = pObj->Query("Set Position", &t);
	lua_pushnumber(pL, 0);
	return 1;
}



INT	LscxBill::Lsc_SetDiffuse(lua_State *pL)
{
	INT		hr = -1;
	INT		nKey=-1;
	INT		nIdx=-1;

	ILcLsc::LscH* p	= NULL;

	INT		n = lua_gettop(pL);

	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey	= (INT)lua_tonumber(pL, 1);
	nIdx	= (INT)lua_tonumber(pL, 2);

	
	D3DXCOLOR	xColor;
	DWORD		d;

	xColor.r	= (FLOAT)lua_tonumber(pL, 0 + 3);
	xColor.g	= (FLOAT)lua_tonumber(pL, 1 + 3);
	xColor.b	= (FLOAT)lua_tonumber(pL, 2 + 3);
	xColor.a	= (FLOAT)lua_tonumber(pL, 3 + 3);

	d	= xColor;

	p	= LscxBill::m_pContAuto->Find(nKey);

	if(NULL ==p || NULL ==p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	ILcxtObj*	pObj = (ILcxtObj*)p->pIns;

	struct _T
	{
		INT		n;
		DWORD	p;
	} t={nIdx, d};

	hr = pObj->Query("Set Diffuse", &t);
	lua_pushnumber(pL, 0);
	return 1;
}


INT	LscxBill::Lsc_SetUV(lua_State *pL)
{
	INT		hr = -1;
	INT		nKey=-1;
	INT		nIdx=-1;
	INT		nUV =-1;

	ILcLsc::LscH* p	= NULL;

	INT		n = lua_gettop(pL);

	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey	= (INT)lua_tonumber(pL, 1);
	nIdx	= (INT)lua_tonumber(pL, 2);
	nUV 	= (INT)lua_tonumber(pL, 3);

	
	FLOAT	uv[2]={0};

	uv[0]	= (FLOAT)lua_tonumber(pL, 0 + 4);
	uv[1]	= (FLOAT)lua_tonumber(pL, 1 + 4);

	p	= LscxBill::m_pContAuto->Find(nKey);

	if(NULL ==p || NULL ==p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	ILcxtObj*	pObj = (ILcxtObj*)p->pIns;


	struct _T
	{
		INT		n;
		INT		tx;
		FLOAT*	p;
	} t={nIdx, nUV, uv};

	hr = pObj->Query("Set UV", &t);	
	lua_pushnumber(pL, 0);
	return 1;
}



INT	LscxBill::Lsc_SetTexPointer(lua_State *pL)
{
	INT		hr = -1;
	INT		nKey=-1;
	INT		nIdx=-1;
	DWORD	nTex=0x0;

	ILcLsc::LscH* p	= NULL;

	INT		n = lua_gettop(pL);

	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey	= (INT)lua_tonumber(pL, 1);
	nIdx	= (INT)lua_tonumber(pL, 2);
	nTex 	= (DWORD)lua_tonumber(pL, 3);

	
	FLOAT	uv[2]={0};

	uv[0]	= (FLOAT)lua_tonumber(pL, 0 + 4);
	uv[1]	= (FLOAT)lua_tonumber(pL, 1 + 4);

	p	= LscxBill::m_pContAuto->Find(nKey);

	if(NULL ==p || NULL ==p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	ILcxtObj*	pObj = (ILcxtObj*)p->pIns;
	void*		pTex = (void*)nTex;

	struct _T
	{
		INT		n;
		void*	p;
	} t={nIdx, pTex};

	hr = pObj->Query("Set Texture", &t);
	lua_pushnumber(pL, 0);
	return 1;
}



INT	LscxBill::Lsc_GetPos(lua_State *pL)
{
	INT		hr = -1;
	INT		nKey=-1;
	INT		nIdx=-1;		// vertex Index

	ILcLsc::LscH* p	= NULL;

	INT		n = lua_gettop(pL);

	if(n<1)
	{
		lua_pushnumber(pL, 0);
		lua_pushnumber(pL, 0);
		lua_pushnumber(pL, 0);
		return 3;
	}
	
	nKey	= (INT)lua_tonumber(pL, 1);
	nIdx	= (INT)lua_tonumber(pL, 2);

	p	= LscxBill::m_pContAuto->Find(nKey);

	if(NULL ==p || NULL ==p->pIns)
	{
		lua_pushnumber(pL, 0);
		lua_pushnumber(pL, 0);
		lua_pushnumber(pL, 0);
		return 3;
	}

	ILcxtObj*	pObj = (ILcxtObj*)p->pIns;
	FLOAT		pos[4]={0};

	struct _T
	{
		INT		n;
		FLOAT*	p;
	} t={nIdx, pos};

	hr = pObj->Query("Get Position", &t);

	lua_pushnumber(pL, 0);

	lua_pushnumber(pL, pos[0]);
	lua_pushnumber(pL, pos[1]);
	lua_pushnumber(pL, pos[2]);

	return 3;
}




INT LscxBill::Lsc_SetupCamera(lua_State *pL)
{
	INT		hr = -1;
	INT		nKey=-1;

	ILcLsc::LscH* p	= NULL;

	INT		n = lua_gettop(pL);

	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey	= (INT)lua_tonumber(pL, 1);
	
	D3DXMATRIX mtTs;
	D3DXVECTOR3	vcEye;
	D3DXVECTOR3	vcLook;
	D3DXVECTOR3	vcUp;


	p	= LscxBill::m_pContAuto->Find(nKey);

	if(NULL ==p || NULL ==p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}


	D3DXMatrixIdentity(&mtTs);

	
	vcEye.x		= (FLOAT)lua_tonumber(pL, 0 + 2);
	vcEye.y		= (FLOAT)lua_tonumber(pL, 1 + 2);
	vcEye.z		= (FLOAT)lua_tonumber(pL, 2 + 2);

	vcLook.x	= (FLOAT)lua_tonumber(pL, 3 + 2);
	vcLook.y	= (FLOAT)lua_tonumber(pL, 4 + 2);
	vcLook.z	= (FLOAT)lua_tonumber(pL, 5 + 2);

	vcUp.x		= (FLOAT)lua_tonumber(pL, 6 + 2);
	vcUp.y		= (FLOAT)lua_tonumber(pL, 7 + 2);
	vcUp.z		= (FLOAT)lua_tonumber(pL, 8 + 2);

	D3DXMatrixLookAtLH(&mtTs, &vcEye, &vcLook, &vcUp);
	D3DXMatrixInverse(&mtTs, NULL, &mtTs);

	ILcxtObj*	pObj = (ILcxtObj*)p->pIns;
	hr = pObj->Query("Set Inverse View Matrix", &mtTs);
	lua_pushnumber(pL, 0);
	return 1;
}


INT LscxBill::Lsc_SetupCameraPointer(lua_State *pL)
{
	INT		hr = -1;
	INT		nKey=-1;

	ILcLsc::LscH* p	= NULL;

	INT		n = lua_gettop(pL);

	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey	= (INT)lua_tonumber(pL, 1);
	p		= LscxBill::m_pContAuto->Find(nKey);

	if(NULL ==p || NULL ==p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	ILcxtObj*	pObj = (ILcxtObj*)p->pIns;


	INT		nCam = (INT)lua_tonumber(pL, 2);

	ILcxCamM*	pM	= (ILcxCamM*)LcDev_GetUserCamera(&m_pDevSys);
	ILcxCam*	pCam= pM->GetCameraPointer(nCam);

	D3DXMATRIX	mtTs = *pCam->GetMatrixViwI();
	hr = pObj->Query("Set Inverse View Matrix", &mtTs);
	lua_pushnumber(pL, 0);
	return 1;
}




INT	LscxBill::ContainerCreate()
{
	LscxBill::m_pContAuto = new LscxBill::ILcCont;

	return 0;
}


INT	LscxBill::ContainerDestroy()
{
	if(LscxBill::m_pContAuto)
	{
		delete LscxBill::m_pContAuto;
		LscxBill::m_pContAuto = NULL;
	}

	return 0;
}


INT	LscxBill::StaticContainerCreate()
{
	if(NULL == LscxBill::m_pContStat)
		LscxBill::m_pContStat = new LscxBill::ILcCont;

	return 0;
}


INT	LscxBill::StaticContainerDestroy()
{
	if(LscxBill::m_pContStat)
	{
		delete LscxBill::m_pContStat;
		LscxBill::m_pContStat = NULL;
	}

	return 0;
}

}// namespace Lua

