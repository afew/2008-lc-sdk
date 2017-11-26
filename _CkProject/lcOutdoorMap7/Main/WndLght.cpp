// Implementation of the CWndLght class.
//
////////////////////////////////////////////////////////////////////////////////


#include "../_StdAfx.h"


CLSS_DLG_DEFINE(CWndLght, MsgPrc);


INT CWndLght::Init()
{
	m_hWnd	= 0;
	m_hWPrn	= 0;

	m_iLg	= 3;
	m_nLgt	= 0;

	memset(m_pLg, 0, sizeof(m_pLg));

	for(int i=0; i<(sizeof(m_pLg)/sizeof(m_pLg[0])); ++i)
	{
		m_pLg[i].l.Type		= D3DLIGHT_DIRECTIONAL;
		m_pLg[i].l.Diffuse	= D3DXCOLOR( .45F, .45F, .45F, 0.F);
		m_pLg[i].l.Direction	= VEC3( 0, -1, 0);
	}


	m_pLg[0].l.Type		= D3DLIGHT_DIRECTIONAL;
	m_pLg[0].l.Diffuse	= D3DXCOLOR( 0.5f, 0.5f, 0.5f, 0.F);
	m_pLg[0].l.Ambient	= D3DXCOLOR( .2f, .2f, .2f, 0.F);
	m_pLg[0].l.Direction	= VEC3( -cosf(D3DXToRadian(45)), -sinf(D3DXToRadian(45)), 0);

	m_pLg[1].l.Type		= D3DLIGHT_DIRECTIONAL;
	m_pLg[1].l.Diffuse	= D3DXCOLOR( 0.5f, 0.5f, 0.5f, 0.F);
	m_pLg[1].l.Ambient	= D3DXCOLOR( .2f, .2f, .2f, 0.F);
	m_pLg[1].l.Direction= VEC3( -cosf(D3DXToRadian(135)), -sinf(D3DXToRadian(135)), 0);
	
	m_pLg[2].l.Type		= D3DLIGHT_DIRECTIONAL;
	m_pLg[2].l.Diffuse	= D3DXCOLOR( 0.5f, 0.5f, 0.5f, 0.F);
	m_pLg[2].l.Ambient	= D3DXCOLOR( .2f, .2f, .2f, 0.F);
	m_pLg[2].l.Direction= VEC3( 0, -sinf(D3DXToRadian(45)), -cosf(D3DXToRadian(45)));

	int iSize = sizeof(m_crT[0])/ sizeof(m_crT[0][0]);
	int jSize = sizeof(m_crT)/ sizeof(m_crT[0]);

	for(int j=0; j< jSize; ++j)
	{
		for(int i=0; i< iSize; ++i)
		{
			m_crT[j][i] = RGB( 128+rand()%128, 128+rand()%128, 128+rand()%128);
		}
	}

	return 0;
}


INT CWndLght::Create(HWND hWnd)
{
	if(m_hWnd)
		return 0;

	m_hWPrn= hWnd;
	HINSTANCE hInst = GetModuleHandle(NULL);
	m_hWnd = CreateDialog(hInst,MAKEINTRESOURCE(IDD_LIGHT), m_hWPrn, CLSS_DLG_WNDPROC(CWndLght));

	m_hWCb	= GetDlgItem(m_hWnd, IDC_LGT_CMB);
	m_hWdf	= GetDlgItem(m_hWnd, IDC_LGT_DF_C);
	m_hWam	= GetDlgItem(m_hWnd, IDC_LGT_AM_C);
	m_hWsp	= GetDlgItem(m_hWnd, IDC_LGT_SP_C);


	RECT	rt1;
	RECT	rt2;
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
	::ShowWindow(m_hWnd, SW_SHOW);

	FillData();

	for (int i=0;i<m_iLg;++i)
		SendMessage(m_hWCb,CB_ADDSTRING,0,(LPARAM)LcStr_Format("%d", i));

	SendMessage(m_hWCb, CB_SETCURSEL, m_nLgt,0);

	return 0;
}



