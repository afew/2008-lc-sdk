// Implementation of the CMain class.
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


INT FileNameOpen(char* sOut, HWND hWnd, char* DefaultExt = "Lcm", char* Filter ="Lcm Files(*.acm)\0*.acm\0All Files(*.*)\0*.*\0", char* sInit = NULL);
INT	LineConvertToDos(char* sLine);
INT LineConvertToEdit(char* sLine);


LRESULT CMain::MsgProc( HWND hWnd, UINT msg, WPARAM wParam,LPARAM lParam)
{
	if(m_pInput && m_pInput->MsgProc(hWnd, msg, wParam, lParam))
		return 0;
	
	WPARAM	wparHi = HIWORD(wParam);
	WPARAM	wparLo = LOWORD(wParam);
	HMENU	hMenu = ::GetMenu(hWnd);
	
	switch( msg )
	{
		case WM_PAINT:
		{
			if( m_bLoadingApp )
			{
				RECT rc;
				HDC hDC = GetDC( hWnd );
				GetClientRect( hWnd, &rc );
				ReleaseDC( hWnd, hDC );
			}
			break;
		}// case WM_PAINT


		case WM_COMMAND:
		{

			if(m_dRscToggle && m_dRscToggle == wparLo)
			{
				if(m_bWindowed)
				{
					ShowWindow(this->m_hWndDlg, SW_HIDE);
					ShowWindow(this->m_hWndAni, SW_HIDE);
				}
				else
				{
					ShowWindow(this->m_hWndDlg, SW_SHOW);
					ShowWindow(this->m_hWndAni, SW_SHOW);
				}
			}


			switch(wparLo)
			{
				case ID_MNU_OPEN_LCM:
				{
					char	sFull[MAX_PATH];

					if(FAILED(FileNameOpen(sFull, hWnd)))
						return 0;

					if(SUCCEEDED(ResetModel(sFull)))
					{
						ResetHlsltoMdl();
					}

					break;
				}

				case ID_MNU_OPEN_LNK:
				{
					char	sFull[MAX_PATH];

					if(FAILED(FileNameOpen(sFull, hWnd)))
						return 0;

					if(SUCCEEDED(ResetLink(sFull)))
					{
						ResetHlsltoMdl();
					}

					break;
				}


				case ID_VIEW_POINT:
				{
					m_bOptFill	= D3DFILL_POINT;
					CheckMenuItem(hMenu, ID_VIEW_POINT, MF_CHECKED);
					CheckMenuItem(hMenu, ID_VIEW_WIRE , MF_UNCHECKED);
					CheckMenuItem(hMenu, ID_VIEW_SOLID, MF_UNCHECKED);
					break;
				}

				case ID_VIEW_WIRE:
				{
					m_bOptFill	= D3DFILL_WIREFRAME;
					CheckMenuItem(hMenu, ID_VIEW_POINT, MF_UNCHECKED);
					CheckMenuItem(hMenu, ID_VIEW_WIRE , MF_CHECKED);
					CheckMenuItem(hMenu, ID_VIEW_SOLID, MF_UNCHECKED);
					break;
				}

				case ID_VIEW_SOLID:
				{
					m_bOptFill	= D3DFILL_SOLID;
					CheckMenuItem(hMenu, ID_VIEW_POINT, MF_UNCHECKED);
					CheckMenuItem(hMenu, ID_VIEW_WIRE , MF_UNCHECKED);
					CheckMenuItem(hMenu, ID_VIEW_SOLID, MF_CHECKED);
					break;
				}


				case ID_VIEW_CULL:
				{
					m_bOptCull ^= 1;
					CheckMenuItem(hMenu, ID_VIEW_CULL, m_bOptCull? MF_CHECKED : MF_UNCHECKED);
					break;
				}

				case ID_VIEW_BOX:
				{
					m_bOptBox ^= 1;
					CheckMenuItem(hMenu, ID_VIEW_BOX, m_bOptBox? MF_CHECKED : MF_UNCHECKED);
					break;
				}

				case ID_VIEW_BONE:
				{
					m_bOptBone^= 1;
					CheckMenuItem(hMenu, ID_VIEW_BONE, m_bOptBone? MF_CHECKED : MF_UNCHECKED);
					break;
				}

				case ID_VIEW_MDL:
				{
					m_bOptMdl ^= 1;
					CheckMenuItem(hMenu, ID_VIEW_MDL , m_bOptMdl?  MF_CHECKED : MF_UNCHECKED);
					break;
				}

				case ID_VIEW_GRID:
				{
					m_bOptGrid ^= 1;
					CheckMenuItem(hMenu, ID_VIEW_GRID, m_bOptGrid? MF_CHECKED : MF_UNCHECKED);
					break;
				}

				case ID_VIEW_FIELD:
				{
					m_bOptField ^= 1;
					CheckMenuItem(hMenu, ID_VIEW_FIELD, m_bOptField? MF_CHECKED : MF_UNCHECKED);
					break;
				}

				case ID_MNU_INFO:
				{
					DialogBox( GetModuleHandle(NULL), MAKEINTRESOURCE(IDFX_INFO), hWnd, (DLGPROC)CMain::FxInfProc);
					break;
				}

			}

			break;
		}	// case WM_COMMAND:


		case WM_MOVE:
		{
			if(!m_hWnd)
				break;

			RECT	rc1;
			RECT	rc2;


			GetWindowRect(m_hWnd, &rc1);
			GetWindowRect(m_hWndDlg, &rc2);

			MoveWindow(m_hWndDlg, rc1.left-471
								, rc1.top
								, 470
								, rc2.bottom - rc2.top
								, TRUE);

			GetWindowRect(m_hWndAni, &rc2);

			GetWindowRect(m_hWndAni, &rc2);
			MoveWindow(m_hWndAni, rc1.left+2
								, rc1.bottom+1
								, 1024+5
								, 768-600-43
								, TRUE);

			SetFocus(m_hWnd);
			break;

		} // case WM_SYSCOMMAND
	}
	
	return CD3DApplication::MsgProc( hWnd, msg, wParam, lParam );
}



