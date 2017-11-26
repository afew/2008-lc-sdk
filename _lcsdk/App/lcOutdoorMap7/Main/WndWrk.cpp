// Implementation of the CWndWrk class.
//
////////////////////////////////////////////////////////////////////////////////


#include "../_StdAfx.h"


CLSS_DLG_DEFINE(CWndWrk, MsgPrc);


INT CWndWrk::Init()
{
	m_hTip	= NULL;
	
	m_nGrp	=0;
	m_nElm	=0;
	m_nLst	=0;
		
	m_iRng	= 4;
	m_fDH	= 5.f;
	m_bAc	= 1;

	m_pMdB	= NULL;


	m_dC= 0X66FFFD84;	

	for(int i=0; i< sizeof(m_crT)/sizeof(m_crT[0]); ++i)
		m_crT[i] = RGB( 128+rand()%128, 128+rand()%128, 128+rand()%128);


	return 0;
}


INT CWndWrk::Create(HWND hWnd)
{
	m_hWnd		= 0;

	RECT rt1;
	RECT rt2;

	INT iWd;
	INT iHg;
	INT iX;
	INT iY;

	HINSTANCE hInst = GetModuleHandle(NULL);

	m_gWrk	= ImageList_LoadImage(hInst, MAKEINTRESOURCE(IDB_WRK), 16, 1, RGB(255,255,255),IMAGE_BITMAP,0);
	m_hWnd	= CreateDialog(hInst, MAKEINTRESOURCE(IDD_WRK), hWnd, CLSS_DLG_WNDPROC(CWndWrk));
	m_hTip	= CreateWindow(TOOLTIPS_CLASS
					,	NULL
					,	0
					,	CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT
					,	m_hWnd
					,	NULL
					,	GetModuleHandle(NULL)
					,	NULL);

	

	m_hGrp = GetDlgItem(m_hWnd, IDC_WRK_GRP);
	m_hElm = GetDlgItem(m_hWnd, IDC_WRK_OBJ);
	m_hLst = GetDlgItem(m_hWnd, IDC_WRK_LST);

	SetDlgItemInt(m_hWnd, IDC_WRK_DH,	m_fDH,	1);
	SetDlgItemInt(m_hWnd, IDC_WRK_RNG, m_iRng,	0);

	SendMessage(GetDlgItem(m_hWnd, IDC_WRK_AC),BM_SETCHECK,m_bAc,0);

	memset(m_bmFld, 0, sizeof(m_bmFld));
	m_bmFld[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ICN26));
	m_bmFld[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ICN27));
	m_bmFld[2] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ICN29));
	m_bmFld[3] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ICN30));
	m_bmFld[4] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ICN31));
	m_bmFld[5] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ICN32));
	m_bmFld[6] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ICN35));

	SendDlgItemMessage(m_hWnd, IDC_WRK_FLD_CRT,	 BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_bmFld[0]);
	SendDlgItemMessage(m_hWnd, IDC_WRK_FLD_LOAD, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_bmFld[3]);
	SendDlgItemMessage(m_hWnd, IDC_WRK_FLD_SAVE, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_bmFld[4]);
	
	SendDlgItemMessage(m_hWnd, IDC_WRK_LCL_CRT,	 BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_bmFld[1]);
	SendDlgItemMessage(m_hWnd, IDC_WRK_LCL_LOAD, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_bmFld[6]);
	SendDlgItemMessage(m_hWnd, IDC_WRK_LCL_SAVE, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_bmFld[5]);
	SendDlgItemMessage(m_hWnd, IDC_WRK_LCL_MOVE, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_bmFld[2]);
	

	
	ReadDataGrp();
	ReadDataElm();
	ReadDataLst();

	SetMdB(m_nGrp, m_nElm, m_nLst);


	m_hCol	= GetDlgItem(m_hWnd, IDC_WRK_COL);

	INT		iWidth;
	INT		iHeight;

	GetWindowRect(GHWND, &rt1);
	GetWindowRect(m_hWnd, &rt2);
	
	iWidth = rt2.right - rt2.left;
	iHeight=  rt2.bottom- rt2.top;

	iX = rt1.left -0;
	iY = rt1.top  +50;
	
	MoveWindow(m_hWnd, iX, iY, iWidth, iHeight, TRUE);

	memset(m_bTool, 0, sizeof(m_bTool));
	memset(m_bmBtn, 0, sizeof(m_bmBtn));

	m_bmBtn[ 0] = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_ICN01));
	m_bmBtn[ 1] = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_ICN02));
	m_bmBtn[ 2] = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_ICN03));
	m_bmBtn[ 3] = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_ICN04));
	m_bmBtn[ 4] = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_ICN21));
	
	m_bmBtn[ 5] = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_ICN22));
	m_bmBtn[ 6] = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_ICN23));
	m_bmBtn[ 7] = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_ICN24));
	m_bmBtn[ 8] = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_ICN25));
	m_bmBtn[ 9] = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_ICN26));
	
	m_bmBtn[10] = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_ICN27));
	m_bmBtn[11] = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_ICN28));
	m_bmBtn[12] = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_ICN29));
	m_bmBtn[13] = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_ICN30));
	m_bmBtn[14] = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_ICN32));
	
	m_bmBtn[15] = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_ICN33));
	m_bmBtn[16] = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_ICN34));
	m_bmBtn[17] = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_ICN35));
	m_bmBtn[18] = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_ICN36));
	m_bmBtn[19] = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_ICN37));
	m_bmBtn[20] = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_ICN38));
	m_bmBtn[21] = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_ICN39));

	SendDlgItemMessage(m_hWnd,IDC_WRK_BT01,BM_SETIMAGE, (WPARAM)IMAGE_BITMAP,(LPARAM)m_bmBtn[ 0]);
	SendDlgItemMessage(m_hWnd,IDC_WRK_BT02,BM_SETIMAGE, (WPARAM)IMAGE_BITMAP,(LPARAM)m_bmBtn[ 1]);
	SendDlgItemMessage(m_hWnd,IDC_WRK_BT03,BM_SETIMAGE, (WPARAM)IMAGE_BITMAP,(LPARAM)m_bmBtn[ 2]);
	SendDlgItemMessage(m_hWnd,IDC_WRK_BT04,BM_SETIMAGE, (WPARAM)IMAGE_BITMAP,(LPARAM)m_bmBtn[ 3]);

	SendDlgItemMessage(m_hWnd,IDC_WRK_BT05,BM_SETIMAGE, (WPARAM)IMAGE_BITMAP,(LPARAM)m_bmBtn[ 4]);
	SendDlgItemMessage(m_hWnd,IDC_WRK_BT06,BM_SETIMAGE, (WPARAM)IMAGE_BITMAP,(LPARAM)m_bmBtn[ 5]);

	SendDlgItemMessage(m_hWnd,IDC_WRK_BT07,BM_SETIMAGE, (WPARAM)IMAGE_BITMAP,(LPARAM)m_bmBtn[18]);
	SendDlgItemMessage(m_hWnd,IDC_WRK_BT08,BM_SETIMAGE, (WPARAM)IMAGE_BITMAP,(LPARAM)m_bmBtn[19]);
	
	SendDlgItemMessage(m_hWnd,IDC_WRK_BT09,BM_SETIMAGE, (WPARAM)IMAGE_BITMAP,(LPARAM)m_bmBtn[20]);
	SendDlgItemMessage(m_hWnd,IDC_WRK_BT10,BM_SETIMAGE, (WPARAM)IMAGE_BITMAP,(LPARAM)m_bmBtn[16]);
	SendDlgItemMessage(m_hWnd,IDC_WRK_BT11,BM_SETIMAGE, (WPARAM)IMAGE_BITMAP,(LPARAM)m_bmBtn[15]);
	SendDlgItemMessage(m_hWnd,IDC_WRK_BT12,BM_SETIMAGE, (WPARAM)IMAGE_BITMAP,(LPARAM)m_bmBtn[11]);
	SendDlgItemMessage(m_hWnd,IDC_WRK_BT13,BM_SETIMAGE, (WPARAM)IMAGE_BITMAP,(LPARAM)m_bmBtn[21]);
	SendDlgItemMessage(m_hWnd,IDC_WRK_BT14,BM_SETIMAGE, (WPARAM)IMAGE_BITMAP,(LPARAM)m_bmBtn[ 7]);

	BYTE* pClr	= LcUtil_GetColorFromARGB(m_dC);
	BYTE ClrR	= pClr[1];
	BYTE ClrG	= pClr[2];
	BYTE ClrB	= pClr[3];

	SetDlgItemInt(m_hWnd, IDC_WRK_COL_R, ClrR, 0);
	SetDlgItemInt(m_hWnd, IDC_WRK_COL_G, ClrG, 0);
	SetDlgItemInt(m_hWnd, IDC_WRK_COL_B, ClrB, 0);
	SetDlgItemHex(m_hWnd, IDC_WRK_COL_ALL, m_dC);

