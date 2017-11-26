// Interface for the CMain class.
//
////////////////////////////////////////////////////////////////////////////////


#ifndef _MAIN_H_
#define _MAIN_H_


class CMain : public CD3DApplication
{
public:
	ID3DXFont*		m_pD3DXFont	;
	CLcInput*		m_pInput	;
	CLcCam*			m_pCam		;
	CLcGrid*		m_pGrid		;
	
	CLcSplt*		m_pSplt		;

public:
	CMain();

	virtual HRESULT Init();
	virtual HRESULT Destroy();

	virtual HRESULT Restore();
	virtual HRESULT Invalidate();
	
	virtual HRESULT FrameMove();
	virtual HRESULT Render();
	
	HRESULT RenderText();
	
public:
	LRESULT MsgProc( HWND, UINT, WPARAM, LPARAM);
};


extern CMain*	g_pApp;
#define GMAIN	g_pApp


#endif