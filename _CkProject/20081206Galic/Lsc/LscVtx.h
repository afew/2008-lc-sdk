// Interface for the LscVtx class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LscVtx_H_
#define _LscVtx_H_


#pragma warning(disable : 4786)
#include <vector>


namespace	LcLua
{
	
	
struct LscVtx
{
	struct ILcCont
	{
		std::vector<ILcLsc::LscH* >	vLscH;
		
		virtual ~ILcCont();

		INT			Alloc(char* sVtxType, char* sPrtType, INT nPrt, INT nVtx);
		INT			ReAlloc(INT nKey, char* sVtxType, char* sPrtType, INT nPrt, INT nVtx);
		INT			Release(INT nKey);
		ILcLsc::LscH* Find(INT nKey);
	};


	static	ILcDev*		m_pDevSys	;
	static	void*		m_pDevD3D	;											// d3d Device
	static	INT			Create(ILcDev* pDev);
	
	static	ILcCont*	m_pContAuto	;											// 지역 변수
	static	ILcCont*	m_pContStat	;											// 정적 변수
	

	static INT	Lsc_Alloc(lua_State *pL);										// Vertex 로딩
	static INT	Lsc_ReAlloc(lua_State *pL);										// Vertex 다시 로딩
	static INT	Lsc_Release(lua_State *pL);										// Vertex 해제
	static INT	Lsc_Render(lua_State *pL);										// Vertex 그리기

	static INT	Lsc_VtxNum(lua_State *pL);										// Vertex 숫자
	static INT	Lsc_VtxLock(lua_State *pL);										// Vertex Lock
	static INT	Lsc_VtxUnLock(lua_State *pL);									// Vertex UnLock
	static INT	Lsc_VtxValueSet(lua_State *pL);									// Vertex Value Set
	static INT	Lsc_VtxValuePosition(lua_State *pL);							// Vertex value Set Position
	static INT	Lsc_VtxValueNormal(lua_State *pL);								// Vertex value Set Normal
	static INT	Lsc_VtxValueDiffuse(lua_State *pL);								// Vertex value Set Diffuse
	static INT	Lsc_VtxValueUV(lua_State *pL);									// Vertex value Set UV

	static INT	OpenLib(lua_State *pL);


	static INT	ContainerCreate();
	static INT	ContainerDestroy();

	static INT	StaticContainerCreate();
	static INT	StaticContainerDestroy();
};

}// namespace Lua

#endif
