// Implementation of the CLcNetSlctE class.
//
////////////////////////////////////////////////////////////////////////////////

#if _MSC_VER >= 1200
#pragma warning(disable : 4996)
#endif

#include <WINSOCK2.H>
#include <windows.h>
#include <stdio.h>

#include "LcNetUtil.h"
#include "ILcNet.h"

#include "LcNetUtil.h"
#include "LcNet.h"
#include "LcNetSlctE.h"



CLcNetSlctE::LcNetSlctHost::LcNetSlctHost()
{
	scH = 0;
	memset(&sdH, 0, sizeof(sdH));
}


CLcNetSlctE::CLcNetSlctE()
{
	m_iNsck	= 0;
	memset(m_rmE, 0, sizeof m_rmE);

	m_hThSend	= NULL;
	m_dThSend	= 0;
	m_nThSend	= 0;

	m_hThRecv	= NULL;
	m_dThRecv	= 0;
	m_nThRecv	= 0;
}


CLcNetSlctE::~CLcNetSlctE()
{
	Destroy();
}


INT CLcNetSlctE::Create(void* p1, void* p2, void* p3, void* p4)
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

void CLcNetSlctE::Destroy()
{
	Close();

	SetEvent( m_rmE[0] );
	
	SAFE_CLOSE_HANDLE(	m_hThRecv	);
	Sleep(100);
	SAFE_CLOSE_HANDLE(	m_hThSend	);

	for(int i=0; i<m_iNsck; ++i)
	{
		SAFE_CLOSE_HANDLE(	m_rmE[i]	);
	}
}




INT CLcNetSlctE::FrameMove()
{
	if( FAILED(m_nThRecv) || FAILED(m_nThSend))
		return -1;

	LcNet_ThreadResume(&m_hThSend);

	return 0;
}



INT CLcNetSlctE::Query(char* sCmd, void* pData)
{
	if(0==_stricmp("Get Client Number", sCmd))
	{
		*((INT*)pData) = m_iNsck-1;
		return 0;
	}
	
	return -1;
}




INT CLcNetSlctE::Connect(char* sIp, char* sPort)
{
	if(sIp  ){	memset(m_sIp, 0, sizeof m_sIp);	strcpy(m_sIp, sIp  );	}
	if(sPort){	memset(m_sPt, 0, sizeof m_sPt);	strcpy(m_sPt, sPort);	}
	
	//2. 소켓 어드레스 세팅
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
	

	m_hEvnt = LcNet_WSAEventCreate();											// 커넥션을 연결 전 네트워크 이벤트 연결

	// EventSelect에 연결. 자동으로 Non-blocking전환
	INT hr = WSAEventSelect(  m_scH
							, m_hEvnt
							, (FD_CONNECT|FD_READ|FD_WRITE|FD_CLOSE) );

	if(FAILED(LcNet_SocketErrorCheck(hr)))
		return -1;

	//4. 커넥션
	hr = LcNet_SocketConnect(m_scH, &m_sdH);

	if(FAILED(LcNet_SocketErrorCheck(hr)))
		return -1;

	m_eNetSt = NETST_CONNECTING;
	
	// Nagle 정지.
	LcNet_SocketNaggleOff(m_scH);

	Ev_Zero();
	Ev_Set(m_scH, &m_sdH, m_hEvnt);

	// Receive, Send 쓰레드
	m_hThRecv = LcNet_ThreadCreate(ThreadRecv, this, 0, &m_dThRecv);
	m_hThSend = LcNet_ThreadCreate(ThreadSend, this, 0, &m_dThSend);
	
	return 0;
}




INT CLcNetSlctE::Close()
{
	// 소켓을 닫는다.
	LcNet_SocketClose(&m_scH);
	
	m_rbSnd.Clear();
	m_rbRcv.Clear();
	
	return 0;
}

INT CLcNetSlctE::Listen()
{
	//2. 소켓 어드레스 세팅
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
	
	
	//3. 소켓 바인딩
	if(FAILED(LcNet_SocketBind(m_scH, &m_sdH)))
		return -1;
	
	// Accept하기 전에 이벤트를 연결한다.
	m_hEvnt = LcNet_WSAEventCreate();
	if(SOCKET_ERROR == WSAEventSelect(m_scH, m_hEvnt, (FD_ACCEPT|FD_READ|FD_WRITE|FD_CLOSE) ))
		return -1;

	if(FAILED(LcNet_SocketListen(m_scH)))
		return -1;
	
	
	Ev_Zero();
	Ev_Set(m_scH, &m_sdH, m_hEvnt);

	// Accept, Receive, Send용 쓰레드를 만든다.
	m_hThRecv = LcNet_ThreadCreate(ThreadRecv, this, 0, &m_dThRecv);
	m_hThSend = LcNet_ThreadCreate(ThreadSend, this, 0, &m_dThSend);
	
	return 0;
}




