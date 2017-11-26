// Implementation of the CMain class.
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


void SetDlgItemFlt(HWND hWnd, UINT id, float z, INT decimal)
{
	char s[128];
	char f[64];
	
	sprintf(f,"%%.%df", decimal);
	memset(s, 0, sizeof(s));
	sprintf(s, f, z);
	SetDlgItemText(hWnd,id,s);
}


CMain::CMain()
{
	strcpy(m_sClassName, "Lcm Viewer");

	m_dwCreationWidth	= 1024;
	m_dwCreationHeight	= m_dwCreationWidth * 3/4;

	m_dScnX			= 471;
	m_dScnY			= 1;

	m_dRscIcon		= IDI_MAIN_ICON;
	m_dRscMenu		= IDM_MENU;
	m_dRscAccel		= IDR_MAIN_ACCEL;

	m_dRscDevice	= IDM_CHANGEDEVICE;
	m_dRscToggle	= IDM_TOGGLEFULLSCREEN;
	m_dRscExit		= IDM_EXIT;
	m_dRscExit		= 0;

	m_pD3DXFont		= NULL;
	m_pInput		= NULL;
	m_pCam			= NULL;

	m_pGrid			= NULL;
	m_pField		= NULL;
	m_pPivot		= NULL;
	m_pEffct		= NULL;

	m_LcmOrg		= NULL;
	m_LcmMdl		= NULL;

	m_LcmLnkO		= NULL;
	m_LcmLnk		= NULL;
	m_LcmLnkN		= 0;
	m_LcmLnkT		= 0;
	m_pAttach		= NULL;

	m_LcmObjN		= 0;
	m_LcmObjT		= 0;

	m_LcmMtlN		= 0;
	m_LcmMtlT		= 0;

	m_LcmFrmN		= 0;
	m_LcmFrmT		= 0;
	m_nFrmCur		= 0;

	m_hWndDlg		= NULL;
	m_hWndAni		= NULL;
	m_hWndEdt		= NULL;


	m_bOptCull	= TRUE;
	m_bOptBox	= FALSE;
	m_bOptBone	= TRUE;
	m_bOptMdl	= TRUE;
	m_bOptGrid	= TRUE;
	m_bOptField	= TRUE;

	m_bOptAnima	= TRUE;
}


