// Client Application
//
////////////////////////////////////////////////////////////////////////////////

//#pragma comment(linker, "/subsystem:windows")
#pragma comment(linker, "/subsystem:console")
#pragma comment(lib, "Winmm.lib")

#include <windows.h>
#include <Mmsystem.h>
#include <stdio.h>


#include <LcNet/ILcNet.h>
#include <LcNet/LcNetUtil.h>


ILcNet*		g_pNet=NULL;				// Network Instance

#define HOST_IP		"192.168.60.158"
//#define HOST_IP		"172.16.61.252"
//#define HOST_IP		"127.0.0.1"

#define HOST_PORT	"20000"

HINSTANCE	m_hInst			;													// Window Instance
char		m_sCls[128]		;													// Window Class Name
char		m_sWin[128]		;													// Window Title
HWND		m_hWnd			;													// Main Window Handle
DWORD		m_dwMsg	= (WM_USER + 1024);


INT		Create();																// Window ����
void	Cleanup();																// ������ ���� �Լ�
LRESULT WINAPI WndProc( HWND, UINT, WPARAM, LPARAM);							// ������ �޽��� ó���Լ�


//int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
int main()
{
	if(FAILED(Create()))
		return 0;
	
	printf("Starting Client...\n");
	
	// ������ �ʱ�ȭ
	if(FAILED(LcNet_WinSockCreate()))
		return 0;

	if(FAILED(LcNet_CreateTcpClientEx("Async Select", &g_pNet, HOST_IP, HOST_PORT, &m_hWnd, &m_dwMsg)))
		return 0;

	printf("\n\n��Ʈ��ũ ����--------------------\n");

	MSG uMsg;
	DWORD dBgn = timeGetTime();
	INT		iCnt=0;

	memset( &uMsg, 0, sizeof(uMsg) );
	
	while( uMsg.message!=WM_QUIT )
	{
		if( PeekMessage( &uMsg, NULL, 0U, 0U, PM_REMOVE ) )
		{
			TranslateMessage( &uMsg );
			DispatchMessage( &uMsg );
		}
		else
		{
			if(g_pNet && ILcNet::NETST_CONNECTED == g_pNet->NetState())
			{
				// Client Rendering.
				DWORD dEnd = dBgn;

				char	sBufSnd[PCK_BUF_MAX_MSG]={0};
				char	sBufRcv[PCK_BUF_MAX_MSG]={0};
				INT		iLenSnd=0;
				INT		iLenRcv=0;
				DWORD	dRcvMsg=0;

				Sleep(10);

				memset(sBufRcv, 0, sizeof sBufRcv);
				g_pNet->Recv(sBufRcv, &iLenRcv, &dRcvMsg);

				if(iLenRcv>0)
				{
					static int c=0;
					++c;
//					char	sMsg[PCK_BUF_MAX_MSG]={0};
					printf("Rcv: %3d Msg:%x Buf:%s\n",c, dRcvMsg, sBufRcv);
//					HDC hDC = GetDC(m_hWnd);
//
//					TextOut(hDC, 0, 0, sMsg, 50);
//					ReleaseDC(m_hWnd, hDC);
				}


				dEnd = timeGetTime();
				
				if(dEnd>(dBgn+50))
				{
					dBgn = dEnd;
					++iCnt;
					sprintf(sBufSnd, "Client"
									" 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890"
//									" 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890"
//									" 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890"
//									" 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890"
//									" 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890"
//									" 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890"
//									
//									" 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890"
//									" 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890"
//									" 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890"
	//								" 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890 1234567890"
									" %d", iCnt);
					iLenSnd=strlen(sBufSnd);

					DWORD nMsgSnd = 0x87654321;
					g_pNet->Send(sBufSnd, iLenSnd, nMsgSnd);
				}

				if(FAILED(g_pNet->FrameMove()))
					break;
			}
		}
	}
	
	UnregisterClass( m_sCls, m_hInst);
	
	printf("\n��Ʈ��ũ ����--------------------\n\n");

	if(g_pNet)
	{
		g_pNet->Destroy();
		delete g_pNet;
	}

	// ��������
	LcNet_WinsockDestroy();

	return 0;
}




INT Create()
{
	m_hInst	= GetModuleHandle(NULL);
	
	strcpy(m_sCls, "AsycSelect Client");
	strcpy(m_sWin, "AsycSelect Client");
	
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
	{
		if(FAILED(g_pNet->MsgProc(hWnd, uMsg, wParam, lParam)))
		{
			SendMessage(hWnd, WM_DESTROY, 0,0);
		}
	}

	
	switch( uMsg )
	{
		case WM_CLOSE:
		case WM_DESTROY:
		{
			Cleanup();
			PostQuitMessage( 0 );
			break;
		}
	}
	
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}