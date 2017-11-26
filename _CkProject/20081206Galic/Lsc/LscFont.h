// Interface for the LscFont cLscss.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LscFont_H_
#define _LscFont_H_


#pragma warning(disable : 4786)
#include <vector>

namespace	LcLua
{

struct LscFont
{
	struct ILcCont																// ContainerFont
	{
		std::vector<ILcLsc::LscH* >	vLscH;

		virtual ~ILcCont();

		INT			Alloc(void** pOut
						, void*	pDev											// LPDIRECT3DDEVICE9
						, char*	sName											// Font Name
						, INT	lHeight=12										// Font Height
						, INT	lWeight=FW_NORMAL								// Font Weight Thin, Normal, Bold... See Go to Definition FW_NORMAL
						, INT	lItalic=FALSE									// Italic?
						, DWORD	dS=0xFFFFFFFF									// String Color for LnFont
						, DWORD	dB=0xFFFFFFFF									// Background Color for LnFont
						, INT	iThckX=0										// Thick Font for LnFont
						, INT	iThckY=0										// Thick Font for LnFont
						);
		INT			Release(INT nKey);
		ILcLsc::LscH* Find(INT nKey);
	};


	static	ILcDev*		m_pDevSys	;
	static	void*		m_pDevD3D	;
	static	INT			Create(ILcDev* pDev);

	
	static	ILcCont*	m_pContAuto	;											// 지역 변수
	static	ILcCont*	m_pContStat	;											// 정적 변수
	
	//Lsc API.
	static INT	Lsc_Alloc(lua_State *pL);										// 폰트 로딩
	static INT	Lsc_Release(lua_State *pL);										// 폰트 해제

	static INT	Lsc_FontDraw(lua_State *pL);									// 폰트 Draw
	static INT	Lsc_FontColorFore(lua_State *pL);								// 폰트 Foreground Color
	static INT	Lsc_FontColorBack(lua_State *pL);								// 폰트 Background Color

	static INT	OpenLib(lua_State *pL);


	static INT	ContainerCreate();
	static INT	ContainerDestroy();

	static INT	StaticContainerCreate();
	static INT	StaticContainerDestroy();
};

}// namespace Lua

#endif