HRESULT CMain::Init()
{
	SAFE_NEWCREATE1(	m_pInput,	CLcInput, m_hWnd	);
	SAFE_NEWCREATE1(	m_pCam,		CLcCam, m_pd3dDevice);
	SAFE_NEWCREATE1(	m_pGrid,	CLcGrid, m_pd3dDevice);
	SAFE_NEWCREATE1(	m_pField,	CLcField, m_pd3dDevice);
	SAFE_NEWCREATE1(	m_pPivot,	CLcxPivot, m_pd3dDevice);


	D3DXFONT_DESC hFont =
	{
		14, 0
		, FW_BOLD, 1, FALSE
		, HANGUL_CHARSET
		, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE
		, "굴림체"
	};

	if( FAILED( D3DXCreateFontIndirect(m_pd3dDevice, &hFont, &m_pD3DXFont) ) )
		return -1;


	// Option Setup
	HMENU	hMenu = ::GetMenu(m_hWnd);
	m_bOptFill	= D3DFILL_SOLID;
	CheckMenuItem(hMenu, ID_VIEW_POINT, MF_UNCHECKED);
	CheckMenuItem(hMenu, ID_VIEW_WIRE, MF_UNCHECKED);
	CheckMenuItem(hMenu, ID_VIEW_SOLID, MF_CHECKED);
	CheckMenuItem(hMenu, ID_VIEW_CULL, m_bOptCull? MF_CHECKED : MF_UNCHECKED);
	CheckMenuItem(hMenu, ID_VIEW_BOX, m_bOptBox? MF_CHECKED : MF_UNCHECKED);
	CheckMenuItem(hMenu, ID_VIEW_BONE, m_bOptBone? MF_CHECKED : MF_UNCHECKED);
	CheckMenuItem(hMenu, ID_VIEW_MDL , m_bOptMdl?  MF_CHECKED : MF_UNCHECKED);
	CheckMenuItem(hMenu, ID_VIEW_GRID, m_bOptGrid? MF_CHECKED : MF_UNCHECKED);
	CheckMenuItem(hMenu, ID_VIEW_FIELD, m_bOptField? MF_CHECKED : MF_UNCHECKED);


	RECT	rc1;
	RECT	rc2;

	GetWindowRect(m_hWnd, &rc1);

	m_hmdRch=LoadLibrary("Riched20.dll");
	m_hWndDlg = CreateDialog( GetModuleHandle(NULL), MAKEINTRESOURCE(IDFX_DIALOG), m_hWnd, (DLGPROC)CMain::FxMainProc);

	if(NULL == m_hWndDlg)
		return -1;

	m_hWndEdt = GetDlgItem(m_hWndDlg, IDFX_EDIT);

	INT			iFontH = 9;
	HDC	hDC = ::GetDC(HWND_DESKTOP);

	iFontH	= MulDiv( iFontH,GetDeviceCaps( hDC ,LOGPIXELSY),72);
    iFontH  *= (1440 / GetDeviceCaps( hDC,LOGPIXELSY));			// for Twip(트윕)
	ReleaseDC(HWND_DESKTOP, hDC);

	
	CHARFORMAT2 cf;
	memset(&cf, 0, sizeof CHARFORMAT2);
	
	cf.cbSize		= sizeof(cf);
	cf.dwMask		= CFM_ALL2;
	cf.crTextColor = RGB(  0,   30, 30);	// 글자색 설정
	cf.crBackColor = RGB(255, 255, 255);	// 글자색 설정
	cf.bCharSet	= ANSI_CHARSET;
	cf.yHeight	= iFontH;
    cf.wWeight	= FW_THIN;
	strcpy(cf.szFaceName, "굴림체"); 

	SendMessage(m_hWndEdt, EM_SETCHARFORMAT, SCF_DEFAULT, ( LPARAM ) &cf );
	GetWindowRect(m_hWndDlg, &rc2);
	MoveWindow(m_hWndDlg, rc1.left-471
						, rc1.top
						, 470
						, rc1.bottom - rc1.top//768
						, TRUE);

	ShowWindow(m_hWndDlg, SW_SHOW);
	


	m_hWndAni = CreateDialog( GetModuleHandle(NULL), MAKEINTRESOURCE(IDFX_ANI), m_hWnd, (DLGPROC)CMain::FxAniProc);

	if(NULL == m_hWndAni)
		return -1;


	GetWindowRect(m_hWndAni, &rc2);
	MoveWindow(m_hWndAni, rc1.left+2
						, rc1.bottom+1
						, 1024+3
						, 768-600-25
						, TRUE);

	ShowWindow(m_hWndAni, SW_SHOW);
	SetFocus(m_hWnd);
	

	HRSRC		hRsc	= NULL;
	HGLOBAL		hMem	= NULL;
	LPVOID		pMem	= NULL;

	hRsc = FindResource( m_hInst, MAKEINTRESOURCE(IDFX_HLSL), "FX");

	if(NULL == hRsc)
		return -1;

	hMem = LoadResource(m_hInst, hRsc);
	if(NULL == hMem)
		return -1;

	pMem = LockResource(hMem);
		SendMessage(m_hWndEdt, EM_REPLACESEL, FALSE, (LPARAM)pMem);
	UnlockResource(hMem);
	
	FreeResource(hMem);	

//	if(FAILED(ResetModel("model/anubis.acm")))
//		return -1;
//
//	if(FAILED(ResetLink("model/sword.acm")))
//		return -1;

	char	sText[36004]={0};
	GetDlgItemText(m_hWndDlg, IDFX_EDIT, sText, 36004);

	
	ResetHlsl("String", sText, 36000);
	ResetHlsltoMdl();

	return S_OK;
}


HRESULT CMain::Destroy()
{
	SAFE_RELEASE( m_pD3DXFont	);
	SAFE_DELETE(	m_pInput	);
	SAFE_DELETE(	m_pCam		);
	SAFE_DELETE(	m_pGrid		);
	SAFE_DELETE(	m_pField	);
	SAFE_DELETE(	m_pPivot	);

	SAFE_DELETE(	m_LcmMdl	);
	SAFE_DELETE(	m_LcmOrg	);

	SAFE_DELETE(	m_LcmLnkO	);
	SAFE_DELETE(	m_LcmLnk	);

	DestroyWindow(m_hWndAni);
	DestroyWindow(m_hWndDlg);
	FreeLibrary(m_hmdRch);		// DLL을 해제

	return S_OK;
}


HRESULT CMain::Restore()
{
	m_pD3DXFont->OnResetDevice();

	if(m_pEffct)
		m_pEffct->OnResetDevice();

	return S_OK;
}


HRESULT CMain::Invalidate()
{
	m_pD3DXFont->OnLostDevice();

	if(m_pEffct)
		m_pEffct->OnLostDevice();

	return S_OK;
}



