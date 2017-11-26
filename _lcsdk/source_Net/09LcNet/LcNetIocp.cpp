// Implementation of the CLcNetIocp class.
//
////////////////////////////////////////////////////////////////////////////////

#if _MSC_VER >= 1200
#pragma warning(disable:4996)
#endif



#include <WINSOCK2.H>
#include <windows.h>
#include <stdio.h>

#include "LcNetUtil.h"
#include "ILcNet.h"

#include "LcNetUtil.h"
#include "LcNet.h"
#include "LcNetIocp.h"

#define SAFE_DELETE(p)			{	if(p){	delete (p);	(p) = NULL;		}	}
#define SAFE_DELETE_ARRAY(p)	{	if(p){	delete [] (p);	(p) = NULL;	}	}


CLcNetIocp::TlnOVERLAP::TlnOVERLAP()
{
	memset(&wsO, 0, sizeof wsO);
	memset(&wsB, 0, sizeof wsB);
	memset(&wsD, 0, sizeof wsD);

	wsB.buf = (char*)wsD;
	wsB.len = sizeof wsD;

	dTran	= 0;
	dFlag	= 0;
	dEnum	= 0;
	dEnbl	= TRUE;
}

void CLcNetIocp::TlnOVERLAP::Reset()
{
	memset(&wsO, 0, sizeof wsO);
	memset(&wsB, 0, sizeof wsB);
	memset(&wsD, 0, sizeof wsD);

	wsB.buf = (char*)wsD;
	wsB.len = sizeof wsD;

	dTran	= 0;
	dFlag	= 0;
	dEnum	= 0;
	dEnbl	= TRUE;
}




CLcNetIocp::LcNetIocpHost::LcNetIocpHost()
{
	scH = 0;
	memset(&sdH  , 0, sizeof(sdH)  );

	olRcv.Reset();
	olSnd.Reset();

	olRcv.dEnum = NETEVT_RECV;
	olSnd.dEnum = NETEVT_SEND;
}

CLcNetIocp::LcNetIocpHost::LcNetIocpHost(SOCKET _scH, const SOCKADDR_IN* _sdH)
{
	scH= _scH;
	memcpy(&sdH, _sdH, sizeof sdH);

	olRcv.Reset();
	olSnd.Reset();

	olRcv.dEnum = NETEVT_RECV;
	olSnd.dEnum = NETEVT_SEND;
}



void CLcNetIocp::LcNetIocpHost::SetSocket(SOCKET _scH)
{
	scH= _scH;
}


void CLcNetIocp::LcNetIocpHost::SetSocketAddr(const SOCKADDR_IN* _sdH)
{
	memcpy(&sdH, _sdH, sizeof sdH);
}


void CLcNetIocp::LcNetIocpHost::Destroy()
{
	LcNet_SocketClose(&scH);

	scH = 0;
	memset(&sdH  , 0, sizeof(sdH)  );

	olRcv.Reset();
	olSnd.Reset();

	olRcv.dEnum = NETEVT_RECV;
	olSnd.dEnum = NETEVT_SEND;
}




INT CLcNetIocp::LcNetIocpHost::AsyncRecv()
{
	INT		hr = -1;

	olRcv.Reset();
	olRcv.dEnum = NETEVT_RECV;
	
	hr = WSARecv(scH
		, &olRcv.wsB
		, 1
		, &olRcv.dTran
		, &olRcv.dFlag
		, (OVERLAPPED*)&olRcv
		, NULL);
	
	if(FAILED(LcNet_SocketErrorCheck(hr)))
		return -1;
	
	return 0;
}


INT CLcNetIocp::LcNetIocpHost::AsyncSend()
{
	INT		hr = -1;
	BYTE	sSnd[PCK_BUF_MAX_MSG]={0};
	WORD	iSize = 0;

	// 보내기가 가능한가?
//	if(FALSE == olSnd.dEnbl)
//		return 0;

	if(FAILED(rbSnd.PopFront(sSnd, &iSize)))
		return 0;

	olSnd.Reset();
	olSnd.dEnum = NETEVT_SEND;

	olSnd.wsB.len = iSize;
	memcpy(olSnd.wsD, sSnd, iSize);

	hr = WSASend(scH
				, &olSnd.wsB
				, 1
				, &olSnd.dTran
				, olSnd.dFlag
				, (OVERLAPPED*)&olSnd
				, NULL);

	// 보내기가 완료 될 때까지 다른 프로세스가 쓰지 못하도록 한다.
	olSnd.dEnbl	= FALSE;
	
	if(FAILED(LcNet_SocketErrorCheck(hr)))
		return -1;

	return 0;
}


