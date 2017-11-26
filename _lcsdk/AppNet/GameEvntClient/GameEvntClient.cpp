// Client Application
//
////////////////////////////////////////////////////////////////////////////////

#pragma comment(lib, "Winmm.lib")

#include <windows.h>
#include <Mmsystem.h>
#include <stdio.h>


#include <LcNet/ILcNet.h>


ILcNet*		g_pNet=NULL;				// Network Instance

#define HOST_IP		"192.168.60.67"
//#define HOST_IP		"127.0.0.1"

#define HOST_PORT	"20000"



void main()
{
	printf("Starting Client...\n");

	// 윈속을 초기화
	if(FAILED(LcNet_WinSockCreate()))
		return;
	
	if(FAILED(LcNet_CreateTcpClient("Event Select", &g_pNet, HOST_IP, HOST_PORT)))
		return;

	printf("\n\n네트워크 실행--------------------\n");


	
	DWORD dBgn = timeGetTime();
	DWORD dEnd = dBgn;

	INT		iCnt=0;

	char	sBufSnd[1024]={0};
	char	sBufRcv[1024]={0};
	INT		iLenSnd=0;
	INT		iLenRcv=0;
	DWORD	dRcvMsg=0;

	while(1)
	{
		Sleep(10);

		BOOL	bConn = g_pNet->IsConnect();

		if(bConn)
		{
			memset(sBufRcv, 0, sizeof sBufRcv);
			g_pNet->Recv(sBufRcv, &iLenRcv, &dRcvMsg);

			if(iLenRcv>0)
			{
				printf("Rcv: Msg:%d Buf:%s\n", dRcvMsg, sBufRcv);
			}


			dEnd = timeGetTime();
			
			if(dEnd>(dBgn+1000))
			{
				dBgn = dEnd;
				++iCnt;
				sprintf(sBufSnd, "Client Send %d", iCnt);
				iLenSnd=strlen(sBufSnd);

				DWORD nMsgSnd = 0xDEADBEEF;
				g_pNet->Send(sBufSnd, iLenSnd, nMsgSnd);
			}

			if(FAILED(g_pNet->FrameMove()))
				break;
		}
	}

	printf("\n네트워크 종료--------------------\n\n");
	delete g_pNet;


	// 윈속해제
	LcNet_WinsockDestroy();
}