//	CheckRadioButton(m_hWnd, IDC_WRK_BT01, IDC_WRK_BT14, IDC_WRK_BT14);

	::ShowWindow(m_hWnd, SW_SHOW);

	GetWindowRect(GHWND, &rt1);
	GetWindowRect(m_hWnd, &rt2);
	
	iWd = rt2.right - rt2.left;
	iHg=  rt2.bottom- rt2.top;

	iX = rt1.left - iWd;
	iY = rt1.top;
	
	MoveWindow(m_hWnd, iX, iY, iWd, iHg, TRUE);
	GetWindowRect(m_hWnd, &rt2);

	TOOLINFO ti;

	for(int i=IDC_WRK_FLD_CRT; i<=IDC_WRK_BT14; ++i)
	{
		HWND hWnd = GetDlgItem(m_hWnd, i);
		char	sText[128];
		
		memset(sText,0, sizeof sText);
		GetWindowText(hWnd, sText, sizeof sText);

		ti.cbSize=sizeof(TOOLINFO);
		ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
		ti.hwnd=m_hWnd;
		ti.uId=(WPARAM)hWnd;
		ti.lpszText= sText;
		
		SendMessage(m_hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti);
	}

	return 0;
}


void CWndWrk::Destroy()
{
	SAFE_DELETE(	m_pMdB	);	
	SAFE_DEL_OBJ_ARRAY(m_bmFld, (sizeof(m_bmFld)/sizeof(m_bmFld[0])) );
	SAFE_DEL_OBJ_ARRAY(m_bmBtn, sizeof(m_bmBtn)/sizeof(m_bmBtn[0]) );

	SAFE_DESTROY_WIN(m_hWnd);
}



