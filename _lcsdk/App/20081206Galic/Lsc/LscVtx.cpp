// Implementation of the LscVtx class.
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

#include <Lc/ILcLsc.h>


#include "LscVtx.h"



namespace	LcLua
{

// For Container
////////////////////////////////////////////////////////////////////////////////

LscVtx::ILcCont::~ILcCont()
{
	if(!vLscH.empty())
	{
		INT iSize = vLscH.size();

		for(INT i=0; i<iSize; ++i)
		{
			ILcVtx* pDst = (ILcVtx*)(vLscH[i]->pIns);
			delete pDst;

			delete vLscH[i];
		}

		vLscH.clear();
	}
}


INT LscVtx::ILcCont::Alloc(char* sVtxType, char* sPrtType, INT nPrt, INT nVtx)
{
	INT		nKey;
	ILcLsc::LscH* p = NULL;

	INT		hr	=-1;
	ILcVtx*	pVtx= NULL;

//	LcVtx::VtxRHWD Vtx[3];
//	
//	Vtx[0]	= LcVtx::VtxRHWD(150.0f,  50.0f, 0.5f, 1.0f, 0xffff0000 );
//	Vtx[1]	= LcVtx::VtxRHWD(250.0f, 250.0f, 0.5f, 1.0f, 0xff00ff00 );
//	Vtx[2]	= LcVtx::VtxRHWD( 50.0f, 250.0f, 0.5f, 1.0f, 0xff00ffff );
//
//											1			2		         3				              4            	  5		  6
//	hr = LcM3d_CreateVB("CE", &m_pLcVB, "RHW Diffuse", m_pDev,           (void*)D3DMPT_TRIANGLELIST,  (void*)3,    (void*)1,  Vtx, (void*)sizeof(Vtx));
	hr = LcM3d_CreateVB("PC", &pVtx,     sVtxType,     LscVtx::m_pDevD3D, sPrtType,                   (void*)nPrt, (void*)nVtx);

	if(FAILED(hr))
		return -1;

	ILcLsc::LscH* pLH = new	ILcLsc::LscH;

	pLH->pIns	= pVtx;
	nKey = vLscH.size();
	vLscH.push_back(pLH);

	return nKey;
}


INT LscVtx::ILcCont::ReAlloc(INT nIdx, char* sVtxType, char* sPrtType, INT nVtx, INT nPrt)
{
	return 0;
}


INT LscVtx::ILcCont::Release(INT nKey)
{
	INT iSize = vLscH.size();

	if(iSize<=nKey || nKey<0)
		return -1;

	ILcLsc::LscH** pLa  = vLscH.begin() + nKey;
	ILcVtx*  pDst = (ILcVtx*)( (*pLa)->pIns);

	if(pDst)
		delete pDst;

	vLscH.erase( pLa);

	return vLscH.size();
}

ILcLsc::LscH* LscVtx::ILcCont::Find(INT nKey)
{
	INT iSize = vLscH.size();

	if(iSize<=nKey || nKey<0)
		return NULL;

	return vLscH[nKey];
}



////////////////////////////////////////////////////////////////////////////////
// Script

ILcDev*				LscVtx::m_pDevSys	= NULL;
void*				LscVtx::m_pDevD3D	= NULL;

LscVtx::ILcCont*	LscVtx::m_pContAuto	= NULL;
LscVtx::ILcCont*	LscVtx::m_pContStat	= NULL;


INT LscVtx::Create(ILcDev* pDev)
{
	LscVtx::m_pDevSys	= pDev;
	LscVtx::m_pDevD3D	= LcDev_GetD3Device(&m_pDevSys);

	if(NULL == LscVtx::m_pDevD3D)
		return -1;

	return 0;
}



static const luaL_reg regLscVtx[] =
{
	{"New"		,	LscVtx::Lsc_Alloc	},
	{"ReAlloc"	,	LscVtx::Lsc_ReAlloc	},
	{"Release"	,	LscVtx::Lsc_Release	},
	{"Render"	,	LscVtx::Lsc_Render	},

	{"VtxNum"	,	LscVtx::Lsc_VtxNum		},
	{"Lock"		,	LscVtx::Lsc_VtxLock		},
	{"UnLock"	,	LscVtx::Lsc_VtxUnLock	},
	{"SetVtx"	,	LscVtx::Lsc_VtxValueSet	},

	{"SetPos"	,	LscVtx::Lsc_VtxValuePosition	},
	{"SetNormal"	,	LscVtx::Lsc_VtxValueNormal	},
	{"SetDiffuse"	,	LscVtx::Lsc_VtxValueDiffuse	},
	{"SetUV"		,	LscVtx::Lsc_VtxValueUV		},

	{NULL, NULL},
};


INT LscVtx::OpenLib(lua_State *pL)
{
	luaL_openlib(pL, "Lvtx", regLscVtx, 0);
	return 1;
}


// Vtx
INT	LscVtx::Lsc_Alloc(lua_State *pL)
{
	INT		nKey=-1;

	char	sVtxType[64] ={0};
	char	sPrtType[64] ={0};
	INT		nPrt= 0;
	INT		nVtx= 0;

	INT		n = lua_gettop(pL);
	
	if(n<4)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	strcpy(sVtxType, lua_tostring(pL, 1));
	strcpy(sPrtType, lua_tostring(pL, 2));

	nPrt	= (INT)lua_tonumber(pL, 3);
	nVtx	= (INT)lua_tonumber(pL, 4);

	nKey	= LscVtx::m_pContAuto->Alloc(sVtxType, sPrtType, nPrt, nVtx);
	
	// Key를 돌려 준다.
	lua_pushnumber(pL, nKey);

	return 1;
}




// Vtx
INT	LscVtx::Lsc_ReAlloc(lua_State *pL)
{
	INT		nKey=-1;

	char	sVtxType[64] ={0};
	char	sPrtType[64] ={0};
	INT		nVtx= 0;
	INT		nPrt= 0;

	INT		n = lua_gettop(pL);
	
	if(n<4)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	strcpy(sVtxType, lua_tostring(pL, 1));
	strcpy(sPrtType, lua_tostring(pL, 2));

	nVtx	= (INT)lua_tonumber(pL, 3);
	nPrt	= (INT)lua_tonumber(pL, 4);

	nKey	= LscVtx::m_pContAuto->Alloc(sVtxType, sPrtType, nPrt, nVtx);
	
	// Key를 돌려 준다.
	lua_pushnumber(pL, nKey);

	return 1;
}




INT LscVtx::Lsc_Release(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	INT iSize = LscVtx::m_pContAuto->Release(nKey);
	
	// 남아 있는 사이즈를 돌려준다.
	lua_pushnumber(pL, iSize);
	
	return 1;
}


INT LscVtx::Lsc_VtxNum(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	ILcLsc::LscH* p	= LscVtx::m_pContAuto->Find(nKey);
	INT		iNum = 0;

	if(NULL ==p || NULL ==p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	ILcVtx*	pVtx = (ILcVtx*)p->pIns;
	iNum		 = pVtx->GetVtxNum();
		
	lua_pushnumber(pL, iNum);

	return 1;
}


INT LscVtx::Lsc_Render(lua_State *pL)
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
	p	= LscVtx::m_pContAuto->Find(nKey);

	if(NULL ==p || NULL ==p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}


	ILcVtx*	pVtx = (ILcVtx*)p->pIns;
	pVtx->Render();
	
	lua_pushnumber(pL, 0);
	return 1;
}


INT	LscVtx::Lsc_VtxLock(lua_State *pL)
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
	p	= LscVtx::m_pContAuto->Find(nKey);
	
