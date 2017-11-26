// Implementation of the CMain class.
//
////////////////////////////////////////////////////////////////////////////////


#include "../_StdAfx.h"


CMain::CMain()
{
	SetClsName(EXC_MTX);
	
	m_dHicon		= IDI_MAIN_ICON;
	m_dHmenu		= IDR_MENU;
	m_dHdevalt		= IDM_CHANGEDEVICE;
	m_dHtoggle		= IDM_TOGGLEFULLSCREEN;
	m_dHexit		= IDM_EXIT;
	m_dHaccel		= IDR_MAIN_ACCEL;
	
	
	m_pDxFnt		= NULL;
	m_pInput		= NULL;
	
	m_pCamG			= NULL;
	m_pCamO			= NULL;
	m_pGrid			= NULL;
	m_pBill			= NULL;
	
	m_pSkyMsh		= NULL;
	
	m_pTbTex		= NULL;
	m_pTbMdl		= NULL;
	
	m_pFld			= NULL;
	
	m_bMnLgt		= FALSE;
	m_bMnFog		= FALSE;
	m_bMnFill		= D3DFILL_SOLID;
	m_bMnCam		= TRUE;
	m_bMnLcl		= TRUE;
	m_bMnFrame		= TRUE;
	m_bMnSky		= FALSE;
	
	m_bMnBndBox		= FALSE;
	m_bCulling		= TRUE;
	
	m_dScnPosX		=230;
	m_dScnPosY		=0;

	m_pEffct		= NULL;
}


CMain::~CMain()
{
}



