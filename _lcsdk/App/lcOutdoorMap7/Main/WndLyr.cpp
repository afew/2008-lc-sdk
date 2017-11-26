// Implementation of the CWndLyr class.
//
////////////////////////////////////////////////////////////////////////////////


#include "../_StdAfx.h"


CLSS_DLG_DEFINE(CWndLyr, MsgPrc);


INT CWndLyr::Init()
{
	m_hWnd	= 0;
	m_hWPrn	= 0;

	m_hPanO	= 0;
	m_hPanD	= 0;

	m_nOrg	= 0;
	m_nDst	= 0;

	m_pTxO	= 0;
	m_pTxD	= 0;
	m_bShow	= 0;

	ReadDataOrg();
	ReadDataDst();

	return 0;
}


INT CWndLyr::Create(HWND hWnd)
{
	INT i;
	
	if(m_hWnd)
		return 0;

	m_hWPrn	= hWnd;
	HINSTANCE hInst = GetModuleHandle(NULL);
	m_hWnd	= CreateDialog(hInst,MAKEINTRESOURCE(IDD_LAYER), m_hWPrn, CLSS_DLG_WNDPROC(CWndLyr));

	m_hOrg	= GetDlgItem(m_hWnd, IDC_LYR_ORG);
	m_hDst	= GetDlgItem(m_hWnd, IDC_LYR_DST);

	m_hPanO	= GetDlgItem(m_hWnd, IDC_LYR_PANEL_ORG);
	m_hPanD	= GetDlgItem(m_hWnd, IDC_LYR_PANEL_DST);
	

	RECT	rt1;
	RECT	rt2;
	INT		iWidth;
	INT		iHeight;
	INT		iX;
	INT		iY;

	INT		iSize=0;

	GetWindowRect(m_hWPrn, &rt1);
	GetWindowRect(m_hWnd, &rt2);
	
	iWidth = rt2.right - rt2.left;
	iHeight=  rt2.bottom- rt2.top;

	//(E) 2007-07-16 Editor: AFEW
	//iHeight /=2;

	iX = rt1.left + 100;
	iY = rt1.top  + 200;
	
	MoveWindow(m_hWnd, iX, iY, iWidth, iHeight, TRUE);
	::ShowWindow(m_hWnd, SW_SHOW);

	m_bShow	= 1;



	SendMessage(m_hOrg, CB_RESETCONTENT, 0, 0);

	iSize = m_vStrOrg.size();
	for (i=0; i<iSize; ++i)
		SendMessage(m_hOrg, CB_ADDSTRING, 0, (LPARAM)m_vStrOrg[i].c_str() );

	SendMessage(m_hOrg, CB_SETCURSEL, m_nOrg, 0);


	SendMessage(m_hDst, CB_RESETCONTENT, 0, 0);

	iSize = m_vStrDst.size();
	for (i=0; i<iSize; ++i)
		SendMessage(m_hDst, CB_ADDSTRING, 0, (LPARAM)m_vStrDst[i].c_str() );

	SendMessage(m_hDst, CB_SETCURSEL, m_nDst, 0);


	SetTxOrg(m_nOrg);
	SetTxDst(m_nDst);

	Restore();

	SetFocus(m_hDst);

	m_bShow	= 1;

	return 0;
}



void CWndLyr::ShowWindow(int _ishw)
{
	m_bShow	= _ishw;
	::ShowWindow(m_hWnd, _ishw);
}



void CWndLyr::Destroy()
{
	m_bShow	= 0;

	SAFE_DESTROY_WIN(m_hWnd);
}



LRESULT CWndLyr::MsgPrc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WPARAM		wparHi = HIWORD(wParam);
	WPARAM		wparLo = LOWORD(wParam);

	
	switch( uMsg )
	{
		case WM_COMMAND:
		{
			switch(wparLo)
			{
				case IDC_LYR_OK:
				{
					SendMessage(hWnd, WM_CLOSE, 0, 0);

					break;
				}

				case IDC_LYR_CLOSE:
				{
					SendMessage(hWnd, WM_CLOSE, 0, 0);
					GMAIN->m_WndWrk.m_bTool[ 9] =0;
					break;
				}

				case IDC_LYR_ORG:
				{
					switch(wparHi)
					{
						case CBN_SELCHANGE:
							m_nOrg = SendMessage(m_hOrg, CB_GETCURSEL,0,0);
							SetTxOrg(m_nOrg);
								
							break;
					}

					break;
				}

				case IDC_LYR_DST:
				{
					switch(wparHi)
					{
						case CBN_SELCHANGE:
							m_nDst = SendMessage(m_hDst, CB_GETCURSEL,0,0);
							SetTxDst(m_nDst);
								
							break;
					}

					break;
				}

				case IDC_LYR_ADD:
				{
					AddLyr();
					break;
				}

				case IDC_LYR_DEL:
				{
					DelLyr();
					break;
				}

			}

			break;
		}

		case WM_CLOSE:
		{
			Destroy();
			m_hWnd	= NULL;
			break;
		}
	}

	return(FALSE);
}




