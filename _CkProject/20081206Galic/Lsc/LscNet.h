// Interface for the LscNet class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LscNet_H_
#define _LscNet_H_


namespace	LcLua
{

struct LscNet
{
	static	ILcDev*		m_pDevSys	;
	static	ILcNet*		m_pNet		;											// Network Pointer
	static	INT			Create(ILcDev* pDev);


	// Network Module
	static INT	Lsc_NetCreate(lua_State *pL);									// Network Create
	static INT	Lsc_NetDestroy(lua_State *pL);									// Network Destroy
	static INT	Lsc_NetSend(lua_State *pL);										// Network Send
	static INT	Lsc_NetRecv(lua_State *pL);										// Network Recv
	static INT	Lsc_NetPacketCnt(lua_State *pL);								// Network Packet count	

	static INT	OpenLib(lua_State *pL);
};

}// namespace Lua

#endif
