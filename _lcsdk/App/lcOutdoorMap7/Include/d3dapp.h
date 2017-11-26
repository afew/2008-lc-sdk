// Interface for the CD3DApplication class.
//
////////////////////////////////////////////////////////////////////////////////


#ifndef _D3DAPP_H_
#define _D3DAPP_H_


class CD3DApplication
{
protected:
	// Internal variables for the state of the app
	BOOL			m_bWindowed;
	BOOL			m_bActive;

	BOOL			m_bDeviceLost;
	BOOL			m_bMinimized;
	BOOL			m_bMaximized;
	BOOL			m_bIgnoreSizeChange;
	BOOL			m_bDeviceObjectsInited;
	BOOL			m_bDeviceObjectsRestored;
	
	// Internal variables used for timing
	BOOL			m_bFrameMoving;
	BOOL			m_bSingleStep;
	
	// Internal error handling function
	HRESULT			DisplayErrorMsg(HRESULT, DWORD);
	
	// Internal functions to manage and render the 3D scene
	static BOOL		ConfirmDeviceHelper(D3DCAPS9*, INT, void* );
	void			BuildPresentParamsFromSettings();
	BOOL			FindBestWindowedMode(BOOL, BOOL);
	BOOL			FindBestFullscreenMode(BOOL, BOOL);
	HRESULT			ChooseInitialD3DSettings();
	HRESULT			Initialize3DEnvironment();
	HRESULT			HandlePossibleSizeChange();
	HRESULT			Reset3DEnvironment();
	HRESULT			ToggleFullscreen();
	HRESULT			ForceWindowed();
	void			Cleanup3DEnvironment();
	HRESULT			Render3DEnvironment();
	virtual HRESULT	AdjustWindowForChange();
	virtual void	UpdateStats();
	
protected:
	// Main objects used for creating and rendering the 3D scene
	DWORD					m_dHicon;
	DWORD					m_dHmenu;
	DWORD					m_dHdevalt;
	DWORD					m_dHtoggle;
	DWORD					m_dHexit;
	DWORD					m_dHaccel;


	HINSTANCE				m_hInst;
	HWND					m_hWnd;												// The main app window
	HWND					m_hWndFocus;										// The D3D focus window (usually same as m_hWnd)
	HDC						m_hDC;												// Backbuffer DC
	HMENU					m_hMenu;											// App menu bar (stored here when fullscreen)
	LPDIRECT3D9				m_pD3D;												// The main D3D object
	LPDIRECT3DDEVICE9		m_pd3dDevice;										// The D3D rendering device
	LPDIRECT3DSURFACE9		m_pBackBuffer;										// Backbuffer point
	LPD3DXSPRITE			m_pd3dSprite;										// 2D Sprite
	DWORD					m_dClearMode;										// Clear Mode
	BOOL					m_bLoadingRnd;										// TRUE, if the Loading Scene Rendering

protected:	
	D3DPRESENT_PARAMETERS	m_d3dpp;											// Parameters for CreateDevice/Reset
	D3DCAPS9				m_d3dCaps;											// Caps for the device
	D3DSURFACE_DESC			m_d3dsdBackBuffer;									// Surface desc of the backbuffer

	DWORD					m_dwCreateFlags;									// Indicate sw or hw vertex processing
	DWORD					m_dwWindowStyle;									// Saved window style for mode switches
	RECT					m_rcWindowBounds;									// Saved window bounds for mode switches
	RECT					m_rcWindowClient;									// Saved client area size for mode switches
	BOOL					m_bLoadingApp;										// TRUE, if the app is loading
	

	// Variables for timing
	FLOAT					m_fTime;											// Current time in seconds
	FLOAT					m_fElapsedTime;										// Time elapsed since last frame
	FLOAT					m_fFPS;												// Instanteous frame rate
	TCHAR					m_strDeviceStats[90];								// String to hold D3D device stats
	TCHAR					m_strFrameStats[90];								// String to hold frame stats
	
	// Overridable variables for the app
	TCHAR					m_sClsName[256];									// Title for the app's window
	DWORD					m_dScnPosX;											// Screen Position
	DWORD					m_dScnPosY;											// Screen Position

	DWORD					m_dwCreationW;										// Width used to create window
	DWORD					m_dwCreationH;										// Height used to create window
	BOOL					m_bShowCursorWhenFullscreen;						// Whether to show cursor when fullscreen
	BOOL					m_bClipCursorWhenFullscreen;						// Whether to limit cursor pos when fullscreen
	BOOL					m_bStartFullscreen;									// Whether to start up the app in fullscreen mode

protected:
	HRESULT		ConfirmDevice(D3DCAPS9*,DWORD, DWORD );
	HRESULT		OneTimeSceneInit();
	HRESULT		FinalCleanup();

protected:
	// Overridable functions for the 3D scene created by the app
	virtual HRESULT			Init()			{ return S_OK; }
	virtual HRESULT			Destroy()		{ return S_OK; }

	virtual HRESULT			Restore()		{ return S_OK; }
	virtual HRESULT			Invalidate()	{ return S_OK; }

	virtual HRESULT			FrameMove()		{ return S_OK; }
	virtual HRESULT			Render()		{ return S_OK; }

public:
	CD3DApplication();
	virtual ~CD3DApplication();

	virtual HRESULT			Create(HINSTANCE);
	virtual INT				Run();
	virtual void			Pause(BOOL);
	virtual LRESULT			MsgProc(HWND,UINT,WPARAM,LPARAM);

public:
	LPDIRECT3DDEVICE9		GetDxDevice();
	LPD3DXSPRITE			GetDxSprite();
	LPDIRECT3DSURFACE9		GetDxSurface();

	void	SetClsName(char* sName);
	BOOL	IsWindow()		;
	BOOL	IsActive()		;
	BOOL	GetDxLoadingRnd();

	HWND	GetDxHwnd()		;
	DWORD	GetDxClearMode();
	UINT	GetDxBackW()	;
	UINT	GetDxBackH()	;
	DWORD	GetDxBackF()	;

	DWORD	GetDxDepthFMT()	;
};

#endif

