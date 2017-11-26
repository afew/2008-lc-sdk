// Interface for the CLcNetSlctE class.
// Network I/O Event Select
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcNetSlctE_H_
#define _LcNetSlctE_H_


class CLcNetSlctE : public CLcNet
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
	WSAEVENT		m_rmE[WSA_MAXIMUM_WAIT_EVENTS];								// 클라이언트 이벤트	rmE: Remote Host Event
	LcNetSlctHost	m_rmH[WSA_MAXIMUM_WAIT_EVENTS];								// 클라이언트 정보		rmH: Remote Host


	HANDLE			m_hThRecv;													// Recv Thread Handle
	DWORD			m_dThRecv;													// Recv Thread Id
	INT				m_nThRecv;													// Recv Thread State

	HANDLE			m_hThSend;													// Send Thread Handle														// Thread Handle
	DWORD			m_dThSend;													// Send Thread Id
	INT				m_nThSend;													// Send Thread State


public:
	CLcNetSlctE();
	virtual ~CLcNetSlctE();
	
	virtual INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0);
	virtual void	Destroy();
	virtual INT		FrameMove();
	virtual INT		Query(char* sCmd, void* pData);

	virtual INT		Close();
	virtual INT		Listen();
	virtual INT		Connect(char* sIp=NULL, char* sPort=NULL);

	virtual INT		Send(const char* pSrc, INT  iSnd, DWORD  dMsg, SOCKET  scH=0xFFFFFFFF);
	virtual INT		Recv(      char* pDst, INT* iRcv, DWORD* dMsg, SOCKET* scH=0);

	virtual DWORD	ProcRecv(void* pParam);
	virtual DWORD	ProcSend(void* pParam);

protected:
	WSAEVENT	m_hEvnt;								// Event Handle
	DWORD		m_dEvnt;								// Event Kind

	INT		SendAllData();			// Send Data

	void	Ev_Set(SOCKET scH, SOCKADDR_IN* sdH, HANDLE hEvt);
	void	Ev_Clr(SOCKET fd);
	void	Ev_Zero();
};

#endif
