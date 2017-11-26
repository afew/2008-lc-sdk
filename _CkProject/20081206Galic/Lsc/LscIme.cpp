// Implementation of the LscIme class.
//
////////////////////////////////////////////////////////////////////////////////


#include <windows.h>

#include <Lua/lua.h>
#include <Lua/lualib.h>
#include <Lua/lauxlib.h>

#include <Lc/ILcIme.h>
#include <Lc/ILcDev.h>

#include "LscIme.h"



namespace	LcLua
{
	
ILcIme* LscIme::m_pSysIme	= NULL;
ILcDev* LscIme::m_pDevSys	= NULL;
void*	LscIme::m_pDevD3D	= NULL;


INT LscIme::Create(ILcDev* pDev)
{
	LscIme::m_pDevSys	= pDev;
	LscIme::m_pDevD3D	= LcDev_GetD3Device(&m_pDevSys);

	if(NULL == LscIme::m_pDevD3D)
		return -1;

	LscIme::m_pSysIme = (ILcIme*)LcDev_GetUserIme(&m_pDevSys);

	if(!LscIme::m_pSysIme)
		return -1;
	
	return 0;
}


static const luaL_reg regLscIme[] =
{
	{"Enable"	,	LscIme::Lsc_ImeEnable	},
	{"IsEnable"	,	LscIme::Lsc_ImeIsEnable	},
	{"Str"		,	LscIme::Lsc_ImeStr		},
	{"StrBeam"	,	LscIme::Lsc_ImeStrBeam	},
	{"StrStar"	,	LscIme::Lsc_ImeStrStar	},
	{"Set"		,	LscIme::Lsc_ImeSet		},
	{"Reset"	,	LscIme::Lsc_ImeReset	},

	{NULL, NULL},
};


INT LscIme::OpenLib(lua_State *pL)
{
	luaL_openlib(pL, "Lime", regLscIme, 0);
	return 1;
}


// Implementations La API.
//
////////////////////////////////////////////////////////////////////////////////
// Hangeul IME

INT LscIme::Lsc_ImeEnable(lua_State *pL)
{
	BOOL	bState = (BOOL)lua_tonumber(pL, 1);
	
	LscIme::m_pSysIme->SetState(bState);
	
	return 0;
}


// Hangeul IME
INT LscIme::Lsc_ImeIsEnable(lua_State *pL)
{
	INT		hr = LscIme::m_pSysIme->GetState();
	
	lua_pushnumber(pL, hr);
	
	return 1;
}



INT LscIme::Lsc_ImeStr(lua_State *pL)
{
	char	s[512]={0};
	LscIme::m_pSysIme->OutString(s);
	
	lua_pushstring(pL, s);
	
	return 1;
}


INT LscIme::Lsc_ImeStrBeam(lua_State *pL)
{
	char	s[512]={0};
	LscIme::m_pSysIme->OutStringBeam(s);
	
	lua_pushstring(pL, s);
	
	return 1;
}


INT LscIme::Lsc_ImeStrStar(lua_State *pL)
{
	char	s[512]={0};
	LscIme::m_pSysIme->OutStringStar(s);
	
	lua_pushstring(pL, s);
	
	return 1;
}



INT LscIme::Lsc_ImeSet(lua_State *pL)
{
	LscIme::m_pSysIme->Set();
	
	return 0;
}


INT LscIme::Lsc_ImeReset(lua_State *pL)
{
	LscIme::m_pSysIme->Reset();
	
	return 0;
}


}// namespace Lua