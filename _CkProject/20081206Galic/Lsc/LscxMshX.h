// Interface for the LscxMshX class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LscxMshX_H_
#define _LscxMshX_H_


#pragma warning(disable : 4786)
#include <vector>


namespace	LcLua
{
	
struct LscxMshX
{
	struct ILcCont
	{
		std::vector<ILcLsc::LscH* >	vLscHorg;
		std::vector<ILcLsc::LscH* >	vLscH;
		
		virtual ~ILcCont();

		INT				Alloc(char* sName, void* pLscHorg);
		INT				Release(INT nKey);
		ILcLsc::LscH*	Find(INT nKey);

		INT				AllocOrg(char* sName, char* sTxFolder, char* sEffect);
		ILcLsc::LscH*	FindOrg(char* sName);
		void			SetOrgElapseTime(FLOAT fTime);
	};


	static	ILcDev*		m_pDevSys	;
	static	void*		m_pDevD3D	;											// d3d Device
	static	INT			Create(ILcDev* pDev);
	
	static	ILcCont*	m_pContAuto	;											// 지역 변수
	static	ILcCont*	m_pContStat	;											// 정적 변수

	static INT	Lsc_Alloc(lua_State *pL);										// X-File 로딩
	static INT	Lsc_Release(lua_State *pL);										// X-File 해제
	static INT	Lsc_FrameMove(lua_State *pL);									// X-File FrameMove
	static INT	Lsc_Render(lua_State *pL);										// X-File 그리기
	
	static INT	Lsc_Scaling(lua_State *pL);										// Scaling
	static INT	Lsc_Rotation(lua_State *pL);									// Rotation
	static INT	Lsc_Position(lua_State *pL);									// Position

	static INT	OpenLib(lua_State *pL);


	static INT	ContainerCreate();
	static INT	ContainerDestroy();

	static INT	StaticContainerCreate();
	static INT	StaticContainerDestroy();
};

}// namespace Lua


#endif