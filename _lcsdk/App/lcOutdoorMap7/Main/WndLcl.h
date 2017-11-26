// Interface for the CWndLcl class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _WNDLCL_H_
#define _WNDLCL_H_

class CWndLcl
{
public:
	HWND			m_hWnd		;
	HWND			m_hWPrn		;

public:
	char			m_sN[128];													// Name

	INT				m_iNx;														// Number of tile for Width
	INT				m_iWx;														// Width of tile for x;
	FLOAT			m_fUV;
	VEC3			m_vcP;

public:
	CLSS_DLG_DECLEAR( CWndLcl );

	INT		Init();
	void	Destroy();

	INT		Create(HWND hWnd);
	LRESULT	MsgPrc(HWND, UINT, WPARAM, LPARAM);
	
	void	ShowWindow(int _ishw = SW_SHOW);
};

#endif