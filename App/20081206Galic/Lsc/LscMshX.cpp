// Implementation of the LscMshX class.
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

#include <Lc/ILcXmsh.h>
#include <Lc/ILcLsc.h>

#include "LscMshX.h"


#ifndef SAFE_DELETE
#	define SAFE_DELETE(p)	{	if(p){delete (p);p = NULL;	}}
#endif



namespace	LcLua
{
	
// For Container
////////////////////////////////////////////////////////////////////////////////

LscMshX::ILcCont::~ILcCont()
{
	if(!vLscH.empty())
	{
		INT iSize = vLscH.size();

		for(INT i=0; i<iSize; ++i)
		{
			ILcXmsh* pDst = (ILcXmsh*)(vLscH[i]->pIns);
			delete pDst;

			delete vLscH[i];
		}

		vLscH.clear();
	}
}


INT LscMshX::ILcCont::Alloc(char* sName, char* sTxFolder)
{
	INT			hr	=-1;
	ILcXmsh*	pObj= NULL;

	hr = LcXmsh_Create("PC Mesh", &pObj, LscMshX::m_pDevD3D, sName);

	if(FAILED(hr))
		return -1;

	ILcLsc::LscH* pLH	= new ILcLsc::LscH;


	pObj->Query("Load Texture", sTxFolder);

	pLH->pIns	= pObj;
	hr = vLscH.size();
	vLscH.push_back(pLH);

	return hr;
}


INT LscMshX::ILcCont::ReAlloc(INT nKey, char* sName, char* sTxFolder)
{
	INT			hr	=-1;
	ILcXmsh*	pObj = NULL;


	// Key를 통해서 해당 인덱스의 텍스춰 포인터를 찾는다.
	ILcLsc::LscH* p = LscMshX::m_pContAuto->Find(nKey);
	
	// 있다면 이전 Object를 해제하고, 새로운 Object를 할당한다.
	// 없으면 메모리를 할당한다.
	hr = LcXmsh_Create("PC Mesh", &pObj, LscMshX::m_pDevD3D, sName);

	if(FAILED(hr))
		return -1;


	if(p && p->pIns)
	{
		ILcXmsh*	pTmp = (ILcXmsh*)p->pIns;
		SAFE_DELETE(	pTmp	);

		p->pIns = pObj;
		pObj->Query("Load Texture", sTxFolder);
		
		strcpy(p->sName, sName);
	}
	
	else
		nKey = LscMshX::m_pContAuto->Alloc(sName, sTxFolder);

	return nKey;
}


INT LscMshX::ILcCont::Release(INT nKey)
{
	INT iSize = vLscH.size();

	if(iSize<=nKey || nKey<0)
		return -1;

	ILcLsc::LscH** pLa  = vLscH.begin() + nKey;
	ILcXmsh*  pDst = (ILcXmsh*)( (*pLa)->pIns);

	if(pDst)
		delete pDst;

	vLscH.erase( pLa);

	return vLscH.size();
}

ILcLsc::LscH* LscMshX::ILcCont::Find(INT nKey)
{
	INT iSize = vLscH.size();

	if(iSize<=nKey || nKey<0)
		return NULL;

	return vLscH[nKey];
}



////////////////////////////////////////////////////////////////////////////////
// Script

ILcDev*				LscMshX::m_pDevSys	= NULL;
void*				LscMshX::m_pDevD3D	= NULL;

LscMshX::ILcCont*	LscMshX::m_pContAuto	= NULL;
LscMshX::ILcCont*	LscMshX::m_pContStat	= NULL;


INT LscMshX::Create(ILcDev* pDev)
{
	LscMshX::m_pDevSys	= pDev;
	LscMshX::m_pDevD3D	= LcDev_GetD3Device(&m_pDevSys);

	if(NULL == LscMshX::m_pDevD3D)
		return -1;

	return 0;
}



static const luaL_reg regLscMshX[] =
{
	{"New"		,	LscMshX::Lsc_Alloc		},
	{"ReAlloc"	,	LscMshX::Lsc_ReAlloc	},
	{"Release"	,	LscMshX::Lsc_Release	},
	{"Render"	,	LscMshX::Lsc_Render		},

	{"GetGeomety"	,	LscMshX::Lsc_GetGeomety	},
	{"GetVtxNum"	,	LscMshX::Lsc_GetVtxNum	},
	{"Lock"			,	LscMshX::Lsc_Lock		},
	{"Unlock"		,	LscMshX::Lsc_Unlock		},

	{"GetPos"		,	LscMshX::Lsc_GetPos		},
	{"SetPos"		,	LscMshX::Lsc_SetPos		},
	
	{NULL, NULL},
};


INT LscMshX::OpenLib(lua_State *pL)
{
	luaL_openlib(pL, "Lxms", regLscMshX, 0);
	return 1;
}


// X-File
INT	LscMshX::Lsc_Alloc(lua_State *pL)
{
	INT		nKey=-1;
	char	sName[160] ={0};
	char	sTxFolder[160] ={0};

	INT		n = lua_gettop(pL);
	
	if(n<2)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	strcpy(sName, lua_tostring(pL, 1));
	strcpy(sTxFolder, lua_tostring(pL, 2));

	nKey	= LscMshX::m_pContAuto->Alloc(sName, sTxFolder);
	
	// Key를 돌려 준다.
	lua_pushnumber(pL, nKey);
	return 1;
}




// X-File
INT	LscMshX::Lsc_ReAlloc(lua_State *pL)
{
	INT		nKey=-1;
	char	sName[160] ={0};
	char	sTxFolder[160] ={0};
	INT		n = lua_gettop(pL);
	
	if(n<2)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	nKey = (INT)lua_tonumber(pL, 1);
	strcpy(sName, lua_tostring(pL, 2));
	strcpy(sTxFolder, lua_tostring(pL, 3));

	nKey	= LscMshX::m_pContAuto->Alloc(sName, sTxFolder);
	
	// Key를 돌려 준다.
	lua_pushnumber(pL, nKey);
	return 1;
}




INT LscMshX::Lsc_Release(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	INT iSize = LscMshX::m_pContAuto->Release(nKey);
	
	// 남아 있는 사이즈를 돌려준다.
	lua_pushnumber(pL, iSize);
	return 1;
}


INT LscMshX::Lsc_Render(lua_State *pL)
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
	p	= LscMshX::m_pContAuto->Find(nKey);

