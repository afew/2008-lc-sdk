// Interface for the LcNetUtil functions.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcNetUtil_H_
#define _LcNetUtil_H_

// 모든 함수는 실패하는 경우 음수 값을 반환.

#define SAFE_CLOSE_HANDLE(p)	{	if(p){	CloseHandle( p );	(p) = NULL;	}	}
#define ERROR_MESSAGE_LINE(pBuf){	printf("Error Line:%3d %s\t%s\n", __LINE__, __FILE__, (pBuf));	}
//#define ERROR_MESSAGE_LINE(pBuf){	printf("Error Line:%3d %s\t%s\t%s\t%s\n", __LINE__, __DATE__, __TIME__, __FILE__, (pBuf));	}


// Error Message
void	LcNet_FormatMessage(char* sMsg, DWORD hr);			// Network Message Formating
void	LcNet_FormatMessage(char* sMsg);					// Network Message Formating
void	LcNet_ErrorMessage(DWORD hr);						// Network Error Message Catching
INT		LcNet_WSAGetError();


#define ERROR_CHECK_MESSAGE(hr)				\
{											\
	char ssLcErrMsg[2048]={0};				\
	LcNet_FormatMessage( ssLcErrMsg, hr);	\
	printf("Error Line:%3d %s\n"			\
			, __LINE__, __FILE__			\
			, (ssLcErrMsg));				\
}


// Winsock DLL
INT		LcNet_WSAStartup();						// 윈속 라이브러리 초기화
void	LcNet_WSACleanup();						// 윈속 라이브러리 해제


// Socket
INT		LcNet_SocketErrorCheck(INT hr);											// Socket Error Checking
void	LcNet_SocketAddr(SOCKADDR_IN* pOut, char* sIp, char* sPort);			// Setting Socket Address
void	LcNet_SocketClose(SOCKET* scH);											// Socket Close
INT		LcNet_SocketTcpCreate(SOCKET* pScH, BOOL bOverLapped=FALSE);			// Create TCP Socket
INT		LcNet_SocketUdpCreate(SOCKET* pScH, BOOL bOverLapped=FALSE);			// Create UDP Socket

INT		LcNet_SocketConnect(SOCKET scH, SOCKADDR_IN* psdH);						// Connection
INT		LcNet_SocketBind(SOCKET scH, SOCKADDR_IN* pSdH);						// Socket Binding
INT		LcNet_SocketListen(SOCKET scH);											// Socket Listen

INT		LcNet_SocketAccept(SOCKET* pscOut			// Output Socket
							, SOCKADDR_IN* psdOut	// Output Socket Address
							, SOCKET scListen		// Listen socket
							);						// Accept

INT		LcNet_SocketSelect(FD_SET* pFdRead, FD_SET* pFdWrite, TIMEVAL* timeout, FD_SET* pFdExcept=NULL);

INT		LcNet_SocketNonBlocking(SOCKET scH, BOOL bOn=TRUE);						// Set NonBlocking
INT		LcNet_SocketNaggleOff(SOCKET scH, BOOL bOff=TRUE);						// Off nagle Algorithm
INT		LcNet_SocketDirectBuffer(SOCKET scH);									// Disable send buffering on the socket


// 쓰레드 관련 함수들
typedef unsigned (__stdcall* _PBEGIN_THREAD_EX)(void*);							// Thread용 함수포인터
typedef _PBEGIN_THREAD_EX LPBEGIN_THREAD_EX;									//

HANDLE	LcNet_ThreadCreate(LPTHREAD_START_ROUTINE pFunc,void* pParam, ULONG dFlag=0, DWORD* dId= NULL);	// _beginthreadex()
void	LcNet_ThreadEnd();																				// _endthreadex(0)
void	LcNet_ThreadClose(HANDLE* hThread);																// Thread Close
DWORD	LcNet_ThreadResume(HANDLE* hThread);									// Thread resume
DWORD	LcNet_ThreadSuspend(HANDLE* hThread);									// Thread Suspend
DWORD	LcNet_ThreadWait(HANDLE* hThread, DWORD dMilliseconds = INFINITE);



// AsyncSelect I/O Model
INT		LcNet_WSAAsyncSelect(	SOCKET		scH
							,	HWND		hWnd
							,	unsigned int wMsg								// Message ID
							,	long lEvents =(FD_ACCEPT|FD_CONNECT|FD_READ|FD_WRITE|FD_CLOSE)
							 );													// WSA Async Select


// Event I/O Model
HANDLE	LcNet_EventCreate(		BOOL bManualReset=FALSE							// FALSE이면 수동으로 Reset(활성화)을 호출 해야 한다.
							,	BOOL bIntitialState=TRUE);						// 초기 상태가 Signal(활성화)상태

void	LcNet_EventResume(HANDLE hEvent);										// 이벤트 활성화
void	LcNet_EventSuspend(HANDLE hEvent);										// 이벤트 비활성화

INT		LcNet_EventWait(HANDLE hEvent, DWORD dWaitMillisecond=INFINITE);		// Event Wait
void	LcNet_EventClose(HANDLE* hEvent);

HANDLE	LcNet_WSAEventCreate();													// WSA Event Create
void	LcNet_WSAEventClose(HANDLE* pEvent);									// WSA Event Close

INT		LcNet_WSAEventSelect(	SOCKET scH
							,	HANDLE evEvt
							,	long lEvents =(FD_ACCEPT|FD_CONNECT|FD_READ|FD_WRITE|FD_CLOSE)
							 );													// WSA Event Select

