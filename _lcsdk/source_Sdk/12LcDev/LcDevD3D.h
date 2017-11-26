// Interface for the CLcDevD3D class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcDevD3D_H_
#define _LcDevD3D_H_


#pragma warning(disable : 4786)

#include <vector>


class CLcDevD3D : public CLcDevBase
{
protected:
	std::vector<TCHAR*>	m_vScFile	;						// Lua Script Files.

	void*		m_pDev2D			;						// 2D Sprite Pointer
	void*		m_pInput			;						// Input Pointer
	void*		m_pCam				;						// Camera Pointer
	void*		m_pNet				;						// Network Pointer
	void*		m_pIme				;						// Ime Pointer
	void*		m_pCmd				;						// 비형식 Message and Command를 처리하기 위한 명령어

	void*		m_pUser1			;						// User Data Pointer 1
	void*		m_pUser2			;						// User Data Pointer 2
	void*		m_pUser3			;						// User Data Pointer 3
	void*		m_pUser4			;						// User Data Pointer 4

	HANDLE		m_ConsoleH			;						// Console Window Handle
	BOOL		m_ConsoleB			;
	INT			m_ConsoleR			;
	FILE*		m_ConsoleF			;
	DWORD		m_ConsoleW			;

	DOUBLE		m_fTimeSrt			;						// Start Time


protected:													// Function pointers for Host Program.
	INT		(*m_pFncMsgProc)(HWND,UINT,WPARAM,LPARAM);		// For Window Message Procedure.
	INT		(*m_pFncInit)();								// For Init.
	INT		(*m_pFncDestroy)();								// For Destroy.
	INT		(*m_pFncRestore)();								// For Restore.
	INT		(*m_pFncInvalidate)();							// For Invalidate.
	INT		(*m_pFncFrameMove)();							// For FrameMove.
	INT		(*m_pFncRender)();								// For Render.


public:
	CLcDevD3D();
	virtual ~CLcDevD3D();
	
	virtual INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0);

	virtual INT		Init();
	virtual void	Destroy();

	virtual INT		Restore();
	virtual void	Invalidate();

	virtual INT		FrameMove();
	virtual INT		Render();

	virtual	INT		Run();

	virtual INT		Query(char* sCmd, void* pData);


	virtual INT		BeginScene();
	virtual INT		EndScene();
	virtual INT		Clear(DWORD Count,CONST RECT* pRects,DWORD Flags,DWORD Color,float Z,DWORD Stencil);

public:
	LRESULT			MsgProc(HWND,UINT,WPARAM,LPARAM);
	static LRESULT	WINAPI WndProc(HWND,UINT,WPARAM,LPARAM);

public:	
	HINSTANCE			GetHinst();
	HWND				GetHwnd();
	LPDIRECT3DDEVICE9	GetDevice();
	LPDIRECT3DSURFACE9	GetBufferSurface();

	void*		GetSprite	()	;						// 2D Sprite Pointer
	void*		GetInput	()	;						// Input Pointer
	void*		GetCamera	()	;						// Camera Pointer
	void*		GetNetwork	()	;						// Network Pointer
	void*		GetIme		()	;						// IME Pointer
	void*		GetCommand	()	;						// Command Pointer

	void*		GetUserData1()	;						// User Data Pointer 1
	void*		GetUserData2()	;						// User Data Pointer 2
	void*		GetUserData3()	;						// User Data Pointer 3
	void*		GetUserData4()	;						// User Data Pointer 4

	INT			SetSprite	(void*)	;
	INT			SetInput	(void*)	;
	INT			SetCamera	(void*)	;
	INT			SetNetwork	(void*)	;
	INT			SetIme		(void*)	;
	INT			SetCommand	(void*)	;

	INT			SetUserData1(void*)	;
	INT			SetUserData2(void*)	;
	INT			SetUserData3(void*)	;
	INT			SetUserData4(void*)	;


