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

		INT				Alloc(void** pOut,char* sFile,DWORD dFlag,void* pDev);	// ���� ����
		INT			  	Release(INT nKey);										// ���� ����
		ILcLsc::LscH*	Find(INT nKey);											// ã��
	};


	static	ILcCont*		m_pContAuto	;										// ���� ����
	static	ILcCont*		m_pContStat	;										// ���� ����

	
	static INT	Lsc_Alloc(lua_State *pL);										// ���� ���� �ε�
	static INT	Lsc_Release(lua_State *pL);										// ���� ���� ����
	
	static INT	Lsc_SoundPlay(lua_State *pL);									// ���� �÷���
	static INT	Lsc_SoundStop(lua_State *pL);									// ���� Stop
	static INT	Lsc_SoundReset(lua_State *pL);									// ���� Reset
	static INT	Lsc_SoundVolume(lua_State *pL);									// ���� Volume

	static INT	Lsc_SetRepeat(lua_State *pL);									// ���� Repeat
	static INT	Lsc_GetState(lua_State *pL);									// ���� State

	static INT	Lsc_Draw(lua_State *pL);										// DShow Draw

	static INT	OpenLib(lua_State *pL);

	
	static INT	ContainerCreate();
	static INT	ContainerDestroy();

	static INT	StaticContainerCreate();
	static INT	StaticContainerDestroy();
};

}// namespace Lua

#endif
