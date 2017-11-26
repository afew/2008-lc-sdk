// Interface for the CLcNetIocp class.
// Network I/O IOCP
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcNetIocp_H_
#define _LcNetIocp_H_

#pragma warning(disable : 4786)
#include <vector>

class CLcNetIocp : public CLcNet
{
public:
	struct TsckMsg
	{
		SOCKET		nScH;

		TsckMsg():nScH(0)	{}
		TsckMsg(SOCKET _scH):nScH(_scH){}
	};

	typedef	BYTE	TlcWSAData;

	struct TlnOVERLAP
	{
		WSAOVERLAPPED	wsO;
		WSABUF			wsB;
		TlcWSAData		wsD[PCK_BUF_MAX_MSG];									// Real Data. wsBuf.buf와 연결됨

		DWORD			dTran;													// For Transffer.
		DWORD			dFlag;													// Send or Receive Flag.
		DWORD			dEnum;													// 입/출력의 종류: for Send: NETEVT_SEND, for Recv: NETEVT_RECV
		DWORD			dEnbl;													// 입/출력이 가능한가? Send에만 해당.

		TlnOVERLAP();
		void	Reset();
	};

	struct LcNetIocpHost
	{
		SOCKET		scH;														// 접속 클라이언트 소켓
		SOCKADDR_IN sdH;														// 접속 클라이언트 어드레스

		TlnOVERLAP	olRcv;
		TlnOVERLAP	olSnd;

		TRingBuf	rbRcv;														// 접속 클라이언트 Receive용 링버퍼
		TRingBuf	rbSnd;														// 접속 클라이언트 Send용 링버퍼

		LcNetIocpHost();
		LcNetIocpHost(SOCKET _scH, const SOCKADDR_IN* _sdH);

		
		void	SetSocket(SOCKET _scH);
		void	SetSocketAddr(const SOCKADDR_IN* _sdH);

		void	Destroy();
		INT		AsyncRecv();
		INT		AsyncSend();
		void	RingBufPush(INT iRcv);
	};

typedef	std::vector<LcNetIocpHost* >	lsIocpH;
typedef	lsIocpH::iterator				itIocpH;

protected:
	LcNetIocpHost	m_pIoH;														// Send/Receive용 Io, 서버일 경우 Listen 소켓
	lsIocpH			m_vIoH;														// 접속 클라이언트
	
	HANDLE			m_hIocp;													// IOCP Handle

	INT				m_nCPU;														// CPU Number
	HANDLE*			m_hThWork;													// Work Thread Handle
	DWORD*			m_dThWork;													// Work Thread Value
	INT				m_nThWork;													// Work Thread State

	HANDLE			m_hThAccp;													// Accept Thread Handle
	DWORD			m_dThAccp;													// Accept Thread Id
	INT				m_nThAccp;													// Accept Thread State
	

public:
	CLcNetIocp();
	virtual ~CLcNetIocp();
	
	virtual INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0);
	virtual void	Destroy();
	virtual INT		FrameMove();
	virtual INT		Query(char* sCmd, void* pData);

	virtual INT		Close();
	virtual INT		Listen();
	virtual INT		Connect(char* sIp=NULL, char* sPort=NULL);

	virtual INT		Send(const char* pSrc, INT  iSnd, DWORD  dMsg, SOCKET  scH=0xFFFFFFFF);
	virtual INT		Recv(      char* pDst, INT* iRcv, DWORD* dMsg, SOCKET* scH=0);

	virtual DWORD	ProcAccp(void* pParam);
	virtual DWORD	ProcWork(void* pParam);

protected:
	TqueCrc<TsckMsg >*	m_pScktMsg;												// 메시지 받는 소켓 리스트
	TqueCrc<TsckMsg >*	m_pScktNew;												// 생성한 소켓 리스트
	TqueCrc<TsckMsg >*	m_pScktCls;												// 닫은 소켓 리스트

	INT				SendAllData();												// Send Data

	void			SocketRecvPush(SOCKET scH);
	INT				SocketRecvPop(SOCKET* scH);
	INT				SocketRecvCount();

	void			SocketNewPush(SOCKET scH);
	INT				SocketNewPop(SOCKET* scH);
	INT				SocketNewCount();

	void			SocketClosePush(SOCKET scH);
	INT				SocketClosePop(SOCKET* scH);
	INT				SocketCloseCount();
};

#endif