INT	CWndLyr::Restore()
{
	HRESULT hr;

	if(!GMAIN->IsWindow())
		return 0;

	if(!m_hWnd)
		return 0;

	if(!m_hPanO)
		return 0;

	if(!m_hPanD)
		return 0;

	m_SwpOrg.Release();
	m_SwpDst.Release();

	m_SwpOrg.hW = m_hPanO;
	
	{
		D3DPRESENT_PARAMETERS	par;
		memset(&par, 0, sizeof(D3DPRESENT_PARAMETERS));
		
		par.SwapEffect = D3DSWAPEFFECT_DISCARD;
		par.Windowed = TRUE;
		
		RECT rt;
		GetWindowRect(m_SwpOrg.hW, &rt);
		par.hDeviceWindow = m_SwpOrg.hW;

		hr = GDEVICE->CreateAdditionalSwapChain(&par, &m_SwpOrg.pC) ;

		if ( FAILED(hr) )
		{
			LcUtil_ErrMsgBox("Create addtional swap chain failed");
			return hr;
		}

		hr = m_SwpOrg.pC->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &m_SwpOrg.pB);

		if ( FAILED(hr) )
		{
			LcUtil_ErrMsgBox("Get back buffer Failed");
			return hr;
		}


		DWORD format = GMAIN->GetDxDepthFMT();
		hr = GDEVICE->CreateDepthStencilSurface(par.BackBufferWidth, par.BackBufferHeight, (D3DFORMAT)format, D3DMULTISAMPLE_NONE, 0, 0, &m_SwpOrg.pS, NULL);

		if ( FAILED(hr))
		{
			LcUtil_ErrMsgBox("Create DepthStencil Surface Failed");
			return -1;
		}
	}

	m_SwpDst.hW = m_hPanD;

	{
		D3DPRESENT_PARAMETERS	par;
		memset(&par, 0, sizeof(D3DPRESENT_PARAMETERS));
		
		par.SwapEffect = D3DSWAPEFFECT_DISCARD;
		par.Windowed = TRUE;
		
		RECT rt;
		GetWindowRect(m_SwpOrg.hW, &rt);
		par.hDeviceWindow = m_SwpDst.hW;

		hr = GDEVICE->CreateAdditionalSwapChain(&par, &m_SwpDst.pC) ;

		if ( FAILED(hr) )
		{
			LcUtil_ErrMsgBox("Create addtional swap chain failed");
			return hr;
		}

		hr = m_SwpDst.pC->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &m_SwpDst.pB);

		if ( FAILED(hr) )
		{
			LcUtil_ErrMsgBox("Get back buffer Failed");
			return hr;
		}


		DWORD format = GMAIN->GetDxDepthFMT();
		hr = GDEVICE->CreateDepthStencilSurface(par.BackBufferWidth, par.BackBufferHeight, (D3DFORMAT)format, D3DMULTISAMPLE_NONE, 0, 0, &m_SwpDst.pS, NULL);

		if ( FAILED(hr))
		{
			LcUtil_ErrMsgBox("Create DepthStencil Surface Failed");
			return -1;
		}
	}

	return 0;
}


void CWndLyr::Invalidate()
{
	m_SwpOrg.Release();
	m_SwpDst.Release();
}


INT CWndLyr::FrameMove()
{
	if(!m_hWnd)
		return 0;

	if(!GMAIN->IsWindow())
		return 0;
		
	return 0;
}


void CWndLyr::Render()
{
	if(!m_hWnd)
		return;

	if(!GMAIN->IsWindow())
		return;

	if(!m_SwpOrg.pC || !m_SwpDst.pC)
		return;

	HRESULT hr=-1;

	GDEVICE->SetRenderTarget(0, m_SwpOrg.pB);
	GDEVICE->SetDepthStencilSurface(m_SwpOrg.pS);
	GDEVICE->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	GDEVICE->Clear( 0L, 0, GMAIN->GetDxClearMode(),  0x00006699, 1.0f, 0L );
	{
		RECT rc={0,0, m_ImgO.Width, m_ImgO.Height};

		if(m_pTxO)
		{
			hr = GSPRITE->Begin(D3DXSPRITE_ALPHABLEND);
			hr = GSPRITE->Draw(m_pTxO, &rc, NULL, NULL, D3DXCOLOR(1,1,1,1));
			hr = GSPRITE->End();
		}

		RECT rt={5,10, 1024,30};
		GMAIN->m_pDxFnt->DrawText(NULL, "Source", -1, &rt, 0, 0XFFFFFFFF);

		

		m_SwpOrg.pC->Present(0, 0, 0, 0, 0);
	}
	
	


	GDEVICE->SetRenderTarget(0, m_SwpDst.pB);
	GDEVICE->SetDepthStencilSurface(m_SwpDst.pS);
	
	GDEVICE->Clear( 0L, 0, GMAIN->GetDxClearMode(),  0x00006699, 1.0f, 0L );
	{
		RECT rc={0,0, m_ImgD.Width, m_ImgD.Height};

		if(m_pTxD)
		{
			hr = GSPRITE->Begin(D3DXSPRITE_ALPHABLEND);
			hr = GSPRITE->Draw(m_pTxD, &rc, NULL, NULL, D3DXCOLOR(1,1,1,1));
			hr = GSPRITE->End();
		}


		RECT rt={5,10, 1024,30};
		GMAIN->m_pDxFnt->DrawText(NULL, "Dest", -1, &rt, 0, 0XFFFFFFFF);

		m_SwpDst.pC->Present(0, 0, 0, 0, 0);
	}

	
}



