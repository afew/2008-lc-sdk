// Interface for the CMain class.
//
////////////////////////////////////////////////////////////////////////////////


#ifndef _MAIN_H_
#define _MAIN_H_


class CMain : public CD3DApplication
{
public:
	char			m_sMsg[512]	;
	LPD3DXFONT		m_pDxFnt	;
	ILcInput*		m_pInput	;				// Keyboard Mouse
	ILcxCam*		m_pCamG		;				// for Global World
	ILcxCam*		m_pCamO		;				// for Object World
	ILcxtObj*		m_pGrid		;				// Grid

	ILcxtObj*		m_pBill		;				// Billboard

	ILcXmsh*		m_pSkyMsh	;				// Sky Dome Mesh

	ILcxTbl*		m_pTbTex	;
	ILcxTbl*		m_pTbMdl	;

public:
	ILcxmFld*		m_pFld		;
	CWndWrk			m_WndWrk	;
	CWndTool		m_WndTool	;

	TswpWn			m_SwpWn		;				// Swap Chain Window
	DVWP			m_Vp		;				// View port

	LPD3DXEFFECT	m_pEffct	;				// For Effect

public:
	INT				m_bMnLgt	;
	INT				m_bMnFog	;
	INT				m_bMnFill	;
	INT				m_bMnCam	;
	INT				m_bMnLcl	;
	INT				m_bMnFrame	;
	INT				m_bMnSky	;
	INT				m_bMnBndBox	;
	INT				m_bCulling	;

public:
	CMain();
	virtual ~CMain();

	virtual HRESULT		Init();
	virtual HRESULT		Destroy();

	virtual HRESULT		Restore();
	virtual HRESULT		Invalidate();

	virtual HRESULT		FrameMove();
	virtual HRESULT		Render();

	virtual LRESULT		MsgProc(HWND, UINT, WPARAM, LPARAM);

	static BOOL WINAPI	AboutPrc(HWND, UINT, WPARAM, LPARAM);
};


// Extern
extern	CMain*		g_pApp	;													// Main application

#ifndef GMAIN
#define GMAIN		g_pApp
#endif


#define GHWND		g_pApp->GetDxHwnd()
#define GDEVICE		g_pApp->GetDxDevice()
#define GSPRITE		g_pApp->GetDxSprite()

#define GINPUT		g_pApp->m_pInput

#define TBTX		g_pApp->m_pTbTex
#define TBMDB		g_pApp->m_pTbMdl

#define GCAMGNL		g_pApp->m_pCamG
#define GCAMOBJ		g_pApp->m_pCamO
#define GFIELD		g_pApp->m_pFld


#endif


