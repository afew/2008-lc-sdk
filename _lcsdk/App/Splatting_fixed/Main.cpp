// Implementation of the CMain class.
//
////////////////////////////////////////////////////////////////////////////////


#include "_StdAfx.h"


CMain::CMain()
{
	strcpy(m_strClassName, "Mackerel");

	m_dwCreationWidth			= 1024;
	m_dwCreationHeight			=  768;

	m_dRscIcon		= IDI_MAIN_ICON;
	m_dRscMenu		= 0;
	m_dRscAccel		= IDR_MAIN_ACCEL;

	m_dRscDevice	= IDM_CHANGEDEVICE;
	m_dRscToggle	= IDM_TOGGLEFULLSCREEN;
	m_dRscExit		= IDM_EXIT;
	
	
	m_pD3DXFont		= NULL;
	m_pInput		= NULL;
	m_pCam			= NULL;

	m_pGrid			= NULL;
	m_pSplt			= NULL;
}


HRESULT CMain::Init()
{
	SAFE_NEWCREATE1(	m_pInput,	CLcInput,	m_hWnd	);
	SAFE_NEWCREATE1(	m_pCam,		CLcCam,		m_pd3dDevice);
	SAFE_NEWCREATE1(	m_pGrid,	CLcGrid,	m_pd3dDevice);

	SAFE_NEWCREATE1(	m_pSplt,	CLcSplt,	m_pd3dDevice);


	D3DXFONT_DESC hFont =
	{
		14, 0
		, FW_BOLD, 1, FALSE
		, HANGUL_CHARSET
		, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE
		, "±¼¸²Ã¼"
	};

	if( FAILED( D3DXCreateFontIndirect(m_pd3dDevice, &hFont, &m_pD3DXFont) ) )
		return -1;

	return S_OK;
}


HRESULT CMain::Destroy()
{
	SAFE_RELEASE( m_pD3DXFont	);
	SAFE_DELETE(	m_pInput	);
	SAFE_DELETE(	m_pCam		);
	SAFE_DELETE(	m_pGrid		);
	
	SAFE_DELETE(	m_pSplt		);

	return S_OK;
}


HRESULT CMain::Restore()
{
	m_pD3DXFont->OnResetDevice();

	SAFE_RESTORE(	m_pSplt	);

	return S_OK;
}



HRESULT CMain::Invalidate()
{
	m_pD3DXFont->OnLostDevice();

	SAFE_INVALIDATE(	m_pSplt	);
	
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
	}

	SAFE_FRAMEMOVE(	m_pCam		);


	return S_OK;
}




HRESULT CMain::Render()
{
	m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, 0xFF006699, 1.0f, 0L);
	
	if( FAILED( m_pd3dDevice->BeginScene() ) )
		return -1;


	SAFE_RENDER(	m_pGrid		);

	SAFE_RENDER(	m_pSplt		);


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

	VEC3 vcCamPos = m_pCam->GetEye();
	
	sprintf(szMsg, "Camera Pos: %.f %.f %.f",vcCamPos.x, vcCamPos.y, vcCamPos.z );
	m_pD3DXFont->DrawText(NULL, szMsg, -1, &rc, 0, D3DXCOLOR(1,1,0,1) );


	
	return S_OK;
}


LRESULT CMain::MsgProc( HWND hWnd, UINT msg, WPARAM wParam,LPARAM lParam)
{
	switch( msg )
	{
	case WM_PAINT:
		{
			if( m_bLoadingApp )
			{
				HDC hDC = GetDC( hWnd );
				TCHAR strMsg[MAX_PATH];
				RECT rc;
				wsprintf( strMsg, TEXT("Loading... Please wait") );
				GetClientRect( hWnd, &rc );
				DrawText( hDC, strMsg, -1, &rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE );
				ReleaseDC( hWnd, hDC );
			}
			break;
		}
		
	}
	
	return CD3DApplication::MsgProc( hWnd, msg, wParam, lParam );
}