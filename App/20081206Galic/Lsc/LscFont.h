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

	
	static	ILcCont*	m_pContAuto	;											// ���� ����
	static	ILcCont*	m_pContStat	;											// ���� ����
	
	//Lsc API.
	static INT	Lsc_Alloc(lua_State *pL);										// ��Ʈ �ε�
	static INT	Lsc_Release(lua_State *pL);										// ��Ʈ ����

	static INT	Lsc_FontDraw(lua_State *pL);									// ��Ʈ Draw
	static INT	Lsc_FontColorFore(lua_State *pL);								// ��Ʈ Foreground Color
	static INT	Lsc_FontColorBack(lua_State *pL);								// ��Ʈ Background Color

	static INT	OpenLib(lua_State *pL);


	static INT	ContainerCreate();
	static INT	ContainerDestroy();

	static INT	StaticContainerCreate();
	static INT	StaticContainerDestroy();
};

}// namespace Lua

#endif