HRESULT CMain::FrameMove()
{
	SAFE_FRAMEMOVE(	m_pInput	);

	HWND hWndFocus = GetFocus();

	if(m_hWnd == hWndFocus)
	{
		FLOAT	fElapsed = m_fElapsedTime;
		VEC3	vcDelta = m_pInput->GetMouseEps();

		FLOAT	fSpeedMov  = 200.f * fElapsed;
		FLOAT	fSpeedRot  =  0.1f;
		FLOAT	fSpeedWheel=  40.f * fElapsed;

		if(vcDelta.z !=0.f)
		{
			m_pCam->MoveForward(-vcDelta.z* fSpeedWheel, 1.f);
		}

		if(m_pInput->KeyPress('W'))					// W
		{
			m_pCam->MoveForward( fSpeedMov, 1.f);
		}

		if(m_pInput->KeyPress('S'))					// S
		{
			m_pCam->MoveForward(-fSpeedMov, 1.f);
		}

		if(m_pInput->KeyPress('A'))					// A
		{
			m_pCam->MoveSideward(-fSpeedMov);
		}

		if(m_pInput->KeyPress('D'))					// D
		{
			m_pCam->MoveSideward(fSpeedMov);
		}

		if(m_pInput->BtnPress(1))
		{
			m_pCam->Rotation(vcDelta.x, vcDelta.y, fSpeedRot);
		}

		SAFE_FRAMEMOVE(	m_pCam		);


		if(m_pInput->KeyPress(VK_LEFT))				// Rotation
		{
			if(m_LcmMdl)
			{
				// For D3DXMatrixRotationAxis();
				// x, y, z is Axis.
				// w is angle.
				D3DXVECTOR4 vcRot;

				m_LcmMdl->GetRotation((FLOAT*) &vcRot);

				if( fabsf(vcRot.x) <0.00001f &&
					fabsf(vcRot.y) <0.00001f &&
					fabsf(vcRot.x) <0.00001f)
				{
					vcRot.x = 0;
					vcRot.y = 1;
					vcRot.z = 0;
				}

				vcRot.w += (1.f *fElapsed);
				m_LcmMdl->SetRotation((FLOAT*)&vcRot);
			}
		}

		if(m_pInput->KeyPress(VK_RIGHT))			// 빼기
		{
			if(m_LcmMdl)
			{
				// For D3DXMatrixRotationAxis();
				// x, y, z is Axis.
				// w is angle.
				D3DXVECTOR4 vcRot;

				m_LcmMdl->GetRotation((FLOAT*) &vcRot);

				if( fabsf(vcRot.x) <0.00001f &&
					fabsf(vcRot.y) <0.00001f &&
					fabsf(vcRot.x) <0.00001f)
				{
					vcRot.x = 0;
					vcRot.y = 1;
					vcRot.z = 0;
				}

				vcRot.w -= (1.f *fElapsed);
				m_LcmMdl->SetRotation((FLOAT*)&vcRot);
			}
		}
	}


	if(m_LcmMdl)	m_LcmMdl->Query("Set Current Frame", &m_nFrmCur);
	if(m_LcmMdl)	m_LcmMdl->Query("Animation Update", &m_bOptAnima);

	SAFE_FRAMEMOVE(	m_LcmMdl	);


	if(m_hWnd == hWndFocus)
	{
		if(m_pInput->KeyDown(VK_UP))
		{
			if(m_LcmMdl)
			{
				++m_LcmLnkN;
				if(m_LcmLnkN >=m_LcmLnkT)
					--m_LcmLnkN;				
			}

			ResetLinkTM();
		}

		if(m_pInput->KeyDown(VK_DOWN))
		{
			if(m_LcmMdl)
			{
				--m_LcmLnkN;
				if(m_LcmLnkN<0)
					m_LcmLnkN = 0;
			}

			ResetLinkTM();
		}
	}

	SAFE_FRAMEMOVE(	m_LcmLnk	);



	

	if(m_LcmMdl)	m_LcmMdl->Query("Show Bone", &m_bOptBone);
	if(m_LcmMdl)	m_LcmMdl->Query("Show Model", &m_bOptMdl);

	// Info..
	if(m_LcmMdl && m_bWindowed)
	{
		FLOAT	fAnimation=0;
		m_LcmMdl->Query("Get Animation Speed", &fAnimation);
		m_LcmMdl->Query("Get Current Frame", &m_nFrmCur);

		SetDlgItemInt(m_hWndAni, IDLCM_FRM_CUR, m_nFrmCur, FALSE);
		SetDlgItemFlt(m_hWndAni, IDLCM_FRM_SPD, fAnimation, 6);

		HWND hTrack = GetDlgItem(m_hWndAni, IDLCM_FRM_TRC);
		INT nBgn = GetDlgItemInt(m_hWndAni, IDLCM_FRM_BGN, 0, TRUE);
		SendMessage(hTrack, TBM_SETPOS,1, m_nFrmCur- nBgn);
	}

	if(m_bOptAnima)
	{
		HWND hTrack = GetDlgItem(m_hWndAni, IDLCM_FRM_TRC);
		INT nBgn = GetDlgItemInt(m_hWndAni, IDLCM_FRM_BGN, 0, TRUE);
		SendMessage(hTrack, TBM_SETPOS,1, m_nFrmCur- nBgn);
	}


	// Link Point
	if(m_LcmLnkT)
	{
		SAFE_RELEASE(	m_pAttach	);
		m_pd3dDevice->CreateVertexBuffer( m_LcmLnkT * sizeof(VtxD), 0, D3DFVF_XYZ, D3DPOOL_MANAGED, &m_pAttach, NULL);

		VtxD*	pVtx;
		m_pAttach->Lock(0, 0, (void**)&pVtx, 0);

		for(int i=0; i<m_LcmLnkT; ++i)
		{
			Lcm::_Tattach*	pDst= &m_LnkLst[i];				
			MATA			tm	= *((MATA*)pDst->tm);
			VEC3			vcP(tm._41, tm._42, tm._43);

			pVtx[i].p	= vcP;
			pVtx[i].d	= 0XFFFF0000;
		}

		m_pAttach->Unlock();
	}

	
	return S_OK;
}




