// Implementation of the CMain class.
//
////////////////////////////////////////////////////////////////////////////////


#include "../_StdAfx.h"


CMain::CMain()
{
	m_pFont		= NULL;

	m_pDevSys	= NULL;
	m_hInst		= NULL;
	m_hWnd		= NULL;
	m_pDevD3D	= NULL;
	m_pBck		= NULL;

	m_pDev2D	= NULL;
	m_pInput	= NULL;
	m_pCamM		= NULL;
	m_pIme		= NULL;

	m_pLsc		= NULL;

	m_ePhCur	= 1;
	m_ePhOld	= 0;

	m_nGmPhs	= 0;
	m_pGmPhs	= NULL;
}



CMain::~CMain()
{
}




INT CMain::Init()
{
	INT		hr=-1;
	INT		i=0;


	m_hInst	= (HINSTANCE)LcDev_GetHinst(&m_pDevSys);
	m_hWnd	= (HWND)	LcDev_GetHwnd(&m_pDevSys);
	m_pDevD3D= (PDEV)	LcDev_GetD3Device(&m_pDevSys);
	m_pBck	= (PDSF)	LcDev_GetBackBufferSurface(&m_pDevSys);


	TLcFnt	Tfont("Arial", 14, FW_BOLD, 0, 0, 0, 0xFFFFFF00, 0xFF0000AA);
	hr = LcFont_Create("PC Font", &m_pFont, m_pDevD3D, &Tfont);

	if(FAILED(hr))
		return -1;


	// Create 2D Sprite
	if(FAILED(LcDev_CreateSprite("PC Sprite", &m_pDev2D, m_pDevD3D)))
		return -1;

	if(FAILED(LcDev_SetUserSprite(&m_pDevSys, m_pDev2D)))
		return -1;


	// Create Input
	if(FAILED(LcInput_Create("API Input", &m_pInput, m_hInst, m_hWnd, m_pDevD3D)))
		return -1;

	if(FAILED(LcDev_SetUserInput(&m_pDevSys, m_pInput)))
		return -1;


	// Create Camera
	if(FAILED(LcxCam_CreateMuti(NULL, &m_pCamM, m_pDevD3D)))
		return -1;

	if(FAILED(LcDev_SetUserCamera(&m_pDevSys, m_pCamM)))
		return -1;


	// Create Hangul IME
	if(FAILED(LcIme_Create("Hangul", &m_pIme, NULL)))
		return -1;

	if(FAILED(LcDev_SetUserIme(&m_pDevSys, m_pIme)))
		return -1;


	
	if(FAILED(LcSmd_DeviceInit(m_hWnd)))										// Direct Sound, 3D Sound 가능 여부 테스트
		return -1;


	if(FAILED(LcNet_WinSockCreate()))											// 윈속을 생성. TCP/IP 를 사용할 수 있는 지 테스트
		return -1;



	IGameData*	pGameData = NULL;
	if(FAILED(LcxApp_CreateGameData(NULL, &pGameData, m_pDevSys)))				// Game Data
		return -1;

	if(FAILED(LcDev_SetUserData1(&m_pDevSys, pGameData)))
		return -1;



	// LuaScript Initialize
	LcLua::LscSys	::Create(m_pDevSys);										// Lua System
	LcLua::LscInput	::Create(m_pDevSys);										// Lua Input Class
	LcLua::LscD3d	::Create(m_pDevSys);										// Lua D3D
	LcLua::LscUtil	::Create(m_pDevSys);										// Lua Utilities
	
	LcLua::LscIme	::Create(m_pDevSys);										// Lua Ime Class
	LcLua::LscNet	::Create(m_pDevSys);										// Lua Network Class

	LcLua::LscFont	::Create(m_pDevSys);										// Lua Font Class
	LcLua::LscTex	::Create(m_pDevSys);										// Lua Texture Class
	LcLua::LscVtx	::Create(m_pDevSys);										// Lua Vertex Class
	LcLua::LscMtl	::Create(m_pDevSys);										// Lua Material Class
	LcLua::LscLgt	::Create(m_pDevSys);										// Lua Lighting Class
	LcLua::LscMshX	::Create(m_pDevSys);										// Lua X-File Class
	LcLua::LscxBill	::Create(m_pDevSys);
	LcLua::LscSmd	::Create(m_pDevSys);										// Lua Sound Class

	LcLua::LscxUtil	::Create(m_pDevSys);										// Init LscLcx Utilities Class
	LcLua::LscxMshX	::Create(m_pDevSys);										//
	LcLua::LscxMapi	::Create(m_pDevSys);										// Init LscLcx Utilities Class
	LcLua::LscMdl	::Create(m_pDevSys);										// Lua Mdl-File Class

	LcLua::LsgApp	::Create(m_pDevSys);										// Lua Game Application
	
	
	m_pDevSys->Query("Get Script File Number", &m_nGmPhs);


	// Game Phase Creating
	m_pGmPhs = (IGmPhase**)new CAppPhase*[m_nGmPhs];
	memset(m_pGmPhs, 0, m_nGmPhs * sizeof(CAppPhase*));


	// Set Start Phase..
	CAppPhase* pPhase = new CAppPhase;
	pPhase->Create(&m_ePhCur, m_pDevSys);
	m_pGmPhs[m_ePhCur] = pPhase;

	return 0;
}