protected:
	HINSTANCE	m_hInst		;						// Window Instance
	HWND		m_hWnd		;						// Main Window Handle

	TCHAR		m_sCls[260]	;						// Window Class Name
	TCHAR		m_sWin[260]	;						// Window Title
	
	DWORD		m_dStyle	;						// Window Style
	DWORD		m_dIcon		;						// Icon ID
	DWORD		m_dAccl		;						// Accelator ID
	DWORD		m_dTggl		;						// Toggle Key
	DWORD		m_dExit		;						// Exit Key

	INT			m_iScnW		;						// Screen Width
	INT			m_iScnH		;						// Screen Height
	INT			m_iPosX		;						// Screen Position X
	INT			m_iPosY		;						// Screen Position Y

	BOOL		m_bStartFull;						// Is Start Full Mode

	BOOL		m_bRender	;						// Is Rendering
	BOOL		m_bActive	;						// FALSE
	BOOL		m_bCusor	;						// Show Cusor
	BOOL		m_bShowStats;						// Show Stats


	// Variables for timing
	DOUBLE		m_dbFPS			;					// Frame Per Second.
	DOUBLE		m_fTime			;					// Current time in seconds
	DOUBLE		m_fTimeNoPause	;					// Current time. no pause
	DOUBLE		m_fElapsedTime	;					// Time elapsed since last frame
	DOUBLE		m_fTimeAv		;					// Time Average
	DOUBLE		m_fMaxFrame		;					// Boundary Frame for Frame Synchronization.


	DWORD					m_dClrMod	;			// Device Clear Mode.
	DWORD					m_dClrClr	;			// Device Clear Color.
	LPDIRECT3D9				m_pD3D		;			// Direct3D
	LPDIRECT3DDEVICE9		m_pDev		;			// Direct3D Device
	D3DPRESENT_PARAMETERS	m_d3dpp		;			// D3D Present Parameters
	LPDIRECT3DSURFACE9		m_pBcBuf	;			// BackBuffer Surface

	D3DCAPS9				m_d3dCaps	;			// Capabilitys for the device
	D3DSURFACE_DESC			m_d3dBcDsc	;			// Surface desc of the backbuffer

	TCHAR					m_sStatsDevice[128]	;	// String to hold D3D device stats
	TCHAR					m_sStatsFrame[128]	;	// String to hold frame stats



// Source Code from D3DXSamples CD3DApplication Wizard Code.
protected:
	CD3DEnum	m_d3dEnum			;
	CD3DSettings m_d3dSettings		;

	// Internal variables for the state of the app
	BOOL		m_bWindowed			;
	BOOL		m_bDeviceLost		;
	BOOL		m_bMinimized		;
	BOOL		m_bMaximized		;
	BOOL		m_bIgnoreSizeChange	;
	BOOL		m_bDeviceInited		;
	BOOL		m_bDeviceRestored	;

	// Internal variables used for timing
	BOOL		m_bFrameMoving		;
	BOOL		m_bSingleStep		;

	// 기타..
	BOOL		m_bLoadingApp		;											// TRUE, if the app is loading
	BOOL		m_bLoadingRnd		;											// TRUE, if the Loading Scene Rendering

	BOOL		m_bDragFullUse		;											// FALSE, Child창이 마우스로 끄는 동안 창내용 표시. TRUE이면 속도 저하.
	BOOL		m_bDragFullWin		;											// 이전 설정이 Child창이 마우스로 끄는 동안 창내용 표시인가?
	BOOL		m_bAntialias		;											// Antialias default is TRUE


	// Internal functions to manage and render the 3D scene
	static BOOL	ConfirmDeviceHelper(D3DCAPS9*, INT nType, void* );
	HRESULT		ConfirmDevice(D3DCAPS9*,DWORD,D3DFORMAT,D3DFORMAT);

	void		BuildPresentParamsFromSettings();
	BOOL		FindBestWindowedMode( BOOL, BOOL);
	BOOL		FindBestFullscreenMode( BOOL, BOOL);
	HRESULT		ChooseInitialD3DSettings();
	HRESULT		Initialize3DEnvironment();
	HRESULT		HandlePossibleSizeChange();
	HRESULT		Reset3DEnvironment();
	HRESULT		ToggleFullscreen();
	HRESULT		ForceWindowed();
	HRESULT		UserSelectNewDevice();
	void		Cleanup3DEnvironment();
	HRESULT		Render3DEnvironment();
	HRESULT		AdjustWindowForChange();
	void		UpdateStats();

	HRESULT		DisplayErrorMsg( HRESULT, DWORD);								// Internal error handling function
	INT			MonitorMaxHz(INT nScnW, INT nScnH, INT nBits);					// Max Hz for screen width and Height

	TCHAR*		D3DFormatToString( D3DFORMAT format, BOOL bWithPrefix=TRUE);
	void		D3DFrameStatsToString();


	// Main objects used for creating and rendering the 3D scene
	HWND		m_hWndFocus					;									// The D3D focus window (usually same as m_hWnd)
	HMENU		m_hMenu						;									// App menu bar (stored here when fullscreen)

	DWORD		m_dwCreateFlags				;									// Indicate sw or hw vertex processing
	RECT		m_rcWindowBounds			;									// Saved window bounds for mode switches
	RECT		m_rcWindowClient			;									// Saved client area size for mode switches

	BOOL		m_bShowCursorWhenFullscreen	;									// Whether to show cursor when fullscreen
	BOOL		m_bClipCursorWhenFullscreen	;									// Whether to limit cursor pos when fullscreen

public:
	HRESULT		OneTimeSceneInit();
	void		Pause(BOOL bPause);
	
	HRESULT		FinalCleanup();
	TCHAR*		GetDeviceStates();
	TCHAR*		GetFrameStates();
};


#endif