HRESULT CMain::Render()
{
	if( FAILED( m_pd3dDevice->BeginScene() ) )
		return -1;

	m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, 0xFF007799, 1.0f, 0L);


	if(m_bOptGrid)
		SAFE_RENDER(	m_pGrid		);


	if(m_bOptField)
		SAFE_RENDER(	m_pField	);


	FLOAT	fPoint = 15.f;
	m_pd3dDevice->SetRenderState( D3DRS_POINTSIZE, *( (DWORD*)(&fPoint) ) );
	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING,  FALSE);
	m_pd3dDevice->SetRenderState( D3DRS_FILLMODE,  m_bOptFill);
	m_pd3dDevice->SetRenderState( D3DRS_CULLMODE,  m_bOptCull? D3DCULL_CCW : D3DCULL_NONE);
	
	SAFE_RENDER(	m_LcmMdl	);
	SAFE_RENDER(	m_LcmLnk	);

	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING,  FALSE);
	m_pd3dDevice->SetRenderState( D3DRS_CULLMODE,  D3DCULL_NONE);
	m_pd3dDevice->SetRenderState( D3DRS_FILLMODE,  D3DFILL_SOLID);

	if(m_bOptBone)
	{
		m_pPivot->Render();
		
		m_pd3dDevice->SetStreamSource(0, m_pAttach, 0, sizeof(VtxD) );
		m_pd3dDevice->SetFVF(VtxD::FVF);
		m_pd3dDevice->DrawPrimitive(D3DPT_POINTLIST, 0, m_LnkLst.size());
	}
	
	RenderText();

	m_pd3dDevice->EndScene();
	
	return S_OK;
}



HRESULT CMain::RenderText()
{
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pd3dDevice->SetTexture( 0, 0);

	TCHAR szMsg[MAX_PATH];
	RECT rc;
	rc.left   = 2;
	rc.right  = m_d3dsdBackBuffer.Width - 20;
	rc.top = 3;
	rc.bottom = rc.top + 20;
	
	sprintf(szMsg, "%s %s",m_strDeviceStats, m_strFrameStats );

	m_pD3DXFont->DrawText(NULL, szMsg, -1, &rc, 0, D3DXCOLOR(1,1,0,1) );

	rc.top = 23;
	rc.bottom = rc.top + 20;


	VEC3 vcCamEye  = m_pCam->GetEye();
	VEC3 vcCamLook = m_pCam->GetLook();

	sprintf(szMsg, "Camera Eye: %3.f %3.f %3.f, Look: %3.f %3.f %3.f"
				, vcCamEye.x, vcCamEye.y, vcCamEye.z
				, vcCamLook.x, vcCamLook.y, vcCamLook.z
				);

	m_pD3DXFont->DrawText(NULL, szMsg, -1, &rc, 0, D3DXCOLOR(0,1,1, 1.f) );

	return S_OK;
}


