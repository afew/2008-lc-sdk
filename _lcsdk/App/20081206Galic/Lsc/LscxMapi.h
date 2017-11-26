// Interface for the LscxMapi class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LscxMapi_H_
#define _LscxMapi_H_


#pragma warning(disable : 4786)
#include <vector>


namespace	LcLua
{
	
struct LscxMapi
{
	static	ILcDev*		m_pDevSys	;
	static	void*		m_pDevD3D	;											// d3d Device
	static	INT			Create(ILcDev* pDev);

	static	void*		m_pTbTex	;
	static	void*		m_pTbMdl	;
	static	void*		m_pFld		;

	static INT	Lsc_Alloc(lua_State*)	;										// Map 로딩
	static INT	Lsc_ReAlloc(lua_State*)	;										// Map 다시 로딩
	static INT	Lsc_Release(lua_State*)	;										// Map 해제
	static INT	Lsc_FrameMove(lua_State*);										// Map 갱신
	static INT	Lsc_Render(lua_State*)	;										// Map 그리기
	
	static INT	OpenLib(lua_State*);

	static INT	ContainerCreate();
	static INT	ContainerDestroy();

	static INT	StaticContainerCreate();
	static INT	StaticContainerDestroy();
};

}// namespace Lua

#endif


