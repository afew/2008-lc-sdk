// Interface for the LscMshX class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LscMshX_H_
#define _LscMshX_H_


#pragma warning(disable : 4786)
#include <vector>

namespace	LcLua
{

struct LscMshX
{
	struct ILcCont
	{
		std::vector<ILcLsc::LscH* >	vLscH;
		
		virtual ~ILcCont();

		INT			Alloc(char* sName, char* sTxFolder);
		INT			ReAlloc(INT nKey, char* sName, char* sTxFolder);
		INT			Release(INT nKey);
		ILcLsc::LscH* Find(INT nKey);
	};


	static	ILcDev*		m_pDevSys	;
	static	void*		m_pDevD3D	;											// d3d Device
	static	INT			Create(ILcDev* pDev);
	
	static	ILcCont*	m_pContAuto	;											// 지역 변수
	static	ILcCont*	m_pContStat	;											// 정적 변수
	

	static INT	Lsc_Alloc(lua_State *pL);										// X-File 로딩
	static INT	Lsc_ReAlloc(lua_State *pL);										// X-File 다시 로딩
	static INT	Lsc_Release(lua_State *pL);										// X-File 해제
	static INT	Lsc_Render(lua_State *pL);										// X-File 그리기

	static INT	Lsc_GetGeomety(lua_State *pL);									// X-File Get Geometries
	static INT	Lsc_GetVtxNum(lua_State *pL);									// X-File Get Vertex Number for Geometry Index
	static INT	Lsc_Lock(lua_State *pL);										// X-File Get Lock for Geometry Index
	static INT	Lsc_Unlock(lua_State *pL);										// X-File Get Unlock for Geometry Index

	static INT	Lsc_GetPos(lua_State *pL);
	static INT	Lsc_SetPos(lua_State *pL);

	static INT	OpenLib(lua_State *pL);


	static INT	ContainerCreate();
	static INT	ContainerDestroy();

	static INT	StaticContainerCreate();
	static INT	StaticContainerDestroy();
};

}// namespace Lua

#endif