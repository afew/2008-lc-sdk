// Interface for the LscMdl class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcmMdl_H_
#define _LcmMdl_H_


#pragma warning(disable : 4786)
#include <vector>


namespace	LcLua
{
	
struct LscMdl
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

		INT				Restore();
		void			Invalidate();
	};


	static	ILcDev*		m_pDevSys	;
	static	void*		m_pDevD3D	;											// d3d Device
	static	INT			Create(ILcDev* pDev);
	
	static	ILcCont*	m_pContAuto	;											// 지역 변수
	static	ILcCont*	m_pContStat	;											// 정적 변수

	static INT	Lsc_Alloc(lua_State *pL);										// Lcm 로딩
	static INT	Lsc_Release(lua_State *pL);										// Lcm 해제
	static INT	Lsc_FrameMove(lua_State *pL);									// Lcm FrameMode
	static INT	Lsc_Render(lua_State *pL);										// Lcm 그리기
	
	static INT	Lsc_LcmRotation(lua_State *pL);									// Lcm Rotation
	static INT	Lsc_LcmScaling(lua_State *pL);									// Lcm Scaling
	static INT	Lsc_LcmPosition(lua_State *pL);									// Lcm Position
	static INT	Lsc_LcmStartFrame(lua_State *pL);								// Lcm Set Start Frame
	

	static INT	OpenLib(lua_State *pL);


	static INT	ContainerCreate();
	static INT	ContainerDestroy();

	static INT	ContainerRestore();
	static INT	ContainerInvalidate();

	static INT	StaticContainerCreate();
	static INT	StaticContainerDestroy();
};

}// namespace Lua


#endif