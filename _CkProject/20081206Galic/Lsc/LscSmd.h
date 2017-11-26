// Interface for the LscSnd class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LUASND_H_
#define _LUASND_H_


#pragma warning(disable : 4786)
#include <vector>


namespace	LcLua
{

struct LscSmd
{
	static	ILcDev*		m_pDevSys	;
	static	void*		m_pDevD3D	;
	static	INT			Create(ILcDev* pDev);


	struct ILcCont																// ContainerFont
	{
		std::vector<ILcLsc::LscH* >	vLscH;

		virtual ~ILcCont();

		INT				Alloc(void** pOut,char* sFile,DWORD dFlag,void* pDev);	// 사운드 생성
		INT			  	Release(INT nKey);										// 사운드 해제
		ILcLsc::LscH*	Find(INT nKey);											// 찾기
	};


	static	ILcCont*		m_pContAuto	;										// 지역 변수
	static	ILcCont*		m_pContStat	;										// 정적 변수

	
	static INT	Lsc_Alloc(lua_State *pL);										// 사운드 파일 로딩
	static INT	Lsc_Release(lua_State *pL);										// 사운드 파일 해제
	
	static INT	Lsc_SoundPlay(lua_State *pL);									// 사운드 플레이
	static INT	Lsc_SoundStop(lua_State *pL);									// 사운드 Stop
	static INT	Lsc_SoundReset(lua_State *pL);									// 사운드 Reset
	static INT	Lsc_SoundVolume(lua_State *pL);									// 사운드 Volume

	static INT	Lsc_SetRepeat(lua_State *pL);									// 사운드 Repeat
	static INT	Lsc_GetState(lua_State *pL);									// 사운드 State

	static INT	Lsc_Draw(lua_State *pL);										// DShow Draw

	static INT	OpenLib(lua_State *pL);

	
	static INT	ContainerCreate();
	static INT	ContainerDestroy();

	static INT	StaticContainerCreate();
	static INT	StaticContainerDestroy();
};

}// namespace Lua

#endif
