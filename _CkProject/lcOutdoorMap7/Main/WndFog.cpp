// Implementation of the CWndFog class.
//
////////////////////////////////////////////////////////////////////////////////


#include "../_StdAfx.h"


CLSS_DLG_DEFINE(CWndFog, MsgPrc);


INT CWndFog::Init()
{
	m_hWnd	= 0;
	m_hWPrn	= 0;


	m_Fog.m_dFt	= D3DFOG_LINEAR;
	m_Fog.m_dFc	= 0xFF336699;
	m_Fog.m_fFb	= 200.0f;
	m_Fog.m_fFe	= 5000.0f;
	m_Fog.m_fFd	= 1.f;

	int iSize = sizeof(m_crT)/ sizeof(m_crT[0]);

	for(int i=0; i< iSize; ++i)
		m_crT[i] = RGB( rand()%256, rand()%256, rand()%256);

	return 0;
}


INT CWndFog::Create(HWND hWnd)
{
	if(m_hWnd)
		return 0;

	m_hWPrn= hWnd;
	HINSTANCE hInst = GetModuleHandle(NULL);
	m_hWnd = CreateDialog(hInst,MAKEINTRESOURCE(IDD_FOG), m_hWPrn, CLSS_DLG_WNDPROC(CWndFog));
	m_hWfc	= GetDlgItem(m_hWnd, IDC_FOG_C);

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


	RECT		rc;
	HWND		hWpnt;
	HDC			hdc;
	BYTE*		pClr;
	BYTE		ClrR;
	BYTE		ClrG;
	BYTE		ClrB;
	HBRUSH		hbrsh;
	COLORREF	clrf;

	hWpnt	= m_hWfc;
	hdc		= GetDC(hWpnt);
	pClr	= LcUtil_GetColorFromARGB(m_Fog.m_dFc);
	ClrR	= pClr[1];
	ClrG	= pClr[2];
	ClrB	= pClr[3];
	clrf	= RGB(ClrR, ClrG, ClrB);
	hbrsh	= CreateSolidBrush(clrf);
	GetClientRect(hWpnt, &rc);
	FillRect(hdc, &rc, hbrsh);
	DeleteObject(hbrsh);
	ReleaseDC(hWpnt, hdc);

	SetDlgItemInt(m_hWnd, IDC_FOG_R, ClrR, 0);
	SetDlgItemInt(m_hWnd, IDC_FOG_G, ClrG, 0);
	SetDlgItemInt(m_hWnd, IDC_FOG_B, ClrB, 0);

	SetDlgItemFlt(m_hWnd, IDC_FOG_BGN, m_Fog.m_fFb, 0);
	SetDlgItemFlt(m_hWnd, IDC_FOG_END, m_Fog.m_fFe, 0);
	SetDlgItemFlt(m_hWnd, IDC_FOG_DNS, m_Fog.m_fFd, 4);

	CheckRadioButton(m_hWnd, IDC_FOG_NONE, IDC_FOG_NONE+4, IDC_FOG_NONE + m_Fog.m_dFt);


	
	return 0;
}



void CWndFog::ShowWindow(int _ishw)
{
	::ShowWindow(m_hWnd, _ishw);
}



void CWndFog::Destroy()
{
	SAFE_DESTROY_WIN(m_hWnd);
}



LRESULT CWndFog::MsgPrc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
			hWpnt	= m_hWfc;
			hdc		= BeginPaint(hWpnt, &ps);
			pClr	= LcUtil_GetColorFromARGB(m_Fog.m_dFc);
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
			
			break;
		}

		case WM_COMMAND:
		{
			switch(wparLo)
			{
				case IDC_FOG_OK:
				{
					GFIELD->Query("Set Fog", &m_Fog);

					break;
				}

				case IDC_FOG_CLOSE:
				{
					SendMessage(hWnd, WM_CLOSE, 0, 0);
					break;
				}


				case IDC_FOG_C:
				{	
					memset(&COL, 0, sizeof(CHOOSECOLOR));
					COL.lStructSize = sizeof(CHOOSECOLOR);
					COL.hwndOwner=hWnd;
					COL.lpCustColors=m_crT;

					if (ChooseColor(&COL))
					{
						SetPickColor(dClr, COL.rgbResult,IDC_FOG_C, TRUE);

						ClrR	= GetRValue(COL.rgbResult);
						ClrG	= GetGValue(COL.rgbResult);
						ClrB	= GetBValue(COL.rgbResult);
						
						m_Fog.m_dFc	= D3DCOLOR_XRGB( ClrR, ClrG, ClrB);

						SetDlgItemInt(m_hWnd, IDC_FOG_R, ClrR, 0);
						SetDlgItemInt(m_hWnd, IDC_FOG_G, ClrG, 0);
						SetDlgItemInt(m_hWnd, IDC_FOG_B, ClrB, 0);
					}

					break;
				}

				case IDC_FOG_BGN:{	switch(wparHi)	{	case EN_CHANGE:	{	m_Fog.m_fFb= GetDlgItemFlt(hWnd, IDC_FOG_BGN);break;	}}	break;	}
				case IDC_FOG_END:{	switch(wparHi)	{	case EN_CHANGE:	{	m_Fog.m_fFe= GetDlgItemFlt(hWnd, IDC_FOG_END);break;	}}	break;	}
				case IDC_FOG_DNS:{	switch(wparHi)	{	case EN_CHANGE:	{	m_Fog.m_fFd= GetDlgItemFlt(hWnd, IDC_FOG_DNS);break;	}}	break;	}

				case IDC_FOG_NONE:	{	m_Fog.m_dFt = D3DFOG_NONE;		break;	}
				case IDC_FOG_EXP:	{	m_Fog.m_dFt = D3DFOG_EXP;		break;	}
				case IDC_FOG_EXP2:	{	m_Fog.m_dFt = D3DFOG_EXP2;		break;	}
				case IDC_FOG_LINE:	{	m_Fog.m_dFt = D3DFOG_LINEAR;	break;	}

				case IDC_FOG_R:	{	switch(wparHi)	{	case EN_CHANGE:	{	OnChangeColor(hWnd, IDC_FOG_C);	break;	}}	break;	}
				case IDC_FOG_G:	{	switch(wparHi)	{	case EN_CHANGE:	{	OnChangeColor(hWnd, IDC_FOG_C);	break;	}}	break;	}
				case IDC_FOG_B:	{	switch(wparHi)	{	case EN_CHANGE:	{	OnChangeColor(hWnd, IDC_FOG_C);	break;	}}	break;	}
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




void CWndFog::SetPickColor(DWORD&	clrOut,DWORD& clrIn, INT nID, BOOL IsBGR)
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




void CWndFog::OnChangeColor(HWND hWnd, INT nID)
{
	BYTE	ClrR = GetDlgItemInt(hWnd, nID + 1, 0,0);
	BYTE	ClrG = GetDlgItemInt(hWnd, nID + 2, 0,0);
	BYTE	ClrB = GetDlgItemInt(hWnd, nID + 3, 0,0);
	DWORD	dClr = RGB(ClrR, ClrG, ClrB);

	SetPickColor(dClr, dClr , nID, TRUE);
	m_Fog.m_dFc	= D3DCOLOR_XRGB( ClrR,ClrG,ClrB);
}