void CMain::Destroy()
{
	INT	i=0;

	LcLua::LsgApp	::Destroy();

	LcLua::LscSys	::Destroy();
	
	LcLua::LscFont	::StaticContainerDestroy();
	LcLua::LscTex	::StaticContainerDestroy();
	LcLua::LscVtx	::StaticContainerDestroy();
	LcLua::LscMtl	::StaticContainerDestroy();
	LcLua::LscLgt	::StaticContainerDestroy();
	LcLua::LscMshX	::StaticContainerDestroy();
	LcLua::LscxBill	::StaticContainerDestroy();
	LcLua::LscSmd	::StaticContainerDestroy();

	LcLua::LscxUtil	::StaticContainerDestroy();
	LcLua::LscxMshX	::StaticContainerDestroy();
	LcLua::LscxMapi	::StaticContainerDestroy();
	LcLua::LscMdl	::StaticContainerDestroy();


	SAFE_DELETE(	m_pLsc		);

	if(m_pGmPhs)
	{
		for(i=0; i< m_nGmPhs;++i)
		{
			delete m_pGmPhs[i];
		}

		delete [] m_pGmPhs;

		m_pGmPhs	= NULL;
		m_nGmPhs	= 0;
	}


	IGameData*	pGameData = (IGameData*)LcDev_GetUserData1(&m_pDevSys);
	if(pGameData)
	{
		delete pGameData;
		pGameData = NULL;
		LcDev_SetUserData1(&m_pDevSys, NULL);
	}

	SAFE_DELETE(	m_pIme		);
	SAFE_DELETE(	m_pInput	);
	SAFE_DELETE(	m_pCamM		);
	SAFE_DELETE(	m_pDev2D	);
	SAFE_DELETE(	m_pFont		);

	::LcD3D_EffectFileDestroy();												// ID3DXEffect File Delete


	LcSmd_DeviceClose();														// 사운드 디바이스는 마지막에 해제
	LcNet_WinsockDestroy();														// 윈속을 생성. TCP/IP 를 사용할 수 있는 지 테스트
}