HRESULT CMain::Init()
{
	if(FAILED(LcInput_Create("API Input", &m_pInput, m_hInst, m_hWnd, GDEVICE)))
		return -1;
	
	if(FAILED(LcxTbl_Create("Texture", &m_pTbTex, "Data/TblTex.dat", GDEVICE)))
		return -1;
	
	if(FAILED(LcxTbl_Create("Model", &m_pTbMdl, "Data/TblMdl.dat", GDEVICE, m_pTbTex)))
		return -1;
	

	VEC3	vEye (500, 300, -500);
	VEC3	vLook(500, 250,    0);
	VEC3	vUp  (  0,   1,    0);


	LcxCam	t=
	{
		FLOAT( GMAIN->GetDxBackW())
			, FLOAT( GMAIN->GetDxBackH())
			, D3DX_PI/4.F
			, 1.F
			, 6000.F

			, vEye.x,  vEye.y,  vEye.z
			, vLook.x, vLook.y, vLook.z
			, vUp.x,   vUp.y,   vUp.z
	};

	
	if(FAILED(LcxCam_Create("First", &m_pCamG,	"General Camera", &t, GDEVICE)))
		return -1;

	m_pCamG->SetType(ILcxCam::LC_CAM_FIRST);

	t.vEyeX	= 300;
	t.vEyeY	= 200;
	t.vEyeZ	=-300;

	t.vLookX=   0;
	t.vLookY= 100;
	t.vLookZ=   0;
	
	if(FAILED(LcxCam_Create("First", &m_pCamO,	"Object Camera", &t, GDEVICE)))
		return -1;

	m_pCamO->SetType(ILcxCam::LC_CAM_FIRST);
	
	if(FAILED(LcxtObj_Create("XYZ Grid", &m_pGrid,	GDEVICE)))
		return -1;
	
	if(FAILED(LcxtObj_Create("billboard", &m_pBill, GDEVICE, (void*)1000)))
		return -1;

	if(FAILED(LcMap_CreateOutdoor("Out Door", &m_pFld, GDEVICE, TBTX, TBMDB	)))
		return -1;
	
	GFIELD->Query("Set Camera", m_pCamG);


	
	HRESULT	hr;
	DWORD dFlag = 0;

	#if defined( _DEBUG ) || defined( DEBUG )
		dFlag |= D3DXSHADER_DEBUG;
	#endif

	LPD3DXBUFFER	pErr = NULL;
	

	hr = D3DXCreateEffectFromFile(
					m_pd3dDevice
					, "Data/MapHlsl.fx"
					, NULL
					, NULL
					, dFlag
					, NULL
					, &m_pEffct
					, &pErr );
	
	if( FAILED(hr))
	{
		char*	sErr	= (char*)pErr->GetBufferPointer();
		MessageBox( GetActiveWindow(), sErr, "Err", MB_OK | MB_ICONERROR);
		pErr->Release();
		return hr;
	}

	m_pFld->Query("Set HLSL", m_pEffct);

//	if(GMAIN->m_WndWrk.m_WndLyr.m_hWnd)
	{
		INT		iSize = GMAIN->m_WndWrk.m_WndLyr.GetSizeDst();
		GFIELD->Query("Clear Layer List", NULL);
		
		
		for(int i=0; i<iSize; ++i)
		{
			if(GMAIN->m_WndWrk.m_WndLyr.GetStrDst(i))
			{
				char* sBase = GMAIN->m_WndWrk.m_WndLyr.GetStrDst(i);

				struct _T	{	INT n;	char* s;	};
				_T	t={i, sBase};
				
				GFIELD->Query("Set Layer List", &t);
			}
		}
	}
	
	
	
	
	m_WndWrk.Create(m_hWnd);
	m_WndTool.Create(m_hWnd);
	SetFocus(GHWND);
	
	
	D3DXFONT_DESC  lFnt= {14, 0, FW_NORMAL, 1, 0, HANGUL_CHARSET, 0, 0, VARIABLE_PITCH | FF_ROMAN, "Arial"};
	D3DXCreateFontIndirect(GDEVICE, &lFnt, &m_pDxFnt);
	
	
	if(FAILED(LcXmsh_Create("PC Mesh", &m_pSkyMsh, GDEVICE, "Model/SKY/skydome.x")))
		return -1;
	
	INT		nGeo=0;
	
	m_pSkyMsh->Query("Number Geometry", &nGeo);
	
	for(int i=0; i<nGeo; ++i)
	{
		char	sTxFile[260]={0};
		memcpy(sTxFile, &i, sizeof(int));
		
		m_pSkyMsh->Query("Get Texture Name", sTxFile);
		
		INT nM=-1, nS=-1;
		PDTX pTx = NULL;
		//		TBTX->SelectIdx(&nM, &nS,	sTxFile);
		TBTX->Select( (void**)&pTx, &nM, &nS,	"Env_Sky.jpg");
		
		
		struct _T {	int n; void* p;	};
		
		_T	t = {i, pTx};
		
		m_pSkyMsh->Query("Set Texture Pointer", &t);
	}
	
	
	
	
	
	if( D3DFILL_SOLID == m_bMnFill)
		CheckMenuItem(m_hMenu, IDM_SOLID, MF_BYCOMMAND | MF_CHECKED);
	else
		CheckMenuItem(m_hMenu, IDM_SOLID, MF_BYCOMMAND | MF_UNCHECKED);
	
	
	if(m_bMnLgt)
		CheckMenuItem(m_hMenu, IDM_LGT, MF_BYCOMMAND | MF_CHECKED);
	else
		CheckMenuItem(m_hMenu, IDM_LGT, MF_BYCOMMAND | MF_UNCHECKED);
	
	
	if(m_bMnFog)
		CheckMenuItem(m_hMenu, IDM_FOG, MF_BYCOMMAND | MF_CHECKED);
	else
		CheckMenuItem(m_hMenu, IDM_FOG, MF_BYCOMMAND | MF_UNCHECKED);
	
	
	if(m_bMnCam)
		CheckMenuItem(m_hMenu, IDM_LINE_CAMERA, MF_BYCOMMAND | MF_CHECKED);
	else
		CheckMenuItem(m_hMenu, IDM_LINE_CAMERA, MF_BYCOMMAND | MF_UNCHECKED);
	
	
	if(m_bMnLcl)
		CheckMenuItem(m_hMenu, IDM_LINE_LOCAL, MF_BYCOMMAND | MF_CHECKED);
	else
		CheckMenuItem(m_hMenu, IDM_LINE_LOCAL, MF_BYCOMMAND | MF_UNCHECKED);
	
	
	if(m_bMnFrame)
		CheckMenuItem(m_hMenu, IDM_SHOW_FRAME, MF_BYCOMMAND | MF_CHECKED);
	else
		CheckMenuItem(m_hMenu, IDM_SHOW_FRAME, MF_BYCOMMAND | MF_UNCHECKED);
	
	
	if(m_bMnSky)
		CheckMenuItem(m_hMenu, IDM_SHOW_SKY, MF_BYCOMMAND | MF_CHECKED);
	else
		CheckMenuItem(m_hMenu, IDM_SHOW_SKY, MF_BYCOMMAND | MF_UNCHECKED);
	
	
	if(m_bMnBndBox)
		CheckMenuItem(m_hMenu, IDM_SHOW_BNDBOX, MF_BYCOMMAND | MF_CHECKED);
	else
		CheckMenuItem(m_hMenu, IDM_SHOW_BNDBOX, MF_BYCOMMAND | MF_UNCHECKED);
	
	
	return S_OK;
}


