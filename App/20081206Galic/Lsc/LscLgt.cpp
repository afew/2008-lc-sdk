// Implementation of the LscLgt class.
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


#include "LscLgt.h"


namespace	LcLua
{
	
// For Container
////////////////////////////////////////////////////////////////////////////////

LscLgt::ILcCont::~ILcCont()
{
	if(!vLscH.empty())
	{
		INT iSize = vLscH.size();

		for(INT i=0; i<iSize; ++i)
		{
			ILcLgt* pDst = (ILcLgt*)(vLscH[i]->pIns);
			delete pDst;

			delete vLscH[i];
		}

		vLscH.clear();
	}
}


INT LscLgt::ILcCont::Alloc(DWORD* pType
						   , FLOAT* pAmb, FLOAT* pDff, FLOAT* pSpc
						   , FLOAT* pPos, FLOAT* pDir
						   , FLOAT* fEtc)
{
	INT		nKey;
	ILcLsc::LscH*	p = NULL;

	INT		hr	=-1;
	ILcLgt*	pLgt= NULL;


	hr = LcM3d_CreateLight("PC Light", &pLgt, LscLgt::m_pDevD3D
								, pType
								, pAmb
								, pDff
								, pSpc
								, pPos
								, pDir
								, fEtc);

	if(FAILED(hr))
		return -1;

	ILcLsc::LscH*	pLH	= new ILcLsc::LscH;

	pLH->pIns	= pLgt;
	nKey = vLscH.size();
	vLscH.push_back(pLH);

	return nKey;
}


INT LscLgt::ILcCont::Release(INT nKey)
{
	INT iSize = vLscH.size();

	if(iSize<=nKey || nKey<0)
		return -1;

	ILcLsc::LscH** pLa  = vLscH.begin() + nKey;
	ILcLgt*  pDst = (ILcLgt*)( (*pLa)->pIns);

	if(pDst)
		delete pDst;

	vLscH.erase( pLa);

	return vLscH.size();
}

ILcLsc::LscH* LscLgt::ILcCont::Find(INT nKey)
{
	INT iSize = vLscH.size();

	if(iSize<=nKey || nKey<0)
		return NULL;

	return vLscH[nKey];
}



////////////////////////////////////////////////////////////////////////////////
// Script

ILcDev*				LscLgt::m_pDevSys	= NULL;
void*				LscLgt::m_pDevD3D	= NULL;

LscLgt::ILcCont*	LscLgt::m_pContAuto	= NULL;
LscLgt::ILcCont*	LscLgt::m_pContStat	= NULL;


INT LscLgt::Create(ILcDev* pDev)
{
	LscLgt::m_pDevSys	= pDev;
	LscLgt::m_pDevD3D	= LcDev_GetD3Device(&m_pDevSys);

	if(NULL == LscLgt::m_pDevD3D)
		return -1;

	return 0;
}



static const luaL_reg regLscLgt[] =
{
	{"New"				,	LscLgt::Lsc_Alloc				},
	{"Release"			,	LscLgt::Lsc_Release				},

	{"SetLight"			,	LscLgt::Lsc_SetLight			},
	{"LightEnable"		,	LscLgt::Lsc_SetLightEnable		},

	{"SetType"			,	LscLgt::Lsc_SetLgtType			},

	{"SetAmbient"		,	LscLgt::Lsc_SetLgtAmbient		},
	{"SetDiffuse"		,	LscLgt::Lsc_SetLgtDiffuse		},
	{"SetSpecular"		,	LscLgt::Lsc_SetLgtSpecular		},

	{"SetPosition"		,	LscLgt::Lsc_SetLgtPosition		},
	{"SetDirection"		,	LscLgt::Lsc_SetLgtDirection		},

	{"SetRange"			,	LscLgt::Lsc_SetLgtRange			},
	{"SetFalloff"		,	LscLgt::Lsc_SetLgtFalloff		},

	{"SetAttenuation0"	,	LscLgt::Lsc_SetLgtAttenuation0	},
	{"SetAttenuation1"	,	LscLgt::Lsc_SetLgtAttenuation1	},
	{"SetAttenuation2"	,	LscLgt::Lsc_SetLgtAttenuation2	},
	{"SetTheta"			,	LscLgt::Lsc_SetLgtTheta			},
	{"SetPhi"			,	LscLgt::Lsc_SetLgtPhi			},


	{"GetType"			,	LscLgt::Lsc_GetLgtType			},

	{"GetAmbient"		,	LscLgt::Lsc_GetLgtAmbient		},
	{"GetDiffuse"		,	LscLgt::Lsc_GetLgtDiffuse		},
	{"GetSpecular"		,	LscLgt::Lsc_GetLgtSpecular		},

	{"GetPosition"		,	LscLgt::Lsc_GetLgtPosition		},
	{"GetDirection"		,	LscLgt::Lsc_GetLgtDirection		},

	{"GetRange"			,	LscLgt::Lsc_GetLgtRange			},
	{"GetFalloff"		,	LscLgt::Lsc_GetLgtFalloff		},

	{"GetAttenuation0"	,	LscLgt::Lsc_GetLgtAttenuation0	},
	{"GetAttenuation1"	,	LscLgt::Lsc_GetLgtAttenuation1	},
	{"GetAttenuation2"	,	LscLgt::Lsc_GetLgtAttenuation2	},
	{"GetTheta"			,	LscLgt::Lsc_GetLgtTheta			},
	{"GetPhi"			,	LscLgt::Lsc_GetLgtPhi			},

	{NULL, NULL},
};


INT LscLgt::OpenLib(lua_State *pL)
{
	luaL_openlib(pL, "Llgt", regLscLgt, 0);
	return 1;
}



INT	LscLgt::Lsc_Alloc(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<13)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	DWORD dType=0;
	char  sLgtType[64] ={0};

