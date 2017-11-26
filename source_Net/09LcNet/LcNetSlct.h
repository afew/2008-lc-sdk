// Interface for the CLcNetSlct class.
// Network I/O Select
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcNetSlct_H_
#define _LcNetSlct_H_


class CLcNetSlct : public CLcNet
{
public:
	struct LcNetSlctHost
	{
		TRingBuf	rbSnd;														// 접속 클라이언트 Send용 링버퍼
		TRingBuf	rbRcv;														// 접속 클라이언트 Receive용 링버퍼
	};

protected:
	TRingBuf		m_rbSnd;													// Send용 링버퍼
	TRingBuf		m_rbRcv;													// Receive용 링버퍼

	FD_SET			m_rmF;														// 소켓 FD_SET	rmE: Remote Host FD
	LcNetSlctHost	m_rmH[FD_SETSIZE];


	HANDLE			m_hThRecv;													// Recv Thread Handle
	DWORD			m_dThRecv;													// Recv Thread Id
	INT				m_nThRecv;													// Recv Thread State

	HANDLE			m_hThSend;													// Send Thread Handle														// Thread Handle
	DWORD			m_dThSend;													// Send Thread Id
	INT				m_nThSend;													// Send Thread State

	HANDLE			m_hThAccp;													// Accept Thread Handle
	DWORD			m_dThAccp;													// Accept Thread Id
	INT				m_nThAccp;													// Accept Thread State


public:
	CLcNetSlct();
	virtual ~CLcNetSlct();
	
	virtual INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0);
	virtual void	Destroy();
	virtual INT		FrameMove();
	virtual INT		Query(char* sCmd, void* pData);

	virtual INT		Close();
	virtual INT		Listen();
	virtual INT		Connect(char* sIp=NULL, char* sPort=NULL);

	virtual INT		Send(const char* pSrc, INT  iSnd, DWORD  dMsg, SOCKET  scH=0xFFFFFFFF);
	virtual INT		Recv(      char* pDst, INT* iRcv, DWORD* dMsg, SOCKET* scH=0);

protected:
	INT		FrameMoveSvr();			// For Server
	INT		FrameMoveCln();			// For Client

	INT		SendAllData();			// Send Data

	void	Fd_Set(SOCKET fd, FD_SET* fds);
	void	Fd_Clr(SOCKET fd, FD_SET* fds);
	void	Fd_Zero(FD_SET* fds);
};

#endif