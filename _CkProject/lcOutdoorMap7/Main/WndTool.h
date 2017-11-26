// Interface for the CWndTool class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _WndTool_H_
#define _WndTool_H_

class CWndTool
{
public:
	HWND		m_hWnd		;
	HWND		m_hWPrn		;

	HWND		m_hPanS[8]	;
	HWND		m_hPanD[4]	;

	TswpWn		m_SwpSrc[8]	;
	TswpWn		m_SwpDst[4]	;

	Lcxmo::lsMpTex	m_vTexS	;
	Lcxmo::lsMpTex	m_vTexD	;

	INT			m_bShow		;

public:
	CLSS_DLG_DECLEAR( CWndTool );

	INT		Init();
	void	Destroy();

	INT		Restore();
	void	Invalidate();

	INT		FrameMove();
	void	Render();

	INT		Create(HWND hWnd);
	LRESULT	MsgPrc(HWND, UINT, WPARAM, LPARAM);

	void	ShowWindow(int _ishw = SW_SHOW);

	void	ReadDataSrc();
	void	ReadDataDst();

	void	SetTxSrc(INT nOrg);
	void	SetTxDst(INT nDst);
};

#endif