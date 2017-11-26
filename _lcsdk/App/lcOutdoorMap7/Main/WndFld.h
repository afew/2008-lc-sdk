// Interface for the CWndFld class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _WNDFLD_H_
#define _WNDFLD_H_

class CWndFld
{
public:
	HWND			m_hWnd	;
	HWND			m_hWPrn	;

public:
	ILcxmT::MpInf	m_Inf	;
	ILcxmT::MpEnv	m_Env	;
	
public:
	CLSS_DLG_DECLEAR( CWndFld );

	INT		Init();
	void	Destroy();

	INT		Create(HWND hWnd);
	LRESULT	MsgPrc(HWND, UINT, WPARAM, LPARAM);
	
	void	ShowWindow(int _ishw = SW_SHOW);
};

#endif