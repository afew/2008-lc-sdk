// Implementation of the LscSmd class.
//
////////////////////////////////////////////////////////////////////////////////


#include <windows.h>
#include <d3dx9.h>

#include <Lua/lua.h>
#include <Lua/lualib.h>
#include <Lua/lauxlib.h>

#include <Lc/ILcDev.h>
#include <Lc/ILcSmd.h>

#include <Lc/ILcLsc.h>


#include "LscSmd.h"



namespace	LcLua
{
	
// For Container
////////////////////////////////////////////////////////////////////////////////

LscSmd::ILcCont::~ILcCont()
{
	if(!vLscH.empty())
	{
		INT iSize = vLscH.size();

		for(INT i=0; i<iSize; ++i)
		{
			ILcSmd* pDst = (ILcSmd*)(vLscH[i]->pIns);

			delete pDst;

			delete vLscH[i];
		}

		vLscH.clear();
	}
}



INT LscSmd::ILcCont::Alloc(void** pOut, char* sFile, DWORD dFlag, void* pDev)
{
	INT			nKey	=-1;
	
	INT			hr		=-1;
	ILcSmd*		pSound	= NULL	;

	nKey = vLscH.size();

	hr = LcSmd_Create("", &pSound, sFile, 0, 0, (void*)(WM_USER+1024+nKey), pDev);

	if(FAILED(hr))
		return -1;

	ILcLsc::LscH* pLH = new	ILcLsc::LscH;

	pLH->pIns	= pSound;

	nKey = vLscH.size();
	vLscH.push_back(pLH);

	return nKey;
}

INT LscSmd::ILcCont::Release(INT nKey)
{
	INT iSize = vLscH.size();

	if(iSize<=nKey || nKey<0)
		return -1;

	ILcLsc::LscH** pLa  = vLscH.begin() + nKey;
	ILcSmd* pDst = (ILcSmd*)( (*pLa)->pIns);

	if(pDst)
		delete pDst;

	vLscH.erase( pLa);

	return vLscH.size();
}

ILcLsc::LscH* LscSmd::ILcCont::Find(INT nKey)
{
	INT iSize = vLscH.size();

	if(iSize<=nKey || nKey<0)
		return NULL;

	return vLscH[nKey];
}



////////////////////////////////////////////////////////////////////////////////
// Script

ILcDev*				LscSmd::m_pDevSys	= NULL;
void*				LscSmd::m_pDevD3D	= NULL;

LscSmd::ILcCont*	LscSmd::m_pContAuto	= NULL;
LscSmd::ILcCont*	LscSmd::m_pContStat	= NULL;


INT LscSmd::Create(ILcDev* pDev)
{
	LscSmd::m_pDevSys	= pDev;
	LscSmd::m_pDevD3D	= LcDev_GetD3Device(&m_pDevSys);

	if(NULL == LscSmd::m_pDevD3D)
		return -1;

	return 0;
}




static const luaL_reg regLaSnd[] =
{
	{"New"			,	LscSmd::Lsc_Alloc		},
	{"Release"		,	LscSmd::Lsc_Release		},

	{"Play"			,	LscSmd::Lsc_SoundPlay	},
	{"Stop"			,	LscSmd::Lsc_SoundStop	},
	{"Reset"		,	LscSmd::Lsc_SoundReset	},
	{"Volume"		,	LscSmd::Lsc_SoundVolume	},
	{"SetRepeat"	,	LscSmd::Lsc_SetRepeat	},
	{"GetState"		,	LscSmd::Lsc_GetState	},
	{"Draw"			,	LscSmd::Lsc_Draw		},

	{NULL, NULL},
};


INT LscSmd::OpenLib(lua_State *pL)
{
	luaL_openlib(pL, "Lsmd", regLaSnd, 0);
	return 1;
}






// sound
INT	LscSmd::Lsc_Alloc(lua_State *pL)
{
	INT		nKey=-1;
	char	sFile[160]={0};
	DWORD	dFlag = 0;

	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	else if(1 == n)
	{
		strcpy(sFile, lua_tostring(pL, 1));
	}
	else if(2 == n)
	{
		strcpy(sFile, lua_tostring(pL, 1));
		dFlag = (DWORD)lua_tonumber(pL, 2);
	}
	else
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	// d: 16 --> DSBCAPS_CTRL3D( 3D Sound )
		
	nKey = LscSmd::m_pContAuto->Alloc(NULL, sFile, dFlag, LscSmd::m_pDevD3D);
	
	lua_pushnumber(pL, nKey);
	
	return 1;
}


INT LscSmd::Lsc_Release(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	INT iSize = LscSmd::m_pContAuto->Release(nKey);
	
	// 남아 있는 사이즈를 돌려준다.
	lua_pushnumber(pL, iSize);
	
	return 1;
}



INT LscSmd::Lsc_SoundPlay(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	ILcLsc::LscH* p	= LscSmd::m_pContAuto->Find(nKey);
	
	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	if(p->pIns)
	{
		ILcSmd* pSnd = (ILcSmd*)p->pIns;
		pSnd->Play();
	}

	lua_pushnumber(pL, 0);
	return 1;
}



INT LscSmd::Lsc_SoundStop(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	

	ILcLsc::LscH* p	= LscSmd::m_pContAuto->Find(nKey);

	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	if(p->pIns)
	{
		ILcSmd* pSnd = (ILcSmd*)p->pIns;
		pSnd->Stop();
	}

	lua_pushnumber(pL, 0);
	return 1;
}




INT LscSmd::Lsc_SoundReset(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	ILcLsc::LscH* p	= LscSmd::m_pContAuto->Find(nKey);

	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	if(p->pIns)
	{
		ILcSmd* pSnd = (ILcSmd*)p->pIns;
		pSnd->Reset();
	}
	

	lua_pushnumber(pL, 0);
	return 1;
}




INT LscSmd::Lsc_SoundVolume(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	LONG	lVolume=0;
	
	if(n<2)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey	= (INT)lua_tonumber(pL, 1);
	lVolume = (LONG)lua_tonumber(pL, 2);

	lVolume *=10;
	lVolume -=10000;
	
	ILcLsc::LscH* p	= LscSmd::m_pContAuto->Find(nKey);

	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	if(p->pIns)
	{
		ILcSmd* pSnd = (ILcSmd*)p->pIns;
		pSnd->SetVolume(lVolume);
	}

	lua_pushnumber(pL, 0);
	return 1;
}


INT LscSmd::Lsc_SetRepeat(lua_State *pL)
{
	INT		nKey=-1;
	INT		nVal=0;
	INT		n = lua_gettop(pL);
	
	if(n<2)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey	= (INT)lua_tonumber(pL, 1);
	nVal	= (INT)lua_tonumber(pL, 2);
	
	ILcLsc::LscH* p	= LscSmd::m_pContAuto->Find(nKey);

	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	ILcSmd* pSnd = (ILcSmd*)p->pIns;

	if(1==nVal)
		pSnd->SetRepeat();
	else
		pSnd->SetRepeat(0);


	lua_pushnumber(pL, 0);
	return 1;
}


INT LscSmd::Lsc_GetState(lua_State *pL)
{
	INT		nKey=-1;
	INT		nVal=0;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey	= (INT)lua_tonumber(pL, 1);
	
	ILcLsc::LscH* p	= LscSmd::m_pContAuto->Find(nKey);

	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	
	ILcSmd* pSnd = (ILcSmd*)p->pIns;
	nVal = pSnd->GetStatus();


	lua_pushnumber(pL, nVal);
	return 1;
}



INT LscSmd::Lsc_Draw(lua_State *pL)
{
	INT		nKey=-1;
	INT		nVal=0;
	INT		iScnX;
	INT		iScnY;
	INT		nScnW;
	INT		nScnH;
	char	sColor[32];
	DWORD	dColor;

	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey	= (INT)lua_tonumber(pL, 1);
	iScnX	= (INT)lua_tonumber(pL, 2);
	iScnY	= (INT)lua_tonumber(pL, 3);
	nScnW	= (INT)lua_tonumber(pL, 4);
	nScnH	= (INT)lua_tonumber(pL, 5);

	strcpy(sColor, lua_tostring(pL, 6));
	sscanf(sColor,"%x", &dColor);

	ILcLsc::LscH* p	= LscSmd::m_pContAuto->Find(nKey);

	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}



	
	ILcSmd* pSnd = (ILcSmd*)p->pIns;

	pSnd->Query("Set Screen Pos X", &iScnX);
	pSnd->Query("Set Screen Pos Y", &iScnY);
	pSnd->Query("Set Screen Width", &nScnW);
	pSnd->Query("Set Screen Height", &nScnH);
	pSnd->Query("Set Diffuse"	, &dColor);

	pSnd->Query("Draw",	&nVal);

	lua_pushnumber(pL, nVal);
	return 1;
}








INT	LscSmd::ContainerCreate()
{
	LscSmd::m_pContAuto = new LscSmd::ILcCont;

	return 0;
}

INT	LscSmd::ContainerDestroy()
{
	if(LscSmd::m_pContAuto)
	{
		delete LscSmd::m_pContAuto;
		LscSmd::m_pContAuto = NULL;
	}

	return 0;
}


INT	LscSmd::StaticContainerCreate()
{
	if(NULL == LscSmd::m_pContStat)
		LscSmd::m_pContStat = new LscSmd::ILcCont;


	return 0;
}


INT	LscSmd::StaticContainerDestroy()
{
	if(LscSmd::m_pContStat)
	{
		delete LscSmd::m_pContStat;
		LscSmd::m_pContStat = NULL;
	}

	return 0;
}


}// namespace Lua