LRESULT CMain::FxMainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WPARAM	wparHi = HIWORD(wParam);
	WPARAM	wparLo = LOWORD(wParam);
	
	switch(uMsg)
	{
		case WM_COMMAND:
		{
			switch(wparLo)
			{
				case IDFX_OK:
				{
					char	sText[36004]={0};
					GetDlgItemText(hWnd, IDFX_EDIT, sText, 36004);
					INT hr = g_pApp->ResetHlsl("String", sText, 36000);

					if(FAILED(hr))
					{
						INT		iLen = abs(hr);
						HWND	hEdit = GetDlgItem(hWnd, IDFX_EDIT);
						SetDlgItemInt(hWnd, IDFX_LINE, iLen, FALSE);

						INT lineindex = Edit_LineIndex(hEdit, iLen);
						SendMessage(hEdit, EM_SETSEL, lineindex, lineindex);
						SetFocus(hEdit);
					}

					break;
				}

				case IDFX_LOAD:
				{
					char	sFile[MAX_PATH];
					if(FAILED(FileNameOpen(sFile, hWnd, "fx", "Hlsl Files(*.fx)\0*.fx\0All Files(*.*)\0*.*\0")))
						return 0;

					HWND	hEdit = GetDlgItem(hWnd, IDFX_EDIT);
					INT		i=0;
					INT		iLen = strlen(sFile);

					for(i=0; i<iLen; ++i)
					{
						if('\\' == sFile[i])
							sFile[i] = '/';
					}

					SetWindowText(hWnd, sFile);



					FILE*	fp= fopen(sFile, "rt");
					char	sLine[512]={0};
					Edit_SetText(hEdit, sLine);

					while(!feof(fp))
					{
						memset(sLine, 0, sizeof sLine);
						fgets(sLine, 512, fp);

						LineConvertToEdit(sLine);

						iLen = GetWindowTextLength(hEdit);

						SendMessage(hEdit, EM_SETSEL, iLen, iLen);
						SendMessage(hEdit, EM_REPLACESEL, FALSE, (LPARAM)sLine);
					}

					fclose(fp);

					char	sText[36004]={0};
					GetDlgItemText(hWnd, IDFX_EDIT, sText, 36004);
					INT hr = g_pApp->ResetHlsl("String", sText, 36000);

					if(FAILED(hr))
					{
						INT		iLen = abs(hr);
						SetDlgItemInt(hWnd, IDFX_LINE, iLen, FALSE);
						SendDlgItemMessage(hWnd, IDFX_EDIT, EM_SETSEL, iLen, iLen);
					}

					break;
				}

				case IDFX_SAVE:
				{
					char	sFile[MAX_PATH];
					if(FAILED(FileNameOpen(sFile, hWnd, "fx", "Hlsl Files(*.fx)\0*.fx\0All Files(*.*)\0*.*\0")))
						return 0;

					HWND	hEdit = GetDlgItem(hWnd, IDFX_EDIT);
					INT		iLine = Edit_GetLineCount(hEdit);
					char	sLine[512];

					FILE*	fp= fopen(sFile, "wt");

					for(INT i=0; i<iLine; ++i)
					{
						memset(sLine, 0, 512);
						Edit_GetLine(hEdit, i, sLine, 512);
						LineConvertToDos(sLine);
						fprintf(fp, "%s", sLine);
					}

					fclose(fp);

					break;
				}
			}

			break;
		}// case WM_COMMAND:

	}


	return FALSE;
}


