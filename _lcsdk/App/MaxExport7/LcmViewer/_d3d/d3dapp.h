// Application class for the Direct3D samples framework library.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _D3DAPP_H_
#define _D3DAPP_H_


class CD3DApplication
{
public:
    CD3DEnum		m_d3dEnum;
    CD3DSetting		m_d3dSettings;

    // Internal variables for the state of the app
    bool			m_bWindowed;
    bool			m_bActive;
    bool			m_bDeviceLost;
    bool			m_bMinimized;
    bool			m_bMaximized;
    bool			m_bIgnoreSizeChange;
    bool			m_bDeviceObjectsInited;
    bool			m_bDeviceObjectsRestored;

    // Internal variables used for timing
    bool			m_bFrameMoving;
    bool			m_bSingleStep;

    // Internal error handling function
    HRESULT DisplayErrorMsg( HRESULT hr, DWORD dwType );

    // Internal functions to manage and render the 3D scene
    static bool ConfirmDeviceHelper( D3DCAPS9*,  DWORD, DWORD);
    void		BuildPresentParamsFromSettings();
    bool		FindBestWindowedMode( bool bRequireHAL, bool bRequireREF );
    bool		FindBestFullscreenMode( bool bRequireHAL, bool bRequireREF );
    HRESULT		ChooseInitialD3DSettings();
    HRESULT		Initialize3DEnvironment();
    HRESULT		HandlePossibleSizeChange();
    HRESULT		Reset3DEnvironment();
    HRESULT		ToggleFullscreen();
    HRESULT		ForceWindowed();

    void		Cleanup3DEnvironment();
    HRESULT		Render3DEnvironment();

    virtual HRESULT	AdjustWindowForChange();
    virtual void	UpdateStats();

protected:
	DWORD					m_dRscIcon		;
	DWORD					m_dRscMenu		;
	DWORD					m_dRscAccel		;
	DWORD					m_dRscDevice	;
	DWORD					m_dRscToggle	;
	DWORD					m_dRscExit		;

	HINSTANCE				m_hInst			;	// Window Instance
    HWND					m_hWnd			;	// The main app window
    HWND					m_hWndFocus		;	// The D3D focus window (usually same as m_hWnd)
    HMENU					m_hMenu			;	// App menu bar (stored here when fullscreen)

	D3DPRESENT_PARAMETERS	m_d3dpp			;	// Parameters for CreateDevice/Reset
    LPDIRECT3D9				m_pD3D			;	// The main D3D object
	LPDIRECT3DDEVICE9		m_pd3dDevice	;	// The D3D rendering device
	LPDIRECT3DSURFACE9		m_pd3dSfBack	;	// BackBuffer

    D3DCAPS9				m_d3dCaps			;	// Caps for the device
    D3DSURFACE_DESC			m_d3dsdBackBuffer	;	// Surface desc of the backbuffer
    DWORD					m_dwCreateFlags		;	// Indicate sw or hw vertex processing
    DWORD					m_dwWindowStyle		;	// Saved window style for mode switches
    RECT					m_rcWindowBounds	;	// Saved window bounds for mode switches
    RECT					m_rcWindowClient	;	// Saved client area size for mode switches

    // Variables for timing
    FLOAT					m_fTime				;	// Current time in seconds
    FLOAT					m_fElapsedTime		;	// Time elapsed since last frame
    FLOAT					m_fFPS				;	// Instanteous frame rate
    TCHAR					m_strDeviceStats[90];	// String to hold D3D device stats
    TCHAR					m_strFrameStats[90]	;	// String to hold frame stats

    // Overridable variables for the app
	BOOL					m_bLoadingApp		;	// TRUE, if the app is loading

    char					m_sClassName[256]	;	// Title for the app's window
    DWORD					m_dwCreationWidth	;	// Width used to create window
    DWORD					m_dwCreationHeight	;	// Height used to create window

	DWORD					m_dScnX				;	// Screen Position X
    DWORD					m_dScnY				;	// Screen Position Y

    bool					m_bShowCursorWhenFullscreen	;	// Whether to show cursor when fullscreen
	bool					m_bShowCursorWhenWindow		;	// Whether to show cursor when fullscreen
    bool					m_bClipCursorWhenFullscreen	;	// Whether to limit cursor pos when fullscreen
    bool					m_bStartFullscreen			;	// Whether to start up the app in fullscreen mode

    // Overridable functions for the 3D scene created by the app
    virtual HRESULT ConfirmDevice(D3DCAPS9*,DWORD, DWORD);
    virtual HRESULT OneTimeSceneInit();
    virtual HRESULT Init()             { return S_OK; }
    virtual HRESULT Restore()          { return S_OK; }
    virtual HRESULT FrameMove()        { return S_OK; }
    virtual HRESULT Render()           { return S_OK; }
    virtual HRESULT Invalidate()       { return S_OK; }
    virtual HRESULT Destroy()          { return S_OK; }
    virtual HRESULT FinalCleanup()     { return S_OK; }

public:
	CD3DApplication();

    virtual HRESULT Create( HINSTANCE hInstance );
    virtual INT     Run();
    virtual LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
    virtual void    Pause( bool bPause );
};



#endif