INT	CWndWrk::Restore()
{
	HRESULT hr;

	if(!GMAIN->IsWindow())
		return 0;

	m_SwpWn.hW = (HWND)GetDlgItem(m_hWnd, IDC_WRK_PANNEL);

	D3DPRESENT_PARAMETERS	par;
	memset(&par, 0, sizeof(D3DPRESENT_PARAMETERS));
	
	par.SwapEffect = D3DSWAPEFFECT_DISCARD;
	par.Windowed = TRUE;
	
	RECT rt;
	GetWindowRect(m_SwpWn.hW, &rt);

	par.BackBufferWidth= 0;
	par.BackBufferHeight= 0;
	par.hDeviceWindow = m_SwpWn.hW;

	hr = GDEVICE->CreateAdditionalSwapChain(&par, &m_SwpWn.pC) ;

	if ( FAILED(hr) )
	{
		LcUtil_ErrMsgBox("Create addtional swap chain failed");
		return hr;
	}

	hr = m_SwpWn.pC->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &m_SwpWn.pB);

	if ( FAILED(hr) )
	{
		LcUtil_ErrMsgBox("Get back buffer Failed");
		return hr;
	}


	DWORD format = GMAIN->GetDxDepthFMT();
	hr = GDEVICE->CreateDepthStencilSurface(par.BackBufferWidth, par.BackBufferHeight, (D3DFORMAT)format, D3DMULTISAMPLE_NONE, 0, 0, &m_SwpWn.pS, NULL);

	if ( FAILED(hr))
	{
		LcUtil_ErrMsgBox("Create DepthStencil Surface Failed");
		return -1;
	}


	SAFE_RESTORE(&m_WndLyr);

	return 0;
}