INT CMain::FrameMove()
{
	IGameData* pGameData = (IGameData*)LcDev_GetUserData1(&m_pDevSys);


	//	VEC3 vcD = m_pInput->GetMouseDelta();
	//	LcUtil_SetWindowTitle(m_hWnd, "%f %f %f"
	//	, vcD.x, vcD.y, vcD.z);
	DOUBLE	fTime = 0.;
	m_pDevSys->Query("TimeGetTime", &fTime);
	fTime *= 0.0003;




	// Update Sequence
	// 1. Update Input
	SAFE_FRMOV(	m_pInput	);

	
	// 2. Get Network Data
	ILcNet*	pNet	= (ILcNet*)LcDev_GetUserNetwork(&m_pDevSys);

	if(pNet && FAILED(pNet->NetState()) )
	{
		LcDev_SetUserNetwork(&m_pDevSys, NULL);
//		MessageBox(m_hWnd, "Connection Error", "Err", MB_ICONERROR);
		delete pNet;
		pNet = NULL;
	}

	if(pNet && ILcNet::NETST_CONNECTED == pNet->NetState())
	{
	}


	// 3. Setup Game Phase
	SAFE_FRMOV(	m_pGmPhs[m_ePhCur]	);


	// 4. 루아에서 보낸 명령어 처리
	deque<LcLua::LsgApp::Tcommand>*	pCmd =
		(deque<LcLua::LsgApp::Tcommand>*)LcDev_GetUserCommand(&m_pDevSys);

	if(pCmd)
	{
		deque<LcLua::LsgApp::Tcommand>::iterator it = pCmd->begin();

		while(!pCmd->empty())
		{
			char sCmd[512]={0};
			char sVal[512]={0};

			strcpy(sCmd, it->sCmd);
			strcpy(sVal, it->sVal);

			printf("%s\n", sVal);
			pCmd->pop_front();
			it = pCmd->begin();

			if(0 == _stricmp("Setup Login UID", sCmd))
				pGameData->Query(sCmd, sVal);

			else if(0 == _stricmp("Setup Login PWD", sCmd))
				pGameData->Query(sCmd, sVal);

			else if(0 == _stricmp("Login", sCmd))
			{
				char	sUid[32]={0};
				char	sPwd[16]={0};
				pGameData->Query("Get Login UID", sUid);
				pGameData->Query("Get Login PWD", sPwd);

				char	sBufSnd[64]={0};
				int		iLenSnd =strlen(sUid) + strlen(sPwd) + 1;

				strcpy(sBufSnd, sUid);
				strcpy(sBufSnd+ strlen(sUid) + 1, sPwd);

				if(pNet && FAILED(pNet->NetState()) )
				{
					DWORD nMsgSnd = 0x001;
					pNet->Send(sBufSnd, iLenSnd, nMsgSnd);
				}
			}



			else if(0 == _stricmp("Add Server List", sCmd))
				pGameData->Query(sCmd, sVal);

			else if(0 == _stricmp("Select Server", sCmd))
			{
				char	sServer[64]={0};
				strcpy(sServer, sVal);

				pGameData->Query(sCmd, sVal);
				

				char	sBufSnd[64]={0};
				int		iLenSnd =strlen(sServer);
				strcpy(sBufSnd, sServer);

				if(pNet && FAILED(pNet->NetState()) )
				{
					DWORD nMsgSnd = 0x002;
					pNet->Send(sBufSnd, iLenSnd, nMsgSnd);
				}
			}



			else if(0 == _stricmp("Add Man Character", sCmd))
				pGameData->Query(sCmd, sVal);

			else if(0 == _stricmp("Add Lady Character", sCmd))
				pGameData->Query(sCmd, sVal);


			else if(0 == _stricmp("Setup Man Character Index", sCmd))
				pGameData->Query(sCmd, sVal);

			else if(0 == _stricmp("Setup Lady Character Index", sCmd))
				pGameData->Query(sCmd, sVal);


			else if(0 == _stricmp("Setup Man Character Name", sCmd))
				pGameData->Query(sCmd, sVal);

			else if(0 == _stricmp("Setup Lady Character Name", sCmd))
				pGameData->Query(sCmd, sVal);


			else if(0 == _stricmp("Select Character", sCmd))
			{
				char	sMan [64]={0};
				char	sLady[64]={0};
				pGameData->Query("Selected Man Character", sMan);
				pGameData->Query("Selected Lady Character", sLady);

				char	sBufSnd[128]={0};
				int		iLenSnd =strlen(sMan) + strlen(sLady) + 1;

				strcpy(sBufSnd, sMan);
				strcpy(sBufSnd+ strlen(sMan) + 1, sLady);

				if(pNet && FAILED(pNet->NetState()) )
				{
					DWORD nMsgSnd = 0x003;
					pNet->Send(sBufSnd, iLenSnd, nMsgSnd);
				}
			}


		}
	}




	
	// 5. Network Update.
	// 패킷을 보낼 때 사용. 패킷 수신은 메시지 프로시저 함수에서 처리
	if(pNet && ILcNet::NETST_CONNECTED == pNet->NetState())
	{
		pNet->FrameMove();
	}

	return 0;
}


