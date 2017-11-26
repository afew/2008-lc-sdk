// Implementation of the CWndMtrl class.
//
////////////////////////////////////////////////////////////////////////////////


#include "../_StdAfx.h"


CLSS_DLG_DEFINE(CWndMtrl, MsgPrc);


INT CWndMtrl::Init()
{
	m_hWnd	= 0;
	m_hWPrn	= 0;

	memset( &m_Mtl, 0, sizeof(m_Mtl) );
    m_Mtl.dcD	= D3DXCOLOR( .7f, .7f, .7f, 0.F);
    m_Mtl.dcA	= D3DXCOLOR( .2f, .2f, .2f, 0.F);
    
	m_dAL= 0x00DDDDDD;


	int iSize = sizeof(m_crT[0])/ sizeof(m_crT[0][0]);
	int jSize = sizeof(m_crT)/ sizeof(m_crT[0]);

	for(int j=0; j< jSize; ++j)
	{
		for(int i=0; i< iSize; ++i)
		{
			m_crT[j][i] = RGB( rand()%256, rand()%256, rand()%256);
		}
	}

	return 0;
}


INT CWndMtrl::Create(HWND hWnd)
{
	if(m_hWnd)
		return 0;

	m_hWPrn= hWnd;
	HINSTANCE hInst = GetModuleHandle(NULL);
	m_hWnd = CreateDialog(hInst,MAKEINTRESOURCE(IDD_MTRL), m_hWPrn, CLSS_DLG_WNDPROC(CWndMtrl));

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

	

	m_hWal	= GetDlgItem(m_hWnd, IDC_MTL_AL_C);
	m_hWdf	= GetDlgItem(m_hWnd, IDC_MTL_DF_C);
	m_hWam	= GetDlgItem(m_hWnd, IDC_MTL_AM_C);
	m_hWsp	= GetDlgItem(m_hWnd, IDC_MTL_SP_C);
	m_hWem	= GetDlgItem(m_hWnd, IDC_MTL_EM_C);

	RECT		rc;
	HWND		hWpnt;
	HDC			hdc;
	BYTE*		pClr;
	BYTE		ClrR;
	BYTE		ClrG;
	BYTE		ClrB;
	HBRUSH		hbrsh;
	COLORREF	clrf;

	hWpnt	= m_hWal;
	hdc		= GetDC(hWpnt);
	pClr	= LcUtil_GetColorFromARGB(m_dAL);
	ClrR	= pClr[1];
	ClrG	= pClr[2];
	ClrB	= pClr[3];
	clrf	= RGB(ClrR, ClrG, ClrB);
	hbrsh	= CreateSolidBrush(clrf);
	GetClientRect(hWpnt, &rc);
	FillRect(hdc, &rc, hbrsh);
	DeleteObject(hbrsh);
	ReleaseDC(hWpnt, hdc);

	SetDlgItemInt(m_hWnd, IDC_MTL_AL_R, ClrR, 0);
	SetDlgItemInt(m_hWnd, IDC_MTL_AL_G, ClrG, 0);
	SetDlgItemInt(m_hWnd, IDC_MTL_AL_B, ClrB, 0);


	hWpnt	= m_hWdf;
	hdc		= GetDC(hWpnt);
	ClrR	= m_Mtl.dcD.r * 255;
	ClrG	= m_Mtl.dcD.g * 255;
	ClrB	= m_Mtl.dcD.b * 255;
	clrf	= RGB(ClrR, ClrG, ClrB);
	hbrsh	= CreateSolidBrush(clrf);
	GetClientRect(hWpnt, &rc);
	FillRect(hdc, &rc, hbrsh);
	DeleteObject(hbrsh);
	ReleaseDC(hWpnt, hdc);

	SetDlgItemInt(m_hWnd, IDC_MTL_DF_R, ClrR, 0);
	SetDlgItemInt(m_hWnd, IDC_MTL_DF_G, ClrG, 0);
	SetDlgItemInt(m_hWnd, IDC_MTL_DF_B, ClrB, 0);


	hWpnt	= m_hWam;
	hdc		= GetDC(hWpnt);
	ClrR	= m_Mtl.dcA.r * 255;
	ClrG	= m_Mtl.dcA.g * 255;
	ClrB	= m_Mtl.dcA.b * 255;
	clrf	= RGB(ClrR, ClrG, ClrB);
	hbrsh	= CreateSolidBrush(clrf);
	GetClientRect(hWpnt, &rc);
	FillRect(hdc, &rc, hbrsh);
	DeleteObject(hbrsh);
	ReleaseDC(hWpnt, hdc);

	SetDlgItemInt(m_hWnd,IDC_MTL_AM_R, ClrR, 0);
	SetDlgItemInt(m_hWnd,IDC_MTL_AM_G, ClrG, 0);
	SetDlgItemInt(m_hWnd,IDC_MTL_AM_B, ClrB, 0);


	hWpnt	= m_hWsp;
	hdc		= GetDC(hWpnt);
	ClrR	= m_Mtl.dcS.r * 255;
	ClrG	= m_Mtl.dcS.g * 255;
	ClrB	= m_Mtl.dcS.b * 255;
	clrf	= RGB(ClrR, ClrG, ClrB);
	hbrsh	= CreateSolidBrush(clrf);
	GetClientRect(hWpnt, &rc);
	FillRect(hdc, &rc, hbrsh);
	DeleteObject(hbrsh);
	ReleaseDC(hWpnt, hdc);

	SetDlgItemInt(m_hWnd,IDC_MTL_SP_R, ClrR, 0);
	SetDlgItemInt(m_hWnd,IDC_MTL_SP_G, ClrG, 0);
	SetDlgItemInt(m_hWnd,IDC_MTL_SP_B, ClrB, 0);


	hWpnt	= m_hWem;
	hdc		= GetDC(hWpnt);
	ClrR	= m_Mtl.dcE.r * 255;
	ClrG	= m_Mtl.dcE.g * 255;
	ClrB	= m_Mtl.dcE.b * 255;
	clrf	= RGB(ClrR, ClrG, ClrB);
	hbrsh	= CreateSolidBrush(clrf);
	GetClientRect(hWpnt, &rc);
	FillRect(hdc, &rc, hbrsh);
	DeleteObject(hbrsh);
	ReleaseDC(hWpnt, hdc);

	SetDlgItemInt(m_hWnd,IDC_MTL_EM_R, ClrR, 0);
	SetDlgItemInt(m_hWnd,IDC_MTL_EM_G, ClrG, 0);
	SetDlgItemInt(m_hWnd,IDC_MTL_EM_B, ClrB, 0);


	SetDlgItemFlt(m_hWnd, IDC_MTL_POW, m_Mtl.fPw);


	return 0;
}



