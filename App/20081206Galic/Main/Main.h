// Interface for the CMain class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _Main_H_
#define _Main_H_


class CMain : public CApp3D
{
public:
	CMain();
	virtual ~CMain();

public:
	virtual INT		Init();
	virtual void	Destroy();

	virtual INT		Restore();
	virtual void	Invalidate();

	virtual INT		FrameMove();
	virtual	INT		Render();

public:
	virtual INT		MsgProc(HWND, UINT, WPARAM, LPARAM);
	virtual INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0);
	virtual INT		Run();

protected:
	ILcFont*		m_pFont		;
	
	// Main Instance for Programming
	ILcDev*			m_pDevSys	;
	LcDev			m_SysInf	;
	HINSTANCE		m_hInst		;				// Window Instance
	HWND			m_hWnd		;				// Window Handle
	PDEV			m_pDevD3D	;				// IDirect3DDevice9*
	PDSF			m_pBck		;				// BackBuffer Surface

	ILcDevSprite*	m_pDev2D	;				// 2D Sprite
	ILcInput*		m_pInput	;				// Input
	ILcxCamM*		m_pCamM		;				// Multi Camera
	ILcIme*			m_pIme		;				// Hangul IME

public:
	HINSTANCE		GetAppInst();				// Instance
	HWND			GetAppHwnd();				// Window Handle
	PDEV			GetAppDxDev();				// Device
	PDSF			GetAppDxBackBuffer();		// BackBuffer Surface

protected:
	ILcLsc*			m_pLsc		;

protected:
	INT				m_ePhCur	;												// Current Phase
	INT				m_ePhOld	;												// Old Phase

	INT				m_nGmPhs	;												// Total Game Phase
	IGmPhase**		m_pGmPhs	;												// Game Phase Class

public:
	void			SetPhase(INT nPhase);
};

extern CMain*	g_pApp;

#ifndef GMAIN
#	define GMAIN	g_pApp
#endif

#endif

