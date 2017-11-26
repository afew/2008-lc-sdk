// Implementation of the LscD3x class.
//
////////////////////////////////////////////////////////////////////////////////


#include <windows.h>
#include <windowsx.h>
#include <tchar.h>

#include <d3d9.h>
#include <d3dx9.h>

#include <Lua/lua.h>
#include <Lua/lualib.h>
#include <Lua/lauxlib.h>

#include <Lc/LcEuclid.h>
#include <Lc/ILcTex.h>
#include <Lc/LcUtil.h>
#include <Lc/ILcDev.h>

#include <Lc/ILcLsc.h>

#include "LscD3x.h"


namespace	LcLua
{

ILcDev* LscD3x::m_pDevSys	= NULL;
void*	LscD3x::m_pDevD3D	= NULL;


INT LscD3x::Create(ILcDev* pDev)
{
	LscD3x::m_pDevSys	= pDev;
	LscD3x::m_pDevD3D	= LcDev_GetD3Device(&m_pDevSys);

	if(NULL == LscD3x::m_pDevD3D)
		return -1;

	return 0;
}



struct LcD3DXType
{
	const char * sName;
	DWORD		 dType;
};



static const luaL_reg regLscD3d[] =
{
	{"MatrixRotationX"		,	LscD3x::Lsc_MatrixRotationX			},
	{"MatrixRotationY"		,	LscD3x::Lsc_MatrixRotationY			},
	{"MatrixRotationZ"		,	LscD3x::Lsc_MatrixRotationZ			},

	{"MatrixLookAt"			,	LscD3x::Lsc_MatrixLookAt			},
	{"MatrixPerspectiveFov"	,	LscD3x::Lsc_MatrixPerspectiveFov	},

	{NULL, NULL},
};


INT LscD3x::OpenLib(lua_State *pL)
{
	luaL_openlib(pL, "Ld3x", regLscD3d, 0);
	return 1;
}


INT LscD3x::Lsc_MatrixRotationX(lua_State *pL)
{
	lua_pushnumber(pL, 0);
	return 1;
}


INT LscD3x::Lsc_MatrixRotationY(lua_State *pL)
{
	lua_pushnumber(pL, 0);
	return 1;
}



INT LscD3x::Lsc_MatrixRotationZ(lua_State *pL)
{
	lua_pushnumber(pL, 0);
	return 1;
}


INT LscD3x::Lsc_MatrixPerspectiveFov(lua_State *pL)
{
	lua_pushnumber(pL, 0);
	return 1;
}


INT LscD3x::Lsc_MatrixLookAt(lua_State *pL)
{
	lua_pushnumber(pL, 0);
	return 1;
}


}// namespace Lua