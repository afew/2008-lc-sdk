// Implementation of the CWndLcl class.
//
////////////////////////////////////////////////////////////////////////////////


#include "../_StdAfx.h"


CLSS_DLG_DEFINE(CWndLcl, MsgPrc);


INT CWndLcl::Init()
{
	m_hWnd	= 0;
	m_hWPrn	= 0;

	m_iNx	= 32;
	m_iWx	= 32;
	m_fUV	= 2.f;

	m_vcP.x	= 0;
	m_vcP.y	= 0;
	m_vcP.z	= 0;

	return 0;
}


INT CWndLcl::Create(HWND hWnd)
{
	if(m_hWnd)
		return 0;

	m_hWPrn= hWnd;
	
	HINSTANCE hInst = GetModuleHandle(NULL);
	m_hWnd = CreateDialog(hInst,MAKEINTRESOURCE(IDD_LCL), m_hWPrn, CLSS_DLG_WNDPROC(CWndLcl));

	RECT rt1;
	RECT rt2;
	INT		iWidth;
	INT		iHeight;
	INT		iX;
	INT		iY;

	GetWindowRect(m_hWPrn, &rt1);
	GetWindowRect(m_hWnd, &rt2);
	
	iWidth = rt2.right - rt2.left;
	iHeight=  rt2.bottom- rt2.top;

	iX = rt1.left + 80;
	iY = rt1.top  + 250;
	
	MoveWindow(m_hWnd, iX, iY, iWidth, iHeight, TRUE);

	ILcxmT::MpInf		Inf;
	GFIELD->Query("Get Info", &Inf);

	m_iWx = Inf.iExt/m_iNx;

	SetDlgItemText(m_hWnd,	IDC_LCL_NAME, "Town 1");
	SetDlgItemInt(m_hWnd,	IDC_LCL_NUM,	m_iNx, 0);
	SetDlgItemInt(m_hWnd,	IDC_LCL_WDT,	m_iWx, 0);
	SetDlgItemFlt(m_hWnd,	IDC_LCL_UV,		m_fUV, 0);

	SetDlgItemFlt(m_hWnd,	IDC_LCL_POS_X, m_vcP.x, 0);
	SetDlgItemFlt(m_hWnd,	IDC_LCL_POS_Z, m_vcP.z, 0);
	
	return 0;
}

void CWndLcl::ShowWindow(int _ishw)
{
	::ShowWindow(m_hWnd, _ishw);
}


void CWndLcl::Destroy()
{
	SAFE_DESTROY_WIN(m_hWnd);
}

LRESULT CWndLcl::MsgPrc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WPARAM		wparHi = HIWORD(wParam);
	WPARAM		wparLo = LOWORD(wParam);

	switch( uMsg )
	{
		case WM_COMMAND:
		{
			switch(wparLo)
			{
				case IDC_LCL_OK:
				{
					GetDlgItemText(hWnd, IDC_LCL_NAME, m_sN, sizeof(m_sN));
					m_iNx	= GetDlgItemInt(hWnd, IDC_LCL_NUM, 0, 0);
					m_iWx	= GetDlgItemInt(hWnd, IDC_LCL_WDT, 0, 0);
					m_fUV	= GetDlgItemFlt(hWnd, IDC_LCL_UV);

					m_vcP.x = GetDlgItemFlt(hWnd, IDC_LCL_POS_X);
					m_vcP.z = GetDlgItemFlt(hWnd, IDC_LCL_POS_Z);

					struct _T
					{
						INT		iN;		// Number of tile for Width
						INT		iW;		// Width of Cell
						FLOAT	UV;		// UV
						VEC3	vP;		// position
					};

					_T t;
					t.iN = m_iNx;
					t.iW = m_iWx;
					t.UV = m_fUV;
					t.vP = m_vcP;


					GFIELD->Query("Create Block", &t);


					SendMessage(hWnd, WM_CLOSE, 0, 0);

					break;
				}

				case IDC_LCL_CLOSE:
				{
					SendMessage(hWnd, WM_CLOSE, 0, 0);
					break;
				}


				case IDC_LCL_NUM:
				{
					switch(wparHi)
					{
						case EN_CHANGE:
						{
							ILcxmT::MpInf		Inf;
							GFIELD->Query("Get Info", &Inf);

							m_iNx = GetDlgItemInt(hWnd, IDC_LCL_NUM, 0,0);
							m_iWx = Inf.iExt/m_iNx;
							SetDlgItemInt(hWnd, IDC_LCL_WDT, m_iWx, 0);
							break;
						}
					}

					break;
				}
			}

			break;
		}

		case WM_CLOSE:
		{
			DestroyWindow( hWnd );
			m_hWnd	= NULL;
			break;
		}
	}

	return(FALSE);
}