void CWndWrk::Invalidate()
{
	m_SwpWn.Release();

	SAFE_INVALID(&m_WndLyr);
}


INT CWndWrk::FrameMove()
{
	SAFE_FRMOV(	m_pMdB	);

	SAFE_FRMOV(&m_WndLyr);

	RECT	rc;
	HDC		hdc;
	hdc		= GetDC(m_hCol);

	GetClientRect(m_hCol, &rc);
	HBRUSH hbrsh = CreateSolidBrush( RGB( (0x00FF0000&m_dC)>>16, (0x0000FF00&m_dC)>>8, (0x000000FF&m_dC) ));
	FillRect(hdc, &rc, hbrsh);
	DeleteObject(hbrsh);
	ReleaseDC(m_hWnd, hdc);

	return 0;
}



void CWndWrk::Render()
{
	if(!GMAIN->IsWindow())
		return;

	static int nCnt=0;
	++nCnt;

	if(0 != nCnt%16)
		return;
	
	nCnt=0;

	GDEVICE->SetRenderTarget(0, m_SwpWn.pB);
	GDEVICE->SetDepthStencilSurface(m_SwpWn.pS);

	if(GCAMOBJ)
	{
		GCAMOBJ->FrameMove();
		GCAMOBJ->SetTransformViw();
		GCAMOBJ->SetTransformPrj();
	}

	
	GDEVICE->Clear( 0L, 0, GMAIN->GetDxClearMode(),  0x00006699, 1.0f, 0L );

	RECT rt={5,10, 1024,30};
	GMAIN->m_pDxFnt->DrawText(NULL, "Object", -1, &rt, 0, 0XFFFFFFFF);

	GDEVICE->SetRenderState(D3DRS_LIGHTING, FALSE);

	SAFE_RENDER(	GMAIN->m_pGrid	);
	SAFE_RENDER(	m_pMdB	);

	m_SwpWn.pC->Present(0, 0, 0, 0, 0);

	SAFE_RENDER(&m_WndLyr);
}


