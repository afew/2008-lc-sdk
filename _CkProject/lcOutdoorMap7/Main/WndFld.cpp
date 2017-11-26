// Implementation of the CWndFld class.
//
////////////////////////////////////////////////////////////////////////////////


#include "../_StdAfx.h"


CLSS_DLG_DEFINE(CWndFld, MsgPrc);


INT CWndFld::Init()
{
	m_hWnd	= 0;
	m_hWPrn	= 0;

	m_Inf.iNfX	= 3;
	m_Inf.iNfZ	= 3;
	m_Inf.iExt	= 1024;

	m_Inf.iNx	= 16;
	m_Inf.fUV	= 2.f;

	return 0;
}


INT CWndFld::Create(HWND hWnd)
{
	if(m_hWnd)
		return 0;

	HINSTANCE hInst = GetModuleHandle(NULL);
	m_hWPrn= hWnd;
	m_hWnd = CreateDialog(hInst,MAKEINTRESOURCE(IDD_FLD), m_hWPrn, CLSS_DLG_WNDPROC(CWndFld));

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

	iX = rt1.left + 200;
	iY = rt1.top  + 150;
	
	MoveWindow(m_hWnd, iX, iY, iWidth, iHeight, TRUE);


	INT	iWx = m_Inf.iExt/m_Inf.iNx;

	SetDlgItemText(m_hWnd,	IDC_FLD_NAME, "Grave Zone");
	SetDlgItemInt(m_hWnd,	IDC_FLD_NUM_X, m_Inf.iNfX, 0);
	SetDlgItemInt(m_hWnd,	IDC_FLD_NUM_Z, m_Inf.iNfZ, 0);
	SetDlgItemInt(m_hWnd,	IDC_FLD_EXT, m_Inf.iExt, 0);

	CheckRadioButton(m_hWnd, IDC_FLD_LCL_Y, IDC_FLD_LCL_N, IDC_FLD_LCL_Y);

	SetDlgItemInt(m_hWnd,	IDC_FLD_LCL_NUM,	m_Inf.iNx, 0);
	SetDlgItemInt(m_hWnd,	IDC_FLD_LCL_WDT,	iWx, 0);
	SetDlgItemFlt(m_hWnd,	IDC_FLD_LCL_UV,		m_Inf.fUV, 0);
	
	return 0;
}

void CWndFld::ShowWindow(int _ishw)
{
	::ShowWindow(m_hWnd, _ishw);
}


void CWndFld::Destroy()
{
	SAFE_DESTROY_WIN(m_hWnd);
}

LRESULT CWndFld::MsgPrc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WPARAM		wparHi = HIWORD(wParam);
	WPARAM		wparLo = LOWORD(wParam);

	switch( uMsg )
	{
		case WM_COMMAND:
		{
			switch(wparLo)
			{
				case IDC_FLD_OK:
				{
					GetDlgItemText(hWnd, IDC_FLD_NAME, m_Inf.sN, sizeof(m_Inf.sN));
					m_Inf.iNfX	= GetDlgItemInt(hWnd, IDC_FLD_NUM_X, 0, 0);
					m_Inf.iNfZ	= GetDlgItemInt(hWnd, IDC_FLD_NUM_Z, 0, 0);
					m_Inf.iExt	= GetDlgItemInt(hWnd, IDC_FLD_EXT,	 0, 0);

					m_Inf.iNx	= GetDlgItemInt(m_hWnd,	IDC_FLD_LCL_NUM, 0, 0);
					m_Inf.fUV	= GetDlgItemFlt(m_hWnd,	IDC_FLD_LCL_UV);

					INT	 iWx	= GetDlgItemInt(m_hWnd,	IDC_FLD_LCL_WDT, 0, 0);

					struct _T{	Lcxmo::MpInf* I; Lcxmo::MpEnv* E; Lcxmo::lsMpTex* L;};
					_T t;
					
					t.I = &m_Inf;
					t.E = &m_Env;
					t.L = &g_pApp->m_WndTool.m_vTexD;

					GFIELD->Query("Create Field", &t);

					SendMessage(hWnd, WM_CLOSE, 0, 0);

					break;
				}

				case IDC_FLD_CLOSE:
				{
					SendMessage(hWnd, WM_CLOSE, 0, 0);
					break;
				}

				case IDC_FLD_LCL_Y:
				{
					::ShowWindow( GetDlgItem(hWnd, IDC_FLD_LCL_NUM),	SW_SHOW);
					::ShowWindow( GetDlgItem(hWnd, IDC_FLD_LCL_WDT),	SW_SHOW);
					::ShowWindow( GetDlgItem(hWnd, IDC_FLD_LCL_UV),		SW_SHOW);
					break;
				}
					
				case IDC_FLD_LCL_N:
				{
					::ShowWindow( GetDlgItem(hWnd, IDC_FLD_LCL_NUM),	SW_HIDE);
					::ShowWindow( GetDlgItem(hWnd, IDC_FLD_LCL_WDT),	SW_HIDE);
					::ShowWindow( GetDlgItem(hWnd, IDC_FLD_LCL_UV),		SW_HIDE);
					break;
				}


				case IDC_FLD_EXT:
				{
					switch(wparHi)
					{
						case EN_CHANGE:
						{
							INT iExt;
							INT iNx;

							iExt = GetDlgItemInt(hWnd, IDC_FLD_EXT,	 0, 0);
							iNx = GetDlgItemInt(hWnd, IDC_FLD_LCL_NUM, 0, 0);

							if(iExt>0)
								m_Inf.iExt = iExt;

							if(iNx>0)
							{
								m_Inf.iNx = iNx;
								INT iWx = m_Inf.iExt/m_Inf.iNx;
								SetDlgItemInt(hWnd, IDC_FLD_LCL_WDT, iWx, 0);
							}

							break;
						}
					}

					break;
				}


				case IDC_FLD_LCL_NUM:
				{
					switch(wparHi)
					{
						case EN_CHANGE:
						{
							INT iExt;
							INT iNx;

							iExt = GetDlgItemInt(hWnd, IDC_FLD_EXT,	 0, 0);
							iNx = GetDlgItemInt(hWnd, IDC_FLD_LCL_NUM, 0, 0);

							if(iExt>0)
								m_Inf.iExt = iExt;

							if(iNx>0)
							{
								m_Inf.iNx = iNx;
								INT iWx = m_Inf.iExt/m_Inf.iNx;
								SetDlgItemInt(hWnd, IDC_FLD_LCL_WDT, iWx, 0);
							}

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