// Implementation of the CWndTool class.
//
////////////////////////////////////////////////////////////////////////////////


#include "../_StdAfx.h"


CLSS_DLG_DEFINE(CWndTool, MsgPrc);


INT CWndTool::Init()
{
	m_hWnd	= 0;
	m_hWPrn	= 0;

	m_bShow	= 0;

	ReadDataSrc();
	ReadDataDst();

	return 0;
}


INT CWndTool::Create(HWND hWnd)
{
	INT i;

	if(m_hWnd)
		return 0;

	m_hWPrn	= hWnd;
	HINSTANCE hInst = GetModuleHandle(NULL);
	m_hWnd	= CreateDialog(hInst,MAKEINTRESOURCE(IDD_TOOL), m_hWPrn, CLSS_DLG_WNDPROC(CWndTool));


	int	iSizeS	= m_vTexS.size();
	int	iSizeD	= m_vTexD.size();

	for(i=0; i<iSizeS; ++i)
		m_hPanS[i] = GetDlgItem(m_hWnd, IDC_TOOL_LYR_SRC0+i);

	for(i=0; i<iSizeD; ++i)
		m_hPanD[i] = GetDlgItem(m_hWnd, IDC_TOOL_LYR_DST0+i);


	RECT	rt1;
	RECT	rt2;
	INT		iWidth;
	INT		iHeight;
	INT		iX;
	INT		iY;

	::GetWindowRect(m_hWPrn, &rt1);
	::GetWindowRect(m_hWnd, &rt2);

	iWidth = rt2.right - rt2.left;
	iHeight=  rt2.bottom- rt2.top;

	iX = rt1.left-227;
	iY = rt1.bottom+2;

	::MoveWindow(m_hWnd, iX, iY, iWidth, iHeight, TRUE);
	::ShowWindow(m_hWnd, SW_SHOW);

	Restore();

	m_bShow	= 1;

	return 0;
}



void CWndTool::ShowWindow(int _ishw)
{
	m_bShow	= _ishw;
	::ShowWindow(m_hWnd, _ishw);
}



void CWndTool::Destroy()
{
	m_bShow	= 0;

	SAFE_DESTROY_WIN(m_hWnd);
}



