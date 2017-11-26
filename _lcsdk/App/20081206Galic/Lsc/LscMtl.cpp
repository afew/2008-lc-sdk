// Implementation of the LscMtl class.
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


#include "LscMtl.h"



namespace	LcLua
{

// For Container
////////////////////////////////////////////////////////////////////////////////

LscMtl::ILcCont::~ILcCont()
{
	if(!vLscH.empty())
	{
		INT iSize = vLscH.size();

		for(INT i=0; i<iSize; ++i)
		{
			ILcMtl* pDst = (ILcMtl*)(vLscH[i]->pIns);
			delete pDst;

			delete vLscH[i];
		}

		vLscH.clear();
	}
}


INT LscMtl::ILcCont::Alloc(FLOAT* pAmb, FLOAT* pDif, FLOAT* pSpc, FLOAT* pEms, FLOAT fPow)
{
	INT		nKey;
	ILcLsc::LscH* p = NULL;

	INT		hr	=-1;
	ILcMtl*	pMtl= NULL;

	hr = LcM3d_CreateMaterial("PC Material", &pMtl, LscMtl::m_pDevD3D, pAmb, pDif, pSpc, pEms, &fPow);

	if(FAILED(hr))
		return -1;

	ILcLsc::LscH* pLH	= new ILcLsc::LscH;

	pLH->pIns	= pMtl;
	nKey = vLscH.size();
	vLscH.push_back(pLH);

	return nKey;
}


INT LscMtl::ILcCont::Release(INT nKey)
{
	INT iSize = vLscH.size();

	if(iSize<=nKey || nKey<0)
		return -1;

	ILcLsc::LscH** pLa  = vLscH.begin() + nKey;
	ILcMtl*  pDst = (ILcMtl*)( (*pLa)->pIns);

	if(pDst)
		delete pDst;

	vLscH.erase( pLa);

	return vLscH.size();
}

ILcLsc::LscH* LscMtl::ILcCont::Find(INT nKey)
{
	INT iSize = vLscH.size();

	if(iSize<=nKey || nKey<0)
		return NULL;

	return vLscH[nKey];
}



////////////////////////////////////////////////////////////////////////////////
// Script

ILcDev*				LscMtl::m_pDevSys	= NULL;
void*				LscMtl::m_pDevD3D	= NULL;

LscMtl::ILcCont*	LscMtl::m_pContAuto	= NULL;
LscMtl::ILcCont*	LscMtl::m_pContStat	= NULL;


INT LscMtl::Create(ILcDev* pDev)
{
	LscMtl::m_pDevSys	= pDev;
	LscMtl::m_pDevD3D	= LcDev_GetD3Device(&m_pDevSys);

	if(NULL == LscMtl::m_pDevD3D)
		return -1;

	return 0;
}



static const luaL_reg regLscMtl[] =
{
	{"New"			,	LscMtl::Lsc_Alloc			},
	{"Release"		,	LscMtl::Lsc_Release			},

	{"SetDiffuse"	,	LscMtl::Lsc_MtlDiffuseSet	},
	{"SetAmbient"	,	LscMtl::Lsc_MtlAmbientSet	},
	{"SetSpecular"	,	LscMtl::Lsc_MtlSpecularSet	},
	{"SetEmissive"	,	LscMtl::Lsc_MtlEmissiveSet	},
	{"SetPower"		,	LscMtl::Lsc_MtlPowerSet		},

	{"GetDiffuse"	,	LscMtl::Lsc_MtlDiffuseGet	},
	{"GetAmbient"	,	LscMtl::Lsc_MtlAmbientGet	},
	{"GetSpecular"	,	LscMtl::Lsc_MtlSpecularGet	},
	{"GetEmissive"	,	LscMtl::Lsc_MtlEmissiveGet	},
	{"GetPower"		,	LscMtl::Lsc_MtlPowerGet		},

	{"SetMaterial"	,	LscMtl::Lsc_SetMaterial	},

	{NULL, NULL},
};


INT LscMtl::OpenLib(lua_State *pL)
{
	luaL_openlib(pL, "Lmtl", regLscMtl, 0);
	return 1;
}



INT	LscMtl::Lsc_Alloc(lua_State *pL)
{
	INT		nKey=-1;
	char	sMtlType[64] ={0};

	INT		n = lua_gettop(pL);
	
	if(n<13)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	strcpy(sMtlType, lua_tostring(pL, 1));

	FLOAT fDif[4]={0, 0, 0, 1};
	FLOAT fAmb[4]={0, 0, 0, 1};
	FLOAT fSpc[4]={0, 0, 0, 1};
	FLOAT fEms[4]={0, 0, 0, 1};
	FLOAT fPow = 0.f;

	fAmb[0]	= (FLOAT)lua_tonumber(pL, 0 + 1);
	fAmb[1]	= (FLOAT)lua_tonumber(pL, 1 + 1);
	fAmb[2]	= (FLOAT)lua_tonumber(pL, 2 + 1);

	fDif[0]	= (FLOAT)lua_tonumber(pL, 3 + 1);
	fDif[1]	= (FLOAT)lua_tonumber(pL, 4 + 1);
	fDif[2]	= (FLOAT)lua_tonumber(pL, 5 + 1);

	fSpc[0]	= (FLOAT)lua_tonumber(pL, 6 + 1);
	fSpc[1]	= (FLOAT)lua_tonumber(pL, 7 + 1);
	fSpc[2]	= (FLOAT)lua_tonumber(pL, 8 + 1);

	fEms[0]	= (FLOAT)lua_tonumber(pL, 9 + 1);
	fEms[1]	= (FLOAT)lua_tonumber(pL,10 + 1);
	fEms[2]	= (FLOAT)lua_tonumber(pL,11 + 1);

	fPow	= (FLOAT)lua_tonumber(pL,12 + 1);

	nKey	= LscMtl::m_pContAuto->Alloc(fAmb, fDif, fSpc, fEms, fPow);
	
	// Key를 돌려 준다.
	lua_pushnumber(pL, nKey);

	return 1;
}



INT LscMtl::Lsc_Release(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	INT iSize = LscMtl::m_pContAuto->Release(nKey);
	
	// 남아 있는 사이즈를 돌려준다.
	lua_pushnumber(pL, iSize);
	
	return 1;
}


INT LscMtl::Lsc_MtlDiffuseSet(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	ILcLsc::LscH* p	= LscMtl::m_pContAuto->Find(nKey);

	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	ILcMtl*	pMtl = (ILcMtl*)p->pIns;

	FLOAT pVal[4]={0, 0, 0, 1};
	pVal[0] = (FLOAT)lua_tonumber(pL, 2);
	pVal[1] = (FLOAT)lua_tonumber(pL, 3);
	pVal[2] = (FLOAT)lua_tonumber(pL, 4);

	pMtl->SetDiffuse(pVal);
	
	
	lua_pushnumber(pL, 0);
	return 1;
}


INT LscMtl::Lsc_MtlAmbientSet(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	ILcLsc::LscH* p	= LscMtl::m_pContAuto->Find(nKey);

	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	ILcMtl*	pMtl = (ILcMtl*)p->pIns;

	FLOAT pVal[4]={0, 0, 0, 1};
	pVal[0] = (FLOAT)lua_tonumber(pL, 2);
	pVal[1] = (FLOAT)lua_tonumber(pL, 3);
	pVal[2] = (FLOAT)lua_tonumber(pL, 4);

	pMtl->SetAmbient(pVal);
	
	
	lua_pushnumber(pL, 0);
	return 1;
}


INT LscMtl::Lsc_MtlSpecularSet(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	ILcLsc::LscH* p	= LscMtl::m_pContAuto->Find(nKey);

	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}


