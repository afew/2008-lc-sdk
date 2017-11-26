// Implementation of the CLcDevBase class.
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <stdio.h>

#include <d3d9.h>
#include <d3dx9.h>

#include <Lc/ILcTex.h>

#include "ILcDev.h"
#include "LcDevBase.h"


CLcDevBase::CLcDevBase()
{
}

CLcDevBase::~CLcDevBase()
{
	CLcDevBase::Destroy();
}


INT CLcDevBase::Create(void* p1, void* p2, void* p3, void* p4)
{
	return 0;
}


INT CLcDevBase::Init()
{
	return 0;
}



void CLcDevBase::Destroy()
{
}



INT CLcDevBase::Restore()
{
	return 0;
}

void CLcDevBase::Invalidate()
{
}


INT	CLcDevBase::FrameMove()
{
	return 0;
}

INT CLcDevBase::Render()
{
	return 0;
}


INT CLcDevBase::Run()
{
	return 0;
}

INT CLcDevBase::Query(char* sCmd, void* pData)
{
	return -1;
}




INT CLcDevBase::BeginScene()
{
	return 0;
}


INT CLcDevBase::EndScene()
{
	return 0;
}


INT CLcDevBase::Clear(DWORD Count,CONST RECT* pRects,DWORD Flags, DWORD Color,float Z,DWORD Stencil)
{
	return 0;
}





//	Timers
//	Windows NT Workstation version 3.5 makes it possible for applications to use the basic system timer with the GetTickCount( ) API.
//	The resolution of this timer is 10 milliseconds.
//	Several CPUs support a high-resolution counter that can be used to get very granular resolution.
//	The Win32 QueryPerformanceCounter( ) API returns the resolution of a high-resolution performance counter.
//	For Intel®-based CPUs, the resolution is about 0.8 microseconds.
//	For MIPS®-based CPUs, the resolution is about twice the clock speed of the processor.
//	You need to call QueryPerformanceFrequency( ) to get the frequency of the high-resolution performance counter.

//ULONGLONG RdtscClock()
//{
//	ULONGLONG	uTick;
//	_asm
//	{
//		rdtsc
//		mov DWORD PTR uTick    , eax
//		mov DWORD PTR uTick + 4, edx
//	}
//
//	return uTick;
//
////	_asm rdtsc;
//}



DOUBLE TimeGetTime()													// 하드웨어에서 지원되는 PerformanceCounter를 이용한 FPS구하기
{
//	typedef union _LARGE_INTEGER
//	{
//		struct
//		{
//			DWORD LowPart;
//			LONG HighPart;
//		};
//
//		struct
//		{
//			DWORD LowPart;
//			LONG HighPart;
//		} u;
//
//	} LARGE_INTEGER;


	static LARGE_INTEGER	lFreq	={0};
	static BOOL				bQuery	=FALSE;
	static BOOL				bGetTime=FALSE;

	static LARGE_INTEGER	lBgn	={0};
	LARGE_INTEGER			lEnd	={0};
	DOUBLE					lElapse = 0.;

	// QueryPerformanceFrequency를 쓸 수 없다면 timeGetTime을 사용한다.
	if(FALSE == bQuery && FALSE ==bGetTime)
	{
		bQuery = QueryPerformanceFrequency( &lFreq);

		if(FALSE == bQuery)
		{
			bGetTime	= TRUE;
			lBgn.LowPart= ::timeGetTime();
		}

		else
			QueryPerformanceCounter(&lBgn);
	}

	if(bQuery)
	{
		QueryPerformanceCounter( &lEnd);
		lElapse = DOUBLE( (lEnd.QuadPart-lBgn.QuadPart) * 1000.)/ lFreq.QuadPart;
	}
	else
	{
		lEnd.LowPart= ::timeGetTime();
		lElapse		= lEnd.LowPart-lBgn.LowPart;
	}


	return lElapse;
}