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
		NETST_WSAERROR	= -3,	// WSA�� Ȯ�� ������ ����
		NETST_CLOSE		= -2,	// CLOSE
		NETST_ERROR		= -1,	// ��Ÿ ��� ERROR

		NETST_NONE		= 0,	// NONE
		NETST_OK		= 1,	// OK
		NETST_CONNECTING= 2,	// While Connecting
		NETST_CONNECTED	= 3,	// Connected
	};


	virtual INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0)=0;	// ������ �ʱ�ȭ�ϰ� ������ �����Ѵ�.
	virtual void	Destroy()=0;												// ������ �Ҹ�ñ�Ű�� ������ �����Ѵ�.
	virtual INT		FrameMove()=0;												// ��Ʈ��ũ���� �ʿ��� �κ��� �����͸� ������Ʈ ��Ų��.
	virtual INT		Query(char* sCmd, void* pData)=0;
	virtual LRESULT	MsgProc(HWND, UINT, WPARAM, LPARAM)=0;						// Async Select��

	virtual INT		Close()=0;													// ���ϸ� �Ҹ��Ų��.
	virtual INT		Listen()=0;													// �������� ���Ǵ� ����
	virtual INT		Connect(char* sIp=NULL, char* sPort=NULL)=0;				// Ŭ���̾�Ʈ���� ���. ������ �ݰ� �ٽ� ������ �� ����Ѵ�. �����ǳ� ��Ʈ�� NULL���ϸ� ���� �����ǿ� ��Ʈ ���

	virtual INT		Send(const char* pSrc, INT  iSnd, DWORD  dMsg
											, SOCKET  scH=0xFFFFFFFF)=0;		// ��Ŷ�� ���� ��
	virtual INT		Recv(      char* pDst, INT* iRcv, DWORD* dMsg
											, SOCKET* scH=NULL)=0;				// ��Ŷ�� ���� ��

	virtual INT		NetState()=0;												// Network ����
	
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