	if(NULL ==p || NULL ==p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}


	ILcXmsh*	pObj = (ILcXmsh*)p->pIns;
	pObj->Render();
	
	lua_pushnumber(pL, 0);
	return 1;
}


INT	LscMshX::Lsc_GetGeomety(lua_State *pL)
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
	p	= LscMshX::m_pContAuto->Find(nKey);

	if(NULL ==p || NULL ==p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}


	ILcXmsh*	pObj = (ILcXmsh*)p->pIns;

	INT	nGeo=-1;
	
	if(FAILED(pObj->Query("Number Geometry", &nGeo)))
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	if(nGeo <1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	lua_pushnumber(pL, nGeo);
	return 1;
}


INT	LscMshX::Lsc_GetVtxNum(lua_State *pL)
{
	INT		hr = -1;
	INT		nKey=-1;
	INT		nGeo=-1;
	ILcLsc::LscH* p	= NULL;

	INT		n = lua_gettop(pL);

	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey= (INT)lua_tonumber(pL, 1);
	nGeo= (INT)lua_tonumber(pL, 2);

	p	= LscMshX::m_pContAuto->Find(nKey);

	if(NULL ==p || NULL ==p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}


	ILcXmsh*	pObj = (ILcXmsh*)p->pIns;

	
	INT	iVtx	= nGeo;
	
	
	//입력은 인덱스, 출력은 인덱스에 해당하는 버텍스 수.
	// 인덱스를 넣으면 이 변수에 버텍수를 저장한다.
	hr = pObj->Query("Get Vertex Number", &iVtx);

	if(FAILED(hr))
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	lua_pushnumber(pL, iVtx);
	return 1;
}


INT	LscMshX::Lsc_Lock(lua_State *pL)
{
	INT		hr = -1;
	INT		nKey=-1;
	INT		nGeo=-1;
	ILcLsc::LscH* p	= NULL;

	INT		n = lua_gettop(pL);

	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey= (INT)lua_tonumber(pL, 1);
	nGeo= (INT)lua_tonumber(pL, 2);

	p	= LscMshX::m_pContAuto->Find(nKey);

	if(NULL ==p || NULL ==p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}


	ILcXmsh*	pObj = (ILcXmsh*)p->pIns;

	
	//입력은 인덱스, 출력은 인덱스에 해당하는 버텍스 수.
	// 인덱스를 넣으면 이 변수에 버텍수를 저장한다.
	hr = pObj->Lock(nGeo, 0, 0, NULL);

	if(FAILED(hr))
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	lua_pushnumber(pL, 0);
	return 1;
}


