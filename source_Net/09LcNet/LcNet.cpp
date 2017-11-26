// Implementation of the CLcNet class.
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <stdio.h>

#include "ILcNet.h"
#include "LcNet.h"


CLcNet::CLcNet()
{
	m_scH		= 0;
	memset(&m_sdH, 0, sizeof m_sdH);
	memset(m_sIp, 0, sizeof m_sdH);
	memset(m_sPt, 0, sizeof m_sPt);

	m_PtcType	= NETPRT_NONE;		// Protocol type
	m_HstType	= NETHST_NONE;		// Client Server?

	m_eNetSt	= NETST_NONE;
}

CLcNet::~CLcNet()
{
	Destroy();
}


INT CLcNet::Create(void* p1, void* p2, void* p3, void* p4)
{
	return 0;
}


void CLcNet::Destroy()
{
}


INT CLcNet::FrameMove()
{
	return 0;
}


INT CLcNet::Query(char* sCmd, void* pData)
{
	return 0;
}




INT CLcNet::Close()
{
	return 0;
}

INT CLcNet::Listen()
{
	return 0;
}


INT CLcNet::Connect(char* sIp, char* sPort)
{
	return 0;
}


INT CLcNet::Send(const char* pSrc, INT  iSnd, DWORD  dMsg, SOCKET  scH)
{
	return 0;
}

INT CLcNet::Recv(char* pDst, INT* iRcv, DWORD* dMsg, SOCKET* scH)
{
	return 0;
}

INT CLcNet::NetState()
{
	return m_eNetSt;
}

LRESULT	CLcNet::MsgProc(HWND, UINT, WPARAM, LPARAM)
{
	return -1;
}


DWORD WINAPI CLcNet::ThreadRecv(void* pParam)
{
	return ((CLcNet*)pParam)->ProcRecv(pParam);
}


DWORD WINAPI CLcNet::ThreadSend(void* pParam)
{
	return ((CLcNet*)pParam)->ProcSend(pParam);
}


DWORD WINAPI CLcNet::ThreadAccp(void* pParam)
{
	return ((CLcNet*)pParam)->ProcAccp(pParam);
}


DWORD WINAPI CLcNet::ThreadWork(void* pParam)
{
	return ((CLcNet*)pParam)->ProcWork(pParam);
}


DWORD CLcNet::ProcRecv(void* pParam)
{
	return 0;
}


DWORD CLcNet::ProcSend(void* pParam)
{
	return 0;
}

DWORD CLcNet::ProcAccp(void* pParam)
{
	return 0;
}


DWORD CLcNet::ProcWork(void* pParam)
{
	return 0;
}


