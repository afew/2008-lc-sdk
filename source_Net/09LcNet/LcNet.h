// Interface for the CLcNet class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcNet_H_
#define _LcNet_H_


class CLcNet : public ILcNet
{
public:
	enum ELcNetPrtcl
	{
		NETPRT_NONE	= 0,				// NONE
		NETPRT_TCP	= 1,				// TCP
		NETPRT_UDP	= 2,				// UDP
	};

	enum ELcNetHstType
	{
		NETHST_NONE		=0,				// NONE
		NETHST_CLIENT	=1,				// Client
		NETHST_SERVER	=2,				// Server
	};

	enum ELcNetEventType
	{
		NETEVT_NONE		=0,				// NONE
		NETEVT_SEND		=1,				// Send Event
		NETEVT_RECV		=2,				// Receive
		NETEVT_ACCP		=4,				// Accept
		NETEVT_CONN		=8,				// Connect
		NETEVT_CLOSE	=16,			// Close
	};

protected:
	SOCKET			m_scH;				// Host socket
	SOCKADDR_IN		m_sdH;				// Host Address
	char			m_sIp[32];			// IP
	char			m_sPt[16];			// Port

	INT				m_PtcType;			// Protocol type
	INT				m_HstType;			// Client or Server?

	INT				m_eNetSt;			// Network State

public:
	CLcNet();
	virtual ~CLcNet();
	
	virtual INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0);
	virtual void	Destroy();
	virtual INT		FrameMove();
	virtual INT		Query(char* sCmd, void* pData);

	virtual INT		Close();
	virtual INT		Listen();
	virtual INT		Connect(char* sIp=NULL, char* sPort=NULL);

	virtual INT		Send(const char* pSrc, INT  iSnd, DWORD  dMsg, SOCKET  scH=0xFFFFFFFF);
	virtual INT		Recv(      char* pDst, INT* iRcv, DWORD* dMsg, SOCKET* scH=0);

	virtual INT		NetState();
	virtual LRESULT	MsgProc(HWND, UINT, WPARAM, LPARAM);						// Async Select용

public:

	static DWORD WINAPI ThreadRecv(void* pParam);		// Receive 쓰레드
	static DWORD WINAPI ThreadSend(void* pParam);		// Send 쓰레드
	static DWORD WINAPI ThreadAccp(void* pParam);		// Accept 쓰레드
	static DWORD WINAPI ThreadWork(void* pParam);		// Work 쓰레드

	virtual DWORD	ProcRecv(void* pParam);
	virtual DWORD	ProcSend(void* pParam);
	virtual DWORD	ProcAccp(void* pParam);
	virtual DWORD	ProcWork(void* pParam);
};

#endif