INT	LscMshX::Lsc_Unlock(lua_State *pL)
{
	INT		hr = -1;
	INT		nKey=-1;
	INT		nGeo=-1;
	ILcLsc::LscH* p	= NULL;

	INT		n = lua_gettop(pL);

	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey= (INT)lua_tonumber(pL, 1);
	nGeo= (INT)lua_tonumber(pL, 2);

	p	= LscMshX::m_pContAuto->Find(nKey);

	if(NULL ==p || NULL ==p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}


	ILcXmsh*	pObj = (ILcXmsh*)p->pIns;

	
	//입력은 인덱스, 출력은 인덱스에 해당하는 버텍스 수.
	// 인덱스를 넣으면 이 변수에 버텍수를 저장한다.
	hr = pObj->Unlock(nGeo);

	if(FAILED(hr))
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	lua_pushnumber(pL, 0);
	return 1;
}



INT	LscMshX::Lsc_GetPos(lua_State *pL)
{
	INT		hr = -1;
	INT		nKey=-1;
	INT		nGeo=-1;		// Geometry Index
	INT		nVtx=-1;		// vertex Index
	ILcLsc::LscH* p	= NULL;

	INT		n = lua_gettop(pL);

	if(n<1)
	{
		lua_pushnumber(pL, 0);
		lua_pushnumber(pL, 0);
		lua_pushnumber(pL, 0);
		lua_pushnumber(pL, 0);
		return 4;
	}
	
	nKey	= (INT)lua_tonumber(pL, 1);
	nGeo	= (INT)lua_tonumber(pL, 2);
	nVtx	= (INT)lua_tonumber(pL, 3);

	p	= LscMshX::m_pContAuto->Find(nKey);

	if(NULL ==p || NULL ==p->pIns)
	{
		lua_pushnumber(pL, 0);
		lua_pushnumber(pL, 0);
		lua_pushnumber(pL, 0);
		lua_pushnumber(pL, 0);
		return 4;
	}

	ILcXmsh*	pObj = (ILcXmsh*)p->pIns;
	FLOAT		pos[4]={0};

	pObj->GetPos(nGeo, nVtx, &pos[0], &pos[1], &pos[2], &pos[3]);

	lua_pushnumber(pL, pos[0]);
	lua_pushnumber(pL, pos[1]);
	lua_pushnumber(pL, pos[2]);
	lua_pushnumber(pL, pos[3]);
	return 4;
}



INT	LscMshX::Lsc_SetPos(lua_State *pL)
{
	INT		hr = -1;
	INT		nKey=-1;
	INT		nGeo=-1;
	INT		nVtx=-1;
	ILcLsc::LscH* p	= NULL;

	INT		n = lua_gettop(pL);

	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey	= (INT)lua_tonumber(pL, 1);
	nGeo	= (INT)lua_tonumber(pL, 2);
	nVtx	= (INT)lua_tonumber(pL, 3);

	
	FLOAT	pos[4]={0};

	pos[0]	= (FLOAT)lua_tonumber(pL, 0 + 4);
	pos[1]	= (FLOAT)lua_tonumber(pL, 1 + 4);
	pos[2]	= (FLOAT)lua_tonumber(pL, 2 + 4);
	pos[3]	= (FLOAT)lua_tonumber(pL, 3 + 4);

	p	= LscMshX::m_pContAuto->Find(nKey);

	if(NULL ==p || NULL ==p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	ILcXmsh*	pObj = (ILcXmsh*)p->pIns;

	pObj->SetPos(nGeo, nVtx, pos[0], pos[1], pos[2], pos[3]);
	
	lua_pushnumber(pL, 0);
	return 1;
}




INT	LscMshX::ContainerCreate()
{
	LscMshX::m_pContAuto = new LscMshX::ILcCont;

	return 0;
}


INT	LscMshX::ContainerDestroy()
{
	if(LscMshX::m_pContAuto)
	{
		delete LscMshX::m_pContAuto;
		LscMshX::m_pContAuto = NULL;
	}

	return 0;
}


INT	LscMshX::StaticContainerCreate()
{
	if(NULL == LscMshX::m_pContStat)
		LscMshX::m_pContStat = new LscMshX::ILcCont;

	return 0;
}


INT	LscMshX::StaticContainerDestroy()
{
	if(LscMshX::m_pContStat)
	{
		delete LscMshX::m_pContStat;
		LscMshX::m_pContStat = NULL;
	}

	return 0;
}

}// namespace Lua