LRESULT CMain::FxAniProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WPARAM	wparHi = HIWORD(wParam);
	WPARAM	wparLo = LOWORD(wParam);

	static HWND hTrack =0;
	
	switch( uMsg )
	{
		case WM_INITDIALOG:
		{
			hTrack = GetDlgItem(hWnd, IDLCM_FRM_TRC);
			break;
		}

		case WM_HSCROLL:
		{
			if ((HWND)lParam == hTrack)
			{
				INT nBgn = GetDlgItemInt(hWnd, IDLCM_FRM_BGN, 0, TRUE);
				INT nEnd = GetDlgItemInt(hWnd, IDLCM_FRM_END, 0, TRUE);

				g_pApp->m_nFrmCur = SendMessage(hTrack, TBM_GETPOS,0,0) + nBgn;
			}

			break;
		}


		case WM_COMMAND:
		{
			switch(wparLo)
			{
				case IDLCM_FRM_PLAY:
				{
					g_pApp->m_bOptAnima = TRUE;
					break;
				}

				case IDLCM_FRM_STOP:
				{
					g_pApp->m_bOptAnima = FALSE;
					break;
				}

				case IDLCM_FRM_BEFORE:
				{
					--g_pApp->m_nFrmCur;

					INT nBgn = GetDlgItemInt(hWnd, IDLCM_FRM_BGN, 0, TRUE);

					if(g_pApp->m_nFrmCur<nBgn)
						g_pApp->m_nFrmCur = nBgn;
					break;
				}

				case IDLCM_FRM_NEXT:
				{
					++g_pApp->m_nFrmCur;

					INT nEnd = GetDlgItemInt(hWnd, IDLCM_FRM_END, 0, TRUE);

					if(g_pApp->m_nFrmCur>nEnd)
						g_pApp->m_nFrmCur = nEnd;

					break;
				}


				case IDLCM_FRM_SPA:
				{
					if(g_pApp->m_LcmMdl)
					{
						FLOAT	fAnimation=0;
						g_pApp->m_LcmMdl->Query("Get Animation Speed", &fAnimation);
						fAnimation +=0.1f;
						g_pApp->m_LcmMdl->Query("Set Animation Speed", &fAnimation);
					}

					break;
				}

				case IDLCM_FRM_SPS:
				{
					if(g_pApp->m_LcmMdl)
					{
						FLOAT	fAnimation=0;
						g_pApp->m_LcmMdl->Query("Get Animation Speed", &fAnimation);
						fAnimation -=0.1f;
						g_pApp->m_LcmMdl->Query("Set Animation Speed", &fAnimation);
					}

					break;
				}


				case IDLCM_FRM_LST:
				{
					switch(wparHi)
					{
						case CBN_SELCHANGE:
						{
							HWND	hWcb = GetDlgItem(hWnd, IDLCM_FRM_LST);
							g_pApp->m_LcmFrmN = SendMessage(hWcb, CB_GETCURSEL,0,0);
							g_pApp->ResetFrameIdx();
								
							break;
						}
					}

					break;
				}

				case IDLCM_LNK_LST:
				{
					switch(wparHi)
					{
						case CBN_SELCHANGE:
						{
							HWND	hWcb = GetDlgItem(hWnd, IDLCM_LNK_LST);
							g_pApp->m_LcmLnkN = SendMessage(hWcb, CB_GETCURSEL,0,0);
							g_pApp->ResetLinkTM();
								
							break;
						}
					}

					break;
				}

				case IDLCM_LNK_DEL:
				{
					SAFE_DELETE(	g_pApp->m_LcmLnkO	);
					SAFE_DELETE(	g_pApp->m_LcmLnk	);
					break;
				}


				case IDLCM_OBJ_LST:
				{
					switch(wparHi)
					{
						case CBN_SELCHANGE:
						{
							HWND	hWcb = GetDlgItem(hWnd, IDLCM_OBJ_LST);
							INT		nObj = SendMessage(hWcb, CB_GETCURSEL,0,0);
							INT		nMtl = g_pApp->m_LnkLst[nObj].nM;

							g_pApp->m_LcmObjN = nObj;
							SetDlgItemInt(g_pApp->m_hWndAni, IDLCM_OBJ_MTL, nMtl, TRUE);

							break;
						}
					}

					break;
				}

				case IDLCM_OBJ_MTL:
				{
					switch (wparHi)
					{
						case EN_CHANGE:
						{
							INT  nMtl = GetDlgItemInt(hWnd, IDLCM_OBJ_MTL, 0, 1);

							HWND hWcb = GetDlgItem(hWnd, IDLCM_OBJ_LST);
							INT	 nObj = SendMessage(hWcb, CB_GETCURSEL,0,0);
							
							g_pApp->ResetObjMtl(nObj, nMtl);

							break;
						}
						
					}

					break;
				}

				case IDLCM_MTL_LST:
				{
					switch(wparHi)
					{
						case CBN_SELCHANGE:
						{
							HWND	hWcb = GetDlgItem(hWnd, IDLCM_MTL_LST);
							g_pApp->m_LcmMtlN = SendMessage(hWcb, CB_GETCURSEL,0,0);
							break;
						}
					}

					break;
				}
			}

			break;
		}// case WM_COMMAND:

	}


	return FALSE;
}