HRESULT CMain::Destroy()
{
	SAFE_RELEASE(	m_pDxFnt	);
	SAFE_DELETE(	m_pInput	);
	SAFE_DELETE(	m_pCamG		);
	SAFE_DELETE(	m_pCamO		);
	
	//	m_pGrid->Destroy();
	SAFE_DELETE(	m_pGrid		);
	
	SAFE_DELETE(	m_pTbTex	);
	SAFE_DELETE(	m_pTbMdl	);
	
	SAFE_DELETE(	GFIELD		);
	
	SAFE_DESTROY(	&m_WndWrk	);
	SAFE_DESTROY(	&m_WndTool	);
	
	SAFE_DELETE(	m_pSkyMsh	);

	SAFE_RELEASE(	m_pEffct	);
	
	return S_OK;
}


HRESULT CMain::Restore()
{
	GDEVICE->GetViewport(&m_Vp);
	
	if(m_bWindowed)
	{
		GDEVICE->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &m_SwpWn.pB);
		GDEVICE->GetDepthStencilSurface(&m_SwpWn.pS);
	}
	
	SAFE_RESTORE(	&m_WndWrk	);
	SAFE_RESTORE(	&m_WndTool	);
	
	
	m_pDxFnt->OnResetDevice();
	m_pEffct->OnResetDevice();


	for(int i=0; i<8; ++i)
	{
		m_pd3dDevice->SetSamplerState(i, D3DSAMP_ADDRESSU, D3DTEXF_LINEAR);
		m_pd3dDevice->SetSamplerState(i, D3DSAMP_ADDRESSV, D3DTEXF_LINEAR);
		m_pd3dDevice->SetSamplerState(i, D3DSAMP_ADDRESSW, D3DTEXF_LINEAR);
	}

	return S_OK;
}



HRESULT CMain::Invalidate()
{
	m_pDxFnt->OnLostDevice();
	
	SAFE_INVALID(	&m_WndWrk	);
	SAFE_INVALID(	&m_WndTool	);
	
	m_SwpWn.Release();

	m_pEffct->OnLostDevice();
	
	return S_OK;
}


