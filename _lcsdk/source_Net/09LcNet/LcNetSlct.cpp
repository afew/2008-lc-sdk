// Implementation of the CLcNetSlct class.
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
#include "LcNetSlct.h"



CLcNetSlct::CLcNetSlct()
{
	m_hThSend	= NULL;
	m_dThSend	= 0;
	m_nThSend	= 0;

	m_hThRecv	= NULL;
	m_dThRecv	= 0;
	m_nThRecv	= 0;

	m_hThAccp	= NULL;
	m_dThAccp	= 0;
	m_nThAccp	= 0;
}


CLcNetSlct::~CLcNetSlct()
{
	Destroy();
}


INT CLcNetSlct::Create(void* p1, void* p2, void* p3, void* p4)
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

void CLcNetSlct::Destroy()
{
	Close();

	SAFE_CLOSE_HANDLE(	m_hThSend	);
	SAFE_CLOSE_HANDLE(	m_hThRecv	);
	SAFE_CLOSE_HANDLE(	m_hThAccp	);
}




INT CLcNetSlct::FrameMove()
{
	INT	hr = -1;

	if(NETHST_CLIENT == m_HstType)
		hr = FrameMoveCln();
	
	else if(NETHST_SERVER == m_HstType)
		hr = FrameMoveSvr();

	if(FAILED(hr))
		return -1;

	return 0;
}



INT CLcNetSlct::Query(char* sCmd, void* pData)
{
	if(0==_stricmp("Get Client Number", sCmd))
	{
		*((INT*)pData) = m_rmF.fd_count-1;
		return 0;
	}
	
	return -1;
}




INT CLcNetSlct::Connect(char* sIp, char* sPort)
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
	
	//4. 커넥션을 시도한다.
	if(SUCCEEDED(LcNet_SocketConnect(m_scH, &m_sdH)))
	{
		m_eNetSt = NETST_CONNECTED;
		
		// Non blocking, Nagle 정지.
		LcNet_SocketNonBlocking(m_scH);
		LcNet_SocketNaggleOff(m_scH);
		
		Fd_Zero(&m_rmF);
		Fd_Set(m_scH, &m_rmF);		//read_fds 카운터 증가.

		return 0;
	}
	
	return -1;
}




INT CLcNetSlct::Close()
{
	// 소켓을 닫는다.
	LcNet_SocketClose(&m_scH);
	
	m_rbSnd.Clear();
	m_rbRcv.Clear();
	
	return 0;
}

INT CLcNetSlct::Listen()
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
	
	if(FAILED(LcNet_SocketListen(m_scH)))
		return -1;
	
	
	Fd_Zero(&m_rmF);
	Fd_Set(m_scH, &m_rmF);
	
	
	return 0;
}




INT CLcNetSlct::Send(const char* pSrc, INT  iSnd, DWORD  dMsg, SOCKET  scH)
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
		UINT	nIdx = 1;

		for(nIdx=1; nIdx<m_rmF.fd_count; ++nIdx)
		{
			if(m_rmF.fd_array[nIdx] == scH)
				break;
		}

		if(nIdx>=m_rmF.fd_count)
			return -1;

		
		iSize	= LcNet_PacketEncode(sBuf, (BYTE*)pSrc, iSnd, dMsg);
		hr		= m_rmH[nIdx].rbSnd.PushBack(sBuf, iSize);
		
		if(FAILED(hr))
			return -1;
	}
	
	return 0;
}

INT CLcNetSlct::Recv(char* pDst, INT* iRcv, DWORD* dMsg, SOCKET* scH)
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
		UINT	nIdx = *scH+1;
		
		*iRcv = 0;

		if(nIdx> m_rmF.fd_count || nIdx<0)
			return -1;

		hr = m_rmH[nIdx].rbRcv.PopFront(sBuf, &iSize);

		if(FAILED(hr))
			return -1;
		
		if(0==iSize)
			return -1;
		
		*iRcv = LcNet_PacketDecode((BYTE*)pDst, dMsg, sBuf, iSize);
		*scH = m_rmF.fd_array[nIdx];
	}

	return 0;
}







