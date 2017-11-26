// Implementation of the LscNet class.
//
////////////////////////////////////////////////////////////////////////////////


#include <windows.h>

#include <Lua/lua.h>
#include <Lua/lualib.h>
#include <Lua/lauxlib.h>

#include <Lc/ILcDev.h>
#include <LcNet/ILcNet.h>

#include "LscNet.h"


namespace	LcLua
{
	
ILcDev* LscNet::m_pDevSys	= NULL;
ILcNet*	LscNet::m_pNet		= NULL;


INT LscNet::Create(ILcDev* pDev)
{
	LscNet::m_pDevSys	= pDev;

	return 0;
}


static const luaL_reg regLscNet[] =
{
	// Network
	{"Create"	,	LscNet::Lsc_NetCreate		},
	{"Destroy"	,	LscNet::Lsc_NetDestroy		},
	{"Send"		,	LscNet::Lsc_NetSend			},
	{"Recv"		,	LscNet::Lsc_NetRecv			},
	{"PacketCnt",	LscNet::Lsc_NetPacketCnt	},

	{NULL, NULL},
};


INT LscNet::OpenLib(lua_State *pL)
{
	luaL_openlib(pL, "Lnet", regLscNet, 0);
	return 1;
}


// Implementations La API.
//
////////////////////////////////////////////////////////////////////////////////


	


INT LscNet::Lsc_NetCreate(lua_State *pL)
{
	INT		hr=-1;
	char	sIp[64]={0};
	char	sPt[32]={0};
	
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}


	strcpy(sIp, (char*)lua_tostring(pL, 1));
	strcpy(sPt, (char*)lua_tostring(pL, 2));


	ILcNet*	pNetwork	= NULL	;
	m_pDevSys->Query("Get Network", &pNetwork);

	if(pNetwork)
		delete pNetwork;


	HWND	hWnd	= (HWND)LcDev_GetHwnd(&m_pDevSys);
	DWORD	dMsgId	= WM_USER+100;

	hr = LcNet_CreateTcpClientEx("Async Select", &pNetwork, sIp, sPt, &hWnd, &dMsgId);

	if(FAILED(hr))
	{
		lua_pushnumber(pL, hr);
		return 1;
	}

	LscNet::m_pNet	= pNetwork;

	hr = LcDev_SetUserNetwork(&m_pDevSys, pNetwork);
	
	lua_pushnumber(pL, hr);
	return 1;
}


INT LscNet::Lsc_NetDestroy(lua_State *pL)
{
	if(LscNet::m_pNet)
	{
		delete LscNet::m_pNet;
		LscNet::m_pNet = NULL;

		LcDev_SetUserNetwork(&m_pDevSys, NULL);
	}
	
	return 0;
}



INT LscNet::Lsc_NetSend(lua_State *pL)
{
	INT		nMsg = (INT)lua_tonumber(pL, 1);
	char*	s	 = (char*)lua_tostring(pL, 2);
	
	INT		iSize= strlen(s);
	
	//	BYTE	sBuf[PCK_MAX_TMP]={0};
	//	memcpy(sBuf, s, iSize);
	//
	//	GMAIN->NetworkSend(sBuf, nMsg, iSize);
	
	return 0;
}



INT LscNet::Lsc_NetRecv(lua_State *pL)
{
	//	BYTE	sBuf[PCK_MAX_TMP]={0};
	//	INT		iSize = 0;
	//	INT		nMsg  = 0;
	//	INT		iRemain=0;
	//
	//	iRemain = GMAIN->NetworkRecv(sBuf, &nMsg, &iSize);
	//
	//	lua_pushnumber(pL, nMsg);
	//	lua_pushstring(pL, (char*)sBuf);
	
	return 2;
}


INT LscNet::Lsc_NetPacketCnt(lua_State *pL)
{
	//	INT		iRemain= GMAIN->NetworkPacketCnt();
	//
	//	lua_pushnumber(pL, iRemain);
	
	return 1;
}


}// namespace Lua