LRESULT CWndTool::MsgPrc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WPARAM		wparHi = HIWORD(wParam);
	WPARAM		wparLo = LOWORD(wParam);


	switch( uMsg )
	{
		case WM_COMMAND:
		{
//			switch(wparLo)
//			{
//				case IDC_TOOL_LYR_OK:
//				{
//					SendMessage(hWnd, WM_CLOSE, 0, 0);
//
//					break;
//				}
//
//				case IDC_TOOL_LYR_DST:
//				{
//					switch(wparHi)
//					{
//						case CBN_SELCHANGE:
//							iSize = SendMessage(m_hPanD, CB_GETCURSEL,0,0);
//							SetTxDst(iSize);
//
//							break;
//					}
//
//					break;
//				}
//
//				case IDC_TOOL_LYR_ADD:
//				{
//					AddLyr();
//					break;
//				}
//
//				case IDC_TOOL_LYR_DEL:
//				{
//					DelLyr();
//					break;
//				}
//
//			}

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




INT	CWndTool::Restore()
{
	INT		i;
	HRESULT hr;

	if(!GMAIN->IsWindow())
		return 0;

	if(!m_hWnd)
		return 0;

	int	iSizeS	= m_vTexS.size();
	int	iSizeD	= m_vTexD.size();


	for(i=0; i<iSizeS; ++i)
		m_SwpSrc[i].Release();

	for(i=0; i<iSizeD; ++i)
		m_SwpDst[i].Release();



	for(i=0; i<iSizeS; ++i)
	{
		m_SwpSrc[i].hW = m_hPanS[i];

		D3DPRESENT_PARAMETERS	par;
		memset(&par, 0, sizeof(D3DPRESENT_PARAMETERS));

		par.SwapEffect = D3DSWAPEFFECT_DISCARD;
		par.Windowed = TRUE;

		RECT rt;
		GetWindowRect(m_SwpSrc[i].hW, &rt);
		par.hDeviceWindow = m_SwpSrc[i].hW;

		hr = GDEVICE->CreateAdditionalSwapChain(&par, &m_SwpSrc[i].pC) ;

		if ( FAILED(hr) )
		{
			LcUtil_ErrMsgBox("Create addtional swap chain failed");
			return hr;
		}

		hr = m_SwpSrc[i].pC->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &m_SwpSrc[i].pB);

		if ( FAILED(hr) )
		{
			LcUtil_ErrMsgBox("Get back buffer Failed");
			return hr;
		}


		DWORD format = GMAIN->GetDxDepthFMT();
		hr = GDEVICE->CreateDepthStencilSurface(par.BackBufferWidth, par.BackBufferHeight, (D3DFORMAT)format, D3DMULTISAMPLE_NONE, 0, 0, &m_SwpSrc[i].pS, NULL);

		if ( FAILED(hr))
		{
			LcUtil_ErrMsgBox("Create DepthStencil Surface Failed");
			return -1;
		}
	}

	for(i=0; i<iSizeD; ++i)
	{
		m_SwpDst[i].hW = m_hPanD[i];
	
		D3DPRESENT_PARAMETERS	par;
		memset(&par, 0, sizeof(D3DPRESENT_PARAMETERS));

		par.SwapEffect = D3DSWAPEFFECT_DISCARD;
		par.Windowed = TRUE;

		RECT rt;
		GetWindowRect(m_SwpSrc[i].hW, &rt);
		par.hDeviceWindow = m_SwpDst[i].hW;

		hr = GDEVICE->CreateAdditionalSwapChain(&par, &m_SwpDst[i].pC) ;

		if ( FAILED(hr) )
		{
			LcUtil_ErrMsgBox("Create addtional swap chain failed");
			return hr;
		}

		hr = m_SwpDst[i].pC->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &m_SwpDst[i].pB);

		if ( FAILED(hr) )
		{
			LcUtil_ErrMsgBox("Get back buffer Failed");
			return hr;
		}


		DWORD format = GMAIN->GetDxDepthFMT();
		hr = GDEVICE->CreateDepthStencilSurface(par.BackBufferWidth, par.BackBufferHeight, (D3DFORMAT)format, D3DMULTISAMPLE_NONE, 0, 0, &m_SwpDst[i].pS, NULL);

		if ( FAILED(hr))
		{
			LcUtil_ErrMsgBox("Create DepthStencil Surface Failed");
			return -1;
		}
	}



	for(i=0; i<iSizeS; ++i)
		SetTxSrc(i);

	for(i=0; i<iSizeD; ++i)
		SetTxDst(i);

	return 0;
}


void CWndTool::Invalidate()
{
	INT		i;

	int	iSizeS	= m_vTexS.size();
	int	iSizeD	= m_vTexD.size();

	for(i=0; i<iSizeS; ++i)
		m_SwpSrc[i].Release();

	for(i=0; i<iSizeD; ++i)
		m_SwpDst[i].Release();
}


INT CWndTool::FrameMove()
{
	if(!m_hWnd)
		return 0;

	if(!GMAIN->IsWindow())
		return 0;

	return 0;
}