INT CMain::Render()
{
	if(m_ePhOld != m_ePhCur)													// Old and Current Phase Different
	{
		m_ePhOld = m_ePhCur;
		return 0;
	}
	
	
	m_pDevSys->BeginScene();

	DWORD	dClrColor=0XFF006699;
	DWORD	dClrMode =0x0;

	m_pDevSys->Query("Get Clear Color", &dClrColor);
	m_pDevSys->Query("Get Clear Mode" , &dClrMode);

	m_pDevSys->Clear(0, NULL, dClrMode, dClrColor, 1, 0L);


	SAFE_RENDER(	m_pGmPhs[m_ePhCur]	);


	BOOL bShowStats=FALSE;
	m_pDevSys->Query("Get Show State", &bShowStats);

	if(bShowStats)
	{
		DOUBLE	dFPS=0;
		DOUBLE	dAvg=0;
		TCHAR	sMsg[128]={0};

		TCHAR	sDeviceState[128]={0};
		TCHAR	sFrameState[128]={0};
		
		m_pDevSys->Query("Get FPS", &dFPS);
		m_pDevSys->Query("Get Elapsed Average Time", &dAvg);
		m_pDevSys->Query("Get Device State", sDeviceState);
		m_pDevSys->Query("Get Frame State", sFrameState);

		sprintf(sMsg, "%s FPS:%4.f(%6.4f)", sFrameState, dFPS, dAvg);

		FLOAT xy[3]={0,0,0};
		m_pFont->SetPos(xy);
		m_pFont->SetString(sMsg);
		
		m_pFont->DrawTxt();
	}
	
	m_pDevSys->EndScene();



	return 0;
}


INT CMain::Restore()
{
	D3DCAPS9	d3dCaps;
	m_pDevD3D->GetDeviceCaps(&d3dCaps);

	for(INT i=0; i<8; ++i)
	{
		// Filtering
		m_pDevD3D->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pDevD3D->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pDevD3D->SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

		// set max anisotropy
		m_pDevD3D->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, min(16, d3dCaps.MaxAnisotropy));
	}

	// enable antialiasing
	m_pDevD3D->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
	m_pDevD3D->SetRenderState (D3DRS_ANTIALIASEDLINEENABLE, TRUE);


	SAFE_RESTORE(	m_pGmPhs[m_ePhCur]	);

	LcLua::LscMdl::ContainerRestore();
	LcLua::LsgApp::ContainerRestore();

	::LcD3D_EffectFileResetDevice();

	return 0;
}


void CMain::Invalidate()
{
	SAFE_INVALID(	m_pGmPhs[m_ePhCur]	);

	LcLua::LscMdl::ContainerInvalidate();
	LcLua::LsgApp::ContainerInvalidate();

	::LcD3D_EffectFileLostDevice();
}





void CMain::SetPhase(INT nPhase)
{
	if(m_ePhOld == nPhase)														// if Phase changed... 
		return;

	m_ePhCur = nPhase;

	SAFE_DELETE(	m_pGmPhs[m_ePhOld]	);


	// Set Start Phase..
	CAppPhase* pPhase = new CAppPhase;
	pPhase->Create(&m_ePhCur, m_pDevSys);
	m_pGmPhs[m_ePhCur] = pPhase;
}




