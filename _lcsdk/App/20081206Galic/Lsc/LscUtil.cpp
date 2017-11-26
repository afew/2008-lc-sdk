// Implementation of the LscUtil class.
//
////////////////////////////////////////////////////////////////////////////////


#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <time.h>

#include <d3dx9.h>

#include <Lua/lua.h>
#include <Lua/lualib.h>
#include <Lua/lauxlib.h>

#include <Lc/LcEuclid.h>
#include <Lc/ILcTex.h>
#include <Lc/LcUtil.h>
#include <Lc/ILcDev.h>

#include <Lc/ILcLsc.h>
#include "LscUtil.h"



namespace	LcLua
{
	
ILcDev* LscUtil::m_pDevSys	= NULL;
void*	LscUtil::m_pDevD3D	= NULL;


INT LscUtil::Create(ILcDev* pDev)
{
	LscUtil::m_pDevSys	= pDev;
	LscUtil::m_pDevD3D	= LcDev_GetD3Device(&m_pDevSys);

	if(NULL == LscUtil::m_pDevD3D)
		return -1;

	srand( (unsigned)time( NULL ) );

	return 0;
}



static const luaL_reg regLscUtil[] =
{
	{"Mod"			,	LscUtil::Lsc_Mod			},
	{"Rand"			,	LscUtil::Lsc_Rand			},
	{"GetTickCount"	,	LscUtil::Lsc_GetTickCount	},
	{"TimeGetTime"	,	LscUtil::Lsc_TimeGetTime	},

	{"Int"			,	LscUtil::Lsc_CastInt		},
	{"GetWord"		,	LscUtil::Lsc_GetWord		},
	{"GetWords"		,	LscUtil::Lsc_GetWords		},
	{"GetWordLen"	,	LscUtil::Lsc_GetWordLen		},

	// String
	{"StrLen"		,	LscUtil::Lsc_StrLen			},
	{"StrCmp"		,	LscUtil::Lsc_StrCmp			},
	{"StrToInt"		,	LscUtil::Lsc_StrToInt		},
	{"StrToDouble"	,	LscUtil::Lsc_StrToDouble	},
	{NULL, NULL},
};


INT LscUtil::OpenLib(lua_State *pL)
{
	luaL_openlib(pL, "Lutil", regLscUtil, 0);
	return 1;
}


// Implementations La API.
//
////////////////////////////////////////////////////////////////////////////////

	

INT	LscUtil::Lsc_Mod(lua_State *pL)
{
	DWORD v1= (DWORD)lua_tonumber(pL, 1);
	DWORD v2= (DWORD)lua_tonumber(pL, 2);
	
	DWORD v3 = v1 %v2;
	
	lua_pushnumber(pL, v3);
	
	return 1;
}



INT LscUtil::Lsc_Rand(lua_State *pL)
{
	DWORD v1= (DWORD)lua_tonumber(pL, 1);
	DWORD v3 = rand() %v1;
	
	lua_pushnumber(pL, v3);
	
	return 1;
}



INT LscUtil::Lsc_GetTickCount(lua_State *pL)
{
//	DWORD v1= ::GetTickCount();
	DWORD v1= ::timeGetTime();
	lua_pushnumber(pL, v1);
	
	return 1;
}


INT LscUtil::Lsc_TimeGetTime(lua_State *pL)
{
	DOUBLE	v1 = 0.;
	m_pDevSys->Query("TimeGetTime", &v1);

	lua_pushnumber(pL, v1);
	return 1;
}





INT	LscUtil::Lsc_CastInt(lua_State *pL)
{
	INT v1= (INT)lua_tonumber(pL, 1);
	
	lua_pushnumber(pL, v1);
	
	return 1;
}



INT	LscUtil::Lsc_GetWord(lua_State *pL)
{
	char* s= (char*)lua_tostring(pL, 1);
	INT nIdx = (INT)lua_tonumber(pL, 2);
	
	
	static char sWord[4];
	char*	p;
	INT		nCnt =0;
	BOOL	b2B= 0;
	
	memset(sWord, 0, sizeof sWord );
	--nIdx;
	p = s;
	
	INT iLen = strlen(s);
	
	while(p<s+iLen)
	{
		// 2바이트면 0보다 작다.
		b2B = ((*p) &0x80);
		
		if(nCnt >= nIdx)
		{
			sWord[0]= *p;
			
			if( b2B)
				sWord[1]= *(p+1);
			
			break;
		}
		
		++nCnt;
		++p;
		
		if(b2B)
			++p;
	}
	
	lua_pushstring(pL, sWord);
	
	return 1;
}

char	sGetWords[65536];

INT LscUtil::Lsc_GetWords(lua_State *pL)
{
	char* s= (char*)lua_tostring(pL, 1);
	INT nIdx = (INT)lua_tonumber(pL, 2);
	
	char*	p;
	char*	pEnd;
	INT		nCnt =0;
	BOOL	b2B= 0;
	
	memset(sGetWords, 0, sizeof sGetWords);
	
	p = s;
	
	INT iLen = strlen(s);
	
	while( p<s+iLen)
	{
		// 2바이트면 0보다 작다.
		b2B = ((*p) &0x80);
		
		if(nCnt >= nIdx)
		{
			pEnd = p;
			
			if( b2B)
				pEnd = p+1;
			
			break;
		}
		
		++nCnt;
		++p;
		
		if(b2B)
			++p;
	}
	
	if(p<s+iLen)
		memcpy(sGetWords, s, pEnd-s);
	else
		strcpy(sGetWords, s);
	
	lua_pushstring(pL, sGetWords);
	
	return 1;
}

INT	LscUtil::Lsc_GetWordLen(lua_State *pL)
{
	char* s= (char*)lua_tostring(pL, 1);
	char*	p;
	INT		nCnt =0;
	BOOL	b2B= false;
	
	p = s;
	
	INT iLen = strlen(s);
	
	while( p<s+iLen)
	{
		// 2바이트면 0보다 작다.
		b2B = ((*p) &0x80);
		++nCnt;
		++p;
		
		if(b2B)
			++p;
	}
	
	lua_pushnumber(pL, nCnt);
	
	return 1;
}



INT	LscUtil::Lsc_StrLen(lua_State *pL)
{
	char* s= (char*)lua_tostring(pL, 1);
	
	INT iLen = strlen(s);
	
	lua_pushnumber(pL, iLen);
	
	return 1;
}


INT	LscUtil::Lsc_StrCmp(lua_State *pL)
{
	char* s1= (char*)lua_tostring(pL, 1);
	char* s2= (char*)lua_tostring(pL, 2);
	
	INT hr = _stricmp(s1, s2);
	
	lua_pushnumber(pL, hr);
	
	return 1;
}


INT	LscUtil::Lsc_StrToInt(lua_State *pL)
{
	char* s= (char*)lua_tostring(pL, 1);
	
	INT hr = atoi(s);
	
	lua_pushnumber(pL, hr);
	
	return 1;
}



INT	LscUtil::Lsc_StrToDouble(lua_State *pL)
{
	char* s= (char*)lua_tostring(pL, 1);
	
	double hr = atof(s);
	
	lua_pushnumber(pL, hr);
	
	return 1;
}

}// namespace Lua