void CWndLyr::ReadDataOrg()
{
	INT		i=0;
	char	sTmp[512];
	INT		nNum=0;

	m_vStrOrg.clear();

	GetPrivateProfileString("Origin", "Num", NULL, sTmp, sizeof(sTmp),"Data/Layer.ini");
	nNum = atoi(sTmp);

	for(i=0; i<nNum; ++i)
	{
		GetPrivateProfileString("Origin", LcStr_Format("%d", i), NULL, sTmp, sizeof(sTmp),"Data/Layer.ini");
		m_vStrOrg.push_back(sTmp);
	}
}


void CWndLyr::ReadDataDst()
{
	INT		i=0;
	char	sTmp[512];
	INT		nNum=0;

	m_vStrDst.clear();

	GetPrivateProfileString("Dest", "Num", NULL, sTmp, sizeof(sTmp),"Data/Layer.ini");
	nNum = atoi(sTmp);

	for(i=0; i<nNum; ++i)
	{
		GetPrivateProfileString("Dest", LcStr_Format("%d", i), NULL, sTmp, sizeof(sTmp),"Data/Layer.ini");
		m_vStrDst.push_back(sTmp);
	}
}


void CWndLyr::SetTxOrg(INT	nOrg)
{
	m_nOrg = nOrg;

	INT nM=-1, nS=-1;

	char* pStr = (char*)m_vStrOrg[m_nOrg].c_str();

	TBTX->Select((void**)&m_pTxO, &nM, &nS,	(char*)m_vStrOrg[m_nOrg].c_str());
	TBTX->SelectInfo((void**)&m_ImgO, nM, nS);
}

void CWndLyr::SetTxDst(INT	nDst)
{
	m_nDst = nDst;

	INT nM=-1, nS=-1;
	TBTX->Select((void**)&m_pTxD, &nM, &nS,	(char*)m_vStrDst[m_nDst].c_str());
	TBTX->SelectInfo((void**)&m_ImgD, nM, nS);
}



void CWndLyr::AddLyr()
{
	int		i;
	INT		iSize = m_vStrDst.size();
	char*	pOrg = (char*)m_vStrOrg[m_nOrg].c_str();

	
	for(i=0; i<iSize; ++i)
	{
		char*	pDst = (char*)m_vStrDst[i].c_str();
		
		if(0 == stricmp(pOrg, pDst))
			break;
	}

	if(i<iSize)
		return;


	m_vStrDst.push_back(pOrg);

	SendMessage(m_hDst, CB_RESETCONTENT, 0, 0);

	iSize = m_vStrDst.size();
	
	for(i=0; i<iSize; ++i)
	{
		char*	pStr = (char*)m_vStrDst[i].c_str();
		SendMessage(m_hDst, CB_ADDSTRING, 0, (LPARAM)pStr );
	}

	SendMessage(m_hDst, CB_SETCURSEL, m_nDst, 0);
}



void CWndLyr::DelLyr()
{
	int		i;
	INT		iSize = m_vStrDst.size();

	if(iSize<=1)
		return;


	itStr	it = m_vStrDst.begin() + m_nDst;

	m_vStrDst.erase(it);

	iSize = m_vStrDst.size();

	if(m_nDst >= iSize)
		m_nDst = iSize -1;

	SendMessage(m_hDst, CB_RESETCONTENT, 0, 0);

	
	
	for(i=0; i<iSize; ++i)
	{
		char*	pStr = (char*)m_vStrDst[i].c_str();
		SendMessage(m_hDst, CB_ADDSTRING, 0, (LPARAM)pStr );
	}

	SendMessage(m_hDst, CB_SETCURSEL, m_nDst, 0);


	INT nM=-1, nS=-1;
	TBTX->Select((void**)&m_pTxD, &nM, &nS,	(char*)m_vStrDst[m_nDst].c_str());
	TBTX->SelectInfo((void**)&m_ImgD, nM, nS);
}



char* CWndLyr::GetStrDst(INT nIdx)
{
	if(m_vStrDst.empty())
		return NULL;

	return (char*)m_vStrDst[nIdx].c_str();
}

INT CWndLyr::GetSizeDst()
{
	return m_vStrDst.size();
}