// Interface for the CWndLght class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _WNDLGHT_H_
#define _WNDLGHT_H_


class CWndLght
{
public:
	HWND			m_hWnd		;
	HWND			m_hWPrn		;

public:
	INT				m_iLg		;															// Lighting Num
	ILcxmT::MpLgt	m_pLg[MAX_LCX_MAP_LIGHT]	;
	
protected:
	INT			m_nLgt		;
	HWND		m_hWCb		;
	HWND		m_hWdf		;
	HWND		m_hWam		;
	HWND		m_hWsp		;

	COLORREF	m_crT[3][32];

public:
	CLSS_DLG_DECLEAR( CWndLght );

	INT		Init();
	void	Destroy();

	INT		Create(HWND hWnd);
	LRESULT	MsgPrc(HWND, UINT, WPARAM, LPARAM);
	
	void			ShowWindow(int _ishw = SW_SHOW);

	INT				GetLgtCnt()	{	return m_iLg;	}
	ILcxmT::MpLgt*	GetLgtPnt()	{	return m_pLg;	}

	void	SetPickColor(DWORD&	clrOut,DWORD& clrIn,INT nID, BOOL IsBGR=FALSE);

	void	OnChangeMtlDF(HWND hWnd, INT nID);
	void	OnChangeMtlAM(HWND hWnd, INT nID);
	void	OnChangeMtlSP(HWND hWnd, INT nID);

	void	FillData();
};

#endif