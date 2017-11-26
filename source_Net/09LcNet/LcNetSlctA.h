// Interface for the CLcNetSlctA class.
// Network I/O Async Select
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcNetSlctA_H_
#define _LcNetSlctA_H_


class CLcNetSlctA : public CLcNet
{
public:
	struct LcNetSlctHost
	{
		SOCKET		scH;														// 접속 클라이언트 소켓
		SOCKADDR_IN sdH;														// 접속 클라이언트 어드레스

		TRingBuf	rbSnd;														// 접속 클라이언트 Send용 링버퍼
		TRingBuf	rbRcv;														// 접속 클라이언트 Receive용 링버퍼

		LcNetSlctHost();
	};

protected:
	TRingBuf		m_rbSnd;													// Send용 링버퍼
	TRingBuf		m_rbRcv;													// Receive용 링버퍼
	
	INT				m_iNsck;													// 접속 소켓 수
	LcNetSlctHost	m_rmH[WSA_MAXIMUM_WAIT_EVENTS];								// 클라이언트 정보		rmH: Remote Host



	HANDLE			m_hThSend;													// Send Thread Handle														// Thread Handle
	DWORD			m_dThSend;													// Send Thread Id
	INT				m_nThSend;													// Send Thread State
	


public:
	CLcNetSlctA();
	virtual ~CLcNetSlctA();
	
	virtual INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0);
	virtual void	Destroy();
	virtual INT		FrameMove();
	virtual INT		Query(char* sCmd, void* pData);

	virtual INT		Close();
	virtual INT		Listen();
	virtual INT		Connect(char* sIp=NULL, char* sPort=NULL);

	virtual INT		Send(const char* pSrc, INT  iSnd, DWORD  dMsg, SOCKET  scH=0xFFFFFFFF);
	virtual INT		Recv(      char* pDst, INT* iRcv, DWORD* dMsg, SOCKET* scH=0);

	virtual DWORD	ProcSend(void* pParam);

protected:
	HWND			m_hWnd ;													// Window Handle
	DWORD			m_dwMsg;													// User Define Message

	LRESULT			MsgProc(HWND, UINT, WPARAM, LPARAM);
	
	INT		SendAllData();			// Send Data

	void	Ev_Set(SOCKET scH, SOCKADDR_IN* sdH);
	void	Ev_Clr(SOCKET fd);
	void	Ev_Zero();
};

#endif