LRESULT CWndWrk::MsgPrc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WPARAM		wparHi = HIWORD(wParam);
	WPARAM		wparLo = LOWORD(wParam);

	BYTE		ClrR;
	BYTE		ClrG;
	BYTE		ClrB;

	switch( uMsg )
	{
		case WM_COMMAND:
		{
			switch(wparLo)
			{
				case IDC_WRK_FLD_CRT:
				{
					m_WndFog.Init();
					m_WndLght.Init();
					m_WndFld.Create(GHWND);
					m_WndFld.ShowWindow();

					break;
				}

				case IDC_WRK_FLD_LOAD:
				{
					if(GFIELD)
					{
						char			sDir[MAX_PATH]="\0";
						OPENFILENAME	OFN;
						char			sF[MAX_PATH];
						char			sFullF[MAX_PATH]="";

						GetCurrentDirectory(MAX_PATH, sDir);

						memset(&OFN, 0, sizeof(OFN));
						OFN.lStructSize 	= sizeof(OFN);
						OFN.hwndOwner		= hWnd;
						OFN.lpstrFilter 	="Field Files(*.bod)\0*.bod\0All Files(*.*)\0*.*\0";
						OFN.lpstrFile		=sFullF;
						OFN.nMaxFile		=MAX_PATH;

						OFN.lpstrTitle		="Filed Open...";
						OFN.lpstrFileTitle	=sF;
						OFN.nMaxFileTitle	=MAX_PATH;
						OFN.lpstrDefExt 	="bod";
						OFN.lpstrInitialDir =sDir;

						if( !GetOpenFileName(&OFN))
						{
							SetCurrentDirectory(sDir);
							return -1;
						}

						SetCurrentDirectory(sDir);

						GFIELD->Query("File Load", sFullF);


						ILcxmT::MpEnv Env;
						GFIELD->Query("Get Environment", &Env);

						m_WndLght.m_iLg	= Env.m_iLg;							// Lighting Num

						memset(m_WndLght.m_pLg, 0, sizeof(m_WndLght.m_pLg));
						memcpy(m_WndLght.m_pLg, Env.m_pLg, sizeof(Env.m_pLg));

						memcpy(&m_WndFog.m_Fog,	&Env.m_Fog, sizeof(Env.m_Fog));	// Fog Table Mode

						SetFocus(GHWND);
					}

					break;
				}

				case IDC_WRK_FLD_SAVE:
				{
					if(GFIELD)
					{
						char			sDir[MAX_PATH]="\0";
						OPENFILENAME	OFN;
						char			sF[MAX_PATH];
						char			sFullF[MAX_PATH]="";

						GetCurrentDirectory(MAX_PATH, sDir);

						memset(&OFN, 0, sizeof(OFN));
						OFN.lStructSize 	= sizeof(OFN);
						OFN.hwndOwner		= hWnd;
						OFN.lpstrFilter 	="Field Files(*.bod)\0*.bod\0All Files(*.*)\0*.*\0";
						OFN.lpstrFile		=sFullF;
						OFN.nMaxFile		=MAX_PATH;

						OFN.lpstrTitle		="Filed Save...";
						OFN.lpstrFileTitle	=sF;
						OFN.nMaxFileTitle	=MAX_PATH;
						OFN.lpstrDefExt 	="bod";
						OFN.lpstrInitialDir =sDir;

						if( !GetOpenFileName(&OFN))
						{
							SetCurrentDirectory(sDir);
							return -1;
						}

						SetCurrentDirectory(sDir);

						GFIELD->Query("File Save", sFullF);

						MessageBox(hWnd, "File Saved", "Msg", MB_OK);

						SetFocus(GHWND);
					}

					break;
				}

				case IDC_WRK_LCL_CRT:
				{
					if( !GFIELD)
					{
						char	sField[256]={0};
						GFIELD->Query("Get Field Name", sField);

						if(2>strlen(sField) )
							MessageBox(hWnd, "You must create field.     ", "Caution", MB_ICONEXCLAMATION);
					}

					m_WndLcl.Create(GHWND);
					m_WndLcl.ShowWindow();

					break;
				}

				case IDC_WRK_RNG:
				{
					switch(wparHi)
					{
						case EN_CHANGE:
							m_iRng = GetDlgItemInt(hWnd, IDC_WRK_RNG, 0,0);
							break;
					}

					break;
				}


				case IDC_WRK_DH:
				{
					switch(wparHi)
					{
						case EN_CHANGE:
							m_fDH= GetDlgItemFlt(hWnd, IDC_WRK_DH);
							break;
					}

					break;
				}


				case IDC_WRK_AC:
				{
					if (SendMessage(GetDlgItem(hWnd, IDC_WRK_AC),BM_GETCHECK,0,0)==BST_CHECKED)
						m_bAc = 1;
					else
						m_bAc =0;

					break;
				}

				case IDC_WRK_GRP:
				{
					switch( wparHi)
					{
						case LBN_SELCHANGE:
							m_nGrp = SendMessage(m_hGrp, CB_GETCURSEL,0,0);
							m_nElm = 0;
							m_nLst = 0;

							ReadDataElm();
							ReadDataLst();
							SetMdB(m_nGrp, m_nElm, m_nLst);
							break;
					}

					break;
				}

				case IDC_WRK_OBJ:
				{
					switch( wparHi)
					{
						case LBN_SELCHANGE:
							m_nElm = SendMessage(m_hElm, CB_GETCURSEL,0,0);
							m_nLst = 0;
							ReadDataLst();
							SetMdB(m_nGrp, m_nElm, m_nLst);
							break;
					}

					break;
				}

				case IDC_WRK_LST:
				{
					switch( wparHi)
					{
						case LBN_SELCHANGE:
							m_nLst = SendMessage(m_hLst, LB_GETCURSEL,0,0);
							SetMdB(m_nGrp, m_nElm, m_nLst);
							break;
					}

					break;
				}

				case IDC_WRK_BT01:	SetActWnd(IDC_WRK_BT01);	break;
				case IDC_WRK_BT02:	SetActWnd(IDC_WRK_BT02);	break;
				case IDC_WRK_BT03:	SetActWnd(IDC_WRK_BT03);	break;
				case IDC_WRK_BT04:	SetActWnd(IDC_WRK_BT04);	break;
				case IDC_WRK_BT05:	SetActWnd(IDC_WRK_BT05);	break;
				case IDC_WRK_BT06:	SetActWnd(IDC_WRK_BT06);	break;
				case IDC_WRK_BT07:	SetActWnd(IDC_WRK_BT07);	break;
				case IDC_WRK_BT08:	SetActWnd(IDC_WRK_BT08);	break;
				case IDC_WRK_BT09:	SetActWnd(IDC_WRK_BT09);	break;
				case IDC_WRK_BT10:	SetActWnd(IDC_WRK_BT10);	break;

				case IDC_WRK_BT11:	SetActWnd(IDC_WRK_BT11);	break;
				case IDC_WRK_BT12:	SetActWnd(IDC_WRK_BT12);	break;
				case IDC_WRK_BT13:	SetActWnd(IDC_WRK_BT13);	break;
				case IDC_WRK_BT14:	SetActWnd(IDC_WRK_BT14);	break;

				case IDC_WRK_COL:
				{
					CHOOSECOLOR COL;
					
					memset(&COL, 0, sizeof(CHOOSECOLOR));
					COL.lStructSize = sizeof(CHOOSECOLOR);
					COL.hwndOwner=hWnd;
					COL.lpCustColors=m_crT;

					if (ChooseColor(&COL))
					{
						HWND hwWrk = GMAIN->m_WndWrk.m_hWnd;
						SetPickColor(COL.rgbResult, TRUE);
						
						BYTE* pClr	= LcUtil_GetColorFromARGB(m_dC);
						ClrR	= pClr[1];
						ClrG	= pClr[2];
						ClrB	= pClr[3];

						SetDlgItemInt(hWnd, IDC_WRK_COL_R, ClrR, 0);
						SetDlgItemInt(hWnd, IDC_WRK_COL_G, ClrG, 0);
						SetDlgItemInt(hWnd, IDC_WRK_COL_B, ClrB, 0);
						SetDlgItemHex(hWnd, IDC_WRK_COL_ALL, m_dC);
					}

					break;
				}

				case IDC_WRK_COL_R:
				{
					switch(wparHi)
					{
						case EN_CHANGE:
						{
							ClrR = GetDlgItemInt(hWnd, IDC_WRK_COL_R, 0, 0);
							ClrG = GetDlgItemInt(hWnd, IDC_WRK_COL_G, 0, 0);
							ClrB = GetDlgItemInt(hWnd, IDC_WRK_COL_B, 0, 0);
							DWORD dC = RGB(ClrR, ClrG, ClrB);
							SetPickColor(dC, TRUE);
							SetDlgItemHex(hWnd, IDC_WRK_COL_ALL, m_dC);
							
							break;
						}
					}

					break;
				}

				case IDC_WRK_COL_G:
				{
					switch(wparHi)
					{
						case EN_CHANGE:
						{
							ClrR = GetDlgItemInt(hWnd, IDC_WRK_COL_R, 0, 0);
							ClrG = GetDlgItemInt(hWnd, IDC_WRK_COL_G, 0, 0);
							ClrB = GetDlgItemInt(hWnd, IDC_WRK_COL_B, 0, 0);
							DWORD dC = RGB(ClrR, ClrG, ClrB);
							SetPickColor(dC, TRUE);
							SetDlgItemHex(hWnd, IDC_WRK_COL_ALL, m_dC);
							
							break;
						}
					}

					break;
				}

				case IDC_WRK_COL_B:
				{
					switch(wparHi)
					{
						case EN_CHANGE:
						{
							ClrR = GetDlgItemInt(hWnd, IDC_WRK_COL_R, 0, 0);
							ClrG = GetDlgItemInt(hWnd, IDC_WRK_COL_G, 0, 0);
							ClrB = GetDlgItemInt(hWnd, IDC_WRK_COL_B, 0, 0);
							DWORD dC = RGB(ClrR, ClrG, ClrB);
							SetPickColor(dC, TRUE);
							SetDlgItemHex(hWnd, IDC_WRK_COL_ALL, m_dC);
							
							break;
						}
					}

					break;
				}


				case IDC_WRK_COL_ALL:
				{
					break;
				}

			}// switch

			break;

		}// case WM_COMMAND


		case WM_NOTIFY:
		{
			break;
		}// case


		

	}

	return(FALSE);
}