INT CLcNetSlctE::Send(const char* pSrc, INT  iSnd, DWORD  dMsg, SOCKET  scH)
{
	BYTE	sBuf[PCK_BUF_MAX_MSG]={0};
	INT		iSize = 0;
	INT		hr = 0;

	if(0xFFFFFFFF==scH)
	{
		iSize	= LcNet_PacketEncode(sBuf, (BYTE*)pSrc, iSnd, dMsg);
		hr		= m_rbSnd.PushBack(sBuf, iSize);

		if(FAILED(hr))
			return -1;
	}

	else
	{
		INT		nIdx = 1;

		for(nIdx=1; nIdx<m_iNsck; ++nIdx)
		{
			if(m_rmH[nIdx].scH == scH)
				break;
		}

		if(nIdx>=m_iNsck)
			return -1;

		
		iSize	= LcNet_PacketEncode(sBuf, (BYTE*)pSrc, iSnd, dMsg);
		hr		= m_rmH[nIdx].rbSnd.PushBack(sBuf, iSize);
		
		if(FAILED(hr))
			return -1;
	}
	
	return 0;
}

INT CLcNetSlctE::Recv(char* pDst, INT* iRcv, DWORD* dMsg, SOCKET* scH)
{
	BYTE	sBuf[PCK_BUF_MAX_MSG]={0};
	WORD	iSize = 0;
	INT		hr	= 0;

	*iRcv = 0;

	if(NULL==scH)
	{
		hr = m_rbRcv.PopFront(sBuf, &iSize);
		
		if(FAILED(hr))
			return -1;

		if(0==iSize)
			return -1;

		*iRcv = LcNet_PacketDecode((BYTE*)pDst, dMsg, sBuf, iSize);
	}

	else
	{
		INT		nIdx = *scH+1;
		
		if(nIdx> m_iNsck || nIdx<0)
			return -1;

		hr  = m_rmH[nIdx].rbRcv.PopFront(sBuf, &iSize);

		if(FAILED(hr))
			return -1;
		
		if(0==iSize)
			return -1;
		
		*iRcv = LcNet_PacketDecode((BYTE*)pDst, dMsg, sBuf, iSize);

		*scH = m_rmH[nIdx].scH;
	}

	return 0;
}




DWORD CLcNetSlctE::ProcRecv(void* pParam)
{
	BYTE	sBuf[PCK_BUF_MAX_MSG]={0};
	INT		iSize = 0;
	INT		hr = -1;
	
	while(1)
	{
		hr =0;

		// Event Wait
		INT	EventArr[WSA_MAXIMUM_WAIT_EVENTS+4]={0};
		INT iNevt=LcNet_WSAEventWaits(EventArr, m_iNsck, m_rmE);

		if(FAILED(iNevt))
			continue;

		for(INT i=0; i<iNevt; ++i)
		{
			INT			n = EventArr[i];
			SOCKET		s = m_rmH[n].scH;
			WSAEVENT	e = m_rmE[n];

			if(FAILED(m_dEvnt = LcNet_WSAEventEnum(s, e)))
			{
				m_nThRecv = -1;
				hr = -1;
				break;
			}

			// Accept
			if(FD_ACCEPT == m_dEvnt)
			{
				SOCKET		scCln;
				SOCKADDR_IN	sdCln;
				WSAEVENT	evCln;

				if(SUCCEEDED(LcNet_SocketAccept(&scCln, &sdCln, m_scH)))
				{
					if(scCln)
					{
						// 이벤트를 만든다.
						evCln = LcNet_WSAEventCreate();

						// 이벤트를 연결한다.
						LcNet_WSAEventSelect(scCln, evCln,( FD_READ|FD_WRITE|FD_CLOSE));
						printf("New Client: %d \n", scCln);

						Ev_Set(scCln, &sdCln, evCln);	// count를 증가 시킨다.
					}
				}
			}

			// Connection
			else if(FD_CONNECT == m_dEvnt)
			{
				m_eNetSt = NETST_CONNECTED;
			}

			// Receive
			else if(FD_READ == m_dEvnt)
			{
				if(NETHST_CLIENT == m_HstType)
				{
					iSize=recv(m_scH, (char*)sBuf, sizeof sBuf, 0);

					if(iSize>0)
					{
						sBuf[iSize]=0;
						m_rbRcv.PushBack(sBuf, iSize);
					}
				}
				
				else if(NETHST_SERVER == m_HstType && s && s!=m_scH)
				{
					iSize=recv(s, (char*)sBuf, sizeof sBuf, 0);

					if(iSize>0)
					{
						sBuf[iSize]=0;
						m_rmH[n].rbRcv.PushBack(sBuf, iSize);
					}
				}
			}

			// Send Event	
			else if(FD_WRITE == m_dEvnt)
			{
			}

			// Close Event
			else if(FD_CLOSE == m_dEvnt)
			{
				if(NETHST_CLIENT == m_HstType)
				{
					m_nThRecv = -1;
					hr = -1;
					break;
				}
				else if(NETHST_SERVER == m_HstType)
				{
					SOCKET sT =s;
					LcNet_SocketClose(&sT);
					Ev_Clr(s);
					printf("Close socket: %d Remain: %d\n", s, (m_iNsck-1));
				}
			}
		}
	
	}

	m_nThRecv = 0;

	return hr;
}