HRESULT CMain::FrameMove()
{
	Lcm::ILcmMdl* pObj = NULL;
	
	if(GFIELD)
		GFIELD->Query("Get Pick Object", &pObj);
	
	
	HWND hWnd = GetActiveWindow();
	
	sprintf(m_sMsg, "%s %s", m_strDeviceStats, m_strFrameStats );
	
	SAFE_FRMOV(	m_pInput	);													// Update Input
	
	VEC3 vcD = GINPUT->GetMouseDelta();
	FLOAT fCamSpeed = 10.f;
	
	if(hWnd && hWnd == GHWND)
	{
		if(vcD.z !=0.f)
			m_pCamG->MoveForward(-vcD.z* 1.f);
		
		if(GINPUT->KeyPress(LCK_W))					// W
			m_pCamG->MoveForward( fCamSpeed);
		
		if(GINPUT->KeyPress(LCK_S))					// S
			m_pCamG->MoveForward(-fCamSpeed);
		
		if(GINPUT->KeyPress(LCK_A))					// A
			m_pCamG->MoveSideward(-fCamSpeed);
		
		if(GINPUT->KeyPress(LCK_D))					// D
			m_pCamG->MoveSideward(fCamSpeed);
		
		if(GINPUT->ButtonPress(1))
		{
			m_pCamG->RotateYaw(vcD.x, 0.1f);
			m_pCamG->RotatePitch(vcD.y, 0.1f);
		}
	}

	else if(hWnd == m_WndWrk.m_hWnd && GMAIN->IsWindow())
	{
		if(GINPUT->KeyPress(LCK_ADD))
			m_pCamO->MoveForward(-2.f, 1.f);
		
		if(GINPUT->KeyPress(LCK_SUBTRACT))
			m_pCamO->MoveForward(2.f, 1.f);
	}


	SAFE_FRMOV(	m_pCamG		);

	
	
	if(GINPUT->KeyPress(LCK_DELETE))
	{
		if(pObj)
		{
			GFIELD->Query("Erase Pick Object", NULL);
			pObj = NULL;
			
		}
	}
	
	
	if(pObj)
	{
		VEC3	vcPos;
		
		pObj->GetPosition((FLOAT*)&vcPos);
		
		if(GINPUT->KeyPress(LCK_LEFT))
			vcPos.x -=1;
		
		if(GINPUT->KeyPress(LCK_RIGHT))
			vcPos.x +=1;
		
		if(GINPUT->KeyPress(LCK_UP))
			vcPos.z +=1;
		
		if(GINPUT->KeyPress(LCK_DOWN))
			vcPos.z -=1;
		
		if(GINPUT->KeyPress(LCK_ADD))
			vcPos.y +=1;
		
		if(GINPUT->KeyPress(LCK_SUBTRACT))
			vcPos.y -=1;
		
		pObj->SetPosition((FLOAT*)&vcPos);
	}
	
	
	GFIELD->Query("Set Culling Enable", &GMAIN->m_bCulling);
	
	FLOAT	fCull		= 3000;
	GFIELD->Query("Set Culling Range", &fCull);
	
	
	BOOL bCtrlPck = GINPUT->ButtonDown(0) || (GINPUT->ButtonPress(0) && GINPUT->KeyPress(LCK_LCONTROL));
	GFIELD->Query("Set Picking Enable", &bCtrlPck);
	
	BOOL bCtrlHeight	= GMAIN->m_WndWrk.IsHeight() && bCtrlPck;
	GFIELD->Query("Set Height Enable", &bCtrlHeight);
	
	BOOL bCtrlSpoid	= GMAIN->m_WndWrk.UseSpoid() && bCtrlPck;
	GFIELD->Query("Spoid Enable", &bCtrlSpoid);
	
	BOOL bCtrlBrush	= GMAIN->m_WndWrk.UseBrush() && bCtrlPck;
	GFIELD->Query("Brush Enable", &bCtrlBrush);
	
	BOOL bCtrlModel	= GMAIN->m_WndWrk.UseMdB() && bCtrlPck;
	GFIELD->Query("Model Setup Enable", &bCtrlModel);
	
	if(bCtrlModel)
	{
		Lcm::ILcmMdl* pMdl = GMAIN->m_WndWrk.GetMdB();
		GFIELD->Query("Set Model Pointer", pMdl);
	}
	
	
	INT	 nCtrlModel	= GMAIN->m_WndWrk.GetLstIdx();
	GFIELD->Query("Set Model Index", &nCtrlModel);
	
	BOOL bCtrlTile		= GMAIN->m_WndWrk.UseTile() && (GINPUT->ButtonPress(0) && GINPUT->KeyPress(LCK_LCONTROL));
	GFIELD->Query("Tiling Enable", &bCtrlTile);
	
	BOOL bCtrlBndBox	= GMAIN->m_bMnBndBox;
	GFIELD->Query("Bound Box Enable", &bCtrlBndBox);
	
	BOOL bCtrlLclLine	= GMAIN->m_bMnLcl;
	GFIELD->Query("Show Local Enable", &bCtrlLclLine);
	
	
	if(bCtrlBrush)
	{
		DWORD	dFillColor = GMAIN->m_WndWrk.GetColor();
		GFIELD->Query("Set Fill Color", &dFillColor);
	}
	
	
	if(bCtrlTile)
	{
		INT		nTile = GMAIN->m_WndWrk.m_WndLyr.GetDstIdx();
		GFIELD->Query("Set Tile Index", &nTile);
	}
	
	
	
	Lcxmo::MpHgt	Hgt;
	
	Hgt.iFlat= GMAIN->m_WndWrk.IsFlat();
	Hgt.iRng= GMAIN->m_WndWrk.GetRng(); 							// Radius
	Hgt.bAc = GMAIN->m_WndWrk.m_bAc;
	Hgt.fH	= Hgt.iFlat * fabsf(GMAIN->m_WndWrk.m_fDH);
	Hgt.fDH = fabsf(GMAIN->m_WndWrk.m_fDH) * GMAIN->m_WndWrk.GetDirection();
	
	GFIELD->Query("Set Field Info", &Hgt);
	
	FLOAT* pvcPos = GINPUT->GetMousePos();
	POINT	pt={ pvcPos[0], pvcPos[1]};
	
	
	VEC3	vcRayDir = GCAMGNL->GetDirectionOfMouse(pt.x, pt.y);
	VEC3	vcCamPos = *GCAMGNL->GetEye();
	
	GFIELD->Query("Set Ray Direction", &vcRayDir);
	GFIELD->Query("Set Camera Position", &vcCamPos);
	
	
	if(GMAIN->m_WndWrk.m_WndLyr.m_hWnd)
	{
		INT		iSize = GMAIN->m_WndWrk.m_WndLyr.GetSizeDst();
		
		GFIELD->Query("Clear Layer List", NULL);
		
		
		for(int i=0; i<iSize; ++i)
		{
			if(GMAIN->m_WndWrk.m_WndLyr.GetStrDst(i))
			{
				char* sBase = GMAIN->m_WndWrk.m_WndLyr.GetStrDst(i);
				
				struct _T	{	INT n;	char* s;	};
				_T	t={i, sBase};
				
				GFIELD->Query("Set Layer List", &t);
			}
		}
	}
	
	
	SAFE_FRMOV(	m_pFld	);
	
	
	if(bCtrlSpoid)
	{
		DWORD	dFillColor=0;
		
		GFIELD->Query("Get Fill Color", &dFillColor);
		GMAIN->m_WndWrk.SetColor(dFillColor);
	}
	
	if(bCtrlPck &&GMAIN->m_WndWrk.m_WndObj.m_hWnd)
	{
		Lcm::ILcmMdl* pMdl = NULL;
		
		GFIELD->Query("Get Model Pointer", &pMdl);
		GMAIN->m_WndWrk.m_WndObj.SetObj(pMdl);
	}
	
	SAFE_FRMOV(	&m_WndWrk	);
	SAFE_FRMOV(	&m_WndTool	);
	
	
	return S_OK;
}


