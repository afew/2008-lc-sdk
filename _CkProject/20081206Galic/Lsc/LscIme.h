// Interface for the LscIme class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LscIme_H_
#define _LscIme_H_

namespace	LcLua
{

struct LscIme
{
	static	ILcDev*		m_pDevSys	;
	static	ILcIme*		m_pSysIme	;
	static	void*		m_pDevD3D	;
	static	INT			Create(ILcDev* pDev);

	// Hangeul IME
	static INT	Lsc_ImeEnable(lua_State *pL);									// Set Use
	static INT	Lsc_ImeIsEnable(lua_State *pL);									// Is Ime Enable
	static INT	Lsc_ImeStr(lua_State *pL);										// Get Ime String
	static INT	Lsc_ImeStrBeam(lua_State *pL);									// Get Ime String with Beam
	static INT	Lsc_ImeStrStar(lua_State *pL);									// Get Ime String with Star
	static INT	Lsc_ImeSet(lua_State *pL);										// Set Ime
	static INT	Lsc_ImeReset(lua_State *pL);									// Reset Ime


	static INT	OpenLib(lua_State *pL);
};

}// namespace Lua

#endif