// Interface for the LscSys class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LscSys_H_
#define _LscSys_H_


#pragma warning(disable : 4786)
#include <deque>

using namespace std;


namespace	LcLua
{
	
struct LscSys
{
	static	ILcDev*		m_pDevSys	;
	static	void*		m_pDevD3D	;
	static	INT			Create(ILcDev* pDev);
	static	INT			Destroy();

	static INT	Lsc_CreateWindow(lua_State *pL);								// Window Create
	static INT	Lsc_ScriptFile(lua_State *pL);									// Script File Load
	static INT	Lsc_Release(lua_State *pL);										// Window Release

	static INT	Lsc_ConsoleAlloc(lua_State *pL);
	static INT	Lsc_ConsoleSet(lua_State *pL);									// Use console
	static INT	Lsc_ConsoleSend(lua_State *pL);									// Send String to colsole

	static INT	Lsc_Sleep(lua_State *pL);										// Sleep
	static INT	Lsc_ShowCursor(lua_State *pL);									// Show Cursor

	static INT	Lsc_GetScnSize(lua_State *pL);									// 화면의 크기를 가져오기
	static INT	Lsc_GetWindowStyle(lua_State *pL);
	static INT	Lsc_SetWindowStyle(lua_State *pL);								// window Style
	static INT	Lsc_ShowState(lua_State *pL);									// State 보이기
	static INT	Lsc_ChangeMode(lua_State *pL);									// Full <--> Window
	static INT	Lsc_SetClearColor(lua_State *pL);								// 배경화면 클리어 색상설정
	static INT	Lsc_GetClearColor(lua_State *pL);								// 배령화면 클리어 색상 가져오기

	static INT	Lsc_GetTime(lua_State *pL);										// 프로그램이 시작한 후부터...

	static INT	Lsc_WindowTitle(lua_State *pL);									// Set Window Title
	static INT	Lsc_ModulePath(lua_State *pL);									// Get Module Path
	static INT	Lsc_DoFile(lua_State *pL);										// Do File
	static INT	Lsc_OpenFile(lua_State *pL);									// Open File

	static INT	Lsc_MessageBox(lua_State *pL);									// Window Box

	static INT	OpenLib(lua_State *pL);
};

}// namespace Lua

#endif