LRESULT CMain::FxInfProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{ 
    switch (uMsg) 
    { 
		case WM_INITDIALOG:
		{
			HWND	hPrn = GetParent(hWnd);
			RECT	rc1  = {0,0,0,0};
			RECT	rc2  = {0,0,0,0};

			if(hPrn)
				GetWindowRect(hPrn, &rc1);

			GetWindowRect(hWnd, &rc2);

			MoveWindow(hWnd
						, (rc1.right  + rc1.left - rc2.right  + rc2.left)/2
						, (rc1.bottom + rc1.top  - rc2.bottom + rc2.top )/2
						, rc2.right	 - rc2.left
						, rc2.bottom - rc2.top
						, TRUE);
		}

        case WM_COMMAND: 
            switch (LOWORD(wParam)) 
            { 
                case IDOK: 
                    EndDialog(hWnd, wParam); 
                    return TRUE; 
            } 
    } 
	
    return FALSE; 
}



INT CMain::ResetModel(char* sFile)
{
	INT				i=0;
	Lcm::ILcmMdl*	pMdl=NULL;

	if(FAILED(Lcm::Mdl_Create("Acm", &pMdl, NULL, sFile, m_pd3dDevice)))
		return -1;

	SAFE_DELETE(	m_LcmOrg	);
	SAFE_DELETE(	m_LcmMdl	);
	m_LcmOrg = pMdl;

	if(FAILED(Lcm::Mdl_Create("Acm", &m_LcmMdl, pMdl, NULL, m_pd3dDevice)))
		return -1;

	if(m_LcmMdl)
		m_LcmMdl->Query("Set HLSL", m_pEffct);


	D3DXVECTOR3 vcPos(10, 0, 20);
	m_LcmMdl->SetPosition((FLOAT*)&vcPos);

	m_LcmMdl->Query("Get Frame List", &m_FrmLst);
	m_LcmFrmT = m_FrmLst.size();

	HWND hCbFrmLst = GetDlgItem(m_hWndAni, IDLCM_FRM_LST);
	SendMessage(hCbFrmLst, CB_RESETCONTENT, 0, 0);

	for(i=0; i<m_LcmFrmT; ++i)
		SendMessage(hCbFrmLst, CB_ADDSTRING, 0, (LPARAM)m_FrmLst[i].sN );

	SendMessage(hCbFrmLst, CB_SETCURSEL, m_LcmFrmN, 0);
	ResetFrameIdx();





	m_LcmMdl->Query("Get Bone List", &m_LnkLst);
	m_LcmLnkT = m_LnkLst.size();
	m_LcmObjT = m_LnkLst.size();

	ResetLinkTM();
	
	// Link List
	{
		HWND hCbFrmLnk = GetDlgItem(m_hWndAni, IDLCM_LNK_LST);
		SendMessage(hCbFrmLnk, CB_RESETCONTENT, 0, 0);

		for(i=0; i<m_LcmLnkT; ++i)
			SendMessage(hCbFrmLnk, CB_ADDSTRING, 0, (LPARAM)m_LnkLst[i].sN );

		SendMessage(hCbFrmLnk, CB_SETCURSEL, m_LcmLnkN, 0);
	}


	// Object List
	{
		HWND hCbFrmObj = GetDlgItem(m_hWndAni, IDLCM_OBJ_LST);
		SendMessage(hCbFrmObj, CB_RESETCONTENT, 0, 0);

		for(i=0; i<m_LcmObjT; ++i)
		{
			SendMessage(hCbFrmObj, CB_ADDSTRING, 0, (LPARAM)m_LnkLst[i].sN );
		}

		SendMessage(hCbFrmObj, CB_SETCURSEL, m_LcmObjN, 0);
		SetDlgItemInt(m_hWndAni, IDLCM_OBJ_MTL, m_LnkLst[m_LcmObjN].nM, TRUE);
	}

	// Material List
	m_LcmMdl->Query("Get Material List", &m_MtlLst);
	m_LcmMtlT = m_MtlLst.size();
	{
		HWND hCbFrmMtl = GetDlgItem(m_hWndAni, IDLCM_MTL_LST);
		SendMessage(hCbFrmMtl, CB_RESETCONTENT, 0, 0);

		for(i=0; i<m_LcmMtlT; ++i)
		{
			char	sIdx[8]={0};
			sprintf(sIdx, "%d", i);
			SendMessage(hCbFrmMtl, CB_ADDSTRING, 0, (LPARAM)sIdx);
		}

		SendMessage(hCbFrmMtl, CB_SETCURSEL, m_LcmMtlN, 0);
	}

	
	return 0;
}