INT CMain::Create(void* p1, void* p2, void* p3, void* p4)
{
	LcDev_CreateWindowInstance("Direct 3D PC", &m_pDevSys);

	TCHAR		sLa[160]={0};

	HINSTANCE	hInst	= (HINSTANCE)p1;
	DWORD		dStyle	= WS_VISIBLE | WS_CAPTION | WS_SYSMENU;
	DWORD		dIcon	= IDI_ICON1;
	DWORD		dAccl	= IDR_MAIN_ACCEL;
	DWORD		dTggl	= IDM_TOGGLE;
	
	
	if(p2)
	{
		_tcscpy(sLa, (TCHAR*)p2);
	}
	else
	{
		_tcscpy(sLa, "_main.lua");
	}

	m_pLsc = new LcLua::CLcLsc;

	if(FAILED(m_pLsc->Create(sLa, m_pDevSys)))
	{
		delete m_pLsc;
		m_pLsc = NULL;

		memset(&m_SysInf, 0, sizeof( m_SysInf));
		_tcscpy(m_SysInf.sCls, "LcEngineApp1");

		m_SysInf.hInst	= hInst;

		m_SysInf.PosX	= 200;
		m_SysInf.PosY	= 100;
		m_SysInf.ScnY	= 640;
		m_SysInf.ScnX	= 480;

		m_pDevSys->Query("Set Class Name"		, m_SysInf.sCls);
		m_pDevSys->Query("Set Screen Width"		, &m_SysInf.ScnX);
		m_pDevSys->Query("Set Screen Height"	, &m_SysInf.ScnY);

		m_pDevSys->Query("Set Screen Pos X"		, &m_SysInf.PosX);
		m_pDevSys->Query("Set Screen Pos Y"		, &m_SysInf.PosY);
	}

	
	m_pDevSys->Query("Set Style Code"		, &dStyle	);
	m_pDevSys->Query("Set Icon Code"		, &dIcon	);
	m_pDevSys->Query("Set Accelator Code"	, &dAccl	);
	m_pDevSys->Query("Set Toggle Code"		, &dTggl	);

	// 
	m_pDevSys->Query("Set FuncMsgProc"		, pFuncWndProc		);
	m_pDevSys->Query("Set FuncInit"			, pFuncInit			);
	m_pDevSys->Query("Set FuncDestoy"		, pFuncDestroy		);
	m_pDevSys->Query("Set FuncRestore"		, pFuncRestore		);
	m_pDevSys->Query("Set FuncInvalidate"	, pFuncInvalidate	);
	m_pDevSys->Query("Set FuncFrameMove"	, pFuncFrameMove	);
	m_pDevSys->Query("Set FuncRender"		, pFuncRender		);

		
	if(FAILED(LcDev_CreateDevice("Direct 3D PC", &m_pDevSys, NULL)))
		return -1;

	SAFE_DELETE(	m_pLsc	);

	return 0;
}


INT CMain::Run()
{
	m_pDevSys->Run();

	SAFE_DELETE(	m_pDevSys	);

	return 0;
}




HINSTANCE CMain::GetAppInst()
{
	return m_hInst;
}


HWND CMain::GetAppHwnd()
{
	return m_hWnd;
}


PDEV CMain::GetAppDxDev()
{
	return m_pDevD3D;
}

PDSF CMain::GetAppDxBackBuffer()
{
	return m_pBck;
}



INT CMain::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(m_pInput)
		m_pInput->MsgProc(hWnd, uMsg, wParam, lParam);


	ILcNet*	pNet	= (ILcNet*)LcDev_GetUserNetwork(&m_pDevSys);

	if(pNet)
	{
		DWORD	hr = pNet->MsgProc(hWnd, uMsg, wParam, lParam);

		if(hr & 0xFF000000)
		{
			hr &=  0x00FFFFFF;

			switch(hr)
			{
			case WSAETIMEDOUT:
			case WSAECONNREFUSED:
//				MessageBox(hWnd, "Connection Error", "Err", MB_ICONERROR);
//				delete pNet;
//				LcDev_SetUserNetwork(&m_pDevSys, NULL);
				break;
			}

			
		}
	}

	if(m_pIme)
		m_pIme->MsgProc(hWnd, uMsg, wParam, lParam);


	switch( uMsg )
    {
		case WM_CLOSE:
//			MessageBox(hWnd, "Close", "Title", 0);
			return FALSE;
	}

	return FALSE;
}
