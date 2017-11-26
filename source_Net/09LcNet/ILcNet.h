// Interface for the ILcNet class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _ILcNet_H_
#define _ILcNet_H_

#ifndef interface
#define interface struct
#endif


#ifndef LC_CLASS_DESTROYER
#define LC_CLASS_DESTROYER( CLASS_NAME )	\
virtual ~CLASS_NAME(){}
#endif


typedef u_int           SOCKET;


interface ILcNet
{
	LC_CLASS_DESTROYER(	ILcNet	);

	enum ELcNetSt				// Network State
	{
		NETST_WSAERROR	= -3,	// WSA로 확인 가능한 에러
		NETST_CLOSE		= -2,	// CLOSE
		NETST_ERROR		= -1,	// 기타 모든 ERROR

		NETST_NONE		= 0,	// NONE
		NETST_OK		= 1,	// OK
		NETST_CONNECTING= 2,	// While Connecting
		NETST_CONNECTED	= 3,	// Connected
	};


	virtual INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0)=0;	// 윈속을 초기화하고 소켓을 생성한다.
	virtual void	Destroy()=0;												// 소켓을 소멸시기키고 윈속을 해제한다.
	virtual INT		FrameMove()=0;												// 네트워크에서 필요한 부분으 데이터를 업데이트 시킨다.
	virtual INT		Query(char* sCmd, void* pData)=0;
	virtual LRESULT	MsgProc(HWND, UINT, WPARAM, LPARAM)=0;						// Async Select용

	virtual INT		Close()=0;													// 소켓만 소멸시킨다.
	virtual INT		Listen()=0;													// 서버에서 사용되는 리슨
	virtual INT		Connect(char* sIp=NULL, char* sPort=NULL)=0;				// 클라이언트에서 사용. 소켓을 닫고 다시 연결할 때 사용한다. 아이피나 포트를 NULL로하면 이전 아이피와 포트 사용

	virtual INT		Send(const char* pSrc, INT  iSnd, DWORD  dMsg
											, SOCKET  scH=0xFFFFFFFF)=0;		// 패킷을 보낼 때
	virtual INT		Recv(      char* pDst, INT* iRcv, DWORD* dMsg
											, SOCKET* scH=NULL)=0;				// 패킷을 받을 때

	virtual INT		NetState()=0;												// Network 상태
	
};


INT		LcNet_WinSockCreate();
void	LcNet_WinsockDestroy();

INT		LcNet_CreateTcpServer(char* sCmd
							 , ILcNet** pData
							 , void* p1			// IP
							 , void* p2			// Port
							 );

INT		LcNet_CreateTcpClient(char* sCmd
							 , ILcNet** pData
							 , void* p1			// IP
							 , void* p2			// Port
							 );


INT		LcNet_CreateTcpServerEx(char* sCmd
							 , ILcNet** pData
							 , void* p1			// IP
							 , void* p2			// Port
							 , void* v1			// Ex Value1
							 , void* v2			// Ex Value2
							 );

INT		LcNet_CreateTcpClientEx(char* sCmd
							 , ILcNet** pData
							 , void* p1			// IP
							 , void* p2			// Port
							 , void* v1			// Ex Value1
							 , void* v2			// Ex Value2
							 );



//WinSock Library
#pragma comment(lib, "WS2_32.LIB")

#ifdef _DEBUG
	#pragma comment(lib, "LcNet_.lib")
#else
	#pragma comment(lib, "LcNet.lib")
#endif


#endif