	ILcMtl*	pMtl = (ILcMtl*)p->pIns;

	FLOAT pVal[4]={0, 0, 0, 1};
	pVal[0] = (FLOAT)lua_tonumber(pL, 2);
	pVal[1] = (FLOAT)lua_tonumber(pL, 3);
	pVal[2] = (FLOAT)lua_tonumber(pL, 4);

	pMtl->SetSpecular(pVal);
	
	lua_pushnumber(pL, 0);
	return 1;
}

INT LscMtl::Lsc_MtlEmissiveSet(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	ILcLsc::LscH* p	= LscMtl::m_pContAuto->Find(nKey);

	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}


	ILcMtl*	pMtl = (ILcMtl*)p->pIns;

	FLOAT pVal[4]={0, 0, 0, 1};
	pVal[0] = (FLOAT)lua_tonumber(pL, 2);
	pVal[1] = (FLOAT)lua_tonumber(pL, 3);
	pVal[2] = (FLOAT)lua_tonumber(pL, 4);

	pMtl->SetEmissive(pVal);
	
	lua_pushnumber(pL, 0);
	return 1;
}


INT LscMtl::Lsc_MtlPowerSet(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	ILcLsc::LscH* p	= LscMtl::m_pContAuto->Find(nKey);

	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}


	ILcMtl*	pMtl = (ILcMtl*)p->pIns;

	FLOAT fPow;

	fPow = (FLOAT)lua_tonumber(pL, 2);

	pMtl->SetPower(fPow);
	
	
	lua_pushnumber(pL, 0);
	return 1;
}