void CLcNetIocp::LcNetIocpHost::RingBufPush(INT iRcv)
{
// 완료(수신)된 버퍼내용을 링버퍼에 복사.
	rbRcv.PushBack(olRcv.wsD, iRcv);
}


	
	
	
CLcNetIocp::CLcNetIocp()
{
	m_hIocp		= NULL;
	
	m_nCPU		= 0;
	m_hThWork	= NULL;
	m_dThWork	= NULL;
	m_nThWork	= 0;

	m_pScktMsg	= NULL;
	m_pScktNew	= NULL;
	m_pScktCls	= NULL;


	m_hThAccp	= NULL;
	m_dThAccp	= 0;
	m_nThAccp	= 0;
}


CLcNetIocp::~CLcNetIocp()
{
	Destroy();
}


INT CLcNetIocp::Create(void* p1, void* p2, void* p3, void* p4)
{
	char* sIp  = (char*)p1;
	char* sPort= (char*)p2;
	char* sPtcl= (char*)p3;
	char* sSvr = (char*)p4;

	memset(m_sIp, 0, sizeof m_sIp);
	memset(m_sPt, 0, sizeof m_sPt);

	if(sIp  )	strcpy(m_sIp, sIp  );
	if(sPort)	strcpy(m_sPt, sPort);
	
	
	if(0==_stricmp("TCP", sPtcl))
		m_PtcType	= NETPRT_TCP;												// Protocol type
	
	else if(0==_stricmp("UDP", sPtcl))
		m_PtcType	= NETPRT_UDP;												// Protocol type
	
	
	if(0==_stricmp("Client", sSvr))
	{
		m_HstType	= NETHST_CLIENT;											// Client
		return Connect(NULL, NULL);
	}
	else if(0==_stricmp("Server", sSvr))
	{
		m_HstType	= NETHST_SERVER;											// Server
		return Listen();
	}
	
	return -1;
}

void CLcNetIocp::Destroy()
{
	INT	i=0;
	INT iSizeH = m_vIoH.size();

	
	Close();


	for(i=0; i<m_nCPU; ++i)
	{
		PostQueuedCompletionStatus(m_hIocp, 0, NULL, NULL);
		Sleep(10);
	}

	// 쓰레드를 해제한다.
	LcNet_ThreadClose(&m_hThAccp);

	for(i=0; i<m_nCPU; ++i)
	{
		Sleep(10);
		LcNet_ThreadClose(&m_hThWork[i]);
	}


	for(INT it=0; it<iSizeH; ++it)
		delete m_vIoH[it];
	
	m_vIoH.clear();


	SAFE_DELETE(	m_pScktMsg	);
	SAFE_DELETE(	m_pScktNew	);
	SAFE_DELETE(	m_pScktCls	);
	
	SAFE_CLOSE_HANDLE(	m_hIocp		);

	for(i=0; i<m_nCPU; ++i)
	{
		SAFE_CLOSE_HANDLE(	m_hThWork[i]	);
	}

	SAFE_DELETE_ARRAY(	m_hThWork	);
	SAFE_DELETE_ARRAY(	m_dThWork	);
}




INT CLcNetIocp::FrameMove()
{
	if(FAILED(m_nThAccp) || FAILED(m_nThWork))
		return -1;

	SendAllData();
	
	return 0;
}