HRESULT CMain::Render()
{
	if(m_bLoadingRnd)
		return 0;
	
	if(!GDEVICE)
		return -1;
	

	m_WndWrk.Render();
	m_WndTool.Render();

	if(m_bWindowed)
	{
		GDEVICE->SetRenderTarget(0, m_SwpWn.pB);
		GDEVICE->SetDepthStencilSurface(m_SwpWn.pS);
	}
	
	m_pCamG->SetTransformPrj();
	m_pCamG->SetTransformViw();
	
	GDEVICE->SetViewport(&m_Vp);
	
	GDEVICE->Clear( 0, 0, GMAIN->GetDxClearMode(), 0x00006699, 1.0f, 0L );
	
	if( FAILED( GDEVICE->BeginScene() ) )
		return -1;
	
	GDEVICE->SetRenderState( D3DRS_LIGHTING,	GMAIN->m_bMnLgt);
	GDEVICE->SetRenderState(D3DRS_FOGENABLE,	GMAIN->m_bMnFog);
	GDEVICE->SetRenderState( D3DRS_FILLMODE,	GMAIN->m_bMnFill);
	
	
	
	
	if(GMAIN->m_bMnCam)
		SAFE_RENDER( m_pGrid	);
	
	
	if(GMAIN->m_bMnSky)
	{
		static DOUBLE	angle =0;
		DWORD			dBgn = timeGetTime();
		
		D3DXMATRIX	mtR;
		D3DXMATRIX	mtW;
		
		VEC3		vcCam = *m_pCamG->GetEye();
		
		angle += dBgn/2000000000.f;
		
		if( angle> 360.)
			angle -=360.;
		
		D3DXMatrixRotationY(&mtR, D3DXToRadian(angle));
		
		D3DXMatrixScaling( &mtW, 16, 10, 16);
		
		mtW *=mtR;
		
		mtW._41 = vcCam.x;
		mtW._42 = vcCam.y-3800;
		mtW._43 = vcCam.z;
		
		GDEVICE->SetTransform( D3DTS_WORLD, &mtW );
		GDEVICE->SetRenderState( D3DRS_LIGHTING,     FALSE );
		
		SAFE_RENDER(	m_pSkyMsh	);
	}
	
	GDEVICE->SetRenderState( D3DRS_LIGHTING,	GMAIN->m_bMnLgt);
	GDEVICE->SetRenderState(D3DRS_FOGENABLE,	GMAIN->m_bMnFog);
	GDEVICE->SetRenderState( D3DRS_FILLMODE,	GMAIN->m_bMnFill);
	
	
	
	SAFE_RENDER(	GFIELD	);
	
	
	GDEVICE->SetRenderState( D3DRS_LIGHTING,	FALSE);
	GDEVICE->SetRenderState(D3DRS_FOGENABLE,	FALSE);
	GDEVICE->SetRenderState( D3DRS_FILLMODE,	D3DFILL_SOLID);
	
	
	if(GMAIN->m_bMnFrame)
	{
		RECT rt={5,5, 800,20};
		GMAIN->m_pDxFnt->DrawText(NULL, GMAIN->m_sMsg, -1, &rt, 0, 0XFFFFFF00);
	}
	
	
	GDEVICE->EndScene();
	
	return S_OK;
}






