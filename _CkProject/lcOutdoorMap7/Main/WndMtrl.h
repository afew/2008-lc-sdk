// Interface for the CWndMtrl class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _WNDMTRL_H_
#define _WNDMTRL_H_

class CWndMtrl
{
public:
	HWND		m_hWnd		;
	HWND		m_hWPrn		;

public:
	Lcxmo::MpMtl m_Mtl		;													// Default Material
	DWORD		m_dAL		;

protected:
	HWND		m_hWal		;
	HWND		m_hWdf		;
	HWND		m_hWam		;
	HWND		m_hWsp		;
	HWND		m_hWem		;

	COLORREF	m_crT[5][32];

public:
	CLSS_DLG_DECLEAR( CWndMtrl );

	INT		Init();
	void	Destroy();

	INT		Create(HWND hWnd);
	LRESULT	MsgPrc(HWND, UINT, WPARAM, LPARAM);
	
	void	ShowWindow(int _ishw = SW_SHOW);

	Lcxmo::MpMtl	GetMtrl()	{	return m_Mtl;	}
	DWORD	GetAmLgt()	{	return m_dAL;	}

	void	SetPickColor(DWORD&	clrOut,DWORD& clrIn,INT nID, BOOL IsBGR=FALSE);

	void	OnChangeMtlAL(HWND hWnd, INT nID);
	void	OnChangeMtlDF(HWND hWnd, INT nID);
	void	OnChangeMtlAM(HWND hWnd, INT nID);
	void	OnChangeMtlSP(HWND hWnd, INT nID);
	void	OnChangeMtlEM(HWND hWnd, INT nID);
};

#endif