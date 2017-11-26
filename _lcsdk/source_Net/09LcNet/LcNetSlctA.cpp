// Implementation of the CLcNetSlctA class.
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
#include "LcNetSlctA.h"



CLcNetSlctA::LcNetSlctHost::LcNetSlctHost()
{
	scH = 0;
	memset(&sdH, 0, sizeof(sdH));
}


CLcNetSlctA::CLcNetSlctA()
{
	m_hThSend	= NULL;
	m_dThSend	= 0;
	m_nThSend	= 0;

	m_iNsck	= 0;

	m_hWnd	= 0;
	m_dwMsg	= 0;	
}


CLcNetSlctA::~CLcNetSlctA()
{
	Destroy();
}




INT CLcNetSlctA::Create(void* p1, void* p2, void* p3, void* p4)
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

void CLcNetSlctA::Destroy()
{
	m_eNetSt = NETST_NONE;
	
	Close();

	if(m_hThSend)
		LcNet_ThreadClose(&m_hThSend	);
}




INT CLcNetSlctA::FrameMove()
{
	if( FAILED(m_nThSend))
		return -1;

	LcNet_ThreadResume(&m_hThSend);

	return 0;
}


INT CLcNetSlctA::Query(char* sCmd, void* pData)
{
	if(0==_stricmp("Get Client Number", sCmd))
	{
		*((INT*)pData) = m_iNsck-1;
		return 0;
	}

	else if(0==_stricmp("Set Window Handle", sCmd))
	{
		m_hWnd = *((HWND*)pData);
		return 0;
	}

	else if(0==_stricmp("Set Message Value", sCmd))
	{
		m_dwMsg = *((DWORD*)pData);
		return 0;
	}

	else if(0==_stricmp("Window Procedure", sCmd))
	{
		char* sData = (char*)pData;

		HWND	hWnd = *((HWND*  )(sData + 0));
		UINT	uMsg = *((UINT*  )(sData + 4));
		WPARAM	wPar = *((WPARAM*)(sData + 8));
		LPARAM  lPar = *((LPARAM*)(sData +12));

		return MsgProc(hWnd, uMsg, wPar, lPar);
	}

	return -1;
}




INT CLcNetSlctA::Connect(char* sIp, char* sPort)
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
	

	// AsycSelect에 연결. 자동으로 Non-blocking전환
	INT hr = WSAAsyncSelect(  m_scH
							, m_hWnd
							, m_dwMsg
							, (FD_CONNECT|FD_READ|FD_WRITE|FD_CLOSE));

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
	Ev_Set(m_scH, &m_sdH);

	// Send 쓰레드
	m_hThSend = LcNet_ThreadCreate(ThreadSend, this, 0, &m_dThSend);

	return 0;
}




INT CLcNetSlctA::Close()
{
	// 소켓을 닫는다.
	LcNet_SocketClose(&m_scH);
	
	return 0;
}

INT CLcNetSlctA::Listen()
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
	
	// 비동기 연결
	// The WSAAsyncSelect function automatically sets socket s to nonblocking mode
	INT hr = WSAAsyncSelect(m_scH, m_hWnd, m_dwMsg, FD_ACCEPT|FD_READ|FD_WRITE|FD_CLOSE);

	if(FAILED(LcNet_SocketErrorCheck(hr)))
		return -1;

	if(FAILED(LcNet_SocketListen(m_scH)))
		return -1;

	Ev_Zero();
	Ev_Set(m_scH, &m_sdH);

	// Send용 쓰레드를 만든다.
	m_hThSend = LcNet_ThreadCreate(ThreadSend, this, 0, &m_dThSend);

	return 0;
}




INT CLcNetSlctA::Send(const char* pSrc, INT  iSnd, DWORD  dMsg, SOCKET  scH)
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

INT CLcNetSlctA::Recv(char* pDst, INT* iRcv, DWORD* dMsg, SOCKET* scH)
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
		
		*iRcv = 0;

		if(nIdx> m_iNsck || nIdx<0)
			return -1;

		if(FAILED(m_rmH[nIdx].rbRcv.PopFront(sBuf, &iSize)))
			return -1;
		
		if(0==iSize)
			return -1;
		
		*iRcv = LcNet_PacketDecode((BYTE*)pDst, dMsg, sBuf, iSize);

		*scH = m_rmH[nIdx].scH;
	}

	return 0;
}