	FLOAT fAmb[4]={0, 0, 0, 1};
	FLOAT fDif[4]={0, 0, 0, 1};
	FLOAT fSpc[4]={0, 0, 0, 1};
	FLOAT pPos[3]={0, 0, 0};
	FLOAT fDir[3]={0, 0, 0};

	FLOAT fRange	= 0;
    FLOAT fFall		= 0;
    FLOAT fAttn0	= 0;
    FLOAT fAttn1	= 0;
    FLOAT fAttn2	= 0;
    FLOAT fTheta	= 0;
    FLOAT fPhi		= 0;

	FLOAT fEtc[7]	= {0};

	strcpy(sLgtType, lua_tostring(pL, 1));

	if( 0 ==_stricmp(sLgtType, "Point"))
		dType = D3DLIGHT_POINT;

	else if( 0 ==_stricmp(sLgtType, "Spot"))
		dType = D3DLIGHT_SPOT;

	else if( 0 ==_stricmp(sLgtType, "Directional"))
		dType = D3DLIGHT_DIRECTIONAL;

	fAmb[0]	= (FLOAT)lua_tonumber(pL, 0 + 2);
	fAmb[1]	= (FLOAT)lua_tonumber(pL, 1 + 2);
	fAmb[2]	= (FLOAT)lua_tonumber(pL, 2 + 2);

	fDif[0]	= (FLOAT)lua_tonumber(pL, 3 + 2);
	fDif[1]	= (FLOAT)lua_tonumber(pL, 4 + 2);
	fDif[2]	= (FLOAT)lua_tonumber(pL, 5 + 2);
	

	fSpc[0]	= (FLOAT)lua_tonumber(pL, 6 + 2);
	fSpc[1]	= (FLOAT)lua_tonumber(pL, 7 + 2);
	fSpc[2]	= (FLOAT)lua_tonumber(pL, 8 + 2);

	pPos[0]	= (FLOAT)lua_tonumber(pL, 9 + 2);
	pPos[1]	= (FLOAT)lua_tonumber(pL,10 + 2);
	pPos[2]	= (FLOAT)lua_tonumber(pL,11 + 2);

	fDir[0]	= (FLOAT)lua_tonumber(pL,12 + 2);
	fDir[1]	= (FLOAT)lua_tonumber(pL,13 + 2);
	fDir[2]	= (FLOAT)lua_tonumber(pL,14 + 2);

	fRange	= (FLOAT)lua_tonumber(pL,15 + 2);
    fFall	= (FLOAT)lua_tonumber(pL,16 + 2);
    fAttn0	= (FLOAT)lua_tonumber(pL,17 + 2);
    fAttn1	= (FLOAT)lua_tonumber(pL,18 + 2);
    fAttn2	= (FLOAT)lua_tonumber(pL,19 + 2);
    fTheta	= (FLOAT)lua_tonumber(pL,20 + 2);
    fPhi	= (FLOAT)lua_tonumber(pL,21 + 2);