INT CLcNetIocp::Query(char* sCmd, void* pData)
{
	if(0==_stricmp("Get Client Number", sCmd))
	{
		INT nCnt =0;
		nCnt = m_vIoH.size();
		
		if(nCnt<=0)
			return -1;
		
		*((INT*)pData) = nCnt;
		return 0;
	}

	else if(0==_stricmp("Receive Socket Count", sCmd))
	{
		INT nCnt =0;
		nCnt = SocketRecvCount();

		if(nCnt<=0)
			return -1;

		*((INT*)pData) = nCnt;
		return 0;
	}

	else if(0==_stricmp("New Socket Count", sCmd))
	{
		INT nCnt =0;
		nCnt = SocketNewCount();
		
		if(nCnt<=0)
			return -1;
		
		*((INT*)pData) = nCnt;
		return 0;
	}

	else if(0==_stricmp("Close Socket Count", sCmd))
	{
		INT nCnt =0;
		nCnt = SocketCloseCount();
		
		if(nCnt<=0)
			return -1;
		
		*((INT*)pData) = nCnt;
		return 0;
	}

	else if(0==_stricmp("Get Receive Socket", sCmd))
	{
		SOCKET scH=0;
		
		if(FAILED(SocketRecvPop(&scH)))
			return -1;
		
		*((SOCKET*)pData) = scH;

		return 0;
	}

	else if(0==_stricmp("Get New Socket", sCmd))
	{
		SOCKET scH=0;
		
		if(FAILED(SocketNewPop(&scH)))
			return -1;

		*((SOCKET*)pData) = scH;

		return 0;
	}

	else if(0==_stricmp("Get Close Socket", sCmd))
	{
		SOCKET scH=0;
		
		if(FAILED(SocketClosePop(&scH)))
			return -1;

		*((SOCKET*)pData) = scH;

		return 0;
	}

	else if(0==_stricmp("Get Socket List", sCmd))
	{
		std::vector<SOCKET >* plsSocket = ((std::vector<SOCKET >*)pData);

		int iSize = m_vIoH.size();

		if(iSize<=0)
			return -1;

		for(int i=0; i<iSize; ++i)
			plsSocket->push_back(m_vIoH[i]->scH );

		return 0;
	}
	
	return -1;
}


INT CLcNetIocp::Close()
{
	// 소켓을 닫는다.
	LcNet_SocketClose(&m_scH);
	m_pIoH.scH = 0;

	return 0;
}


INT CLcNetIocp::Connect(char* sIp, char* sPort)
{
	if(sIp  ){	memset(m_sIp, 0, sizeof m_sIp);	strcpy(m_sIp, sIp  );	}
	if(sPort){	memset(m_sPt, 0, sizeof m_sPt);	strcpy(m_sPt, sPort);	}
	
	//2. 소켓 어드레스 세팅
	LcNet_SocketAddr(&m_sdH, m_sIp, m_sPt);
	
	
	//3. 소켓을 만든다.
	if(NETPRT_TCP==m_PtcType)
	{
		if(FAILED(LcNet_SocketTcpCreate(&m_scH, TRUE)))							// Overlapped TCP소켓을 만든다.
			return -1;
		
	}
	else if(NETPRT_UDP==m_PtcType)
	{
		if(FAILED(LcNet_SocketUdpCreate(&m_scH, TRUE)))							// UDP소켓을 만든다.
			return -1;
	}
	

	//4. 커넥션을 시도한다.
	if(SUCCEEDED(LcNet_SocketConnect(m_scH, &m_sdH)))
	{
		m_eNetSt = NETST_CONNECTED;
		
		// CPU의 숫자를 구하고, Completion Port(IOCP)생성.
		m_nCPU	= LcNet_GetSystemProcessNumber() * 2;
		m_hIocp = LcNet_IocpPortCreate((SOCKET)INVALID_HANDLE_VALUE, NULL, 0);

		if(!m_hIocp)
			return -1;

		// Nagle 정지.
		LcNet_SocketNaggleOff(m_scH);
		
		m_hThWork = new HANDLE[m_nCPU];											// Thread Handle
		m_dThWork = new DWORD [m_nCPU];											// Thread Handle

		// 입출력 완료를 대기 쓰레드: CPU 개수 * 2
		for(INT i=0; i<m_nCPU; ++i)
			m_hThWork[i] = LcNet_ThreadCreate(ThreadWork, this, 0, &m_dThWork[i]);

		//IOCP클라이언트에 소켓을 연결한다.
		m_pIoH.SetSocket(m_scH);
		m_pIoH.SetSocketAddr(&m_sdH);
		

		if(NULL == LcNet_IocpPortCreate(m_pIoH.scH, m_hIocp, &m_pIoH))
			return -1;

		m_pIoH.AsyncRecv();
		
		return 0;
	}
	
	return -1;
}



