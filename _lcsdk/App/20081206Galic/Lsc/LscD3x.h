// Interface for the LscD3x cLscss.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LscD3x_H_
#define _LscD3x_H_


namespace	LcLua
{

struct LscD3x
{
	static	ILcDev*		m_pDevSys	;
	static	void*		m_pDevD3D	;
	static	INT			Create(ILcDev* pDev);
	
	
	static INT	Lsc_MatrixRotationX		(lua_State *pL);						// Rotation Matrix with Axis X
	static INT	Lsc_MatrixRotationY		(lua_State *pL);						// Rotation Matrix with Axis Y
	static INT	Lsc_MatrixRotationZ		(lua_State *pL);						// Rotation Matrix with Axis Z

	static INT	Lsc_MatrixLookAt		(lua_State *pL);						// View Matrix LH
	static INT	Lsc_MatrixPerspectiveFov(lua_State *pL);						// Projection Matrix LH

	static INT	OpenLib(lua_State *pL);
};


}// namespace Lua


#endif