	if(NULL == p || NULL== p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	ILcVtx*	pVtx = (ILcVtx*)p->pIns;

	hr = pVtx->Lock(0, 0, NULL, 0);

	lua_pushnumber(pL, hr);
	
	return 1;
}

INT	LscVtx::Lsc_VtxUnLock(lua_State *pL)
{
	INT		nKey=-1;

	RECT	pRc;
	VEC2	vcPos;
	VEC2	vcScl(1,1);
	VEC2	vcRot(0,0);
	FLOAT	fAngle = 0;
	DWORD	dC=D3DXCOLOR(1,1,1,1);
	char	sC[16]={0};

	ILcVtx*	pTx = NULL;
	
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	pRc.left	= (LONG)lua_tonumber(pL, 2);
	pRc.top		= (LONG)lua_tonumber(pL, 3);
	pRc.right	= (LONG)lua_tonumber(pL, 4);
	pRc.bottom	= (LONG)lua_tonumber(pL, 5);
	
	// 위치
	vcPos.x		= (FLOAT)lua_tonumber(pL, 6);
	vcPos.y		= (FLOAT)lua_tonumber(pL, 7);
	
	// 스케일
	if(n>7)
	{
		vcScl.x		= (FLOAT)lua_tonumber(pL, 8);
		vcScl.y		= (FLOAT)lua_tonumber(pL, 9);
	}
	
	// 색상
	if(n>9)
	{
		strcpy(sC, lua_tostring(pL, 10));
		sscanf(sC, "%x", &dC);
	}
	
	
	ILcLsc::LscH* p	= LscVtx::m_pContAuto->Find(nKey);
	
	if(NULL == p || NULL== p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	lua_pushnumber(pL, 0);
	
	return 1;
}

INT	LscVtx::Lsc_VtxValueSet(lua_State *pL)
{
	INT		hr = -1;
	INT		nKey=-1;

	ILcLsc::LscH* p	= NULL;

	INT		n = lua_gettop(pL);

	ILcVtx*	pTx = NULL;
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	// 버텍스 리스트에서 퍼텍스를 찾는다.
	nKey= (INT)lua_tonumber(pL, 1);
	p	= LscVtx::m_pContAuto->Find(nKey);
	
	if(NULL == p || NULL== p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	ILcVtx*	pVtx = (ILcVtx*)p->pIns;

	// 이 버텍스의 FVF 를 알아낸다.
	DWORD dFVF = pVtx->GetFVF();
	
	// FVF에 따라 읽기를 달리한다.
	if(LcVtx::VtxD::FVF == dFVF)
	{
		INT		nArr= (INT)lua_tonumber(pL, 2);
		
		FLOAT	x	= (FLOAT)lua_tonumber(pL, 3);
		FLOAT	y	= (FLOAT)lua_tonumber(pL, 4);
		FLOAT	z	= (FLOAT)lua_tonumber(pL, 5);

		FLOAT	r	= (FLOAT)lua_tonumber(pL, 6);
		FLOAT	g	= (FLOAT)lua_tonumber(pL, 7);
		FLOAT	b	= (FLOAT)lua_tonumber(pL, 8);
		FLOAT	a	= (FLOAT)lua_tonumber(pL, 9);

		pVtx->SetPos( nArr, x, y, z);
		pVtx->SetDiffuse( nArr, r, g, b, a);
	}

	else if(LcVtx::VtxDUV1::FVF == dFVF)
	{
		INT		nArr= (INT)lua_tonumber(pL, 2);
		
		FLOAT	x	= (FLOAT)lua_tonumber(pL, 3);
		FLOAT	y	= (FLOAT)lua_tonumber(pL, 4);
		FLOAT	z	= (FLOAT)lua_tonumber(pL, 5);

		FLOAT	u	= (FLOAT)lua_tonumber(pL, 6);
		FLOAT	v	= (FLOAT)lua_tonumber(pL, 7);

		FLOAT	r	= (FLOAT)lua_tonumber(pL, 8);
		FLOAT	g	= (FLOAT)lua_tonumber(pL, 9);
		FLOAT	b	= (FLOAT)lua_tonumber(pL,10);
		FLOAT	a	= (FLOAT)lua_tonumber(pL,11);

		pVtx->SetPos( nArr, x, y, z);
		pVtx->SetDiffuse( nArr, r, g, b, a);
		pVtx->SetUV( nArr, 0, u, v);
	}

	else if(LcVtx::VtxN::FVF == dFVF)
	{
		INT		nArr= (INT)lua_tonumber(pL, 2);
		
		FLOAT	Px	= (FLOAT)lua_tonumber(pL, 3);
		FLOAT	Py	= (FLOAT)lua_tonumber(pL, 4);
		FLOAT	Pz	= (FLOAT)lua_tonumber(pL, 5);

		FLOAT	Nx	= (FLOAT)lua_tonumber(pL, 6);
		FLOAT	Ny	= (FLOAT)lua_tonumber(pL, 7);
		FLOAT	Nz	= (FLOAT)lua_tonumber(pL, 8);

		pVtx->SetPos( nArr, Px, Py, Pz);
		pVtx->SetNormal( nArr, Nx, Ny, Nz);
	}
	
	else if(LcVtx::VtxNUV1::FVF == dFVF)
	{
		INT		nArr= (INT)lua_tonumber(pL, 2);
		
		FLOAT	Px	= (FLOAT)lua_tonumber(pL, 3);
		FLOAT	Py	= (FLOAT)lua_tonumber(pL, 4);
		FLOAT	Pz	= (FLOAT)lua_tonumber(pL, 5);

		FLOAT	Nx	= (FLOAT)lua_tonumber(pL, 6);
		FLOAT	Ny	= (FLOAT)lua_tonumber(pL, 7);
		FLOAT	Nz	= (FLOAT)lua_tonumber(pL, 8);

		FLOAT	u	= (FLOAT)lua_tonumber(pL, 9);
		FLOAT	v	= (FLOAT)lua_tonumber(pL,10);

		pVtx->SetPos( nArr, Px, Py, Pz);
		pVtx->SetNormal( nArr, Nx, Ny, Nz);
		pVtx->SetUV( nArr, 0, u, v);
	}

	else if(LcVtx::VtxNDUV1::FVF == dFVF)
	{
		INT		nArr= (INT)lua_tonumber(pL, 2);
		
		FLOAT	Px	= (FLOAT)lua_tonumber(pL, 3);
		FLOAT	Py	= (FLOAT)lua_tonumber(pL, 4);
		FLOAT	Pz	= (FLOAT)lua_tonumber(pL, 5);

		FLOAT	Nx	= (FLOAT)lua_tonumber(pL, 6);
		FLOAT	Ny	= (FLOAT)lua_tonumber(pL, 7);
		FLOAT	Nz	= (FLOAT)lua_tonumber(pL, 8);

		FLOAT	u	= (FLOAT)lua_tonumber(pL, 9);
		FLOAT	v	= (FLOAT)lua_tonumber(pL,10);

		FLOAT	r	= (FLOAT)lua_tonumber(pL,11);
		FLOAT	g	= (FLOAT)lua_tonumber(pL,12);
		FLOAT	b	= (FLOAT)lua_tonumber(pL,13);
		FLOAT	a	= (FLOAT)lua_tonumber(pL,14);

		pVtx->SetPos( nArr, Px, Py, Pz);
		pVtx->SetNormal( nArr, Nx, Ny, Nz);
		pVtx->SetDiffuse( nArr, r, g, b, a);
		pVtx->SetUV( nArr, 0, u, v);		
	}

	//--			vtx,  array Index,      x,      y,    z,   w , color
	else if(LcVtx::VtxRHWD::FVF == dFVF)
	{
		INT		nArr= (INT)lua_tonumber(pL, 2);
		
		FLOAT	x	= (FLOAT)lua_tonumber(pL, 3);
		FLOAT	y	= (FLOAT)lua_tonumber(pL, 4);
		FLOAT	z	= (FLOAT)lua_tonumber(pL, 5);
		FLOAT	w	= (FLOAT)lua_tonumber(pL, 6);

		FLOAT	r	= (FLOAT)lua_tonumber(pL, 7);
		FLOAT	g	= (FLOAT)lua_tonumber(pL, 8);
		FLOAT	b	= (FLOAT)lua_tonumber(pL, 9);
		FLOAT	a	= (FLOAT)lua_tonumber(pL,10);

		pVtx->SetPos( nArr, x, y, z, w);
		pVtx->SetDiffuse( nArr, r, g, b, a);
	}

	lua_pushnumber(pL, 0);
	return 1;
}



INT	LscVtx::Lsc_VtxValuePosition(lua_State *pL)
{
	INT		hr = -1;
	INT		nKey=-1;

	ILcLsc::LscH* p	= NULL;

	INT		n = lua_gettop(pL);

	ILcVtx*	pTx = NULL;
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	// 버텍스 리스트에서 퍼텍스를 찾는다.
	nKey= (INT)lua_tonumber(pL, 1);
	p	= LscVtx::m_pContAuto->Find(nKey);
	
	if(NULL == p || NULL== p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	ILcVtx*	pVtx = (ILcVtx*)p->pIns;

	INT		nArr= (INT)lua_tonumber(pL, 2);
	FLOAT	Px	= (FLOAT)lua_tonumber(pL, 3);
	FLOAT	Py	= (FLOAT)lua_tonumber(pL, 4);
	FLOAT	Pz	= (FLOAT)lua_tonumber(pL, 5);

	pVtx->SetPos( nArr, Px, Py, Pz);

	lua_pushnumber(pL, 0);

	return 1;
}



INT	LscVtx::Lsc_VtxValueNormal(lua_State *pL)
{
	INT		hr = -1;
	INT		nKey=-1;

	ILcLsc::LscH* p	= NULL;	
	ILcVtx*	pTx = NULL;

	INT		n = lua_gettop(pL);

	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	// 버텍스 리스트에서 퍼텍스를 찾는다.
	nKey= (INT)lua_tonumber(pL, 1);
	p	= LscVtx::m_pContAuto->Find(nKey);
	
	if(NULL == p || NULL== p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	ILcVtx*	pVtx = (ILcVtx*)p->pIns;

	INT		nArr= (INT)lua_tonumber(pL, 2);
	FLOAT	Nx	= (FLOAT)lua_tonumber(pL, 3);
	FLOAT	Ny	= (FLOAT)lua_tonumber(pL, 4);
	FLOAT	Nz	= (FLOAT)lua_tonumber(pL, 5);

	pVtx->SetNormal( nArr, Nx, Ny, Nz);

	lua_pushnumber(pL, 0);

	return 1;
}



INT	LscVtx::Lsc_VtxValueDiffuse(lua_State *pL)
{
	INT		hr = -1;
	INT		nKey=-1;

	ILcLsc::LscH* p	= NULL;
	ILcVtx*	pTx = NULL;

	INT		n = lua_gettop(pL);

	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	// 버텍스 리스트에서 퍼텍스를 찾는다.
	nKey= (INT)lua_tonumber(pL, 1);
	p	= LscVtx::m_pContAuto->Find(nKey);
	
	if(NULL == p || NULL== p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	ILcVtx*	pVtx = (ILcVtx*)p->pIns;

	INT		nArr= (INT  )lua_tonumber(pL, 2);
	FLOAT	r	= (FLOAT)lua_tonumber(pL, 3);
	FLOAT	g	= (FLOAT)lua_tonumber(pL, 4);
	FLOAT	b	= (FLOAT)lua_tonumber(pL, 5);
	FLOAT	a	= (FLOAT)lua_tonumber(pL, 6);

	pVtx->SetDiffuse( nArr, r, g, b, a);

	lua_pushnumber(pL, 0);
	return 1;
}



INT	LscVtx::Lsc_VtxValueUV(lua_State *pL)
{
	INT		hr = -1;
	INT		nKey=-1;

	ILcLsc::LscH* p	= NULL;
	ILcVtx*	pTx = NULL;

	INT		n = lua_gettop(pL);
	
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	// 버텍스 리스트에서 퍼텍스를 찾는다.
	nKey= (INT)lua_tonumber(pL, 1);
	p	= LscVtx::m_pContAuto->Find(nKey);
	
	if(NULL == p || NULL== p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	ILcVtx*	pVtx = (ILcVtx*)p->pIns;

	INT		nArr= (INT  )lua_tonumber(pL, 2);
	INT		nUV	= (INT  )lua_tonumber(pL, 3);
	FLOAT	u	= (FLOAT)lua_tonumber(pL, 4);
	FLOAT	v	= (FLOAT)lua_tonumber(pL, 5);

	pVtx->SetUV( nArr, nUV, u, v);

	lua_pushnumber(pL, 0);

	return 1;
}




INT	LscVtx::ContainerCreate()
{
	LscVtx::m_pContAuto = new LscVtx::ILcCont;

	return 0;
}

INT	LscVtx::ContainerDestroy()
{
	if(LscVtx::m_pContAuto)
	{
		delete LscVtx::m_pContAuto;
		LscVtx::m_pContAuto = NULL;
	}

	return 0;
}


INT	LscVtx::StaticContainerCreate()
{
	if(NULL == LscVtx::m_pContStat)
		LscVtx::m_pContStat = new LscVtx::ILcCont;

	return 0;
}


INT	LscVtx::StaticContainerDestroy()
{
	if(LscVtx::m_pContStat)
	{
		delete LscVtx::m_pContStat;
		LscVtx::m_pContStat = NULL;
	}

	return 0;
}

}// namespace Lua