INT CLcNetIocp::Listen()
{
	//2. 소켓 어드레스 세팅, 소켓생성
	LcNet_SocketAddr(&m_sdH, m_sIp, m_sPt);

	
	//3. 소켓을 만든다.
	if(NETPRT_TCP==m_PtcType)
	{
		if(FAILED(LcNet_SocketTcpCreate(&m_scH)))								// TCP소켓을 만든다.
			return -1;
		
	}
	else if(NETPRT_UDP==m_PtcType)
	{
		if(FAILED(LcNet_SocketUdpCreate(&m_scH)))								// UDP소켓을 만든다.
			return -1;
	}
	
	//4. 소켓 바인딩
	if(FAILED(LcNet_SocketBind(m_scH, &m_sdH)))
		return -1;


	//5. CPU의 숫자를 구하고, Completion Port(IOCP)생성.
	m_nCPU	= LcNet_GetSystemProcessNumber() * 2;
	m_hIocp = LcNet_IocpPortCreate((SOCKET)INVALID_HANDLE_VALUE, NULL, 0);

	if(!m_hIocp)
		return -1;

	// 입/출력 패킷에 관련된 큐 설정
	m_pScktMsg = new TqueCrc<CLcNetIocp::TsckMsg >(65536);
	m_pScktNew = new TqueCrc<CLcNetIocp::TsckMsg >(65536);
	m_pScktCls = new TqueCrc<CLcNetIocp::TsckMsg >(65536);




	//6. Completion Port 에서 입출력 완료를 대기 하는 쓰레드를 CPU 개수 * 2 만큼 생성.
	m_hThWork = new HANDLE[m_nCPU];												// Thread Handle
	m_dThWork = new DWORD [m_nCPU];												// Thread Handle
	
	// IOCP에서는 송신/수신을 일꾼 쓰레드 하나에서 다 처리
	for(INT i=0; i<m_nCPU; ++i)
		m_hThWork[i] = LcNet_ThreadCreate(ThreadWork, this, 0, &m_dThWork[i]);

	
	//6. 소켓 리쓴
	if(FAILED(LcNet_SocketListen(m_scH)))
		return -1;

	// Accept용 쓰레드를 만든다.
	m_hThAccp = LcNet_ThreadCreate(ThreadAccp, this, 0, &m_dThAccp);

	if(NULL == m_hThAccp)
		return -1;

	
	return 0;
}




INT CLcNetIocp::Send(const char* pSrc, INT  iSnd, DWORD  dMsg, SOCKET  scH)
{
	BYTE	sBuf[PCK_BUF_MAX_MSG]={0};
	INT		iSize = 0;
	INT		hr = 0;
		
	if(0xFFFFFFFF==scH)
	{
		iSize	= LcNet_PacketEncode(sBuf, (BYTE*)pSrc, iSnd, dMsg);
		hr		= m_pIoH.rbSnd.PushBack(sBuf, iSize);

		if(FAILED(hr))
			return -1;
	}

	else
	{
		INT		nIdx = -1;
		INT		iN = m_vIoH.size();

		for(nIdx=0; nIdx<iN; ++nIdx)
		{
			LcNetIocpHost*	pHst = m_vIoH[nIdx];

			if(pHst->scH == scH)
				break;
		}

		if(nIdx>=iN)
			return -1;

		
		iSize	= LcNet_PacketEncode(sBuf, (BYTE*)pSrc, iSnd, dMsg);
		hr		= m_vIoH[nIdx]->rbSnd.PushBack(sBuf, iSize);
		
		if(FAILED(hr))
			return -1;
	}
	
	return 0;
}

