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
		TlcWSAData		wsD[PCK_BUF_MAX_MSG];									// Real Data. wsBuf.buf�� �����

		DWORD			dTran;													// For Transffer.
		DWORD			dFlag;													// Send or Receive Flag.
		DWORD			dEnum;													// ��/����� ����: for Send: NETEVT_SEND, for Recv: NETEVT_RECV
		DWORD			dEnbl;													// ��/����� �����Ѱ�? Send���� �ش�.

		TlnOVERLAP();
		void	Reset();
	};

	struct LcNetIocpHost
	{
		SOCKET		scH;														// ���� Ŭ���̾�Ʈ ����
		SOCKADDR_IN sdH;														// ���� Ŭ���̾�Ʈ ��巹��

		TlnOVERLAP	olRcv;
		TlnOVERLAP	olSnd;

		TRingBuf	rbRcv;														// ���� Ŭ���̾�Ʈ Receive�� ������
		TRingBuf	rbSnd;														// ���� Ŭ���̾�Ʈ Send�� ������

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
	LcNetIocpHost	m_pIoH;														// Send/Receive�� Io, ������ ��� Listen ����
	lsIocpH			m_vIoH;														// ���� Ŭ���̾�Ʈ
	
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
	TqueCrc<TsckMsg >*	m_pScktMsg;												// �޽��� �޴� ���� ����Ʈ
	TqueCrc<TsckMsg >*	m_pScktNew;												// ������ ���� ����Ʈ
	TqueCrc<TsckMsg >*	m_pScktCls;												// ���� ���� ����Ʈ

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
