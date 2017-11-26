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

	// �����Ⱑ �����Ѱ�?
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

	// �����Ⱑ �Ϸ� �� ������ �ٸ� ���μ����� ���� ���ϵ��� �Ѵ�.
	olSnd.dEnbl	= FALSE;
	
	if(FAILED(LcNet_SocketErrorCheck(hr)))
		return -1;

	return 0;
}


void CLcNetIocp::LcNetIocpHost::RingBufPush(INT iRcv)
{
// �Ϸ�(����)�� ���۳����� �����ۿ� ����.
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

	// �����带 �����Ѵ�.
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
	// ������ �ݴ´�.
	LcNet_SocketClose(&m_scH);
	m_pIoH.scH = 0;

	return 0;
}


INT CLcNetIocp::Connect(char* sIp, char* sPort)
{
	if(sIp  ){	memset(m_sIp, 0, sizeof m_sIp);	strcpy(m_sIp, sIp  );	}
	if(sPort){	memset(m_sPt, 0, sizeof m_sPt);	strcpy(m_sPt, sPort);	}
	
	//2. ���� ��巹�� ����
	LcNet_SocketAddr(&m_sdH, m_sIp, m_sPt);
	
	
	//3. ������ �����.
	if(NETPRT_TCP==m_PtcType)
	{
		if(FAILED(LcNet_SocketTcpCreate(&m_scH, TRUE)))							// Overlapped TCP������ �����.
			return -1;
		
	}
	else if(NETPRT_UDP==m_PtcType)
	{
		if(FAILED(LcNet_SocketUdpCreate(&m_scH, TRUE)))							// UDP������ �����.
			return -1;
	}
	

	//4. Ŀ�ؼ��� �õ��Ѵ�.
	if(SUCCEEDED(LcNet_SocketConnect(m_scH, &m_sdH)))
	{
		m_eNetSt = NETST_CONNECTED;
		
		// CPU�� ���ڸ� ���ϰ�, Completion Port(IOCP)����.
		m_nCPU	= LcNet_GetSystemProcessNumber() * 2;
		m_hIocp = LcNet_IocpPortCreate((SOCKET)INVALID_HANDLE_VALUE, NULL, 0);

		if(!m_hIocp)
			return -1;

		// Nagle ����.
		LcNet_SocketNaggleOff(m_scH);
		
		m_hThWork = new HANDLE[m_nCPU];											// Thread Handle
		m_dThWork = new DWORD [m_nCPU];											// Thread Handle

		// ����� �ϷḦ ��� ������: CPU ���� * 2
		for(INT i=0; i<m_nCPU; ++i)
			m_hThWork[i] = LcNet_ThreadCreate(ThreadWork, this, 0, &m_dThWork[i]);

		//IOCPŬ���̾�Ʈ�� ������ �����Ѵ�.
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
	//2. ���� ��巹�� ����, ���ϻ���
	LcNet_SocketAddr(&m_sdH, m_sIp, m_sPt);

	
	//3. ������ �����.
	if(NETPRT_TCP==m_PtcType)
	{
		if(FAILED(LcNet_SocketTcpCreate(&m_scH)))								// TCP������ �����.
			return -1;
		
	}
	else if(NETPRT_UDP==m_PtcType)
	{
		if(FAILED(LcNet_SocketUdpCreate(&m_scH)))								// UDP������ �����.
			return -1;
	}
	
	//4. ���� ���ε�
	if(FAILED(LcNet_SocketBind(m_scH, &m_sdH)))
		return -1;


	//5. CPU�� ���ڸ� ���ϰ�, Completion Port(IOCP)����.
	m_nCPU	= LcNet_GetSystemProcessNumber() * 2;
	m_hIocp = LcNet_IocpPortCreate((SOCKET)INVALID_HANDLE_VALUE, NULL, 0);

	if(!m_hIocp)
		return -1;

	// ��/��� ��Ŷ�� ���õ� ť ����
	m_pScktMsg = new TqueCrc<CLcNetIocp::TsckMsg >(65536);
	m_pScktNew = new TqueCrc<CLcNetIocp::TsckMsg >(65536);
	m_pScktCls = new TqueCrc<CLcNetIocp::TsckMsg >(65536);




	//6. Completion Port ���� ����� �ϷḦ ��� �ϴ� �����带 CPU ���� * 2 ��ŭ ����.
	m_hThWork = new HANDLE[m_nCPU];												// Thread Handle
	m_dThWork = new DWORD [m_nCPU];												// Thread Handle
	
	// IOCP������ �۽�/������ �ϲ� ������ �ϳ����� �� ó��
	for(INT i=0; i<m_nCPU; ++i)
		m_hThWork[i] = LcNet_ThreadCreate(ThreadWork, this, 0, &m_dThWork[i]);

	
	//6. ���� ����
	if(FAILED(LcNet_SocketListen(m_scH)))
		return -1;

	// Accept�� �����带 �����.
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
	// ������ ����
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
	LcNetIocpHost*	pIoH	= NULL;								// �Ϸ� ���� �ּ�
	TlnOVERLAP*		pOL		= NULL;								// Overlapped�ּ� Send: olSnd, Recv: olRcv
	DWORD			dTran	= 0;								// ���۵� ������ ũ��
	INT				hr		=-1;


	while(1)
	{
		pIoH	= NULL;
		pOL		= NULL;
		dTran	= 0;
		hr		=-1;

		hr = GetQueuedCompletionStatus(
										m_hIocp					// Completion Port
									,	&dTran					// �Ϸ�(ó��) ����Ʈ ��
									,	(LPDWORD)&pIoH			// pIoH �ּ�
									,	(LPOVERLAPPED*)&pOL		// pIoH�� olSnd or olRcv �ּ�
									,	INFINITE
									);

		// ������ ��/���
		if(0<hr && dTran && pIoH && pOL)
		{
			// ���� Complete
			if(NETEVT_RECV == pOL->dEnum)
			{
				if(NETHST_CLIENT == m_HstType)
				{
					pIoH->RingBufPush(dTran);
					pIoH->AsyncRecv();
				}
				else if(NETHST_SERVER == m_HstType)
				{
					// 12. ���� �����͸� �����ۿ� ����.
					pIoH->RingBufPush(dTran);

					SocketRecvPush(pIoH->scH);

					// 13. �ٽ� �񵿱� ������ �Է� ��û.(�ʼ�)
					pIoH->AsyncRecv();
				}
			}

			// Send complete
			else if(NETEVT_SEND == pOL->dEnum)
			{ 
				// ��/����� �����ϵ��� �����Ѵ�.
				pOL->dEnbl	= TRUE;
//				printf("Send complete: %d\n", pIoH->scH);
			}
		}

			
		//Transferred �� �����Ͱ� 0�� ���� ��Ʈ��ũ �����̰ų�
		// Ŭ���̾�Ʈ Ȥ�� ������ ���� ������ �� �߻�
		else if(0 == dTran)
		{
			// Ŭ���̾�Ʈ�� ��� �������� ������ ���ų� �ڽ��� ��Ʈ��ũ����
			// ������ �����ϰ� �ֱ�
			if(NETHST_CLIENT == m_HstType)
			{
				m_eNetSt = NETST_CLOSE;
				hr = -1;
				break;
			}

			else if(NETHST_SERVER == m_HstType)
			{

				// Ŭ���̾�Ʈ ���� ����
				if(pIoH)
				{
					SOCKET scT = pIoH->scH;
					
					//������ ����				
					pIoH->Destroy();

					// ����Ʈ���� �����Ѵ�.
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
						// ������ ���ϸ���Ʈ�� ���
						SocketClosePush(scT);
					}
				}

				// �ڱ� �ڽ��� ����
				else
				{
					hr = -1;
					break;
				}
			}
		}

		// ��Ÿ ����
		else
		{
			//������ ���� ������.
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

			// ���� ���ϸ���Ʈ�� ���
			SocketNewPush(scH);

			// Ŭ���̾�Ʈ�� ���� �޽��� ����
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

	// �޽��� ���ۿ��� ������ �����´�.
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

	// New ���� ���ۿ��� ������ �����´�.
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

	// Close �� ���ۿ��� ������ �����´�.
	m_pScktCls->PopFront(&msg);
	*scH = msg.nScH;

	return m_pScktCls->GetSize();
}

INT CLcNetIocp::SocketCloseCount()
{
	return m_pScktCls->GetSize();
}