INT CLcNetIocp::Recv(char* pDst, INT* iRcv, DWORD* dMsg, SOCKET* scH)
{
	BYTE	sBuf[PCK_BUF_MAX_MSG]={0};
	WORD	iSize = 0;
	INT		hr	= 0;

	*iRcv = 0;

	if(NULL==scH)
	{
		hr = m_pIoH.rbRcv.PopFront(sBuf, &iSize);

		if(FAILED(hr))
			return -1;

		if(0==iSize)
			return -1;

		*iRcv = LcNet_PacketDecode((BYTE*)pDst, dMsg, sBuf, iSize);
	}

	else
	{
		SOCKET	scT	 =*scH;	
		INT		nIdx = -1;
		INT		iN = m_vIoH.size();
		
		*iRcv = 0;

		itIocpH itHst = LcNet_Find_Socket(m_vIoH.begin(), m_vIoH.end(), scT);

		if(itHst == m_vIoH.end())
			return -1;

		INT hr = (*itHst)->rbRcv.PopFront(sBuf, &iSize);

		if(FAILED( hr ))
			return -1;
		
		if(0==iSize)
			return -1;
		
		*iRcv = LcNet_PacketDecode((BYTE*)pDst, dMsg, sBuf, iSize);
	}

	return 0;
}






INT CLcNetIocp::SendAllData()
{
	// 데이터 전송
	if(NETHST_CLIENT == m_HstType)
	{
		m_pIoH.AsyncSend();
	}
	else if(NETHST_SERVER == m_HstType)
	{
		INT iSizeH = m_vIoH.size();

		for(INT i=0;i<iSizeH; ++i)
		{
			CLcNetIocp::LcNetIocpHost* pIoH = m_vIoH[i];

			pIoH->AsyncSend();
		}
	}

	return 0;
}



DWORD CLcNetIocp::ProcWork(void* pParam)
{
	LcNetIocpHost*	pIoH	= NULL;								// 완료 통지 주소
	TlnOVERLAP*		pOL		= NULL;								// Overlapped주소 Send: olSnd, Recv: olRcv
	DWORD			dTran	= 0;								// 전송된 데이터 크기
	INT				hr		=-1;


	while(1)
	{
		pIoH	= NULL;
		pOL		= NULL;
		dTran	= 0;
		hr		=-1;

		hr = GetQueuedCompletionStatus(
										m_hIocp					// Completion Port
									,	&dTran					// 완료(처리) 바이트 수
									,	(LPDWORD)&pIoH			// pIoH 주소
									,	(LPOVERLAPPED*)&pOL		// pIoH의 olSnd or olRcv 주소
									,	INFINITE
									);

		// 완전한 입/출력
		if(0<hr && dTran && pIoH && pOL)
		{
			// 수신 Complete
			if(NETEVT_RECV == pOL->dEnum)
			{
				if(NETHST_CLIENT == m_HstType)
				{
					pIoH->RingBufPush(dTran);
					pIoH->AsyncRecv();
				}
				else if(NETHST_SERVER == m_HstType)
				{
					// 12. 받은 데이터를 링버퍼에 저장.
					pIoH->RingBufPush(dTran);

					SocketRecvPush(pIoH->scH);

					// 13. 다시 비동기 데이터 입력 요청.(필수)
					pIoH->AsyncRecv();
				}
			}

			// Send complete
			else if(NETEVT_SEND == pOL->dEnum)
			{ 
				// 입/출력이 가능하도록 설정한다.
				pOL->dEnbl	= TRUE;
//				printf("Send complete: %d\n", pIoH->scH);
			}
		}

			
		//Transferred 된 데이터가 0인 겻은 네트워크 에러이거나
		// 클라이언트 혹은 서버의 접속 종료일 때 발생
		else if(0 == dTran)
		{
			// 클라이언트의 경우 서버에서 접속을 끊거나 자신의 네트워크에서
			// 에러와 동일하게 최급
			if(NETHST_CLIENT == m_HstType)
			{
				m_eNetSt = NETST_CLOSE;
				hr = -1;
				break;
			}

			else if(NETHST_SERVER == m_HstType)
			{

				// 클라이언트 접속 종료
				if(pIoH)
				{
					SOCKET scT = pIoH->scH;
					
					//접속을 끊고				
					pIoH->Destroy();

					// 리스트에서 제거한다.
					itIocpH itIoH = m_vIoH.begin();

					for(; itIoH != m_vIoH.end(); ++itIoH)
					{
						if(pIoH == (*itIoH) )
							break;
					}
					
					if(itIoH != m_vIoH.end())
					{
						delete pIoH;
						m_vIoH.erase(itIoH);

						printf("Close: %d, Remain: %d\n", scT, m_vIoH.size());
						// 삭제할 소켓리스트에 등록
						SocketClosePush(scT);
					}
				}

				// 자기 자신의 에러
				else
				{
					hr = -1;
					break;
				}
			}
		}

		// 기타 에러
		else
		{
			//무조건 빠져 나간다.
			hr = -1;
			break;
		}

		
	}


	m_nThWork	= -1;

	return hr;
}