LRESULT CMain::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	WPARAM	wparHi = HIWORD(wParam);
	WPARAM	wparLo = LOWORD(wParam);

	if(m_pInput)
	{
		if(m_pInput->MsgProc(hWnd, uMsg, wParam, lParam))
			return 0;
	}
	
	switch( uMsg )
	{
		case WM_PAINT:
		{
			if( m_bLoadingApp )
			{
				HDC hDC = GetDC( hWnd );
				ReleaseDC( hWnd, hDC );
			}
			break;
		}
		
		case WM_MOVE:
		{
			if(FALSE == m_bLoadingApp)
			{
				RECT rt1;
				RECT rt2;
				INT iWidth;
				INT iHeight;
				INT iX;
				INT iY;

				GetWindowRect(GHWND, &rt1);
				GetWindowRect(m_WndWrk.m_hWnd, &rt2);

				iWidth = rt2.right - rt2.left;
				iHeight=  rt2.bottom- rt2.top;

				iX = rt1.left - iWidth;
				iY = rt1.top;// + iHeight;

				MoveWindow(m_WndWrk.m_hWnd, iX, iY, iWidth, iHeight, TRUE);


				GetWindowRect(m_WndTool.m_hWnd, &rt2);

				iWidth = rt2.right - rt2.left;
				iHeight=  rt2.bottom- rt2.top;

				iX = rt1.left-227;
				iY = rt1.bottom+2;

				MoveWindow(m_WndTool.m_hWnd, iX, iY, iWidth, iHeight, TRUE);
			}
			
			break;
		}// case WM_MOVE
		
		
		case WM_COMMAND:
		{
			switch(wparLo)
			{
				case IDM_TOGGLEFULLSCREEN:
				{
					if(m_bWindowed)
					{
						ShowWindow(m_WndWrk.m_hWnd, SW_HIDE);
						
						if(m_WndWrk.m_WndLyr.m_bShow)
						{
							ShowWindow(m_WndWrk.m_WndLyr.m_hWnd, SW_HIDE);
						}
					}
					else
					{
						ShowWindow(m_WndWrk.m_hWnd, SW_SHOW);
						
						if(m_WndWrk.m_WndLyr.m_bShow)
						{
							ShowWindow(m_WndWrk.m_WndLyr.m_hWnd, SW_SHOW);
						}
					}
					
					
					break;
				}
				
				case IDM_NEW:
				{
					this->m_WndWrk.m_WndFog.Init();
					this->m_WndWrk.m_WndLght.Init();
					this->m_WndWrk.m_WndFld.Create(GHWND);
					this->m_WndWrk.m_WndFld.ShowWindow();
					
					break;
				}
				
				case IDM_OPEN:
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
						OFN.hwndOwner		= GetActiveWindow();
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
						
						Lcxmo::MpEnv Env;
						GFIELD->Query("Get Environment", &Env);
						
						m_WndWrk.m_WndLght.m_iLg	= Env.m_iLg;							// Lighting Num
						
						memset(m_WndWrk.m_WndLght.m_pLg, 0, sizeof(m_WndWrk.m_WndLght.m_pLg));
						memcpy(m_WndWrk.m_WndLght.m_pLg, Env.m_pLg, sizeof(Env.m_pLg));
						
						memcpy(&m_WndWrk.m_WndFog.m_Fog,	&Env.m_Fog, sizeof(Env.m_Fog));	// Fog Table Mode
						
					}
					
					break;
				}
				
				case IDM_SAVE:
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
						OFN.hwndOwner		= GetActiveWindow();
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
					}
					
					break;
				}
				
				case IDM_SOLID:
				{
					if( D3DFILL_SOLID == m_bMnFill)
						m_bMnFill = D3DFILL_WIREFRAME;
					
					else
						m_bMnFill = D3DFILL_SOLID;
					
					if( D3DFILL_SOLID == m_bMnFill)
						CheckMenuItem(m_hMenu, IDM_SOLID, MF_BYCOMMAND | MF_CHECKED);
					else
						CheckMenuItem(m_hMenu, IDM_SOLID, MF_BYCOMMAND | MF_UNCHECKED);
					
					break;
				}
				
				case IDM_LGT:
				{
					m_bMnLgt ^=1;
					
					if(m_bMnLgt)
						CheckMenuItem(m_hMenu, IDM_LGT, MF_BYCOMMAND | MF_CHECKED);
					else
						CheckMenuItem(m_hMenu, IDM_LGT, MF_BYCOMMAND | MF_UNCHECKED);
					
					break;
				}
				
				case IDM_FOG:
				{
					m_bMnFog ^=1;
					
					if(m_bMnFog)
						CheckMenuItem(m_hMenu, IDM_FOG, MF_BYCOMMAND | MF_CHECKED);
					else
						CheckMenuItem(m_hMenu, IDM_FOG, MF_BYCOMMAND | MF_UNCHECKED);
					
					break;
				}
				
				case IDM_LINE_CAMERA:
				{
					m_bMnCam ^=1;
					
					if(m_bMnCam)
						CheckMenuItem(m_hMenu, IDM_LINE_CAMERA, MF_BYCOMMAND | MF_CHECKED);
					else
						CheckMenuItem(m_hMenu, IDM_LINE_CAMERA, MF_BYCOMMAND | MF_UNCHECKED);
					
					break;
				}
				
				case IDM_LINE_LOCAL:
				{
					m_bMnLcl ^=1;
					
					if(m_bMnLcl)
						CheckMenuItem(m_hMenu, IDM_LINE_LOCAL, MF_BYCOMMAND | MF_CHECKED);
					else
						CheckMenuItem(m_hMenu, IDM_LINE_LOCAL, MF_BYCOMMAND | MF_UNCHECKED);
					
					break;
				}
				
				
				case IDM_SHOW_FRAME:
				{
					m_bMnFrame ^=1;
					
					if(m_bMnFrame)
						CheckMenuItem(m_hMenu, IDM_SHOW_FRAME, MF_BYCOMMAND | MF_CHECKED);
					else
						CheckMenuItem(m_hMenu, IDM_SHOW_FRAME, MF_BYCOMMAND | MF_UNCHECKED);
					
					break;
				}
				
				case IDM_SHOW_SKY:
				{
					m_bMnSky ^=1;
					
					if(m_bMnSky)
						CheckMenuItem(m_hMenu, IDM_SHOW_SKY, MF_BYCOMMAND | MF_CHECKED);
					else
						CheckMenuItem(m_hMenu, IDM_SHOW_SKY, MF_BYCOMMAND | MF_UNCHECKED);
					
					break;
				}
				
				case IDM_SHOW_BNDBOX:
				{
					m_bMnBndBox ^=1;
					
					if(m_bMnBndBox)
						CheckMenuItem(m_hMenu, IDM_SHOW_BNDBOX, MF_BYCOMMAND | MF_CHECKED);
					else
						CheckMenuItem(m_hMenu, IDM_SHOW_BNDBOX, MF_BYCOMMAND | MF_UNCHECKED);
					
					break;
				}
				
				
				case IDM_CULLING_FRUSTUM:
				{
					m_bCulling ^=1;
					
					if(m_bCulling)
						CheckMenuItem(m_hMenu, IDM_CULLING_FRUSTUM, MF_BYCOMMAND | MF_CHECKED);
					else
						CheckMenuItem(m_hMenu, IDM_CULLING_FRUSTUM, MF_BYCOMMAND | MF_UNCHECKED);
					
					break;
				}
				
				
				case IDM_HELP:
				{
					if(IDOK == DialogBox( GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_HELP),	hWnd, CMain::AboutPrc))
					{
					}
					
					break;
				}
				
			}

			break;

		}// case WM_COMMAND

	}

	return CD3DApplication::MsgProc( hWnd, uMsg, wParam, lParam );
}



BOOL WINAPI CMain::AboutPrc(HWND hDlg,UINT nMsg,WPARAM wParam,LPARAM lParam)
{
	switch(nMsg)
	{
	case WM_INITDIALOG:
		{
			RECT rt1;
			RECT rt2;
			
			INT iWidth;
			INT iHeight;
			
			INT iX;
			INT iY;
			
			
			GetWindowRect(GHWND, &rt1);
			GetWindowRect(hDlg, &rt2);
			
			iWidth = rt2.right - rt2.left;
			iHeight=  rt2.bottom- rt2.top;
			
			iX = rt1.left + 100;
			iY = rt1.top + 100;
			
			MoveWindow(hDlg, iX, iY, iWidth, iHeight, TRUE);
			
			return TRUE;
		}
		
	case WM_COMMAND:
		{
			switch (wParam)
			{
			case IDC_HELP_CLOSE:
				{
					EndDialog(hDlg,IDOK);
					
					return TRUE;
				}
				
			}
			
			break;
		}
	}
	
	return FALSE;
}