INT CMain::ResetLink(char* sFile)
{
	Lcm::ILcmMdl*	pMdl = NULL;

	if(FAILED(Lcm::Mdl_Create("Acm", &pMdl, NULL, sFile, m_pd3dDevice)))
		return -1;

	SAFE_DELETE(	m_LcmLnkO	);
	SAFE_DELETE(	m_LcmLnk	);
	m_LcmLnkO = pMdl;

	if(FAILED(Lcm::Mdl_Create("Acm", &m_LcmLnk, pMdl, NULL, m_pd3dDevice)))
		return -1;

	if(m_LcmLnk)
		m_LcmLnk->Query("Set HLSL", m_pEffct);

	ResetLinkTM();

	return 0;
}




void CMain::ResetHlsltoMdl()
{
	if(m_LcmMdl)
		m_LcmMdl->Query("Set HLSL", m_pEffct);

	if(m_LcmLnk)
		m_LcmLnk->Query("Set HLSL", m_pEffct);
}



INT	CMain::ResetHlsl(char* sCmd, void* pSrc, INT iLen)
{
	INT				hr;
	LPD3DXEFFECT	pEft = NULL;

	hr = HlslBuild(sCmd, (void**)&pEft, m_pd3dDevice, pSrc, iLen);

	if(FAILED(hr))
		return hr;

	SAFE_RELEASE(	m_pEffct	);

	m_pEffct	= pEft;
	ResetHlsltoMdl();

	return hr;
}



