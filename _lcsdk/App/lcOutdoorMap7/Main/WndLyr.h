// Interface for the CWndLyr class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _WNDLYR_H_
#define _WNDLYR_H_

class CWndLyr
{
public:
	HWND		m_hWnd		;
	HWND		m_hWPrn		;

	TswpWn		m_SwpOrg	;
	TswpWn		m_SwpDst	;

	HWND		m_hPanO		;
	HWND		m_hPanD		;

	HWND		m_hOrg		;
	HWND		m_hDst		;

	INT			m_nOrg		;
	INT			m_nDst		;

	lsStr		m_vStrOrg	;
	lsStr		m_vStrDst	;

	DIMG		m_ImgO		;
	DIMG		m_ImgD		;

	PDTX		m_pTxO		;
	PDTX		m_pTxD		;

	INT			m_bShow		;

public:
	CLSS_DLG_DECLEAR( CWndLyr );

	INT		Init();
	void	Destroy();

	INT		Restore();
	void	Invalidate();

	INT		FrameMove();
	void	Render();

	INT		Create(HWND hWnd);
	LRESULT	MsgPrc(HWND, UINT, WPARAM, LPARAM);
	
	void	ShowWindow(int _ishw = SW_SHOW);

	void	ReadDataOrg();
	void	ReadDataDst();

	void	SetTxOrg(INT	nOrg);
	void	SetTxDst(INT	nDst);

	void	AddLyr();
	void	DelLyr();

	char*	GetStrDst(INT nIdx);
	INT		GetSizeDst();

	INT		GetDstIdx()		{	return m_nDst;		}
};

#endif