INT CWndWrk::SetMdB(WORD nI1, WORD nI2, WORD nI3)
{
	char		sFile[MAX_PATH];

	SendMessage(m_hLst, LB_GETTEXT, m_nLst, (LPARAM)sFile);

	SAFE_DELETE(	m_pMdB	);
	
	INT				nM=-1, nS=-1;
	WORD			wIds[4] = {nI1, nI2, nI3, 0xFFFF};
	Lcm::ILcmMesh*	pSrc = NULL;

	if(SUCCEEDED(TBMDB->Select((void**)&pSrc, &nM, &nS, sFile)))
	{
		PDEV			pDev = NULL;
		Lcm::TbndAB*	pBnd = (Lcm::TbndAB*)pSrc->GetBndInf();

		pSrc->Query("Get Device", &pDev);

		if(12 == pSrc->GetType())
		{
			if(FAILED(Lcm::LcxMdl_Create("billboard", &m_pMdB, pDev, pSrc)))
				return -1;
		}

		else if(13 == pSrc->GetType())
		{
			if(FAILED(Lcm::LcxMdl_Create("solid", &m_pMdB, pDev, pSrc)))
				return -1;
		}

		m_pMdB->SetRotation((FLOAT*)&VEC3(0,0, 0));
		m_pMdB->SetScaling(VEC3(1,1, 1));
		m_pMdB->SetPosition((FLOAT*)&VEC3(0,0, -5));

		m_pMdB->Query("Set Camera", GCAMOBJ);
		m_pMdB->Query("set bound", pBnd);
		m_pMdB->Query("set ids", wIds);
	}
	

	return 0;
}



