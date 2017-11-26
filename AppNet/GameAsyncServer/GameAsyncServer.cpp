// Server Application
//
////////////////////////////////////////////////////////////////////////////////

#pragma comment(linker, "/subsystem:windows")

#pragma comment(lib, "Winmm.lib")

#include <windows.h>
#include <Mmsystem.h>
#include <stdio.h>


#include <LcNet/ILcNet.h>
#include <LcNet/LcNetUtil.h>


ILcNet*		g_pNet=NULL;				// Network Instance

#define HOST_PORT	"20000"

#define MSG_NETWORK		(WM_USER + 1)

HINSTANCE	m_hInst			;													// Window Instance
CHAR		m_sCls[128]		;													// Window Class Name
CHAR		m_sWin[128]		;													// Window Title
HWND		m_hWnd			;													// Main Window Handle
DWORD		m_dwMsg	= MSG_NETWORK;


INT		Create();																// Window 생성
INT		Run();																	// Winodw Main Loop 함수
void	Cleanup();																// 윈도우 해제 함수
LRESULT WINAPI WndProc( HWND, UINT, WPARAM, LPARAM);							// 윈도우 메시지 처리함수


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
//void main()
{
	// 윈속을 초기화
	if(FAILED(LcNet_WinSockCreate()))
		return 0;

	char	sIp[64]={0};
	LcNet_LocalIpAddress(sIp);
	printf("Starting Server...: %s:%s\n", sIp, HOST_PORT);



	
	if(FAILED(Create()))
		return 0;

	
	if(FAILED(LcNet_CreateTcpServerEx("Async Select", &g_pNet, NULL, HOST_PORT, &m_hWnd, &m_dwMsg)))
		return 0;

	printf("\n\n네트워크 실행--------------------\n");

	Run();
	
	printf("\n네트워크 종료--------------------\n\n");
	delete g_pNet;

	// 윈속해제
	LcNet_WinsockDestroy();

	return 0;
}




INT Create()
{
	m_hInst	= GetModuleHandle(NULL);
	
	strcpy(m_sCls, "AsycSelect Server");
	strcpy(m_sWin, "AsycSelect Server");
	
	WNDCLASS wc =								// Register the window class
	{
		CS_CLASSDC
		, WndProc
		, 0L
		, 0L
		, m_hInst
		, NULL
		, LoadCursor(NULL,IDC_ARROW)
		, (HBRUSH)GetStockObject(WHITE_BRUSH)
		, NULL
		, m_sCls
	};


	RegisterClass( &wc );


	RECT rc;									//Create the application's window
	
	SetRect( &rc, 0, 0, 400, 300);
	AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW| WS_VISIBLE, false );
	
	m_hWnd = CreateWindow( m_sCls
		, m_sWin
		, WS_OVERLAPPEDWINDOW| WS_VISIBLE
		, 600
		, 10
		, (rc.right-rc.left)
		, (rc.bottom-rc.top)
		, NULL
		, NULL
		, m_hInst
		, NULL );
	
	ShowWindow( m_hWnd, SW_SHOW );
	UpdateWindow( m_hWnd );

	return S_OK;
}

void Cleanup()
{
	if(m_hInst)
		UnregisterClass( m_sCls, m_hInst);
}


LRESULT WINAPI WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if(g_pNet)
		g_pNet->MsgProc(hWnd, uMsg, wParam, lParam);

	switch( uMsg )
	{
		case WM_CLOSE:
		case WM_DESTROY:
		{
			Cleanup();
			PostQuitMessage( 0 );
			return 0;
		}
	}
	
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}




INT Run()
{	    
	MSG		uMsg;
	DWORD	dBgn = timeGetTime();
	INT		iCnt=0;

	memset( &uMsg, 0, sizeof(uMsg) );
	
	while( uMsg.message!=WM_QUIT )
	{
		if( PeekMessage( &uMsg, NULL, 0U, 0U, PM_REMOVE ) )
		{
			TranslateMessage( &uMsg );
			DispatchMessage( &uMsg );
		}

		
		
		char	sBufSnd[PCK_BUF_MAX_MSG]={0};
		char	sBufRcv[PCK_BUF_MAX_MSG]={0};
		INT		iLenSnd=0;
		INT		iLenRcv=0;
		DWORD	dRcvMsg=0;

		Sleep(10);

		INT nCnt =0;

		if(SUCCEEDED(g_pNet->Query("Get Client Number", &nCnt)))
		{
			for(INT i=0; i<nCnt; ++i)
			{
				SOCKET scH = i;
				
				memset(sBufRcv, 0, sizeof sBufRcv);
				g_pNet->Recv(sBufRcv, &iLenRcv, &dRcvMsg, &scH);

				if(iLenRcv>0)
				{
//					printf("Rcv: Msg:%x Buf:%s\n", dRcvMsg, sBufRcv);
					sprintf(sBufSnd, "Echo: %s", sBufRcv);
					iLenSnd=strlen(sBufSnd);

					INT nMsgSnd = scH;

					g_pNet->Send(sBufSnd, iLenSnd, dRcvMsg, scH);
				}
			}
		}

		if(FAILED(g_pNet->FrameMove()))
			break;
	}
	
	UnregisterClass( m_sCls, m_hInst);
	
	return 1;
}