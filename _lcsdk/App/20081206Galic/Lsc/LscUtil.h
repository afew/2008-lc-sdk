// Interface for the LscUtil class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LscUtil_H_
#define _LscUtil_H_


namespace	LcLua
{
	
struct LscUtil
{
	static	ILcDev*		m_pDevSys	;
	static	void*		m_pDevD3D	;
	static	INT			Create(ILcDev* pDev);

	//Lsc API.
	static INT	Lsc_Mod(lua_State *pL);											// Modulate
	static INT	Lsc_Rand(lua_State *pL);										// Random
	static INT	Lsc_GetTickCount(lua_State *pL);								// Get Tick Count
	static INT	Lsc_TimeGetTime(lua_State *pL);									// TimeGetTime
	
	static INT	Lsc_CastInt(lua_State *pL);										// Casting Integer
	static INT	Lsc_GetWord(lua_State *pL);										// Get Word from String
	static INT	Lsc_GetWords(lua_State *pL);									// Get Word from Strings
	static INT	Lsc_GetWordLen(lua_State *pL);									// Get Word Length from Strings
	static INT	Lsc_StrLen(lua_State *pL);										// Get String Length

	// string Module
	static INT	Lsc_StrCmp(lua_State *pL);										// String Compare
	static INT	Lsc_StrToInt(lua_State *pL);									// String To Int
	static INT	Lsc_StrToDouble(lua_State *pL);									// String To Double

	static INT	OpenLib(lua_State *pL);
};

}// namespace Lua

#endif