void CWndTool::Render()
{
	INT		i;

	if(!m_hWnd)
		return;

	if(!GMAIN->IsWindow())
		return;

	static int nCnt=0;
	++nCnt;

	if(0 != nCnt%16)
		return;
	
	nCnt=0;

	HRESULT hr=-1;

	GDEVICE->SetRenderState(D3DRS_LIGHTING, FALSE);

	RECT rc={0,0, 256, 256};


	int	iSizeS	= m_vTexS.size();
	int	iSizeD	= m_vTexD.size();

	for(i=0; i<iSizeS; ++i)
	{
		GDEVICE->SetRenderTarget(0, m_SwpSrc[i].pB);
		GDEVICE->SetDepthStencilSurface(m_SwpSrc[i].pS);
		GDEVICE->Clear( 0L, 0, GMAIN->GetDxClearMode(),  0x00006699, 1.0f, 0L );

		ILcxmT::MpTex*	pTex = &m_vTexS[i];
		PDTX	pTx = pTex->TexB.pTex;
		
		if(pTx)
		{
			hr = GSPRITE->Begin(D3DXSPRITE_ALPHABLEND);
			hr = GSPRITE->Draw(pTx, &rc, NULL, NULL, D3DXCOLOR(1,1,1,1));
			hr = GSPRITE->End();
		}

//		RECT rt={0,0, 100,30};
//		char	sMsg[32];
//		sprintf(sMsg, "(%d)", i);
//		GMAIN->m_pDxFnt->DrawText(NULL, sMsg, -1, &rt, 0, 0XFF00FFFF);

		m_SwpSrc[i].pC->Present(0, 0, 0, 0, 0);
	}

	for(i=0; i<iSizeD; ++i)
	{
		GDEVICE->SetRenderTarget(0, m_SwpDst[i].pB);
		GDEVICE->SetDepthStencilSurface(m_SwpDst[i].pS);
		GDEVICE->Clear( 0L, 0, GMAIN->GetDxClearMode(),  0x00006699, 1.0f, 0L );

		PDTX	pTx = m_vTexD[i].TexB.pTex;
		if(pTx)
		{
			hr = GSPRITE->Begin(D3DXSPRITE_ALPHABLEND);
			hr = GSPRITE->Draw(pTx, &rc, NULL, NULL, D3DXCOLOR(1,1,1,1));
			hr = GSPRITE->End();
		}


//		RECT rt={0,0, 100,30};
//		char	sMsg[32];
//		sprintf(sMsg, "(%d)", i);
//		GMAIN->m_pDxFnt->DrawText(NULL, sMsg, -1, &rt, 0, 0XFFFFFF00);

		m_SwpDst[i].pC->Present(0, 0, 0, 0, 0);
	}


}



void CWndTool::ReadDataSrc()
{
	INT		i=0;
	char	sTmp[512];
	INT		nNum=0;

	m_vTexS.clear();

	GetPrivateProfileString("Origin", "Num", NULL, sTmp, sizeof(sTmp),"Data/Layer.ini");
	nNum = atoi(sTmp);

	for(i=0; i<nNum; ++i)
	{
		GetPrivateProfileString("Origin", LcStr_Format("%d", i), NULL, sTmp, sizeof(sTmp),"Data/Layer.ini");

		ILcxmT::MpTex	pLayer;
		strcpy(pLayer.TexB.sName, sTmp);
		
		m_vTexS.push_back(pLayer);
	}
}


void CWndTool::ReadDataDst()
{
	INT		i=0;
	char	sTmp[512];
	INT		nNum=0;

	m_vTexD.clear();

	GetPrivateProfileString("Dest", "Num", NULL, sTmp, sizeof(sTmp),"Data/Layer.ini");
	nNum = atoi(sTmp);

	for(i=0; i<nNum; ++i)
	{
		GetPrivateProfileString("Dest", LcStr_Format("%d", i), NULL, sTmp, sizeof(sTmp),"Data/Layer.ini");
		
		ILcxmT::MpTex	pLayer;
		strcpy(pLayer.TexB.sName, sTmp);
		
		m_vTexD.push_back(pLayer);
	}
}


void CWndTool::SetTxSrc(INT	nSrc)
{
	INT nM=-1, nS=-1;

	char* sName = m_vTexS[nSrc].TexB.sName;

	PDTX	pTexB=NULL;
	DIMG*	pImgB=&m_vTexS[nSrc].TexB.Image;

	g_pApp->m_pTbTex->Select((void**)&pTexB, &nM, &nS,	sName);
	g_pApp->m_pTbTex->SelectInfo((void**)pImgB, nM, nS);

	m_vTexS[nSrc].TexB.pTex = pTexB;
}

void CWndTool::SetTxDst(INT	nDst)
{
	INT nM=-1, nS=-1;

	char* sName = m_vTexD[nDst].TexB.sName;

	PDTX	pTexB=NULL;
	DIMG*	pImgB=&m_vTexD[nDst].TexB.Image;

	g_pApp->m_pTbTex->Select((void**)&pTexB, &nM, &nS,	sName);
	g_pApp->m_pTbTex->SelectInfo((void**)pImgB, nM, nS);

	m_vTexD[nDst].TexB.pTex = pTexB;
}