	fEtc[0]	= fRange	;
	fEtc[1]	= fFall		;
	fEtc[2]	= fAttn0	;
	fEtc[3]	= fAttn1	;
	fEtc[4]	= fAttn2	;
	fEtc[5]	= fTheta	;
	fEtc[6]	= fPhi		;


	D3DXVECTOR3	vcDir(fDir);
	D3DXVec3Normalize(&vcDir, &vcDir);
	memcpy(fDir, &vcDir.x, sizeof(D3DXVECTOR3));

	nKey	= LscLgt::m_pContAuto->Alloc(&dType, fAmb, fDif, fSpc, pPos, fDir, fEtc);
	
	// Key를 돌려 준다.
	lua_pushnumber(pL, nKey);
	return 1;
}


INT LscLgt::Lsc_Release(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	INT iSize = LscLgt::m_pContAuto->Release(nKey);
	
	// 남아 있는 사이즈를 돌려준다.
	lua_pushnumber(pL, iSize);
	return 1;
}


INT	LscLgt::Lsc_SetLight(lua_State *pL)
{
	INT		nKey=-1;

	INT		nIdx=0;

	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	nIdx = (INT)lua_tonumber(pL, 2);
	
	ILcLsc::LscH* p	= LscLgt::m_pContAuto->Find(nKey);

	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	ILcLgt*	pLgt= (ILcLgt*)p->pIns;

	pLgt->SetLight(nIdx);

	lua_pushnumber(pL, 0);
	return 1;
}


INT	LscLgt::Lsc_SetLightEnable(lua_State *pL)
{
	INT		nKey=-1;
	BOOL	bEnable =0;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	bEnable = (BOOL)lua_tonumber(pL, 2);
	
	ILcLsc::LscH* p	= LscLgt::m_pContAuto->Find(nKey);

	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	ILcLgt*	pLgt= (ILcLgt*)p->pIns;

	pLgt->SetLightEnable(bEnable);


	lua_pushnumber(pL, 0);
	return 1;
}
	


INT	LscLgt::Lsc_SetLgtType(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	ILcLsc::LscH* p	= LscLgt::m_pContAuto->Find(nKey);

	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	ILcLgt*	pLgt = (ILcLgt*)p->pIns;

	
	DWORD	pVal=0;
	char	sLgtType[64] ={0};
	strcpy(sLgtType, lua_tostring(pL, 2));

	if( 0 ==_stricmp(sLgtType, "Point"))
		pVal = D3DLIGHT_POINT;

	else if( 0 ==_stricmp(sLgtType, "Spot"))
		pVal = D3DLIGHT_SPOT;

	else if( 0 ==_stricmp(sLgtType, "Directional"))
		pVal = D3DLIGHT_DIRECTIONAL;


	pLgt->SetLgtType(pVal);
	
	lua_pushnumber(pL, 0);
	return 1;
}



	
INT LscLgt::Lsc_SetLgtAmbient(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	ILcLsc::LscH* p	= LscLgt::m_pContAuto->Find(nKey);

	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	ILcLgt*	pLgt = (ILcLgt*)p->pIns;

	FLOAT pVal[4]={0, 0, 0, 1};
	pVal[0] = (FLOAT)lua_tonumber(pL, 2);
	pVal[1] = (FLOAT)lua_tonumber(pL, 3);
	pVal[2] = (FLOAT)lua_tonumber(pL, 4);

	pLgt->SetLgtAmbient(pVal);
	
	lua_pushnumber(pL, 0);
	return 1;
}



INT LscLgt::Lsc_SetLgtDiffuse(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	ILcLsc::LscH* p	= LscLgt::m_pContAuto->Find(nKey);
	
	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	ILcLgt*	pLgt = (ILcLgt*)p->pIns;

	FLOAT pVal[4]={0, 0, 0, 1};
	pVal[0] = (FLOAT)lua_tonumber(pL, 2);
	pVal[1] = (FLOAT)lua_tonumber(pL, 3);
	pVal[2] = (FLOAT)lua_tonumber(pL, 4);

	pLgt->SetLgtDiffuse(pVal);
	
	lua_pushnumber(pL, 0);
	return 1;
}