void CWndLght::ShowWindow(int _ishw)
{
	::ShowWindow(m_hWnd, _ishw);
}



void CWndLght::Destroy()
{
	SAFE_DESTROY_WIN(m_hWnd);
}



LRESULT CWndLght::MsgPrc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WPARAM		wparHi = HIWORD(wParam);
	WPARAM		wparLo = LOWORD(wParam);
	CHOOSECOLOR	COL;

	BYTE		ClrR;
	BYTE		ClrG;
	BYTE		ClrB;

	RECT		rc;
	HWND		hWpnt;
	HDC			hdc;
	HBRUSH		hbrsh;
	COLORREF	clrf;
	PAINTSTRUCT ps;

	DWORD		dClr;
	

	switch( uMsg )
	{
		case WM_PAINT:
		{
			hWpnt	= m_hWdf;
			hdc		= BeginPaint(hWpnt, &ps);
			ClrR	= m_pLg[m_nLgt].l.Diffuse.r * 255;
			ClrG	= m_pLg[m_nLgt].l.Diffuse.g * 255;
			ClrB	= m_pLg[m_nLgt].l.Diffuse.b * 255;
			clrf	= RGB(ClrR, ClrG, ClrB);
			hbrsh	= CreateSolidBrush(clrf);
			GetClientRect(hWpnt, &rc);
			FillRect(hdc, &rc, hbrsh);
			DeleteObject(hbrsh);
			ReleaseDC(hWpnt, hdc);
			EndPaint(hWpnt, &ps);


			hWpnt	= m_hWam;
			hdc		= BeginPaint(hWpnt, &ps);
			ClrR	= m_pLg[m_nLgt].l.Ambient.r * 255;
			ClrG	= m_pLg[m_nLgt].l.Ambient.g * 255;
			ClrB	= m_pLg[m_nLgt].l.Ambient.b * 255;
			clrf	= RGB(ClrR, ClrG, ClrB);
			hbrsh	= CreateSolidBrush(clrf);
			GetClientRect(hWpnt, &rc);
			FillRect(hdc, &rc, hbrsh);
			DeleteObject(hbrsh);
			ReleaseDC(hWpnt, hdc);
			EndPaint(hWpnt, &ps);


			hWpnt	= m_hWsp;
			hdc		= BeginPaint(hWpnt, &ps);
			ClrR	= m_pLg[m_nLgt].l.Specular.r * 255;
			ClrG	= m_pLg[m_nLgt].l.Specular.g * 255;
			ClrB	= m_pLg[m_nLgt].l.Specular.b * 255;
			clrf	= RGB(ClrR, ClrG, ClrB);
			hbrsh	= CreateSolidBrush(clrf);
			GetClientRect(hWpnt, &rc);
			FillRect(hdc, &rc, hbrsh);
			DeleteObject(hbrsh);
			ReleaseDC(hWpnt, hdc);
			EndPaint(hWpnt, &ps);

			break;
		}

		case WM_COMMAND:
		{
			switch(wparLo)
			{
				case IDC_LGT_OK:
				{
					struct _T
					{
						INT		n;			// Number of Lighting
						void*	p;			// Lighting Data
					} t=
						{m_iLg, m_pLg};

					GFIELD->Query("Set Light", &t);

					break;
				}

				case IDC_LGT_CLOSE:
				{
					SendMessage(hWnd, WM_CLOSE, 0, 0);
					break;
				}


				case IDC_LGT_CMB:
				{
					switch(wparHi)
					{
						case CBN_SELCHANGE:
							m_nLgt = SendMessage(m_hWCb, CB_GETCURSEL,0,0);
							FillData();
								
							break;
					}

					break;
				}

				case IDC_LGT_ADD:
				{
					if(m_iLg>=(sizeof(m_pLg)/sizeof(m_pLg[0])) )
						break;

					m_nLgt = m_iLg;
					++m_iLg;


					memset(&m_pLg[m_iLg-1], 0, sizeof(m_pLg[m_iLg-1]));
					m_pLg[m_iLg-1].l.Type		= D3DLIGHT_DIRECTIONAL;
					m_pLg[m_iLg-1].l.Diffuse	= D3DXCOLOR( .45F, .45F, .45F, 0.F);
					m_pLg[m_iLg-1].l.Direction	= VEC3( 0, -1, 0);

					SendMessage(m_hWCb, CB_RESETCONTENT, 0, 0);

					for (int i=0;i<m_iLg;++i)
						SendMessage(m_hWCb,CB_ADDSTRING,0,(LPARAM)LcStr_Format("%d", i));

					SendMessage(m_hWCb, CB_SETCURSEL,m_nLgt,0);

					FillData();
					
					break;
				}

				
				case IDC_LGT_SUB:
				{
					if(m_iLg<=1)
						break;

					std::vector<Lcxmo::MpLgt>			vDlgt;
					std::vector<Lcxmo::MpLgt>::iterator itDlgt;

					for(int i=0; i<m_iLg; ++i)
						vDlgt.push_back(m_pLg[i]);

					memset(m_pLg, 0, sizeof(m_pLg));

					for(i=0; i<(sizeof(m_pLg)/sizeof(m_pLg[0])); ++i)
					{
						m_pLg[i].l.Type		= D3DLIGHT_DIRECTIONAL;
						m_pLg[i].l.Diffuse	= D3DXCOLOR( .45F, .45F, .45F, 0.F);
						m_pLg[i].l.Direction	= VEC3( 0, -1, 0);
					}


					itDlgt = vDlgt.begin() + m_nLgt;
					vDlgt.erase(itDlgt);

					--m_iLg;
					
					if(m_nLgt >= m_iLg)
						m_nLgt =m_iLg-1;

					if(m_nLgt <0)
						m_nLgt =0;




					for(i=0; i<m_iLg; ++i)
						m_pLg[i]	= vDlgt[i];

					vDlgt.clear();
					
					SendMessage(m_hWCb, CB_RESETCONTENT, 0, 0);

					for ( i=0;i<m_iLg;++i)
						SendMessage(m_hWCb,CB_ADDSTRING,0,(LPARAM)LcStr_Format("%d", i));

					SendMessage(m_hWCb, CB_SETCURSEL,m_nLgt,0);

					FillData();
					
					break;
				}


				case IDC_LGT_TYP_P:	{	m_pLg[m_nLgt].l.Type = D3DLIGHT_POINT;		break;	}
				case IDC_LGT_TYP_S:	{	m_pLg[m_nLgt].l.Type = D3DLIGHT_SPOT;			break;	}
				case IDC_LGT_TYP_D:	{	m_pLg[m_nLgt].l.Type = D3DLIGHT_DIRECTIONAL;	break;	}

				case IDC_LGT_DF_C:
				{	
					memset(&COL, 0, sizeof(CHOOSECOLOR));
					COL.lStructSize = sizeof(CHOOSECOLOR);
					COL.hwndOwner=hWnd;
					COL.lpCustColors=m_crT[1];

					if (ChooseColor(&COL))
					{
						SetPickColor(dClr, COL.rgbResult,IDC_LGT_DF_C, TRUE);

						ClrR	= GetRValue(COL.rgbResult);
						ClrG	= GetGValue(COL.rgbResult);
						ClrB	= GetBValue(COL.rgbResult);

						m_pLg[m_nLgt].l.Diffuse = D3DXCOLOR( ClrR/255.f,ClrG/255.f,ClrB/255.f,1.f);

						SetDlgItemInt(m_hWnd, IDC_LGT_DF_R, ClrR, 0);
						SetDlgItemInt(m_hWnd, IDC_LGT_DF_G, ClrG, 0);
						SetDlgItemInt(m_hWnd, IDC_LGT_DF_B, ClrB, 0);
					}

					break;
				}

				case IDC_LGT_AM_C:
				{	
					memset(&COL, 0, sizeof(CHOOSECOLOR));
					COL.lStructSize = sizeof(CHOOSECOLOR);
					COL.hwndOwner=hWnd;
					COL.lpCustColors=m_crT[2];

					if (ChooseColor(&COL))
					{
						SetPickColor(dClr, COL.rgbResult,IDC_LGT_AM_C, TRUE);

						ClrR	= GetRValue(COL.rgbResult);
						ClrG	= GetGValue(COL.rgbResult);
						ClrB	= GetBValue(COL.rgbResult);

						m_pLg[m_nLgt].l.Ambient = D3DXCOLOR( ClrR/255.f,ClrG/255.f,ClrB/255.f, 1.f);

						SetDlgItemInt(m_hWnd, IDC_LGT_AM_R, ClrR, 0);
						SetDlgItemInt(m_hWnd, IDC_LGT_AM_G, ClrG, 0);
						SetDlgItemInt(m_hWnd, IDC_LGT_AM_B, ClrB, 0);
					}

					break;
				}

				case IDC_LGT_SP_C:
				{
					memset(&COL, 0, sizeof(CHOOSECOLOR));
					COL.lStructSize = sizeof(CHOOSECOLOR);
					COL.hwndOwner=hWnd;
					COL.lpCustColors=m_crT[3];

					if (ChooseColor(&COL))
					{
						SetPickColor(dClr, COL.rgbResult,IDC_LGT_SP_C, TRUE);

						ClrR	= GetRValue(COL.rgbResult);
						ClrG	= GetGValue(COL.rgbResult);
						ClrB	= GetBValue(COL.rgbResult);

						m_pLg[m_nLgt].l.Specular = D3DXCOLOR( ClrR/255.f,ClrG/255.f,ClrB/255.f, 1.f);

						SetDlgItemInt(m_hWnd, IDC_LGT_SP_R, ClrR, 0);
						SetDlgItemInt(m_hWnd, IDC_LGT_SP_G, ClrG, 0);
						SetDlgItemInt(m_hWnd, IDC_LGT_SP_B, ClrB, 0);
					}

					break;
				}

				case IDC_LGT_DF_R:	{	switch(wparHi)	{	case EN_CHANGE:	{	OnChangeMtlDF(hWnd, IDC_LGT_DF_C);	break;	}}	break;	}
				case IDC_LGT_DF_G:	{	switch(wparHi)	{	case EN_CHANGE:	{	OnChangeMtlDF(hWnd, IDC_LGT_DF_C);	break;	}}	break;	}
				case IDC_LGT_DF_B:	{	switch(wparHi)	{	case EN_CHANGE:	{	OnChangeMtlDF(hWnd, IDC_LGT_DF_C);	break;	}}	break;	}

				case IDC_LGT_AM_R:	{	switch(wparHi)	{	case EN_CHANGE:	{	OnChangeMtlAM(hWnd, IDC_LGT_AM_C);	break;	}}	break;	}
				case IDC_LGT_AM_G:	{	switch(wparHi)	{	case EN_CHANGE:	{	OnChangeMtlAM(hWnd, IDC_LGT_AM_C);	break;	}}	break;	}
				case IDC_LGT_AM_B:	{	switch(wparHi)	{	case EN_CHANGE:	{	OnChangeMtlAM(hWnd, IDC_LGT_AM_C);	break;	}}	break;	}

				case IDC_LGT_SP_R:	{	switch(wparHi)	{	case EN_CHANGE:	{	OnChangeMtlSP(hWnd, IDC_LGT_SP_C);	break;	}}	break;	}
				case IDC_LGT_SP_G:	{	switch(wparHi)	{	case EN_CHANGE:	{	OnChangeMtlSP(hWnd, IDC_LGT_SP_C);	break;	}}	break;	}
				case IDC_LGT_SP_B:	{	switch(wparHi)	{	case EN_CHANGE:	{	OnChangeMtlSP(hWnd, IDC_LGT_SP_C);	break;	}}	break;	}

				case IDC_LGT_POS_X:	{	switch(wparHi)	{	case EN_CHANGE:	{	m_pLg[m_nLgt].l.Position.x	= GetDlgItemFlt(hWnd, IDC_LGT_POS_X);	break;	}	}	break;	}
				case IDC_LGT_POS_Y:	{	switch(wparHi)	{	case EN_CHANGE:	{	m_pLg[m_nLgt].l.Position.y	= GetDlgItemFlt(hWnd, IDC_LGT_POS_Y);	break;	}	}	break;	}
				case IDC_LGT_POS_Z:	{	switch(wparHi)	{	case EN_CHANGE:	{	m_pLg[m_nLgt].l.Position.z	= GetDlgItemFlt(hWnd, IDC_LGT_POS_Z);	break;	}	}	break;	}

				case IDC_LGT_DIR_X:	{	switch(wparHi)	{	case EN_CHANGE:	{	m_pLg[m_nLgt].l.Direction.x	= GetDlgItemFlt(hWnd, IDC_LGT_DIR_X);	break;	}	}	break;	}
				case IDC_LGT_DIR_Y:	{	switch(wparHi)	{	case EN_CHANGE:	{	m_pLg[m_nLgt].l.Direction.y	= GetDlgItemFlt(hWnd, IDC_LGT_DIR_Y);	break;	}	}	break;	}
				case IDC_LGT_DIR_Z:	{	switch(wparHi)	{	case EN_CHANGE:	{	m_pLg[m_nLgt].l.Direction.z	= GetDlgItemFlt(hWnd, IDC_LGT_DIR_Z);	break;	}	}	break;	}

				case IDC_LGT_RNG:	{	switch(wparHi)	{	case EN_CHANGE:	{	m_pLg[m_nLgt].l.Range			= GetDlgItemFlt(hWnd, IDC_LGT_RNG);	break;	}	}	break;	}
				case IDC_LGT_FAL:	{	switch(wparHi)	{	case EN_CHANGE:	{	m_pLg[m_nLgt].l.Falloff		= GetDlgItemFlt(hWnd, IDC_LGT_FAL);	break;	}	}	break;	}
				case IDC_LGT_THT:	{	switch(wparHi)	{	case EN_CHANGE:	{	m_pLg[m_nLgt].l.Theta			= GetDlgItemFlt(hWnd, IDC_LGT_THT);	break;	}	}	break;	}
				case IDC_LGT_PHI:	{	switch(wparHi)	{	case EN_CHANGE:	{	m_pLg[m_nLgt].l.Phi			= GetDlgItemFlt(hWnd, IDC_LGT_PHI);	break;	}	}	break;	}
				case IDC_LGT_AT0:	{	switch(wparHi)	{	case EN_CHANGE:	{	m_pLg[m_nLgt].l.Attenuation0	= GetDlgItemFlt(hWnd, IDC_LGT_AT0);	break;	}	}	break;	}
				case IDC_LGT_AT1:	{	switch(wparHi)	{	case EN_CHANGE:	{	m_pLg[m_nLgt].l.Attenuation1	= GetDlgItemFlt(hWnd, IDC_LGT_AT1);	break;	}	}	break;	}
				case IDC_LGT_AT2:	{	switch(wparHi)	{	case EN_CHANGE:	{	m_pLg[m_nLgt].l.Attenuation2	= GetDlgItemFlt(hWnd, IDC_LGT_AT2);	break;	}	}	break;	}

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




void CWndLght::SetPickColor(DWORD&	clrOut,DWORD& clrIn, INT nID, BOOL IsBGR)
{
	COLORREF	col;
	BYTE*		bgr;
	RECT		rc;
	HWND		hWnd;
	HDC			hdc;
	HBRUSH		hbrsh;

	if(IsBGR)
		bgr	= LcUtil_GetColorFromBGR(clrIn);

	else
		bgr = LcUtil_GetColorFromARGB(clrIn);


	col = RGB(bgr[1],bgr[2], bgr[3]);
	clrOut = D3DCOLOR_XRGB(bgr[1],bgr[2], bgr[3]);
						
	hWnd= GetDlgItem(m_hWnd, nID);
	hdc = GetDC(hWnd);
	hbrsh = CreateSolidBrush(col);

	GetClientRect(hWnd, &rc);
	InvalidateRect(hWnd, &rc, 1);
	FillRect(hdc, &rc, hbrsh);
	DeleteObject(hbrsh);
	ReleaseDC(hWnd, hdc);
	
}




void CWndLght::OnChangeMtlDF(HWND hWnd, INT nID)
{
	BYTE	ClrR = GetDlgItemInt(hWnd, nID + 1, 0,0);
	BYTE	ClrG = GetDlgItemInt(hWnd, nID + 2, 0,0);
	BYTE	ClrB = GetDlgItemInt(hWnd, nID + 3, 0,0);
	DWORD	dClr = RGB(ClrR, ClrG, ClrB);

	SetPickColor(dClr, dClr , nID, TRUE);
	m_pLg[m_nLgt].l.Diffuse= D3DXCOLOR( ClrR/255.f,ClrG/255.f,ClrB/255.f,1.f);
}


void CWndLght::OnChangeMtlAM(HWND hWnd, INT nID)
{
	BYTE	ClrR = GetDlgItemInt(hWnd, nID + 1, 0,0);
	BYTE	ClrG = GetDlgItemInt(hWnd, nID + 2, 0,0);
	BYTE	ClrB = GetDlgItemInt(hWnd, nID + 3, 0,0);
	DWORD	dClr = RGB(ClrR, ClrG, ClrB);

	SetPickColor(dClr, dClr , nID, TRUE);
	m_pLg[m_nLgt].l.Ambient= D3DXCOLOR( ClrR/255.f,ClrG/255.f,ClrB/255.f,1.f);
}


void CWndLght::OnChangeMtlSP(HWND hWnd, INT nID)
{
	BYTE	ClrR = GetDlgItemInt(hWnd, nID + 1, 0,0);
	BYTE	ClrG = GetDlgItemInt(hWnd, nID + 2, 0,0);
	BYTE	ClrB = GetDlgItemInt(hWnd, nID + 3, 0,0);
	DWORD	dClr = RGB(ClrR, ClrG, ClrB);

	SetPickColor(dClr, dClr , nID, TRUE);
	m_pLg[m_nLgt].l.Specular= D3DXCOLOR( ClrR/255.f,ClrG/255.f,ClrB/255.f,1.f);
}



void CWndLght::FillData()
{
	RECT		rc;
	HWND		hWpnt;
	HDC			hdc;
	BYTE		ClrR;
	BYTE		ClrG;
	BYTE		ClrB;
	HBRUSH		hbrsh;
	COLORREF	clrf;

	hWpnt	= m_hWdf;
	hdc		= GetDC(hWpnt);
	ClrR	= m_pLg[m_nLgt].l.Diffuse.r * 255;
	ClrG	= m_pLg[m_nLgt].l.Diffuse.g * 255;
	ClrB	= m_pLg[m_nLgt].l.Diffuse.b * 255;
	clrf	= RGB(ClrR, ClrG, ClrB);
	hbrsh	= CreateSolidBrush(clrf);
	GetClientRect(hWpnt, &rc);
	FillRect(hdc, &rc, hbrsh);
	DeleteObject(hbrsh);
	ReleaseDC(hWpnt, hdc);

	SetDlgItemInt(m_hWnd, IDC_LGT_DF_R, ClrR, 0);
	SetDlgItemInt(m_hWnd, IDC_LGT_DF_G, ClrG, 0);
	SetDlgItemInt(m_hWnd, IDC_LGT_DF_B, ClrB, 0);


	hWpnt	= m_hWam;
	hdc		= GetDC(hWpnt);
	ClrR	= m_pLg[m_nLgt].l.Ambient.r * 255;
	ClrG	= m_pLg[m_nLgt].l.Ambient.g * 255;
	ClrB	= m_pLg[m_nLgt].l.Ambient.b * 255;
	clrf	= RGB(ClrR, ClrG, ClrB);
	hbrsh	= CreateSolidBrush(clrf);
	GetClientRect(hWpnt, &rc);
	FillRect(hdc, &rc, hbrsh);
	DeleteObject(hbrsh);
	ReleaseDC(hWpnt, hdc);

	SetDlgItemInt(m_hWnd,IDC_LGT_AM_R, ClrR, 0);
	SetDlgItemInt(m_hWnd,IDC_LGT_AM_G, ClrG, 0);
	SetDlgItemInt(m_hWnd,IDC_LGT_AM_B, ClrB, 0);


	hWpnt	= m_hWsp;
	hdc		= GetDC(hWpnt);
	ClrR	= m_pLg[m_nLgt].l.Specular.r * 255;
	ClrG	= m_pLg[m_nLgt].l.Specular.g * 255;
	ClrB	= m_pLg[m_nLgt].l.Specular.b * 255;
	clrf	= RGB(ClrR, ClrG, ClrB);
	hbrsh	= CreateSolidBrush(clrf);
	GetClientRect(hWpnt, &rc);
	FillRect(hdc, &rc, hbrsh);
	DeleteObject(hbrsh);
	ReleaseDC(hWpnt, hdc);

	SetDlgItemInt(m_hWnd,IDC_LGT_SP_R, ClrR, 0);
	SetDlgItemInt(m_hWnd,IDC_LGT_SP_G, ClrG, 0);
	SetDlgItemInt(m_hWnd,IDC_LGT_SP_B, ClrB, 0);

	SetDlgItemFlt(m_hWnd, IDC_LGT_POS_X, m_pLg[m_nLgt].l.Position.x, 3);
	SetDlgItemFlt(m_hWnd, IDC_LGT_POS_Y, m_pLg[m_nLgt].l.Position.y, 3);
	SetDlgItemFlt(m_hWnd, IDC_LGT_POS_Z, m_pLg[m_nLgt].l.Position.z, 3);

	SetDlgItemFlt(m_hWnd, IDC_LGT_DIR_X, m_pLg[m_nLgt].l.Direction.x, 3);
	SetDlgItemFlt(m_hWnd, IDC_LGT_DIR_Y, m_pLg[m_nLgt].l.Direction.y, 3);
	SetDlgItemFlt(m_hWnd, IDC_LGT_DIR_Z, m_pLg[m_nLgt].l.Direction.z, 3);

	SetDlgItemFlt(m_hWnd, IDC_LGT_RNG, m_pLg[m_nLgt].l.Range			, 3);
	SetDlgItemFlt(m_hWnd, IDC_LGT_FAL, m_pLg[m_nLgt].l.Falloff		, 3);
	SetDlgItemFlt(m_hWnd, IDC_LGT_THT, m_pLg[m_nLgt].l.Theta			, 3);
	SetDlgItemFlt(m_hWnd, IDC_LGT_PHI, m_pLg[m_nLgt].l.Phi			, 3);
	SetDlgItemFlt(m_hWnd, IDC_LGT_AT0, m_pLg[m_nLgt].l.Attenuation0	, 3);
	SetDlgItemFlt(m_hWnd, IDC_LGT_AT1, m_pLg[m_nLgt].l.Attenuation1	, 3);
	SetDlgItemFlt(m_hWnd, IDC_LGT_AT2, m_pLg[m_nLgt].l.Attenuation2	, 3);

	CheckRadioButton(m_hWnd, IDC_LGT_TYP_P, IDC_LGT_TYP_D, IDC_LGT_TYP_P + m_pLg[m_nLgt].l.Type-1);
}