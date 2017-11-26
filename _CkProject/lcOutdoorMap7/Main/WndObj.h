// Interface for the CWndObj class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _WndObj_H_
#define _WndObj_H_

class CWndObj
{
public:
	HWND		m_hWnd		;
	HWND		m_hWPrn		;

protected:
	void*		m_pObj		;													// Object Point
	D3DXVECTOR3	m_vcR		;													// Rotation
	D3DXVECTOR3	m_vcS		;													// Scaling
	D3DXVECTOR3	m_vcT		;													// Translation

	
public:
	CLSS_DLG_DECLEAR( CWndObj );

	INT		Init();
	void	Destroy();

	INT		Create(HWND hWnd);
	LRESULT	MsgPrc(HWND, UINT, WPARAM, LPARAM);
	
	void	ShowWindow(int _ishw = SW_SHOW);

public:
	void	SetObj(void* pObj);

	D3DXVECTOR3*	GetRotation()	{	return &m_vcR;	}
	D3DXVECTOR3*	GetScaling()	{	return &m_vcS;	}
	D3DXVECTOR3*	GetPosition()	{	return &m_vcT;	}

protected:
	void	SetVal();
};

#endif