void CWndMtrl::ShowWindow(int _ishw)
{
	::ShowWindow(m_hWnd, _ishw);
}



void CWndMtrl::Destroy()
{
	SAFE_DESTROY_WIN(m_hWnd);
}



LRESULT CWndMtrl::MsgPrc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
	BYTE*		pClr;
	HBRUSH		hbrsh;
	COLORREF	clrf;
	PAINTSTRUCT ps;

	DWORD		dClr;
	

	switch( uMsg )
	{
		case WM_PAINT:
		{
			hWpnt	= m_hWal;
			hdc		= BeginPaint(hWpnt, &ps);
			pClr	= LcUtil_GetColorFromARGB(m_dAL);
			ClrR	= pClr[1];
			ClrG	= pClr[2];
			ClrB	= pClr[3];
			clrf	= RGB(ClrR, ClrG, ClrB);
			hbrsh	= CreateSolidBrush(clrf);
			GetClientRect(hWpnt, &rc);
			InvalidateRect(hWpnt, &rc, 1);
			FillRect(hdc, &rc, hbrsh);
			DeleteObject(hbrsh);
			ReleaseDC(hWpnt, hdc);
			EndPaint(hWpnt, &ps);


			hWpnt	= m_hWdf;
			hdc		= BeginPaint(hWpnt, &ps);
			ClrR	= m_Mtl.dcD.r * 255;
			ClrG	= m_Mtl.dcD.g * 255;
			ClrB	= m_Mtl.dcD.b * 255;
			clrf	= RGB(ClrR, ClrG, ClrB);
			hbrsh	= CreateSolidBrush(clrf);
			GetClientRect(hWpnt, &rc);
			FillRect(hdc, &rc, hbrsh);
			DeleteObject(hbrsh);
			ReleaseDC(hWpnt, hdc);
			EndPaint(hWpnt, &ps);


			hWpnt	= m_hWam;
			hdc		= BeginPaint(hWpnt, &ps);
			ClrR	= m_Mtl.dcA.r * 255;
			ClrG	= m_Mtl.dcA.g * 255;
			ClrB	= m_Mtl.dcA.b * 255;
			clrf	= RGB(ClrR, ClrG, ClrB);
			hbrsh	= CreateSolidBrush(clrf);
			GetClientRect(hWpnt, &rc);
			FillRect(hdc, &rc, hbrsh);
			DeleteObject(hbrsh);
			ReleaseDC(hWpnt, hdc);
			EndPaint(hWpnt, &ps);


			hWpnt	= m_hWsp;
			hdc		= BeginPaint(hWpnt, &ps);
			ClrR	= m_Mtl.dcS.r * 255;
			ClrG	= m_Mtl.dcS.g * 255;
			ClrB	= m_Mtl.dcS.b * 255;
			clrf	= RGB(ClrR, ClrG, ClrB);
			hbrsh	= CreateSolidBrush(clrf);
			GetClientRect(hWpnt, &rc);
			FillRect(hdc, &rc, hbrsh);
			DeleteObject(hbrsh);
			ReleaseDC(hWpnt, hdc);
			EndPaint(hWpnt, &ps);


			hWpnt	= m_hWem;
			hdc		= BeginPaint(hWpnt, &ps);
			ClrR	= m_Mtl.dcE.r * 255;
			ClrG	= m_Mtl.dcE.g * 255;
			ClrB	= m_Mtl.dcE.b * 255;
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
				case IDC_MTL_OK:
				{
					GFIELD->Query("Set Material", &m_Mtl);
					GFIELD->Query("Set Ambient Light", &m_dAL);
					
					break;
				}

				case IDC_MTL_CLOSE:
				{
					SendMessage(hWnd, WM_CLOSE, 0, 0);
					break;
				}

				case IDC_MTL_AL_C:
				{	
					memset(&COL, 0, sizeof(CHOOSECOLOR));
					COL.lStructSize = sizeof(CHOOSECOLOR);
					COL.hwndOwner=hWnd;
					COL.lpCustColors=m_crT[0];

					if (ChooseColor(&COL))
					{
						SetPickColor(dClr, COL.rgbResult,IDC_MTL_AL_C, TRUE);

						ClrR	= GetRValue(COL.rgbResult);
						ClrG	= GetGValue(COL.rgbResult);
						ClrB	= GetBValue(COL.rgbResult);
						
						m_dAL	= D3DXCOLOR( ClrR/255.f,ClrG/255.f,ClrB/255.f,1.f);

						SetDlgItemInt(m_hWnd, IDC_MTL_AL_R, ClrR, 0);
						SetDlgItemInt(m_hWnd, IDC_MTL_AL_G, ClrG, 0);
						SetDlgItemInt(m_hWnd, IDC_MTL_AL_B, ClrB, 0);
					}

					break;
				}



				case IDC_MTL_DF_C:
				{	
					memset(&COL, 0, sizeof(CHOOSECOLOR));
					COL.lStructSize = sizeof(CHOOSECOLOR);
					COL.hwndOwner=hWnd;
					COL.lpCustColors=m_crT[1];

					if (ChooseColor(&COL))
					{
						SetPickColor(dClr, COL.rgbResult,IDC_MTL_DF_C, TRUE);

						ClrR	= GetRValue(COL.rgbResult);
						ClrG	= GetGValue(COL.rgbResult);
						ClrB	= GetBValue(COL.rgbResult);

						m_Mtl.dcD = D3DXCOLOR( ClrR/255.f,ClrG/255.f,ClrB/255.f,1.f);

						SetDlgItemInt(m_hWnd, IDC_MTL_DF_R, ClrR, 0);
						SetDlgItemInt(m_hWnd, IDC_MTL_DF_G, ClrG, 0);
						SetDlgItemInt(m_hWnd, IDC_MTL_DF_B, ClrB, 0);
					}

					break;
				}

				case IDC_MTL_AM_C:
				{	
					memset(&COL, 0, sizeof(CHOOSECOLOR));
					COL.lStructSize = sizeof(CHOOSECOLOR);
					COL.hwndOwner=hWnd;
					COL.lpCustColors=m_crT[2];

					if (ChooseColor(&COL))
					{
						SetPickColor(dClr, COL.rgbResult,IDC_MTL_AM_C, TRUE);

						ClrR	= GetRValue(COL.rgbResult);
						ClrG	= GetGValue(COL.rgbResult);
						ClrB	= GetBValue(COL.rgbResult);

						m_Mtl.dcA = D3DXCOLOR( ClrR/255.f,ClrG/255.f,ClrB/255.f, 1.f);

						SetDlgItemInt(m_hWnd, IDC_MTL_AM_R, ClrR, 0);
						SetDlgItemInt(m_hWnd, IDC_MTL_AM_G, ClrG, 0);
						SetDlgItemInt(m_hWnd, IDC_MTL_AM_B, ClrB, 0);
					}

					break;
				}

				case IDC_MTL_SP_C:
				{	
					memset(&COL, 0, sizeof(CHOOSECOLOR));
					COL.lStructSize = sizeof(CHOOSECOLOR);
					COL.hwndOwner=hWnd;
					COL.lpCustColors=m_crT[3];

					if (ChooseColor(&COL))
					{
						SetPickColor(dClr, COL.rgbResult,IDC_MTL_SP_C, TRUE);

						ClrR	= GetRValue(COL.rgbResult);
						ClrG	= GetGValue(COL.rgbResult);
						ClrB	= GetBValue(COL.rgbResult);

						m_Mtl.dcS = D3DXCOLOR( ClrR/255.f,ClrG/255.f,ClrB/255.f, 1.f);

						SetDlgItemInt(m_hWnd, IDC_MTL_SP_R, ClrR, 0);
						SetDlgItemInt(m_hWnd, IDC_MTL_SP_G, ClrG, 0);
						SetDlgItemInt(m_hWnd, IDC_MTL_SP_B, ClrB, 0);
					}

					break;
				}

				case IDC_MTL_EM_C:
				{	
					memset(&COL, 0, sizeof(CHOOSECOLOR));
					COL.lStructSize = sizeof(CHOOSECOLOR);
					COL.hwndOwner=hWnd;
					COL.lpCustColors=m_crT[4];

					if (ChooseColor(&COL))
					{
						SetPickColor(dClr, COL.rgbResult,IDC_MTL_EM_C, TRUE);

						ClrR	= GetRValue(COL.rgbResult);
						ClrG	= GetGValue(COL.rgbResult);
						ClrB	= GetBValue(COL.rgbResult);

						m_Mtl.dcE= D3DXCOLOR( ClrR/255.f,ClrG/255.f,ClrB/255.f, 1.f);

						SetDlgItemInt(m_hWnd, IDC_MTL_EM_R, ClrR, 0);
						SetDlgItemInt(m_hWnd, IDC_MTL_EM_G, ClrG, 0);
						SetDlgItemInt(m_hWnd, IDC_MTL_EM_B, ClrB, 0);
					}

					break;
				}


				case IDC_MTL_AL_R:	{	switch(wparHi)	{	case EN_CHANGE:	{	OnChangeMtlAL(hWnd, IDC_MTL_AL_C);	break;	}}	break;	}
				case IDC_MTL_AL_G:	{	switch(wparHi)	{	case EN_CHANGE:	{	OnChangeMtlAL(hWnd, IDC_MTL_AL_C);	break;	}}	break;	}
				case IDC_MTL_AL_B:	{	switch(wparHi)	{	case EN_CHANGE:	{	OnChangeMtlAL(hWnd, IDC_MTL_AL_C);	break;	}}	break;	}

				case IDC_MTL_DF_R:	{	switch(wparHi)	{	case EN_CHANGE:	{	OnChangeMtlDF(hWnd, IDC_MTL_DF_C);	break;	}}	break;	}
				case IDC_MTL_DF_G:	{	switch(wparHi)	{	case EN_CHANGE:	{	OnChangeMtlDF(hWnd, IDC_MTL_DF_C);	break;	}}	break;	}
				case IDC_MTL_DF_B:	{	switch(wparHi)	{	case EN_CHANGE:	{	OnChangeMtlDF(hWnd, IDC_MTL_DF_C);	break;	}}	break;	}

				case IDC_MTL_AM_R:	{	switch(wparHi)	{	case EN_CHANGE:	{	OnChangeMtlAM(hWnd, IDC_MTL_AM_C);	break;	}}	break;	}
				case IDC_MTL_AM_G:	{	switch(wparHi)	{	case EN_CHANGE:	{	OnChangeMtlAM(hWnd, IDC_MTL_AM_C);	break;	}}	break;	}
				case IDC_MTL_AM_B:	{	switch(wparHi)	{	case EN_CHANGE:	{	OnChangeMtlAM(hWnd, IDC_MTL_AM_C);	break;	}}	break;	}

				case IDC_MTL_SP_R:	{	switch(wparHi)	{	case EN_CHANGE:	{	OnChangeMtlSP(hWnd, IDC_MTL_SP_C);	break;	}}	break;	}
				case IDC_MTL_SP_G:	{	switch(wparHi)	{	case EN_CHANGE:	{	OnChangeMtlSP(hWnd, IDC_MTL_SP_C);	break;	}}	break;	}
				case IDC_MTL_SP_B:	{	switch(wparHi)	{	case EN_CHANGE:	{	OnChangeMtlSP(hWnd, IDC_MTL_SP_C);	break;	}}	break;	}

				case IDC_MTL_EM_R:	{	switch(wparHi)	{	case EN_CHANGE:	{	OnChangeMtlEM(hWnd, IDC_MTL_EM_C);	break;	}}	break;	}
				case IDC_MTL_EM_G:	{	switch(wparHi)	{	case EN_CHANGE:	{	OnChangeMtlEM(hWnd, IDC_MTL_EM_C);	break;	}}	break;	}
				case IDC_MTL_EM_B:	{	switch(wparHi)	{	case EN_CHANGE:	{	OnChangeMtlEM(hWnd, IDC_MTL_EM_C);	break;	}}	break;	}

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




void CWndMtrl::SetPickColor(DWORD&	clrOut,DWORD& clrIn, INT nID, BOOL IsBGR)
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




void CWndMtrl::OnChangeMtlAL(HWND hWnd, INT nID)
{
	BYTE	ClrR = GetDlgItemInt(hWnd, nID + 1, 0,0);
	BYTE	ClrG = GetDlgItemInt(hWnd, nID + 2, 0,0);
	BYTE	ClrB = GetDlgItemInt(hWnd, nID + 3, 0,0);
	DWORD	dClr = RGB(ClrR, ClrG, ClrB);

	SetPickColor(dClr, dClr , nID, TRUE);
	m_dAL	= D3DXCOLOR( ClrR/255.f,ClrG/255.f,ClrB/255.f,1.f);
}



void CWndMtrl::OnChangeMtlDF(HWND hWnd, INT nID)
{
	BYTE	ClrR = GetDlgItemInt(hWnd, nID + 1, 0,0);
	BYTE	ClrG = GetDlgItemInt(hWnd, nID + 2, 0,0);
	BYTE	ClrB = GetDlgItemInt(hWnd, nID + 3, 0,0);
	DWORD	dClr = RGB(ClrR, ClrG, ClrB);

	SetPickColor(dClr, dClr , nID, TRUE);
	m_Mtl.dcD= D3DXCOLOR( ClrR/255.f,ClrG/255.f,ClrB/255.f,1.f);
}


void CWndMtrl::OnChangeMtlAM(HWND hWnd, INT nID)
{
	BYTE	ClrR = GetDlgItemInt(hWnd, nID + 1, 0,0);
	BYTE	ClrG = GetDlgItemInt(hWnd, nID + 2, 0,0);
	BYTE	ClrB = GetDlgItemInt(hWnd, nID + 3, 0,0);
	DWORD	dClr = RGB(ClrR, ClrG, ClrB);

	SetPickColor(dClr, dClr , nID, TRUE);
	m_Mtl.dcA= D3DXCOLOR( ClrR/255.f,ClrG/255.f,ClrB/255.f,1.f);
}


void CWndMtrl::OnChangeMtlSP(HWND hWnd, INT nID)
{
	BYTE	ClrR = GetDlgItemInt(hWnd, nID + 1, 0,0);
	BYTE	ClrG = GetDlgItemInt(hWnd, nID + 2, 0,0);
	BYTE	ClrB = GetDlgItemInt(hWnd, nID + 3, 0,0);
	DWORD	dClr = RGB(ClrR, ClrG, ClrB);

	SetPickColor(dClr, dClr , nID, TRUE);
	m_Mtl.dcS= D3DXCOLOR( ClrR/255.f,ClrG/255.f,ClrB/255.f,1.f);
}


void CWndMtrl::OnChangeMtlEM(HWND hWnd, INT nID)
{
	BYTE	ClrR = GetDlgItemInt(hWnd, nID + 1, 0,0);
	BYTE	ClrG = GetDlgItemInt(hWnd, nID + 2, 0,0);
	BYTE	ClrB = GetDlgItemInt(hWnd, nID + 3, 0,0);
	DWORD	dClr = RGB(ClrR, ClrG, ClrB);

	SetPickColor(dClr, dClr , nID, TRUE);
	m_Mtl.dcE= D3DXCOLOR( ClrR/255.f,ClrG/255.f,ClrB/255.f,1.f);
}