INT LscLgt::Lsc_SetLgtSpecular(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	ILcLsc::LscH* p	= LscLgt::m_pContAuto->Find(nKey);

	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	ILcLgt*	pLgt = (ILcLgt*)p->pIns;

	FLOAT pVal[4]={0, 0, 0, 1};
	pVal[0] = (FLOAT)lua_tonumber(pL, 2);
	pVal[1] = (FLOAT)lua_tonumber(pL, 3);
	pVal[2] = (FLOAT)lua_tonumber(pL, 4);

	pLgt->SetLgtSpecular(pVal);
	
	lua_pushnumber(pL, 0);
	return 1;
}


INT LscLgt::Lsc_SetLgtPosition(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	ILcLsc::LscH* p	= LscLgt::m_pContAuto->Find(nKey);

	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	ILcLgt*	pLgt = (ILcLgt*)p->pIns;

	FLOAT pVal[3]={0, 0, 0};
	pVal[0] = (FLOAT)lua_tonumber(pL, 2);
	pVal[1] = (FLOAT)lua_tonumber(pL, 3);
	pVal[2] = (FLOAT)lua_tonumber(pL, 4);

	pLgt->SetLgtPosition(pVal);
	
	lua_pushnumber(pL, 0);
	return 1;
}


INT LscLgt::Lsc_SetLgtDirection(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	ILcLsc::LscH* p	= LscLgt::m_pContAuto->Find(nKey);

	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	ILcLgt*	pLgt = (ILcLgt*)p->pIns;

	FLOAT pVal[3]={0, 0, 0};
	pVal[0] = (FLOAT)lua_tonumber(pL, 2);
	pVal[1] = (FLOAT)lua_tonumber(pL, 3);
	pVal[2] = (FLOAT)lua_tonumber(pL, 4);


	D3DXVECTOR3	vcDir(pVal);
	D3DXVec3Normalize(&vcDir, &vcDir);
	memcpy(pVal, &vcDir.x, sizeof(D3DXVECTOR3));

	pLgt->SetLgtDirection(pVal);
	
	lua_pushnumber(pL, 0);
	return 1;
}



INT LscLgt::Lsc_SetLgtRange(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	ILcLsc::LscH* p	= LscLgt::m_pContAuto->Find(nKey);

	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	ILcLgt*	pLgt = (ILcLgt*)p->pIns;
	FLOAT	pVal=0;

	pVal	= (FLOAT)lua_tonumber(pL, 2);

	pLgt->SetLgtRange(pVal);
	
	lua_pushnumber(pL, 0);
	return 1;
}


INT LscLgt::Lsc_SetLgtFalloff(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	ILcLsc::LscH* p	= LscLgt::m_pContAuto->Find(nKey);

	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	ILcLgt*	pLgt = (ILcLgt*)p->pIns;
	FLOAT	pVal=0;

	pVal	= (FLOAT)lua_tonumber(pL, 2);

	pLgt->SetLgtFalloff(pVal);
	
	lua_pushnumber(pL, 0);
	return 1;
}


INT LscLgt::Lsc_SetLgtAttenuation0(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	ILcLsc::LscH* p	= LscLgt::m_pContAuto->Find(nKey);

	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	ILcLgt*	pLgt = (ILcLgt*)p->pIns;
	FLOAT	pVal=0;

	pVal	= (FLOAT)lua_tonumber(pL, 2);

	pLgt->SetLgtAttenuation0(pVal);
	
	lua_pushnumber(pL, 0);
	return 1;
}


INT LscLgt::Lsc_SetLgtAttenuation1(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	ILcLsc::LscH* p	= LscLgt::m_pContAuto->Find(nKey);

	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	ILcLgt*	pLgt = (ILcLgt*)p->pIns;
	FLOAT	pVal=0;

	pVal	= (FLOAT)lua_tonumber(pL, 2);

	pLgt->SetLgtAttenuation1(pVal);
	
	lua_pushnumber(pL, 0);
	return 1;
}


INT LscLgt::Lsc_SetLgtAttenuation2(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	ILcLsc::LscH* p	= LscLgt::m_pContAuto->Find(nKey);

	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	ILcLgt*	pLgt = (ILcLgt*)p->pIns;
	FLOAT	pVal=0;

	pVal	= (FLOAT)lua_tonumber(pL, 2);

	pLgt->SetLgtAttenuation2(pVal);
	
	lua_pushnumber(pL, 0);
	return 1;
}