void CMain::ResetFrameIdx()
{
	if(m_LcmFrmN>=m_LcmFrmT)
		m_LcmFrmN=m_LcmFrmT-1;


	if(m_LcmFrmN<0)
		m_LcmFrmN=0;

	
	if(0<m_LcmFrmT)
	{
		if(m_LcmMdl)
			m_LcmMdl->Query("Set Frame Index", &m_LcmFrmN);

		SetDlgItemInt(m_hWndAni, IDLCM_FRM_BGN, m_FrmLst[m_LcmFrmN].nB, TRUE);
		SetDlgItemInt(m_hWndAni, IDLCM_FRM_END, m_FrmLst[m_LcmFrmN].nE, TRUE);


		INT		nFrmTot=0;
		m_LcmMdl->Query("Get Total Frame", &nFrmTot);
		SetDlgItemInt(m_hWndAni, IDLCM_FRM_TOT, nFrmTot, FALSE);

		HWND hTrack = GetDlgItem(m_hWndAni, IDLCM_FRM_TRC);
		INT nBgn = GetDlgItemInt(m_hWndAni, IDLCM_FRM_BGN, 0, TRUE);
		INT nEnd = GetDlgItemInt(m_hWndAni, IDLCM_FRM_END, 0, TRUE);

		SendMessage(hTrack, TBM_SETRANGE, FALSE, MAKELPARAM(0,nEnd-nBgn));
		SendMessage(hTrack, TBM_SETPOS,1,0);
		InvalidateRect(hTrack, NULL, FALSE);
	}
	else
	{
		if(m_LcmMdl)
			m_LcmMdl->Query("Set Frame Index", &m_LcmFrmN);

		SetDlgItemInt(m_hWndAni, IDLCM_FRM_BGN, 0, TRUE);
		SetDlgItemInt(m_hWndAni, IDLCM_FRM_END, 0, TRUE);

		
		INT		nFrmTot=0;
		m_LcmMdl->Query("Get Total Frame", &nFrmTot);
		SetDlgItemInt(m_hWndAni, IDLCM_FRM_TOT, nFrmTot, FALSE);

		HWND hTrack = GetDlgItem(m_hWndAni, IDLCM_FRM_TRC);
		INT nBgn = GetDlgItemInt(m_hWndAni, IDLCM_FRM_BGN, 0, TRUE);
		INT nEnd = GetDlgItemInt(m_hWndAni, IDLCM_FRM_END, 0, TRUE);

		SendMessage(hTrack, TBM_SETRANGE, FALSE, MAKELPARAM(0, nFrmTot));
		SendMessage(hTrack, TBM_SETPOS,1,0);
		InvalidateRect(hTrack, NULL, FALSE);
	}
	
}



void CMain::ResetLinkTM()
{
	if(m_LcmLnkN>=m_LcmLnkT)
		m_LcmLnkN=m_LcmLnkT-1;

	if(m_LcmLnkN<0)
		m_LcmLnkN=0;

	if(m_LcmMdl)
	{
		if(0<m_LcmLnkT)
		{
			Lcm::_Tattach*	pLnk= &m_LnkLst[m_LcmLnkN];
			MATA*			tm	= (MATA*)pLnk->tm;
			m_pPivot->SetTMPointer(tm);

			if(m_LcmLnk)
			{
				m_LcmLnk->Query("Set World TM Pointer", tm);
			}
		}
		else
		{
			if(m_LcmLnk)
			{
				m_LcmLnk->Query("Set World TM Pointer", NULL);
			}
		}
	}
}





void CMain::ResetObjMtl(INT nObj, INT nMtl)
{
	INT hr =-1;
	struct _T
	{
		int nObj;
		int nMtl;
	} t={nObj, nMtl};

	hr = m_LcmMdl->Query("Reset Material Index", &t);

	hr = 0;
}







