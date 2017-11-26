// Interface for the CWndWrk class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _WNDWRK_H_
#define _WNDWRK_H_


#define TL_BTN_MX		40


class CWndWrk
{
public:
	HWND		m_hWnd		;

	HIMAGELIST	m_gWrk		;													// Image List for Tab and List control
	TswpWn		m_SwpWn		;													// Swap Chain Window

	HWND		m_hGrp		;													// Group
	HWND		m_hElm		;													// Object
	HWND		m_hLst		;													// List

	INT			m_nGrp		;
	INT			m_nElm		;
	INT			m_nLst		;
	Lcm::ILcmMdl*		m_pMdB		;

	INT			m_iRng		;
	FLOAT		m_fDH		;
	INT			m_bAc		;												//

protected:
	HBITMAP		m_bmFld[20]	;

	lsStr		m_vStrGrp	;
	lsStr		m_vStrElm	;
	lsStr		m_vStrLst	;

protected:
	HWND		m_hCol		;
	HWND		m_hTip		;

	DWORD		m_dC		;													// color
	COLORREF	m_crT[32]	;
	HBITMAP		m_bmBtn[TL_BTN_MX]	;

public:
	BOOL		m_bTool[TL_BTN_MX]	;

public:
	CWndFld		m_WndFld	;
	CWndLcl		m_WndLcl	;
	
	CWndLyr		m_WndLyr	;
	CWndMtrl	m_WndMtrl	;
	CWndLght	m_WndLght	;
	CWndFog		m_WndFog	;
	CWndObj		m_WndObj	;

public:
	CLSS_DLG_DECLEAR( CWndWrk );

	INT		Init();
	void	Destroy();

	INT		Create(HWND hWnd);
	LRESULT	MsgPrc(HWND, UINT, WPARAM, LPARAM);
	
	INT		Restore();
	void	Invalidate();

	INT		FrameMove();
	void	Render();


	void	ReadDataGrp();
	void	ReadDataElm();
	void	ReadDataLst();


public:
	INT		GetRng()	{	return m_iRng;	}
	INT		GetLstIdx()	{	return m_nLst;	}
	INT		SetMdB(WORD wI1, WORD wI2, WORD wI3);
	Lcm::ILcmMdl*	GetMdB()	{	return m_pMdB;	}


public:
	void	SetPickColor(DWORD dC, BOOL IsBGR=0);
	
	FLOAT	GetDirection();
	INT		IsFlat()	{	return (m_bTool[2]*1 - m_bTool[ 3]*1);	}
	BOOL	IsHeight()	{	return (m_bTool[0]||m_bTool[1]||m_bTool[2]||m_bTool[3]);}
	BOOL	UseBrush()	{	return m_bTool[4];						}
	BOOL	UseSpoid()	{	return m_bTool[5];						}
	BOOL	UseMdB()	{	return m_bTool[10];						}
	BOOL	UseTile()
	{
		return m_bTool[ 9];// && m_WndLyr.m_hWnd;
	}

	void	SetColor(DWORD dc);
	DWORD	GetColor()	{	return m_dC;	}
	void	SetActWnd(INT nId);
};

#endif