DWORD CLcNetIocp::ProcAccp(void* pParam)
{
	INT		hr=-1;
	INT		i=0;

	while(1)
	{
		// Accept
		SOCKET			scH;
		SOCKADDR_IN		sdH;
		hr = LcNet_SocketAccept(&scH, &sdH, m_scH);

		if(FAILED(hr))
		{
			hr = -1;
			break;
		}

		if(scH)
		{
			printf("New Client socket:%d\n", scH);

			LcNetIocpHost* pIoH = new LcNetIocpHost(scH, &sdH);
			m_vIoH.push_back(pIoH);

			LcNet_IocpPortCreate(pIoH->scH, m_hIocp, pIoH);
			pIoH->AsyncRecv();

			// 생성 소켓리스트에 등록
			SocketNewPush(scH);

			// 클라이언트에 접속 메시지 전달
			int		iLenSnd=0;
			char	sBufSnd[64]={0};
			DWORD	dMsg = 0x01;
			sprintf(sBufSnd, "SocketId:%d", scH);
			iLenSnd=strlen(sBufSnd);
			this->Send(sBufSnd, iLenSnd, dMsg, scH);
		}
	}

	m_nThAccp = -1;

	return hr;
}



void CLcNetIocp::SocketRecvPush(SOCKET scH)
{
	CLcNetIocp::TsckMsg msg(scH);
	m_pScktMsg->PushBack(&msg);
}

INT CLcNetIocp::SocketRecvPop(SOCKET* scH)
{
	if(m_pScktMsg->IsEmpty())
		return -1;

	CLcNetIocp::TsckMsg msg;

	// 메시지 버퍼에서 소켓을 꺼내온다.
	m_pScktMsg->PopFront(&msg);
	*scH = msg.nScH;

	return m_pScktMsg->GetSize();
}

INT CLcNetIocp::SocketRecvCount()
{
	return m_pScktMsg->GetSize();
}



void CLcNetIocp::SocketNewPush(SOCKET scH)
{
	CLcNetIocp::TsckMsg msg(scH);
	m_pScktNew->PushBack(&msg);
}

INT CLcNetIocp::SocketNewPop(SOCKET* scH)
{
	if(m_pScktNew->IsEmpty())
		return -1;

	CLcNetIocp::TsckMsg msg;

	// New 소켓 버퍼에서 소켓을 꺼내온다.
	m_pScktNew->PopFront(&msg);
	*scH = msg.nScH;

	return m_pScktNew->GetSize();
}

INT CLcNetIocp::SocketNewCount()
{
	return m_pScktNew->GetSize();
}



void CLcNetIocp::SocketClosePush(SOCKET scH)
{
	CLcNetIocp::TsckMsg msg(scH);
	m_pScktCls->PushBack(&msg);
}

INT CLcNetIocp::SocketClosePop(SOCKET* scH)
{
	if(m_pScktCls->IsEmpty())
		return -1;

	CLcNetIocp::TsckMsg msg;

	// Close 소 버퍼에서 소켓을 꺼내온다.
	m_pScktCls->PopFront(&msg);
	*scH = msg.nScH;

	return m_pScktCls->GetSize();
}

INT CLcNetIocp::SocketCloseCount()
{
	return m_pScktCls->GetSize();
}