INT		LcNet_WSAEventWaits(	INT* pArr	// In: Event Array. 배열에는 이벤트 숫자 만큼 이벤트 인덱스가 기록 되어 있다.
							,	INT nSrc	// In: Event Array Number.
							,	HANDLE* pSrc// In:
							);													// Return is Event 숫자. Failse is -1

INT		LcNet_WSAEventEnum(SOCKET s, HANDLE e);									// WSA Enum Network Event


// 기타
INT		LcNet_GetSystemProcessNumber();											// Number of CPU
HANDLE	LcNet_IocpPortCreate(SOCKET scH, HANDLE hIocp, void* pAddress);			// IOCP 포트 생성

INT		LcNet_LocalIpAddress(	char* sIp	///*List*/
							,	INT* piN=NULL/*Count*/
							,	INT* iWidth=NULL);	// Get Local Ip Addresses





// Interface for the TRingBuf class.
//
////////////////////////////////////////////////////////////////////////////////

#define PCK_USE_BUF_POINTER	0
#define PCK_BUF_MAX_MSG		256
#define PCK_BUF_MAX_QUEUE	1024
#define PCK_BUF_HEAD		2
#define PCK_BUF_CRYPT		4
#define PCK_BUF_KIND		4
#define PCK_BUF_TAIL		4


class TRingBuf
{
protected:
	INT		F;						// Header
	INT		L;						// Rear(Last)
	INT		W;						// Width
	INT		S;						// Stored

#if	PCK_USE_BUF_POINTER
	BYTE*	B;						// Buffer
#else
	BYTE	B[PCK_BUF_MAX_QUEUE];
#endif

public:
	TRingBuf();
	TRingBuf(INT iSize);
	virtual ~TRingBuf();

	void	SetSize(INT iSize);
	void	Clear()	;

	INT		End()	;
	INT		Front()	;
	INT		GetStore();

	INT		PushBack(BYTE* /* In*/pSrc, INT   iSize/*Length:  In*/);
	INT		PopFront(BYTE* /*Out*/pDst, WORD* iSize/*Length: Out*/);
};





// Circular Queue
template<class T>
class TqueCrc
{
protected:
	INT		F;						// Header
	INT		L;						// Rear(Last)
	INT		N;						// Queue Count
	INT		W;						// Width
	
	INT		S;						// Space (Unused Memory)

	T*		B;						// Buffer

public:
	TqueCrc(INT iSize): F(0), L(0), N(0), W(0), S(0), B(0)
	{
		W	= iSize;
		B	= new T[W];

		memset(B, 0, W* sizeof(T));
	}
	
	~TqueCrc()
	{
		if(B)	{	delete [] B; B= NULL;	}
	}
	
	void SetSize(INT iSize)
	{
		F	= 0;
		L	= 0;
		N	= 0;
		W	= iSize;
		S	= W;		
		
		if(B)	{	delete [] B; B= NULL;	}
		
		B	= new T[W];
		memset(B, 0, W* sizeof(T));
	}
	
	
	INT PushBack(T* t)
	{
		// *(B+L) = *t;
		memcpy((B+L), t, sizeof T);

		--S;
		++L;
		L %= W;
		++N;
		
		return 0;
	}
	
	void PopFront(T* t)
	{
		if(N<=0)
			return;
		
		// *t = *(B+L);
		memcpy(t, (B+F), sizeof T);

		++S;
		++F;
		F %= W;
		--N;
	}


	INT		GetSize()	{	return N;		}

	INT		End()		{	return L;		}
	INT		Front()		{	return F;		}
	BOOL	IsEmpty()	{	return !N;		}
};




template<class T> inline
T LcNet_Find_Socket(T _F, T _L, SOCKET scH)
{
	for (; _F != _L; ++_F)
	{
		if (scH == (*_F)->scH)
			break;
	}
	
	return _F;
}



template<class T> inline
INT LcNet_Find_Idx(T _F, T _L, void* p)
{
	for (INT i=0; _F != _L; ++_F, ++i)
	{
		if (p == (void*)(*_F))
			return i;
	}
	
	return -1;
}




INT	 LcNet_PacketEncode(BYTE* pOut						// Output Packet
					  , BYTE* sMsg						// Packet contents
					  , INT iSndM						// Pakcet contents length
					  , DWORD nMsg						// Send Message Kind
					  , INT iPstL=(PCK_BUF_HEAD)		// Packet Structure Length	= 2 Byte
					  ,	INT	iPstC=(PCK_BUF_CRYPT)		// Packet Cryptography Length= 4 Byte
					  , INT iPstM=(PCK_BUF_KIND)		// Packet Structure Message = 4 Byte
					  , INT iPstT=(PCK_BUF_TAIL)		// Packet End Mark			= 4 Byte
					  );


INT	 LcNet_PacketDecode(BYTE* sMsg						// Output Message
					  , DWORD* nMsg						// Receive Message Kind
					  , BYTE* pIn						// Receive Packets
					  , INT iRcvM						// Receive Packet Length
					  , INT iPstL=(PCK_BUF_HEAD)		// Packet Structure Length	= 2 Byte
					  ,	INT	iPstC=(PCK_BUF_CRYPT)		// Packet Cryptography Length= 4 Byte
					  , INT iPstM=(PCK_BUF_KIND)		// Packet Structure Message = 4 Byte
					  , INT iPstT=(PCK_BUF_TAIL)		// Packet End Mark			= 4 Byte
					  );



#endif

