// Interface for the LscxBill class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LscxBill_H_
#define _LscxBill_H_


#pragma warning(disable : 4786)
#include <vector>


namespace	LcLua
{

struct LscxBill
{
	struct ILcCont
	{
		std::vector<ILcLsc::LscH* >	vLscH;
		
		virtual ~ILcCont();

		INT			Alloc(INT nCnt);
		INT			ReAlloc(INT nKey, INT nCnt);
		INT			Release(INT nKey);
		ILcLsc::LscH* Find(INT nKey);
	};


	static	ILcDev*		m_pDevSys	;
	static	void*		m_pDevD3D	;											// d3d Device
	static	INT			Create(ILcDev* pDev);
	
	static	ILcCont*	m_pContAuto	;											// 지역 변수
	static	ILcCont*	m_pContStat	;											// 정적 변수
	

	static INT	Lsc_Alloc(lua_State *pL);										// Billboard 로딩
	static INT	Lsc_ReAlloc(lua_State *pL);										// Billboard 다시 로딩
	static INT	Lsc_Release(lua_State *pL);										// Billboard 해제
	static INT	Lsc_FrameMove(lua_State *pL);
	static INT	Lsc_Render(lua_State *pL);										// Billboard 그리기

	static INT	Lsc_GetBillNum(lua_State *pL);									// Billboard Get Vertex Number for Geometry Index
	static INT	Lsc_Lock(lua_State *pL);										// Billboard Get Lock for Geometry Index
	static INT	Lsc_Unlock(lua_State *pL);										// Billboard Get Unlock for Geometry Index

	static INT	Lsc_SetDimension(lua_State *pL);
	static INT	Lsc_SetPos(lua_State *pL);
	static INT	Lsc_SetDiffuse(lua_State *pL);
	static INT	Lsc_SetUV(lua_State *pL);
	static INT	Lsc_SetTexPointer(lua_State *pL);
	
	static INT	Lsc_GetPos(lua_State *pL);
	static INT	Lsc_SetupCamera(lua_State *pL);
	static INT	Lsc_SetupVtxBuf(lua_State *pL);
		
	static INT	OpenLib(lua_State *pL);


	static INT	ContainerCreate();
	static INT	ContainerDestroy();

	static INT	StaticContainerCreate();
	static INT	StaticContainerDestroy();
};

}// namespace Lua

#endif
