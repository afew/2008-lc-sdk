// Interface for the CWndFog class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _WNDFOG_H_
#define _WNDFOG_H_

class CWndFog
{
public:
	HWND		m_hWnd	;
	HWND		m_hWPrn	;

public:
	HWND		m_hWfc	;

	Lcxmo::MpFog m_Fog	;													// Fog
	
	COLORREF	m_crT[32];

public:
	CLSS_DLG_DECLEAR( CWndFog );

	INT		Init();
	void	Destroy();

	INT		Create(HWND hWnd);
	LRESULT	MsgPrc(HWND, UINT, WPARAM, LPARAM);
	
	void	ShowWindow(int _ishw = SW_SHOW);

	Lcxmo::MpFog GetFog()	{	return	m_Fog	;	}

protected:
	void	SetPickColor(DWORD&	clrOut,DWORD& clrIn,INT nID, BOOL IsBGR=FALSE);
	void	OnChangeColor(HWND hWnd, INT nID);
};

#endif