DWORD CLcNetSlctE::ProcSend(void* pParam)
{
	// 보내기 링버퍼에 있는 내용을 꺼내와서 패킷을 보낸다.
	BYTE	sBuf[PCK_BUF_MAX_MSG]={0};
	WORD	iSize = 0;
	INT		hr = -1;

	while(1)
	{
		hr =0;

		LcNet_ThreadSuspend(&m_hThSend);

		// 데이터 전송
		if(NETHST_CLIENT == m_HstType)
		{
			if(FAILED(m_rbSnd.PopFront(sBuf, &iSize)))
				continue;

			INT hr = send(m_scH, (char*)sBuf, iSize, 0);

			if(FAILED(LcNet_SocketErrorCheck(hr)))
			{
				m_nThSend = -1;
				hr = -1;
				break;
			}
		}

		else if(NETHST_SERVER == m_HstType)
		{
			for(INT i=0;i<m_iNsck; ++i)
			{
				if(m_scH != m_rmH[i].scH)
				{
					if(SUCCEEDED(m_rmH[i].rbSnd.PopFront(sBuf, &iSize)))
					{
						INT hr = send(m_rmH[i].scH, (char*)sBuf, iSize, 0);

						if(FAILED(LcNet_SocketErrorCheck(hr)))
						{
							continue;
						}
					}
				}
			}
		}
	}

	m_nThSend = 0;

	return hr;
}



INT CLcNetSlctE::SendAllData()
{
	INT		hr=-1;
	
	// 보내기 링버퍼에 있는 내용을 꺼내와서 패킷을 보낸다.
	BYTE	sBuf[PCK_BUF_MAX_MSG]={0};
	WORD	iSize = 0;
	
	while(1)
	{
		hr = 0;

		// 데이터 전송
		if(NETHST_CLIENT == m_HstType)
		{
			if(FAILED(m_rbSnd.PopFront(sBuf, &iSize)))
			{
				break;
			}

			
			hr = send(m_scH, (char*)sBuf, iSize, 0);
		}
		
		else if(NETHST_SERVER == m_HstType)
		{
			for(INT j=0;j<m_iNsck; ++j)
			{
				if(m_scH != m_rmH[j].scH)
				{
					if(SUCCEEDED(m_rmH[j].rbSnd.PopFront(sBuf, &iSize)))
					{
						hr = send(m_rmH[j].scH, (char*)sBuf, iSize, 0);
					}
				}
			}
			
			break;
		}


		if(FAILED(LcNet_SocketErrorCheck(iSize)))			// Error Check
		{
			m_nThSend = -1;
			hr =-1;
			break;
		}
	}

	m_nThSend = 0;
	return hr;
}






void CLcNetSlctE::Ev_Set(SOCKET scH, SOCKADDR_IN* sdH, HANDLE hEvt)
{
	INT i;
	
	for (i=0; i<m_iNsck; ++i)
	{
		if (m_rmH[i].scH == scH)
		{
			break;
		}
	}

	if (i == m_iNsck)
	{
		if (m_iNsck < WSA_MAXIMUM_WAIT_EVENTS)
		{
			m_rmH[i].scH = scH;
			m_rmE[i]     = hEvt;

			m_iNsck++;
		}
	}
}


void CLcNetSlctE::Ev_Clr(SOCKET scH)
{
	INT i;

	for (i=0; i<m_iNsck; ++i)
	{
		if (m_rmH[i].scH == scH)
		{
			while (i < m_iNsck-1)
			{
				memcpy(&m_rmH[i], &m_rmH[i+1], sizeof(CLcNetSlctE::LcNetSlctHost));
				m_rmE[i] = m_rmE[i+1];
				
				++i;
			}
			
			INT nCnt = m_iNsck-1;

			m_rmE[nCnt] = 0;
			m_rmH[nCnt].rbRcv.Clear();
			m_rmH[nCnt].rbSnd.Clear();

			m_iNsck--;
			break;
		}
	}
}


void CLcNetSlctE::Ev_Zero()
{
	m_iNsck =0;
}