void CWndWrk::ReadDataGrp()
{
	INT		i=0;
	char	sTmp[512];
	INT		iNt=0;

	m_vStrGrp.clear();

	GetCurrentDirectory(sizeof(sTmp), sTmp);

	GetPrivateProfileString("Group", "Num", NULL, sTmp, sizeof(sTmp),"Data/Model.ini");
	iNt = atoi(sTmp);

	for(i=0; i<iNt; ++i)
	{
		GetPrivateProfileString("Group", LcStr_Format("%d", i), NULL, sTmp, sizeof(sTmp),"Data/Model.ini");
		m_vStrGrp.push_back(sTmp);
	}

	SendMessage(m_hGrp, CB_RESETCONTENT, 0, 0);

	for (i=0; i<m_vStrGrp.size(); ++i)
		SendMessage(m_hGrp, CB_ADDSTRING, 0, (LPARAM)m_vStrGrp[i].c_str() );

	SendMessage(m_hGrp, CB_SETCURSEL, m_nGrp, 0);
}


void CWndWrk::ReadDataElm()
{
	char	sGrp[128];
	INT		i=0;
	char	sTmp[512];
	INT		iNt=0;
	
	memset(sGrp, 0, sizeof(sGrp));
	strcpy(sGrp, (char*)m_vStrGrp[m_nGrp].c_str());

	m_vStrElm.clear();

	GetPrivateProfileString(sGrp, "Num", NULL, sTmp, sizeof(sTmp),"Data/Model.ini");
	iNt = atoi(sTmp);

	for(i=0; i<iNt; ++i)
	{
		GetPrivateProfileString(sGrp, LcStr_Format("%d", i), NULL, sTmp, sizeof(sTmp),"Data/Model.ini");
		m_vStrElm.push_back(sTmp);
	}

	SendMessage(m_hElm, CB_RESETCONTENT, 0, 0);

	for (i=0; i<m_vStrElm.size(); ++i)
		SendMessage(m_hElm, CB_ADDSTRING, 0, (LPARAM)m_vStrElm[i].c_str() );

	SendMessage(m_hElm, CB_SETCURSEL, m_nElm, 0);
}


