// Server Application
//
////////////////////////////////////////////////////////////////////////////////

#pragma comment(lib, "Winmm.lib")

#include <vector>

#include <windows.h>
#include <Mmsystem.h>
#include <stdio.h>


#include <LcNet/ILcNet.h>
#include <LcNet/LcNetUtil.h>


ILcNet*		g_pNet=NULL;				// Network Instance


#define HOST_PORT	"20000"


void main()
{
	// 윈속을 초기화
	if(FAILED(LcNet_WinSockCreate()))
		return;

	char	sIp[64]={0};
	LcNet_LocalIpAddress(sIp);
	printf("Starting Server...: %s:%s\n", sIp, HOST_PORT);



	if(FAILED(LcNet_CreateTcpServer("IOCP", &g_pNet, NULL, HOST_PORT)))
		return;

	printf("\n\n네트워크 실행--------------------\n");


	char	sBufSnd[1024]={0};
	char	sBufRcv[1024]={0};
	INT		iLenSnd=0;
	INT		iLenRcv=0;
	DWORD	dRcvMsg=0;

	while(1)
	{
		Sleep(1);

		INT nCnt =0;
		INT nNewCount=0;

		if(SUCCEEDED(g_pNet->Query("New Socket Count", &nNewCount)))
		{
 			while(nNewCount>0)
			{
				SOCKET scNew=0;

				if(SUCCEEDED(g_pNet->Query("Get New Socket", &scNew)))
				{
					int		iLenSnd=0;
					char	sBufSnd[1024]={0};
					sprintf(sBufSnd, "SocketId:%d", scNew);
					iLenSnd=strlen(sBufSnd);

					DWORD nMsgSnd = scNew;
					g_pNet->Send(sBufSnd, iLenSnd, nMsgSnd);
				}

				--nNewCount;
			}
		}

		

		std::vector<SOCKET > plsSocket;

		if(SUCCEEDED(g_pNet->Query("Get Socket List", &plsSocket)))
		{
//			printf("Socket List: ");
//			int iSize =plsSocket.size();
//
//
//			for(int i=0; i<iSize; ++i)
//			{
//				printf("%5d", plsSocket[i]);
//			}
//
//			printf("\n");
		}


		if(SUCCEEDED(g_pNet->Query("Receive Socket Count", &nCnt)))
		{
			for(int i=0; i<nCnt; ++i)
			{
				SOCKET scH = 0;

				if(SUCCEEDED(g_pNet->Query("Get Receive Socket", &scH)))
				{
					memset(sBufRcv, 0, sizeof sBufRcv);
					g_pNet->Recv(sBufRcv, &iLenRcv, &dRcvMsg, &scH);

					if(iLenRcv>0)
					{
						printf("Recv:%s\n", sBufRcv);
						sprintf(sBufSnd, "RecvOk %4d : %s",scH, sBufRcv);
						iLenSnd=strlen(sBufSnd);

						g_pNet->Send(sBufSnd, iLenSnd, scH, scH);

						sprintf(sBufSnd, "Echo %4d : %s",scH, sBufRcv);
						iLenSnd=strlen(sBufSnd);

 						int iSocketSize = plsSocket.size();
 
 						for(int i=0; i<iSocketSize; ++i)
 						{
 							SOCKET scT = plsSocket[i];
 
 							if( scH != scT)
 								g_pNet->Send(sBufSnd, iLenSnd, scT, scT);
 						}
					}
				}

				
			}
		}


		if(FAILED(g_pNet->FrameMove()))
			break;
	}

	printf("\n네트워크 종료--------------------\n\n");
	delete g_pNet;

	// 윈속해제
	LcNet_WinsockDestroy();
}