LRESULT CLcNetSlctA::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	SOCKET	scHost	= 0;
	DWORD	dError	= 0;
	DWORD	dEvent	= 0;

	if(uMsg != m_dwMsg || FAILED(m_nThSend))
		return 0;
	
	scHost	= (SOCKET)wParam;
	dError	= WSAGETSELECTERROR(lParam);
	dEvent	= WSAGETSELECTEVENT(lParam);

	if(dError)
	{
		// 클라이언트의 경우 비정상적인 에러
		printf("Network Error\n");

		if(NETHST_CLIENT == m_HstType || m_scH == scHost)
		{
			m_nThSend =-1;
			m_eNetSt = NETST_WSAERROR;
			LcNet_ThreadResume(&m_hThSend);
			Sleep(10);
			return (0XFF000000 | dError);
		}
		else
		{
			SOCKET sT =scHost;
			LcNet_SocketClose(&sT);
			Ev_Clr(scHost);
			printf("Close socket: %d Remain: %d\n", scHost, (m_iNsck-1));

			return 0;
		}		
	}


	if(FD_ACCEPT == dEvent)						// Accept
	{
		SOCKET		scCln;
		SOCKADDR_IN	sdCln;

		if(SUCCEEDED(LcNet_SocketAccept(&scCln, &sdCln, m_scH)))
		{
			if(scCln)
			{
				// 비동기 셀렉트에 연결한다.
				WSAAsyncSelect(scCln, hWnd, m_dwMsg, (FD_READ|FD_WRITE|FD_CLOSE));

				Ev_Set(scCln, &sdCln);	// 증가...
				printf("New Client: %d \n", scCln);
			}
		}

		return 0;
	}

	else if(FD_CONNECT == dEvent)
	{
		m_eNetSt = NETST_CONNECTED;
		return 0;
	}

	else if(FD_READ == dEvent)					// Receive
	{
		BYTE	sBuf[PCK_BUF_MAX_MSG]={0};
		INT		iSize = 0;

		if(NETHST_CLIENT == m_HstType && scHost == m_scH)
		{	
			iSize=recv(m_scH, (char*)sBuf, sizeof sBuf, 0);

			if(iSize>0)
			{
				sBuf[iSize]=0;
				m_rbRcv.PushBack(sBuf, iSize);
			}
		}
		
		else if(NETHST_SERVER == m_HstType)
		{
			for(INT i=1; i<m_iNsck; ++i)
			{
				SOCKET	s = m_rmH[i].scH;

				if(s == scHost)
				{
					iSize=recv(s, (char*)sBuf, sizeof sBuf, 0);

					if(iSize>0)
					{
						sBuf[iSize]=0;
						m_rmH[i].rbRcv.PushBack(sBuf, iSize);
					}
				}
			}
		}

		return 0;
	}

	else if(FD_CLOSE == dEvent)
	{
		if(NETHST_CLIENT == m_HstType)
		{
			m_nThSend =-1;
			m_eNetSt = NETST_CLOSE;
			LcNet_ThreadResume(&m_hThSend);
			Sleep(10);
			return -1;
		}
		else if(NETHST_SERVER == m_HstType)
		{
			SOCKET sT =scHost;
			LcNet_SocketClose(&sT);
			Ev_Clr(scHost);

			printf("Close socket: %d Remain: %d\n", scHost, (m_iNsck-1));
		}

		return 0;
	}

	else if(FD_WRITE == dEvent)					// Sending
	{
		printf("Message Sending...\n");
		return 0;
	}
	
	return 0;
}



DWORD CLcNetSlctA::ProcSend(void* pParam)
{
	// 보내기 링버퍼에 있는 내용을 꺼내와서 패킷을 보낸다.
	BYTE	sBuf[PCK_BUF_MAX_MSG]={0};
	WORD	iSize = 0;
	INT		hr = -1;

	while(SUCCEEDED(m_nThSend))
	{
		hr = 0;

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

		LcNet_ThreadSuspend(&m_hThSend);
	}

	m_nThSend = -1;

	return hr;
}


INT CLcNetSlctA::SendAllData()
{
	INT		hr=-1;
	
	// 보내기 링버퍼에 있는 내용을 꺼내와서 패킷을 보낸다.
	BYTE	sBuf[PCK_BUF_MAX_MSG]={0};
	WORD	iSize = 0;
	
	while(SUCCEEDED(m_nThSend))
	{
		// 데이터 전송
		if(NETHST_CLIENT == m_HstType)
		{
			if(FAILED(m_rbSnd.PopFront(sBuf, &iSize)))
				return 0;
			
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
			
			return 0;			
		}


		if(FAILED(LcNet_SocketErrorCheck(iSize)))			// Error Check
		{
			m_nThSend = -1;
			return -1;
		}
	}

	m_nThSend = -1;
	return 0;
}


void CLcNetSlctA::Ev_Set(SOCKET scH, SOCKADDR_IN* sdH)
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

			m_iNsck++;
		}
	}
}


void CLcNetSlctA::Ev_Clr(SOCKET scH)
{
	INT i;

	for (i=0; i<m_iNsck; ++i)
	{
		if (m_rmH[i].scH == scH)
		{
			while (i < m_iNsck-1)
			{
				memcpy(&m_rmH[i], &m_rmH[i+1], sizeof(CLcNetSlctA::LcNetSlctHost));
				
				++i;
			}
			
			INT nCnt = m_iNsck-1;

			m_rmH[nCnt].rbRcv.Clear();
			m_rmH[nCnt].rbSnd.Clear();

			m_iNsck--;
			break;
		}
	}
}

void CLcNetSlctA::Ev_Zero()
{
	m_iNsck =0;
}