INT LscLgt::Lsc_SetLgtTheta(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	ILcLsc::LscH* p	= LscLgt::m_pContAuto->Find(nKey);

	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	ILcLgt*	pLgt = (ILcLgt*)p->pIns;
	FLOAT	pVal=0;

	pVal	= (FLOAT)lua_tonumber(pL, 2);

	pLgt->SetLgtTheta(pVal);
	
	lua_pushnumber(pL, 0);
	return 1;
}


INT LscLgt::Lsc_SetLgtPhi(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	ILcLsc::LscH* p	= LscLgt::m_pContAuto->Find(nKey);

	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	ILcLgt*	pLgt = (ILcLgt*)p->pIns;
	FLOAT	pVal=0;

	pVal	= (FLOAT)lua_tonumber(pL, 2);

	pLgt->SetLgtPhi(pVal);
	
	lua_pushnumber(pL, 0);
	return 1;
}



INT LscLgt::Lsc_GetLgtType(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	ILcLsc::LscH* p	= LscLgt::m_pContAuto->Find(nKey);

	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	ILcLgt*	pLgt = (ILcLgt*)p->pIns;

	DWORD pVal = pLgt->GetLgtType();

	lua_pushnumber(pL, pVal);
	return 1;
}



INT LscLgt::Lsc_GetLgtAmbient(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		lua_pushnumber(pL, -1);
		lua_pushnumber(pL, -1);
		return 3;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	ILcLsc::LscH* p	= LscLgt::m_pContAuto->Find(nKey);

	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		lua_pushnumber(pL, -1);
		lua_pushnumber(pL, -1);
		return 3;
	}

	ILcLgt*	pLgt = (ILcLgt*)p->pIns;

	FLOAT pVal[4]={0, 0, 0, 1};
	memcpy(pVal, pLgt->GetLgtAmbient(), sizeof pVal);

	lua_pushnumber(pL, pVal[0]);
	lua_pushnumber(pL, pVal[1]);
	lua_pushnumber(pL, pVal[2]);
	return 3;
}



INT LscLgt::Lsc_GetLgtDiffuse(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		lua_pushnumber(pL, -1);
		lua_pushnumber(pL, -1);
		return 3;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	ILcLsc::LscH* p	= LscLgt::m_pContAuto->Find(nKey);

	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		lua_pushnumber(pL, -1);
		lua_pushnumber(pL, -1);
		return 3;
	}

	ILcLgt*	pLgt = (ILcLgt*)p->pIns;

	FLOAT pVal[4]={0, 0, 0, 1};
	memcpy(pVal, pLgt->GetLgtDiffuse(), sizeof pVal);

	lua_pushnumber(pL, pVal[0]);
	lua_pushnumber(pL, pVal[1]);
	lua_pushnumber(pL, pVal[2]);
	return 3;
}


INT LscLgt::Lsc_GetLgtSpecular(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		lua_pushnumber(pL, -1);
		lua_pushnumber(pL, -1);
		return 3;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	ILcLsc::LscH* p	= LscLgt::m_pContAuto->Find(nKey);

	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		lua_pushnumber(pL, -1);
		lua_pushnumber(pL, -1);
		return 3;
	}

	ILcLgt*	pLgt = (ILcLgt*)p->pIns;

	FLOAT pVal[4]={0, 0, 0, 1};
	memcpy(pVal, pLgt->GetLgtSpecular(), sizeof pVal);

	lua_pushnumber(pL, pVal[0]);
	lua_pushnumber(pL, pVal[1]);
	lua_pushnumber(pL, pVal[2]);
	return 3;
}


INT LscLgt::Lsc_GetLgtPosition(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		lua_pushnumber(pL, -1);
		lua_pushnumber(pL, -1);
		return 3;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	ILcLsc::LscH* p	= LscLgt::m_pContAuto->Find(nKey);

	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		lua_pushnumber(pL, -1);
		lua_pushnumber(pL, -1);
		return 3;
	}

	ILcLgt*	pLgt = (ILcLgt*)p->pIns;

	FLOAT pVal[3]={0, 0, 0};
	memcpy(pVal, pLgt->GetLgtPosition(), sizeof pVal);

	lua_pushnumber(pL, pVal[0]);
	lua_pushnumber(pL, pVal[1]);
	lua_pushnumber(pL, pVal[2]);
	return 3;
}