INT HlslBuild(char* sCmd, void** pData, LPDIRECT3DDEVICE9 pDev, void* sEft, INT iLen)
{
	// Effect »ý¼º
	LPD3DXBUFFER	pErr = NULL;
	LPD3DXEFFECT	pEft = NULL;
	char			sErr[2048]={0};
	INT				hr =0;

	if(0 == _stricmp(sCmd, "FILE"))
	{
		if( FAILED(D3DXCreateEffectFromFile(pDev, "Model/hlsl.fx", NULL, NULL, 0 , NULL, &pEft, &pErr )))
		{
			sprintf(sErr, (LPCTSTR)pErr->GetBufferPointer());
			hr = -atoi(sErr+1);
			MessageBox( GetActiveWindow(), sErr, "ERROR", MB_OK);
			SAFE_RELEASE(	pErr );
			return hr;
		}
	}
	else if(0 == _stricmp(sCmd, "String"))
	{
		if( FAILED(D3DXCreateEffect(pDev, sEft, iLen, NULL, NULL, 0 , NULL, &pEft, &pErr )))
		{
			sprintf(sErr, (LPCTSTR)pErr->GetBufferPointer());
			hr = -atoi(sErr+1);
			MessageBox( GetActiveWindow(), sErr, "ERROR", MB_OK);
			SAFE_RELEASE(	pErr );
			return hr;
		}
	}

	else if(0 == _stricmp(sCmd, "Resource"))
	{
		HINSTANCE	hInst	= NULL;
		HRSRC		hRsc	= NULL;
		DWORD		dwSize	= 0;
		HGLOBAL		hMem	= NULL;
		LPVOID		pMem	= NULL;
		INT			nResourceId = (INT)sEft;

		hInst = GetModuleHandle(NULL);
		hRsc = FindResource( hInst, MAKEINTRESOURCE(nResourceId), "FX");

		if(NULL == hRsc)
			return -1;

		dwSize = SizeofResource(hInst,hRsc);

		if(0==dwSize)
			return -1;

		hMem = LoadResource(hInst, hRsc);

		if(NULL == hMem)
			return -1;

		pMem = LockResource(hMem);


		hr = D3DXCreateEffect(pDev, pMem, dwSize, NULL, NULL, 0 , NULL, &pEft, &pErr );

		UnlockResource(hMem);
		FreeResource(hMem);

		if(FAILED(hr))
		{
			sprintf(sErr, (LPCTSTR)pErr->GetBufferPointer());
			hr = -atoi(sErr+1);
			MessageBox( GetActiveWindow(), sErr, "ERROR", MB_OK);
			SAFE_RELEASE(	pErr );
			return hr;
		}
	}

	if(pData)
		*pData = pEft;

	return hr;
}





INT FileNameOpen(char* sOut, HWND hWnd, char* DefaultExt, char* Filter, char* sInit)
{
	CHAR			sDir[MAX_PATH]="\0";
	CHAR			sF[MAX_PATH];
	CHAR			sFull[MAX_PATH]="";
	CHAR			InitDir[MAX_PATH]={0};

	OPENFILENAME	OFN;
	
	GetCurrentDirectory(MAX_PATH, sDir);

	strcpy(InitDir, sDir);

	memset(&OFN, 0, sizeof(OFN));
	OFN.lStructSize = sizeof(OFN);
	OFN.hwndOwner	= hWnd;
	OFN.lpstrFilter	= Filter;
	OFN.lpstrFile	=sFull;
	OFN.nMaxFile	=MAX_PATH;
	
	OFN.lpstrTitle="Choose the file...";
	OFN.lpstrFileTitle=sF;
	OFN.nMaxFileTitle=MAX_PATH;
	OFN.lpstrDefExt= DefaultExt;

	OFN.lpstrInitialDir	= InitDir;

	if( !GetOpenFileName(&OFN))
		return -1;

	SetCurrentDirectory(sDir);

	strcpy(sOut, sFull);

	return 0;
}



INT LineConvertToDos(char* sLine)
{
	INT iLen = strlen(sLine);

	if(0==iLen)
	{
		return 0;
	}

	else if(1 == iLen)
	{
		if('\r' == sLine[0])
		{
			sLine[0] = '\n';
			sLine[1] =0;
		}
	}

	else
	{
		if('\r' == sLine[iLen-1] && '\n' != sLine[iLen-2])
		{
			sLine[iLen-1] = '\n';
			sLine[iLen] = 0;
		}
	}

	return 0;
}


INT LineConvertToEdit(char* sLine)
{
	INT iLen = strlen(sLine);

	if(0 == iLen)
	{
		sLine[0] = '\r';
		sLine[1] = '\n';
	}
	
	else if(1 == iLen)
	{
		if('\n' == sLine[iLen-1])
		{
			sLine[iLen-1] = '\r';
			sLine[iLen+0] = '\n';
			sLine[iLen+1] =0;
		}
	}

	else
	{
		if('\n' == sLine[iLen-1] && '\r' != sLine[iLen-2])
		{
			sLine[iLen-1] = '\r';
			sLine[iLen+0] = '\n';
			sLine[iLen+1] =0;
		}
	}

	return 0;
}