INT LscMtl::Lsc_MtlDiffuseGet(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	ILcLsc::LscH* p	= LscMtl::m_pContAuto->Find(nKey);

	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		lua_pushnumber(pL, -1);
		lua_pushnumber(pL, -1);
		return 3;
	}


	ILcMtl*	pMtl = (ILcMtl*)p->pIns;

	FLOAT pVal[4]={0, 0, 0, 1};
	memcpy(pVal, pMtl->GetDiffuse(), sizeof pVal);

	lua_pushnumber(pL, pVal[0]);
	lua_pushnumber(pL, pVal[1]);
	lua_pushnumber(pL, pVal[2]);
	return 3;
}



INT LscMtl::Lsc_MtlAmbientGet(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	ILcLsc::LscH* p	= LscMtl::m_pContAuto->Find(nKey);

	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		lua_pushnumber(pL, -1);
		lua_pushnumber(pL, -1);
		return 3;
	}

	ILcMtl*	pMtl = (ILcMtl*)p->pIns;

	FLOAT pVal[4]={0, 0, 0, 1};
	memcpy(pVal, pMtl->GetAmbient(), sizeof pVal);
	
	lua_pushnumber(pL, pVal[0]);
	lua_pushnumber(pL, pVal[1]);
	lua_pushnumber(pL, pVal[2]);
	return 3;
}



INT LscMtl::Lsc_MtlSpecularGet(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	ILcLsc::LscH* p	= LscMtl::m_pContAuto->Find(nKey);

	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		lua_pushnumber(pL, -1);
		lua_pushnumber(pL, -1);
		return 3;
	}

	ILcMtl*	pMtl = (ILcMtl*)p->pIns;

	FLOAT pVal[4]={0, 0, 0, 1};
	memcpy(pVal, pMtl->GetSpecular(), sizeof pVal);


	lua_pushnumber(pL, pVal[0]);
	lua_pushnumber(pL, pVal[1]);
	lua_pushnumber(pL, pVal[2]);
	return 3;
}


INT LscMtl::Lsc_MtlEmissiveGet(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	ILcLsc::LscH* p	= LscMtl::m_pContAuto->Find(nKey);

	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		lua_pushnumber(pL, -1);
		lua_pushnumber(pL, -1);
		return 3;
	}


	ILcMtl*	pMtl = (ILcMtl*)p->pIns;

	FLOAT pVal[4]={0, 0, 0, 1};
	memcpy(pVal, pMtl->GetEmissive(), sizeof pVal);

	lua_pushnumber(pL, pVal[0]);
	lua_pushnumber(pL, pVal[1]);
	lua_pushnumber(pL, pVal[2]);
	return 3;
}


INT LscMtl::Lsc_MtlPowerGet(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	ILcLsc::LscH* p	= LscMtl::m_pContAuto->Find(nKey);

	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	ILcMtl*	pMtl= (ILcMtl*)p->pIns;
	FLOAT fPow	= pMtl->GetPower();

	
	lua_pushnumber(pL, fPow);
	return 1;
}


INT LscMtl::Lsc_SetMaterial(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	ILcLsc::LscH* p	= LscMtl::m_pContAuto->Find(nKey);

	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	ILcMtl*	pMtl= (ILcMtl*)p->pIns;

	pMtl->SetMaterial();

	lua_pushnumber(pL, 0);
	return 0;
}




INT	LscMtl::ContainerCreate()
{
	LscMtl::m_pContAuto = new LscMtl::ILcCont;

	return 0;
}

INT	LscMtl::ContainerDestroy()
{
	if(LscMtl::m_pContAuto)
	{
		delete LscMtl::m_pContAuto;
		LscMtl::m_pContAuto = NULL;
	}

	return 0;
}


INT	LscMtl::StaticContainerCreate()
{
	if(NULL == LscMtl::m_pContStat)
		LscMtl::m_pContStat = new LscMtl::ILcCont;

	return 0;
}


INT	LscMtl::StaticContainerDestroy()
{
	if(LscMtl::m_pContStat)
	{
		delete LscMtl::m_pContStat;
		LscMtl::m_pContStat = NULL;
	}

	return 0;
}

}// namespace Lua