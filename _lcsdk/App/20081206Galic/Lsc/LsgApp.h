// Interface for the LsgApp class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LsgApp_H_
#define _LsgApp_H_


#pragma warning(disable : 4786)
#include <deque>

using namespace std;


namespace	LcLua
{
	
struct LsgApp
{
	static	ILcDev*		m_pDevSys	;
	static	void*		m_pDevD3D	;
	static	INT			Create(ILcDev* pDev);
	static	INT			Destroy();


	struct Tcommand
	{
		char sCmd[512];
		char sVal[512];

		Tcommand()
		{
			memset(sCmd, 0, sizeof sCmd);
			memset(sVal, 0, sizeof sVal);
		}

		Tcommand(char* Msg, char* Val)
		{
			memset(sCmd, 0, sizeof sCmd);
			memset(sVal, 0, sizeof sVal);

			strcpy(sCmd, Msg);
			strcpy(sVal, Val);
		}
	};


	static deque<Tcommand>*	m_vCommand;
	
	static INT	Lsc_SetCommand(lua_State *pL);									// Command
	static INT	Lsc_GetCommand(lua_State *pL);									// Command


	// Application Module
	static INT	Lsc_GetServerName(lua_State *pL);								// Selected Server Name
	static INT	Lsc_GetCharNameMan(lua_State *pL);								// Main Character Name1
	static INT	Lsc_GetCharNameLady(lua_State *pL);								// Main Character Name2

	static INT	Lsc_FieldLoad(lua_State *pL);									// Load Field
	static INT	Lsc_FieldViewRange(lua_State *pL);								// Max View Range
	static INT	Lsc_FieldCameraSet(lua_State *pL);								// Set Field Camera
	
	static INT	Lsc_SegmentLoad(lua_State *pL);									// Load Segment
	static INT	Lsc_SegmentFrameMove(lua_State *pL);							// FrameMove Segment
	static INT	Lsc_SegmentRender(lua_State *pL);								// Render Segment

	static INT	Lsc_MateModel(lua_State *pL);									// Model Load
	static INT	Lsc_MateActive(lua_State *pL);									// Mate Activation
	static INT	Lsc_MateRotation(lua_State *pL);								// Rotation
	static INT	Lsc_MatePosition(lua_State *pL);								// Position


	static INT	OpenLib(lua_State *pL);


	static INT	ContainerRestore();
	static INT	ContainerInvalidate();

	static	void*	m_pMainField;
	static	void*	m_pGameData;
	static	void*	m_pGameSgmnt;
};

}// namespace Lua

#endif

