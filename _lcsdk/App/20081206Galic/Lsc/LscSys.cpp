// Implementation of the LscSys class.
//
////////////////////////////////////////////////////////////////////////////////


#include <windows.h>
#include <windowsx.h>
#include <tchar.h>

#include <d3dx9.h>

#include <Lua/lua.h>
#include <Lua/lualib.h>
#include <Lua/lauxlib.h>

#include <Lc/LcEuclid.h>
#include <Lc/ILcTex.h>
#include <Lc/LcUtil.h>
#include <Lc/ILcDev.h>

#include <Lc/LcUtil.h>
#include <Lc/LcString.h>

#include "LscSys.h"



namespace	LcLua
{
	
ILcDev* LscSys::m_pDevSys	= NULL;
void*	LscSys::m_pDevD3D	= NULL;


INT LscSys::Create(ILcDev* pDev)
{
	INT		hr=-1;
	
	LscSys::m_pDevSys	= pDev;
	LscSys::m_pDevD3D	= LcDev_GetD3Device(&m_pDevSys);

	if(NULL == LscSys::m_pDevD3D)
		return -1;

	return 0;
}


INT LscSys::Destroy()
{
	return 0;
}



static const luaL_reg regLscSys[] =
{
	{"CreateWindow"		,	LscSys::Lsc_CreateWindow	},
	{"ScriptFile"		,	LscSys::Lsc_ScriptFile		},
	{"Release"			,	LscSys::Lsc_Release			},

	{"ConsoleAlloc"		,	LscSys::Lsc_ConsoleAlloc	},
	{"ConsoleSet"		,	LscSys::Lsc_ConsoleSet		},
	{"ConsoleSend"		,	LscSys::Lsc_ConsoleSend		},

	{"Sleep"			,	LscSys::Lsc_Sleep			},
	{"ShowCursor"		,	LscSys::Lsc_ShowCursor		},

	{"GetScnSize"		,	LscSys::Lsc_GetScnSize		},
	{"GetWindowStyle"	,	LscSys::Lsc_GetWindowStyle	},
	{"SetWindowStyle"	,	LscSys::Lsc_SetWindowStyle	},
	{"ShowState"		,	LscSys::Lsc_ShowState		},
	{"ChangeMode"		,	LscSys::Lsc_ChangeMode		},
	{"SetClearColor"	,	LscSys::Lsc_SetClearColor	},
	{"GetClearColor"	,	LscSys::Lsc_GetClearColor	},

	{"GetTime"			,	LscSys::Lsc_GetTime			},

	{"WindowTitle"		,	LscSys::Lsc_WindowTitle		},
	{"ModulePath"		,	LscSys::Lsc_ModulePath		},
	{"DoFile"			,	LscSys::Lsc_DoFile			},
	{"Open"				,	LscSys::Lsc_OpenFile		},

	{"MessageBox"		,	LscSys::Lsc_MessageBox		},

	{NULL, NULL},
};


INT LscSys::OpenLib(lua_State *pL)
{
	luaL_openlib(pL, "Lsys", regLscSys, 0);
	return 1;
}


// Implementations La API.
//
////////////////////////////////////////////////////////////////////////////////



INT LscSys::Lsc_CreateWindow(lua_State *pL)
{
	INT		hr=-1;
	INT		n = lua_gettop(pL);
	
	if(n<4)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	INT		iScnX	= 0;
	INT		iScnY	= 0;
	INT		iScnW	= 0;
	INT		iScnH	= 0;
	char	sClsName[160]={0};
	BOOL	bFull	= 0;

	iScnX	= (INT)lua_tonumber(pL, 1);
	iScnY	= (INT)lua_tonumber(pL, 2);
	iScnW	= (INT)lua_tonumber(pL, 3);
	iScnH	= (INT)lua_tonumber(pL, 4);
	
	_tcscpy(sClsName, (char*)lua_tostring(pL, 5));
	bFull = (INT)lua_tonumber(pL, 6)? 1: 0;
	

	LscSys::m_pDevSys->Query("Set Screen Pos X"		, &iScnX	);
	LscSys::m_pDevSys->Query("Set Screen Pos Y"		, &iScnY	);
	LscSys::m_pDevSys->Query("Set Screen Width"		, &iScnW	);
	LscSys::m_pDevSys->Query("Set Screen Height"	, &iScnH	);
	LscSys::m_pDevSys->Query("Set Class Name"		, sClsName	);
	LscSys::m_pDevSys->Query("Set Window Title"		, sClsName	);

	LscSys::m_pDevSys->Query("Set FullMode Start"	, &bFull	);
	
	return 0;
}




INT LscSys::Lsc_ScriptFile(lua_State *pL)
{
	INT		hr=-1;
	INT		n = lua_gettop(pL);
	
	if(n<2)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	char sFile[160+4]={0};
	INT nIdx = (INT)lua_tonumber(pL, 1);
	strcpy(sFile+ 4, (char*)lua_tostring(pL, 2) );

	memcpy(sFile, &nIdx, sizeof nIdx);
	
	LscSys::m_pDevSys->Query("Set Script File", sFile	);
	
	return 0;
}



INT LscSys::Lsc_Release(lua_State *pL)
{
	//	printf("La Release\n");
	
	return 0;
}




INT LscSys::Lsc_ConsoleAlloc(lua_State *pL)
{
	LscSys::m_pDevSys->Query("Console Alloc", NULL);
	
	return 0;
}

INT LscSys::Lsc_ConsoleSet(lua_State *pL)
{
	INT v1= (INT)lua_tonumber(pL, 1);

	LscSys::m_pDevSys->Query("Set Console", &v1);
	
	return 0;
}


INT LscSys::Lsc_ConsoleSend(lua_State *pL)
{
	char* v1= (char*)lua_tostring(pL, 1);

	LscSys::m_pDevSys->Query("Send Console", v1);
	
	return 0;
}





INT LscSys::Lsc_Sleep(lua_State *pL)
{
	INT		n = lua_gettop(pL);
	
	if(n<1)
		Sleep(1000);
	
	INT nMilliSecond = (INT)lua_tonumber(pL, 1);
	
	Sleep(nMilliSecond);
	
	return 0;
}


INT LscSys::Lsc_ShowCursor(lua_State *pL)
{
	INT		n = lua_gettop(pL);
	
	if(n<1)
		Sleep(1000);
	
	INT nShow = (INT)lua_tonumber(pL, 1);
	
	(nShow)? ::ShowCursor(TRUE): ::ShowCursor(FALSE);
	
	return 0;
}


INT LscSys::Lsc_GetScnSize(lua_State *pL)
{
	INT		iScnW	= 0;
	INT		iScnH	= 0;
	
	LscSys::m_pDevSys->Query("Get Screen Width"		, &iScnW	);
	LscSys::m_pDevSys->Query("Get Screen Height"	, &iScnH	);
	
	lua_pushnumber(pL, iScnW);
	lua_pushnumber(pL, iScnH);
	
	return 2;
}



INT LscSys::Lsc_WindowTitle(lua_State *pL)
{
	char	sMsg[512]={0};
	
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	for(INT i=1; i<=n; ++i)
	{
		char sRcv[512]={0};
		
		if (!lua_isnumber(pL, i))
		{
			sprintf(sRcv, " %s", lua_tostring(pL, i));
			strcat(sMsg, sRcv);
		}
		
		else
		{
			double fRcvNumber = lua_tonumber(pL, i);
			sprintf(sRcv, " %6.3f", fRcvNumber);
			strcat(sMsg, sRcv);
		}
	}
	

	HWND hWnd = (HWND)LcDev_GetHwnd(&LscSys::m_pDevSys);
	LcUtil_SetWindowTitle(hWnd, sMsg);
	
	return 0;
}




INT LscSys::Lsc_ModulePath(lua_State *pL)
{
	char	s[512]={0};
	
	TCHAR stPath[MAX_PATH];

#ifdef _UNICODE
	LcStr_UnicodeToAnsi(sPath, stPath, MAX_PATH);
#else
	strcpy(s, stPath);
#endif

	lua_pushstring(pL, s);
	return 1;
}




INT LscSys::Lsc_DoFile(lua_State *pL)
{
	INT		hr = 0;
	char	sPath[MAX_PATH]={0};
	TCHAR	stPath[MAX_PATH]={0};

	const char *fname = luaL_optstring(pL, 1, NULL);


#ifdef _UNICODE
	LcUtil_GetModulePath(stPath);
	LcStr_UnicodeToAnsi(sPath, stPath, MAX_PATH);

	strcat(sPath, "\\");
	strcat(sPath, fname);
#else
	strcpy(sPath, fname);
#endif

	INT		n = lua_gettop(pL);

	hr = luaL_loadfile(pL, sPath);

	if (hr != 0)
		lua_error(pL);

	lua_call(pL, 0, LUA_MULTRET);

	return lua_gettop(pL) - n;
}



INT LscSys::Lsc_OpenFile(lua_State *pL)
{
	INT		hr = 0;
	char	sPath[MAX_PATH]={0};
	TCHAR	stPath[MAX_PATH]={0};

	const char *fname = luaL_checkstring(pL, 1);
	const char *mode = luaL_optstring(pL, 2, "r");

	
#ifdef _UNICODE
	LcUtil_GetModulePath(stPath);
	LcStr_UnicodeToAnsi(sPath, stPath, MAX_PATH);
	
	strcat(sPath, "\\");
	strcat(sPath, fname);
#else
	strcpy(sPath, fname);
#endif

	FILE **pf = (FILE **)lua_newuserdata(pL, sizeof(FILE *));
	*pf = NULL;  /* file handle is currently `closed' */
	luaL_getmetatable(pL, "FILE*");
	lua_setmetatable(pL, -2);

	*pf = fopen(sPath, mode);

	return (*pf == NULL) ? luaL_pushresult(pL, 0, sPath) : 1;
}



INT LscSys::Lsc_MessageBox(lua_State *pL)
{
	char	sMsg[512]={0};
	char	sCap[128]={0};
	INT		uType = 0;
	
	INT		n = lua_gettop(pL);
	HWND	hWnd = (HWND)LcDev_GetHwnd(&LscSys::m_pDevSys);
	
	if(1 == n)
	{
		sprintf(sMsg, "%s", lua_tostring(pL, 1));
	}
	
	else if(2 == n)
	{
		sprintf(sMsg, "%s", lua_tostring(pL, 1));
		sprintf(sCap, "%s", lua_tostring(pL, 2));
	}
	
	else if(3 <= n)
	{
		sprintf(sMsg, "%s", lua_tostring(pL, 1));
		sprintf(sCap, "%s", lua_tostring(pL, 2));
		uType = lua_tonumber(pL, 3);		
	}
	
	MessageBox(hWnd, sMsg, sCap, uType);
	return 0;
}


INT LscSys::Lsc_SetWindowStyle(lua_State *pL)
{
	return 0;
}

INT LscSys::Lsc_GetWindowStyle(lua_State *pL)
{
	HWND hWnd = (HWND)LcDev_GetHwnd(&LscSys::m_pDevSys);

	lua_pushnumber(pL, GetWindowStyle(hWnd));
	return 1;
}


INT	LscSys::Lsc_ShowState(lua_State *pL)
{
	INT		bShowState= FALSE;
	INT		n = lua_gettop(pL);
	
	if(n>0)
		bShowState = (INT)lua_tonumber(pL, 1);

	LscSys::m_pDevSys->Query("Set Show State", &bShowState);
	
	return 0;
}


INT	LscSys::Lsc_ChangeMode(lua_State *pL)
{
	LscSys::m_pDevSys->Query("Set Toggle Full", NULL);
	return 0;
}


INT	LscSys::Lsc_SetClearColor(lua_State *pL)
{
	char	sColor[32]={0};
	DWORD	dColor = 0x0;

	INT		n = lua_gettop(pL);
	
	strcpy(sColor, lua_tostring(pL, 1));
	sscanf(sColor, "%x", &dColor);

	LscSys::m_pDevSys->Query("Set Clear Color", &dColor);

	return 0;
}

INT	LscSys::Lsc_GetClearColor(lua_State *pL)
{
	DWORD	dColor = 0x0;
	LscSys::m_pDevSys->Query("Get Clear Color", &dColor);
	
	lua_pushnumber(pL, dColor);
	return 1;
}



INT	LscSys::Lsc_GetTime(lua_State *pL)
{
	DOUBLE dTime =0.;
	LscSys::m_pDevSys->Query("Get Time Stored", &dTime);

	lua_pushnumber(pL, dTime);
	return 1;
}


}// namespace Lua