void CWndWrk::ReadDataLst()
{
	char	sElm[128];
	INT		i=0, j=0;
	char	sTmp[512];
	INT		iNt=0;
	
	memset(sElm, 0, sizeof(sElm));
	strcpy(sElm, (char*)m_vStrElm[m_nElm].c_str());

	m_vStrLst.clear();

	GetPrivateProfileString(sElm, "Num", NULL, sTmp, sizeof(sTmp),"Data/Model.ini");
	iNt = atoi(sTmp);

	for(i=0; i<iNt; ++i)
	{
		GetPrivateProfileString(sElm, LcStr_Format("%d", i), NULL, sTmp, sizeof(sTmp),"Data/Model.ini");
		m_vStrLst.push_back(sTmp);
	}

	SendMessage(m_hLst, LB_RESETCONTENT, 0, 0);

	for (i=0; i<m_vStrLst.size(); ++i)
		SendMessage(m_hLst, LB_ADDSTRING, 0, (LPARAM)m_vStrLst[i].c_str() );

	SendMessage(m_hLst, LB_SETCURSEL, m_nLst, 0);
}



void CWndWrk::SetPickColor(DWORD dC, BOOL IsBGR)
{
	COLORREF	col;
	BYTE		bgr[4];

	memset(bgr, 0, sizeof(bgr));
	memcpy(bgr, &dC, sizeof(bgr));

	if(IsBGR)
	{
		m_dC = D3DCOLOR_ARGB(0x66, bgr[0],bgr[1], bgr[2]);
		col = dC;
	}
	else
	{
		m_dC = dC;
		col = RGB(bgr[2],bgr[1], bgr[0]);
	}
}

FLOAT CWndWrk::GetDirection()
{
	return (m_bTool[ 0]*1.f - m_bTool[ 1]*1.f);
}



void CWndWrk::SetColor(DWORD dc)
{
	m_dC = dc;
	BYTE* pClr	= LcUtil_GetColorFromARGB(m_dC);
	BYTE ClrR	= pClr[1];
	BYTE ClrG	= pClr[2];
	BYTE ClrB	= pClr[3];

	SetDlgItemInt(m_hWnd, IDC_WRK_COL_R, ClrR, 0);
	SetDlgItemInt(m_hWnd, IDC_WRK_COL_G, ClrG, 0);
	SetDlgItemInt(m_hWnd, IDC_WRK_COL_B, ClrB, 0);
}


void CWndWrk::SetActWnd(INT nId)
{
	//초기화
	memset(m_bTool, 0, sizeof(m_bTool));

	// 활성화
	m_bTool[nId - IDC_WRK_BT01] = 1;

	if(nId != IDC_WRK_BT10 && m_WndLyr.m_hWnd)
		SendMessage(m_WndLyr.m_hWnd, WM_CLOSE, 0, 0);


	// Material
	if(IDC_WRK_BT07 == nId)
	{
		m_WndMtrl.Create(GHWND);
		m_WndMtrl.ShowWindow();
	}

	// Lighting
	if(IDC_WRK_BT08 == nId)
	{
		m_WndLght.Create(GHWND);
		m_WndLght.ShowWindow();
	}

	// Fog
	if(IDC_WRK_BT09 == nId)
	{
		m_WndFog.Create(GHWND);
		m_WndFog.ShowWindow();
	}

	// Layer
	if(IDC_WRK_BT10 == nId)
	{
		m_WndLyr.Create(GHWND);
		m_WndLyr.ShowWindow();
	}

	// Object
	if(IDC_WRK_BT13 == nId)
	{
		if(SUCCEEDED(m_WndObj.Create(GHWND)))
			m_WndObj.ShowWindow();
	}

	
}