INT LscLgt::Lsc_GetLgtDirection(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		lua_pushnumber(pL, -1);
		lua_pushnumber(pL, -1);
		return 3;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	ILcLsc::LscH* p	= LscLgt::m_pContAuto->Find(nKey);

	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		lua_pushnumber(pL, -1);
		lua_pushnumber(pL, -1);
		return 3;
	}

	ILcLgt*	pLgt = (ILcLgt*)p->pIns;

	FLOAT pVal[3]={0, 0, 0};
	memcpy(pVal, pLgt->GetLgtDirection(), sizeof pVal);

	lua_pushnumber(pL, pVal[0]);
	lua_pushnumber(pL, pVal[1]);
	lua_pushnumber(pL, pVal[2]);
	return 3;
}



INT LscLgt::Lsc_GetLgtRange(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	ILcLsc::LscH* p	= LscLgt::m_pContAuto->Find(nKey);

	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	ILcLgt*	pLgt = (ILcLgt*)p->pIns;
	FLOAT pVal = pLgt->GetLgtRange();

	lua_pushnumber(pL, pVal);
	return 1;
}


INT LscLgt::Lsc_GetLgtFalloff(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	ILcLsc::LscH* p	= LscLgt::m_pContAuto->Find(nKey);

	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	ILcLgt*	pLgt = (ILcLgt*)p->pIns;
	FLOAT pVal = pLgt->GetLgtFalloff();

	lua_pushnumber(pL, pVal);
	return 1;
}


INT LscLgt::Lsc_GetLgtAttenuation0(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	ILcLsc::LscH* p	= LscLgt::m_pContAuto->Find(nKey);

	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	ILcLgt*	pLgt = (ILcLgt*)p->pIns;
	FLOAT pVal = pLgt->GetLgtAttenuation0();

	lua_pushnumber(pL, pVal);
	return 1;
}


INT LscLgt::Lsc_GetLgtAttenuation1(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	ILcLsc::LscH* p	= LscLgt::m_pContAuto->Find(nKey);

	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	ILcLgt*	pLgt = (ILcLgt*)p->pIns;
	FLOAT pVal = pLgt->GetLgtAttenuation1();

	lua_pushnumber(pL, pVal);
	return 1;
}



INT LscLgt::Lsc_GetLgtAttenuation2(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	ILcLsc::LscH* p	= LscLgt::m_pContAuto->Find(nKey);

	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	ILcLgt*	pLgt = (ILcLgt*)p->pIns;
	FLOAT pVal = pLgt->GetLgtAttenuation2();

	lua_pushnumber(pL, pVal);
	return 1;
}


INT LscLgt::Lsc_GetLgtTheta(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	ILcLsc::LscH* p	= LscLgt::m_pContAuto->Find(nKey);

	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	ILcLgt*	pLgt = (ILcLgt*)p->pIns;
	FLOAT pVal = pLgt->GetLgtTheta();

	lua_pushnumber(pL, pVal);
	return 1;
}


INT LscLgt::Lsc_GetLgtPhi(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	ILcLsc::LscH* p	= LscLgt::m_pContAuto->Find(nKey);

	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	ILcLgt*	pLgt = (ILcLgt*)p->pIns;
	FLOAT pVal = pLgt->GetLgtPhi();

	lua_pushnumber(pL, pVal);
	return 1;
}




INT	LscLgt::ContainerCreate()
{
	LscLgt::m_pContAuto = new LscLgt::ILcCont;

	return 0;
}

INT	LscLgt::ContainerDestroy()
{
	if(LscLgt::m_pContAuto)
	{
		delete LscLgt::m_pContAuto;
		LscLgt::m_pContAuto = NULL;
	}

	return 0;
}


INT	LscLgt::StaticContainerCreate()
{
	if(NULL == LscLgt::m_pContStat)
		LscLgt::m_pContStat = new LscLgt::ILcCont;

	return 0;
}


INT	LscLgt::StaticContainerDestroy()
{
	if(LscLgt::m_pContStat)
	{
		delete LscLgt::m_pContStat;
		LscLgt::m_pContStat = NULL;
	}

	return 0;
}

}// namespace Lua