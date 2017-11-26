// Implementation of the CLcInputD class.
//
////////////////////////////////////////////////////////////////////////////////

#include <windows.h>

#define DIRECTINPUT_VERSION		0x0800
#include <dinput.h>

#include <D3D9.h>
#include <d3dx9.h>

#include "ILcInput.h"
#include "LcInput.h"
#include "LcInputD.h"


CLcInputD::CLcInputD()
{
	m_eType		= LN_INPUT_D;
	m_pDev		= NULL;

	m_pDiI		= NULL;
	m_pDiK		= NULL;
	m_pDiM		= NULL;	
}



CLcInputD::~CLcInputD()
{
	Destroy();
}


void CLcInputD::Destroy()
{
	if(m_pDiK){	m_pDiK->Release();	m_pDiK = NULL;	}
	if(m_pDiM){	m_pDiM->Release();	m_pDiM = NULL;	}
	if(m_pDiI){	m_pDiI->Release();	m_pDiI = NULL;	}
}


INT CLcInputD::Query(char* sCmd, void* pData)
{
	if(0==_stricmp("Reset", sCmd))
	{
		OnReset();
		return 0;
	}

	return -1;
}


INT CLcInputD::Create(void* p1, void* p2, void* p3, void* p4)
{
	m_hInst= (HINSTANCE)p1;
	m_hWnd = (HWND)p2;
	m_pDev = (LPDIRECT3DDEVICE9)p3;


	if (FAILED(DirectInput8Create(	m_hInst,	DIRECTINPUT_VERSION,	IID_IDirectInput8,	(void **)&m_pDiI,	NULL)))
		return -1;
	
	
	// Keyboard
	if (FAILED(m_pDiI->CreateDevice(GUID_SysKeyboard, &m_pDiK, NULL)))
		return -1;
	
	if (FAILED(m_pDiK->SetDataFormat(&c_dfDIKeyboard)))
		return -1;
	
	if (FAILED(m_pDiK->SetCooperativeLevel(m_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
		return -1;

	
	// Mouse
	if (FAILED(m_pDiI->CreateDevice(GUID_SysMouse, &m_pDiM, NULL)))
		return -1;
	
	if (FAILED(m_pDiM->SetDataFormat(&c_dfDIMouse2)))
		return -1;
	
	if (FAILED(m_pDiM->SetCooperativeLevel(m_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY)))
		return -1;
	
	return 0;
}




INT CLcInputD::FrameMove()
{
	INT		i=0;
	POINT	pt;
	SHORT	Val = 0;

	memcpy(m_KeyOld, m_KeyCur, sizeof m_KeyOld);
	memcpy(m_BtnOld, m_BtnCur, sizeof m_BtnOld);
	memcpy(m_vMsOld, m_vMsCur, sizeof m_vMsOld);

	memset(m_KeyCur,		0, sizeof m_KeyCur);
	memset(m_BtnCur,		0, sizeof m_BtnCur);
	memset(m_KeyMap,		0, sizeof m_KeyMap);
	memset(m_BtnMap,		0, sizeof m_BtnMap);


	// Keyboard
	if (FAILED(m_pDiK->GetDeviceState(sizeof(m_KeyCur), (LPVOID)m_KeyCur)))
	{	
		if (FAILED(m_pDiK->Acquire()))
			return -1;
	}

	
	// Mouse
	DIMOUSESTATE2	MsCur={0};
	if (FAILED(m_pDiM->GetDeviceState(sizeof(MsCur), (LPVOID)&MsCur)))
	{
		if (FAILED(m_pDiM->Acquire()))
			return -1;
	}

	for(i=0; i<256; ++i)
	{
		Val = m_KeyCur[i] & 0x80;
		m_KeyCur[i] = (Val)? 1: 0;
	
		
		INT nOld = m_KeyOld[i];
		INT nCur = m_KeyCur[i];

		if		(0 == nOld && 1 ==nCur)	m_KeyMap[i] = INPUTST_DOWN	;		// Down
		else if	(1 == nOld && 0 ==nCur)	m_KeyMap[i] = INPUTST_UP	;		// Up
		else if	(1 == nOld && 1 ==nCur)	m_KeyMap[i] = INPUTST_PRESS	;		// Press
	}

	for(i=0; i<8; ++i)
	{
		Val = MsCur.rgbButtons[i];
		m_BtnCur[i] = (Val)? 1: 0;
	

		INT nOld = m_BtnOld[i];
		INT nCur = m_BtnCur[i];

		if		(0 == nOld && 1 ==nCur)	m_BtnMap[i] = INPUTST_DOWN	;		// Down
		else if	(1 == nOld && 0 ==nCur)	m_BtnMap[i] = INPUTST_UP	;		// Up
		else if	(1 == nOld && 1 ==nCur)	m_BtnMap[i] = INPUTST_PRESS	;		// Press
	}


	::GetCursorPos(&pt);
	::ScreenToClient( m_hWnd, &pt);

	m_vMsCur[0] = FLOAT(pt.x);
	m_vMsCur[1] = FLOAT(pt.y);

	if(-786420000.f<m_vMsCur[2] && m_vMsCur[2] <786420000.f)
		m_vMsCur[2] += FLOAT(MsCur.lZ);

	
	m_vDelta[0] = m_vMsCur[0] - m_vMsOld[0];
	m_vDelta[1] = m_vMsCur[1] - m_vMsOld[1];
	m_vDelta[2] = m_vMsCur[2] - m_vMsOld[2];

	m_pDev->SetCursorPosition( pt.x, pt.y, 0 );
	
	return 0;
}



void CLcInputD::SetMousePos(FLOAT* vcPos)
{
	OnReset();

	m_vMsCur[0] = vcPos[0];
	m_vMsCur[1] = vcPos[1];
	m_vMsCur[2] = vcPos[2];

	POINT	pt;
	pt.x= (INT)vcPos[0];
	pt.y= (INT)vcPos[1];

	m_pDev->SetCursorPosition( pt.x, pt.y, 0 );
	::ClientToScreen(m_hWnd, &pt);
	SetCursorPos(pt.x, pt.y);
}



void CLcInputD::OnReset()
{
	memset(m_KeyCur, 0, sizeof m_KeyCur);
	memset(m_KeyOld, 0, sizeof m_KeyOld);
	memset(m_KeyMap, 0, sizeof m_KeyMap);
	
	memset(m_BtnCur, 0, sizeof m_BtnCur);
	memset(m_BtnOld, 0, sizeof m_BtnOld);
	memset(m_BtnMap, 0, sizeof m_BtnMap);
	
	memset(m_vMsCur, 0, sizeof m_vMsCur);
	memset(m_vMsOld, 0, sizeof m_vMsOld);


	// 마우스는 현재 위치 유지
	POINT	pt;

	::GetCursorPos(&pt);
	::ScreenToClient( m_hWnd, &pt);

	m_vMsCur[0] = FLOAT(pt.x);
	m_vMsCur[1] = FLOAT(pt.y);
	m_vMsCur[2] = 0.f;

	m_pDev->SetCursorPosition( pt.x, pt.y, 0 );

	memcpy(m_vMsOld, m_vMsCur, sizeof m_vMsCur);
	memset(m_vDelta,        0, sizeof m_vDelta);
}