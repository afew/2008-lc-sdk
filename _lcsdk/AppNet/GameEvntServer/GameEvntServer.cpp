// Server Application
//
////////////////////////////////////////////////////////////////////////////////

#pragma comment(lib, "Winmm.lib")

#include <windows.h>
#include <Mmsystem.h>
#include <stdio.h>


#include <LcNet/ILcNet.h>
#include <LcNet/LcNetUtil.h>


ILcNet*		g_pNet=NULL;				// Network Instance

#define HOST_PORT	"20000"


void main()
{
	// ������ �ʱ�ȭ
	if(FAILED(LcNet_WinSockCreate()))
		return;

	char	sIp[64]={0};
	LcNet_LocalIpAddress(sIp);
	printf("Starting Server...: %s:%s\n", sIp, HOST_PORT);


	if(FAILED(LcNet_CreateTcpServer("Event Select", &g_pNet, NULL, HOST_PORT)))
		return;

	printf("\n\n��Ʈ��ũ ����--------------------\n");


	char	sBufSnd[1024]={0};
	char	sBufRcv[1024]={0};
	INT		iLenSnd=0;
	INT		iLenRcv=0;
	DWORD	dRcvMsg=0;


	while(1)
	{
		Sleep(10);

		INT nCnt =0;

		if(SUCCEEDED(g_pNet->Query("Get Client Number", &nCnt)))
		{
			for(int i=0; i<nCnt; ++i)
			{
				SOCKET scH = i;
				
				memset(sBufRcv, 0, sizeof sBufRcv);
				g_pNet->Recv(sBufRcv, &iLenRcv, &dRcvMsg, &scH);

				if(iLenRcv>0)
				{
					printf("Recv:%s\n", sBufRcv);
					sprintf(sBufSnd, "Echo:%s", sBufRcv);
					iLenSnd=strlen(sBufSnd);

					g_pNet->Send(sBufSnd, iLenSnd, dRcvMsg, scH);
				}
			}
		}

		if(FAILED(g_pNet->FrameMove()))
			break;
	}

	printf("\n��Ʈ��ũ ����--------------------\n\n");
	delete g_pNet;

	// ��������
	LcNet_WinsockDestroy();
}



