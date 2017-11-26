// Implementation of the LscInput class.
//
////////////////////////////////////////////////////////////////////////////////


#include <windows.h>
#include <d3dx9.h>

#include <Lua/lua.h>
#include <Lua/lualib.h>
#include <Lua/lauxlib.h>

#include <Lc/ILcInput.h>
#include <Lc/ILcDev.h>

#include "LscInput.h"


namespace	LcLua
{
	
	
ILcDev*		LscInput::m_pDevSys		= NULL;
ILcInput*	LscInput::m_pInput		= NULL;


INT LscInput::Create(ILcDev* pDev)
{
	LscInput::m_pDevSys	= pDev;

	void*	pData = NULL;
	if(FAILED(m_pDevSys->Query("Get Input", &pData)))
		return -1;

	LscInput::m_pInput = (ILcInput*)pData;

	return 0;
}



static const luaL_reg regLscInput[] =
{
	{"KeyboardAll"	,	LscInput::Lsc_KeyboardAll	},
	{"KeyboardOne"	,	LscInput::Lsc_KeyboardOne	},
	{"MousePos"		,	LscInput::Lsc_MousePos		},
	{"MouseDelta"	,	LscInput::Lsc_MouseDelta	},
	{"MouseEvnt"	,	LscInput::Lsc_MouseEvnt		},

	{NULL, NULL},
};


INT LscInput::OpenLib(lua_State *pL)
{
	luaL_openlib(pL, "Lin", regLscInput, 0);
	return 1;
}


// Implementations La API.
//
////////////////////////////////////////////////////////////////////////////////


INT LscInput::Lsc_KeyboardAll(lua_State *pL)
{
	BYTE pKeyMap[256]={0};
	
	memcpy(pKeyMap, LscInput::m_pInput->GetKeyMap(), sizeof pKeyMap);
	
	for(INT i=0; i<256; ++i)
		lua_pushnumber(pL, pKeyMap[i]);
	
	return 256;
}


INT LscInput::Lsc_KeyboardOne(lua_State *pL)
{
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		//		->SetShowState(FALSE);
		lua_pushnumber(pL, 0);
		return 1;
	}
	
	INT		nKey = (INT)lua_tonumber(pL, 1);
	
	
	if(nKey<0 || nKey>=255)
	{
		lua_pushnumber(pL, 0);
		return 1;
	}
	
	INT KeySt = LscInput::m_pInput->KeyState(nKey);
	
	lua_pushnumber(pL, KeySt);
	return 1;
}




INT LscInput::Lsc_MousePos(lua_State *pL)
{
	D3DXVECTOR3 vcMouse = LscInput::m_pInput->GetMousePos();
	
	INT	mouseX	= INT(vcMouse.x);
	INT	mouseY	= INT(vcMouse.y);
	INT	mouseZ	= INT(vcMouse.z);
	
	lua_pushnumber(pL, mouseX);
	lua_pushnumber(pL, mouseY);
	lua_pushnumber(pL, mouseZ);
	
	return 3;
}


INT LscInput::Lsc_MouseDelta(lua_State *pL)
{
	D3DXVECTOR3 vcDelta = LscInput::m_pInput->GetMouseDelta();
	
	lua_pushnumber(pL, vcDelta.x);
	lua_pushnumber(pL, vcDelta.y);
	lua_pushnumber(pL, vcDelta.z);
	
	return 3;
}


INT LscInput::Lsc_MouseEvnt(lua_State *pL)
{
	BYTE* pMouseMap = LscInput::m_pInput->GetButtonMap();
	
	lua_pushnumber(pL, pMouseMap[0]);
	lua_pushnumber(pL, pMouseMap[1]);
	lua_pushnumber(pL, pMouseMap[2]);
	
	return 3;
}


}// namespace Lua