INT CLcNetSlct::FrameMoveSvr()
{
	INT		hr=0;
	FD_SET	fdsRead;
	FD_SET	fdsWrite;
	TIMEVAL	timeout;
	
	if(!m_scH)
		return -1;
	
	fdsRead = m_rmF;
	fdsWrite= m_rmF;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;


	// 네트워크에 쓸 수 있는지, 즉 접속이 되어 있는지 검사.
	hr = LcNet_SocketSelect(NULL, &fdsWrite, &timeout);
	
	if(FAILED(hr))
		return -1;

	if(hr)
	{
		if(FAILED(hr= SendAllData()))
			return -1;
	}

	timeout.tv_sec = 0;
	timeout.tv_usec = 0;

	
	hr = LcNet_SocketSelect(&fdsRead, NULL, &timeout);
	

	if(FAILED(LcNet_SocketErrorCheck(hr)))			// Error Check
		return -1;

	
	for(UINT i=0; i<m_rmF.fd_count; ++i)
	{
		if(FD_ISSET(m_rmF.fd_array[i], &fdsRead))
		{
			if(m_scH == m_rmF.fd_array[i])
			{
				SOCKET		scCln;
				SOCKADDR_IN	sdCln;
				
				if(SUCCEEDED(LcNet_SocketAccept(&scCln, &sdCln, m_scH)))
				{
					if(scCln)
					{
						Fd_Set(scCln, &m_rmF);			// m_rmF.fd_count를 증가 시킨다.
						printf("New Client:%d, Remain: %d\n", scCln, (m_rmF.fd_count-1));
					}
				}
			}
			
			else
			{
				BYTE	sBuf[PCK_BUF_MAX_MSG]={0};
				INT		iSize = 0;
				
				iSize = recv(m_rmF.fd_array[i], (char*)sBuf, sizeof sBuf, 0/*MSm_OOB*/);
				
				if(iSize>0)
				{
					sBuf[iSize]=0;

					// 받은 메시지를 저장
					m_rmH[i].rbRcv.PushBack(sBuf, iSize);
				}
				else
				{
					// 소켓을 닫고, m_rmF.fd_count를 감소 시킨다.
					SOCKET scT = m_rmF.fd_array[i];

					LcNet_SocketClose(&m_rmF.fd_array[i]);
					Fd_Clr(m_rmF.fd_array[i], &m_rmF);
					printf("Close:%d Remain: %d \n", scT, (m_rmF.fd_count-1));
				}
			}
		}
	}
	
	Sleep(1);
	
	return 0;
}




INT CLcNetSlct::FrameMoveCln()
{
	INT		hr=0;
	FD_SET	fdsRead;
	FD_SET	fdsWrite;
	TIMEVAL	timeout;
	
	if(!m_scH)
		return -1;

	fdsRead = m_rmF;
	fdsWrite= m_rmF;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;

	
	// 네트워크에 쓸 수 있는지, 즉 접속이 되어 있는지 검사.
	hr = LcNet_SocketSelect(NULL, &fdsWrite, &timeout);
	
	if(FAILED(hr))
		return -1;

	if(hr)
	{
		// 네트워크에 쓸 수 있다면 데이터를 보낸다.
		if(FAILED(hr= SendAllData()))
			return -1;
	}
	
	// 받은 데이터가 있는지 확인한다.
	hr = LcNet_SocketSelect(&fdsRead, NULL, &timeout);
	
	if(FAILED(hr))
	{
		LcNet_SocketClose(&m_scH);
		return -1;
	}
	
	else
	{
		// 데이터 수신
		if(FD_ISSET(m_scH, &fdsRead))
		{
			BYTE	sBuf[PCK_BUF_MAX_MSG]={0};
			INT		iSize = 0;

			iSize=recv(m_scH, (char*)sBuf, sizeof sBuf, 0);


			if(FAILED(LcNet_SocketErrorCheck(iSize)))			// Error Check
			{
				// 종료
				LcNet_SocketClose(&m_scH);
				m_eNetSt = NETST_CLOSE;
				return -1;
			}

			if(iSize<=0)
				return 0;

			sBuf[iSize]=0;
			m_rbRcv.PushBack(sBuf, iSize);			
		}
	}
	
	Sleep(1);
	
	return 0;
}



INT CLcNetSlct::SendAllData()
{
	INT		hr=-1;
	
	// 보내기 링버퍼에 있는 내용을 꺼내와서 패킷을 보낸다.
	BYTE	sBuf[PCK_BUF_MAX_MSG]={0};
	WORD	iSize = 0;
	
	while(1)
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
			for(UINT j=0;j<m_rmF.fd_count; ++j)
			{
				if(m_scH != m_rmF.fd_array[j])
				{
					if(SUCCEEDED(m_rmH[j].rbSnd.PopFront(sBuf, &iSize)))
					{
						hr = send(m_rmF.fd_array[j], (char*)sBuf, iSize, 0);
					}
				}
			}
			
			return 0;
		}


		if(FAILED(LcNet_SocketErrorCheck(hr)))			// Error Check
		{
			LcNet_FormatMessage(hr);
			LcNet_SocketClose(&m_scH);
			return -1;
		}
	}
	
	return 0;
}




void CLcNetSlct::Fd_Set(SOCKET fd, FD_SET* fds)
{
	do
	{
		u_int i;
		
		for (i = 0; i < fds->fd_count; i++)
		{
			if (fds->fd_array[i] == (fd))
			{
				break;
			}
		}

		if (i == fds->fd_count)
		{
			if (fds->fd_count < FD_SETSIZE)
			{
				fds->fd_array[i] = (fd);
				fds->fd_count++;
			}
		}
	} while(0);
}


void CLcNetSlct::Fd_Clr(SOCKET fd, FD_SET* fds)
{
	do
	{
		u_int i;

		for (i = 0; i < fds->fd_count ; i++)
		{
			if (fds->fd_array[i] == fd)
			{
				while (i < fds->fd_count-1)
				{
					fds->fd_array[i] = fds->fd_array[i+1];
					memcpy(&m_rmH[i], &m_rmH[i+1], sizeof(CLcNetSlct::LcNetSlctHost));
					i++;
				}
				
				INT nCnt = fds->fd_count-1;
				m_rmH[nCnt].rbRcv.Clear();
				m_rmH[nCnt].rbSnd.Clear();

				fds->fd_count--;
				break;
			}
		}

	} while(0);

}


void CLcNetSlct::Fd_Zero(FD_SET* fds)
{
	fds->fd_count =0;
}
