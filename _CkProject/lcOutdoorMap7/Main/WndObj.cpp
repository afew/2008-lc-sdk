// Implementation of the CWndObj class.
//
////////////////////////////////////////////////////////////////////////////////


#include "../_StdAfx.h"


CLSS_DLG_DEFINE(CWndObj, MsgPrc);


INT CWndObj::Init()
{
	m_hWnd	= 0;
	m_hWPrn	= 0;

	m_pObj	= NULL;

	memset( &m_vcR, 0, sizeof m_vcR);
	memset( &m_vcS, 0, sizeof m_vcS);
	memset( &m_vcT, 0, sizeof m_vcT);

	return 0;
}


INT CWndObj::Create(HWND hWnd)
{
	if(m_hWnd)
		return 0;

	if(!GFIELD)
		return 0;

	if(GFIELD)
		GFIELD->Query("Get Pick Object", &m_pObj);


	if(!m_pObj)
		return -1;


	Lcm::ILcmMdl*	pObj = (Lcm::ILcmMdl*)m_pObj;
	Lcm::ILcmMesh*	pSrc = NULL;
	
	pObj->Query("Get Mesh Source", &pSrc);


	m_hWPrn= hWnd;
	HINSTANCE hInst = GetModuleHandle(NULL);
	m_hWnd = CreateDialog(hInst,MAKEINTRESOURCE(IDD_OBJ), m_hWPrn, CLSS_DLG_WNDPROC(CWndObj));

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

	iX = rt1.left + 5;
	iY = rt1.top  + 45;
	
	MoveWindow(m_hWnd, iX, iY, iWidth, iHeight, TRUE);
	::ShowWindow(m_hWnd, SW_SHOW);


	char sBuf[32]={0};
	sprintf(sBuf, "0x%x" , pObj);

	SetDlgItemText(m_hWnd, IDC_OBJ_ID, sBuf);

	Lcm::TmshInf*	pGeo = (Lcm::TmshInf*)pSrc->GetGeometry();
	SetDlgItemText(m_hWnd, IDC_OBJ_NAME, pGeo->GetMdlName());

	pObj->GetRotation((FLOAT*)&m_vcR);
	pObj->GetScaling((FLOAT*)&m_vcS);
	pObj->GetPosition((FLOAT*)&m_vcT);

	SetDlgItemFlt(m_hWnd, IDC_OBJ_R_X, m_vcR.x);
	SetDlgItemFlt(m_hWnd, IDC_OBJ_R_Y, m_vcR.y);
	SetDlgItemFlt(m_hWnd, IDC_OBJ_R_Z, m_vcR.z);

	SetDlgItemFlt(m_hWnd, IDC_OBJ_S_X, m_vcS.x);
	SetDlgItemFlt(m_hWnd, IDC_OBJ_S_Y, m_vcS.y);
	SetDlgItemFlt(m_hWnd, IDC_OBJ_S_Z, m_vcS.z);

	SetDlgItemFlt(m_hWnd, IDC_OBJ_T_X, m_vcT.x);
	SetDlgItemFlt(m_hWnd, IDC_OBJ_T_Y, m_vcT.y);
	SetDlgItemFlt(m_hWnd, IDC_OBJ_T_Z, m_vcT.z);

	SetFocus(NULL);

	return 0;
}



void CWndObj::ShowWindow(int _ishw)
{
	::ShowWindow(m_hWnd, _ishw);
}



void CWndObj::Destroy()
{
	SAFE_DESTROY_WIN(m_hWnd);
}



LRESULT CWndObj::MsgPrc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WPARAM		wparHi = HIWORD(wParam);
	WPARAM		wparLo = LOWORD(wParam);

	switch( uMsg )
	{
		case WM_PAINT:
		{
			break;
		}

		case WM_COMMAND:
		{
			switch(wparLo)
			{
				case IDC_OBJ_OK:
				{
					m_vcR.x = GetDlgItemFlt(m_hWnd, IDC_OBJ_R_X);
					m_vcR.y = GetDlgItemFlt(m_hWnd, IDC_OBJ_R_Y);
					m_vcR.z = GetDlgItemFlt(m_hWnd, IDC_OBJ_R_Z);

					m_vcS.x = GetDlgItemFlt(m_hWnd, IDC_OBJ_S_X);
					m_vcS.y = GetDlgItemFlt(m_hWnd, IDC_OBJ_S_Y);
					m_vcS.z = GetDlgItemFlt(m_hWnd, IDC_OBJ_S_Z);

					m_vcT.x = GetDlgItemFlt(m_hWnd, IDC_OBJ_T_X);
					m_vcT.y = GetDlgItemFlt(m_hWnd, IDC_OBJ_T_Y);
					m_vcT.z = GetDlgItemFlt(m_hWnd, IDC_OBJ_T_Z);

					SetVal();

					break;
				}

				case IDC_OBJ_CLOSE:
				{
					m_pObj = NULL;
					SendMessage(hWnd, WM_CLOSE, 0, 0);
					break;
				}

				case IDC_OBJ_R_X:{	switch(wparHi)	{	case EN_CHANGE:	{	m_vcR.x = GetDlgItemFlt(hWnd, IDC_OBJ_R_X);	break;	}}	break;	}
				case IDC_OBJ_R_Y:{	switch(wparHi)	{	case EN_CHANGE:	{	m_vcR.y = GetDlgItemFlt(hWnd, IDC_OBJ_R_Y);	break;	}}	break;	}
				case IDC_OBJ_R_Z:{	switch(wparHi)	{	case EN_CHANGE:	{	m_vcR.z = GetDlgItemFlt(hWnd, IDC_OBJ_R_Z);	break;	}}	break;	}

				case IDC_OBJ_S_X:{	switch(wparHi)	{	case EN_CHANGE:	{	m_vcS.x = GetDlgItemFlt(hWnd, IDC_OBJ_S_X);	break;	}}	break;	}
				case IDC_OBJ_S_Y:{	switch(wparHi)	{	case EN_CHANGE:	{	m_vcS.y = GetDlgItemFlt(hWnd, IDC_OBJ_S_Y);	break;	}}	break;	}
				case IDC_OBJ_S_Z:{	switch(wparHi)	{	case EN_CHANGE:	{	m_vcS.z = GetDlgItemFlt(hWnd, IDC_OBJ_S_Z);	break;	}}	break;	}

				case IDC_OBJ_T_X:{	switch(wparHi)	{	case EN_CHANGE:	{	m_vcT.x = GetDlgItemFlt(hWnd, IDC_OBJ_T_X);	break;	}}	break;	}
				case IDC_OBJ_T_Y:{	switch(wparHi)	{	case EN_CHANGE:	{	m_vcT.y = GetDlgItemFlt(hWnd, IDC_OBJ_T_Y);	break;	}}	break;	}
				case IDC_OBJ_T_Z:{	switch(wparHi)	{	case EN_CHANGE:	{	m_vcT.z = GetDlgItemFlt(hWnd, IDC_OBJ_T_Z);	break;	}}	break;	}

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



void CWndObj::SetObj(void* pMdB)
{
	Lcm::ILcmMdl*	pObj = NULL;
	Lcm::ILcmMesh*	pSrc = NULL;

	m_pObj = pMdB;

	if(m_pObj)
	{
		pObj = (Lcm::ILcmMdl*)m_pObj;
		pObj->Query("Get Mesh Source", &pSrc);
	}

	SetDlgItemText(m_hWnd, IDC_OBJ_ID, "");
	SetDlgItemText(m_hWnd, IDC_OBJ_NAME, "");


	SetDlgItemFlt(m_hWnd, IDC_OBJ_R_X, 0.0F);
	SetDlgItemFlt(m_hWnd, IDC_OBJ_R_Y, 0.0F);
	SetDlgItemFlt(m_hWnd, IDC_OBJ_R_Z, 0.0F);

	SetDlgItemFlt(m_hWnd, IDC_OBJ_S_X, 0.0F);
	SetDlgItemFlt(m_hWnd, IDC_OBJ_S_Y, 0.0F);
	SetDlgItemFlt(m_hWnd, IDC_OBJ_S_Z, 0.0F);

	SetDlgItemFlt(m_hWnd, IDC_OBJ_T_X, 0.0F);
	SetDlgItemFlt(m_hWnd, IDC_OBJ_T_Y, 0.0F);
	SetDlgItemFlt(m_hWnd, IDC_OBJ_T_Z, 0.0F);


	if(pObj)
	{
		char sBuf[32]={0};
		sprintf(sBuf, "0x%x" , pObj);

		SetDlgItemText(m_hWnd, IDC_OBJ_ID, sBuf);

		Lcm::TmshInf*	pGeo = (Lcm::TmshInf*)pSrc->GetGeometry();
		SetDlgItemText(m_hWnd, IDC_OBJ_NAME, pGeo->GetMdlName());

		VEC3	vcR;
		VEC3	vcS;
		VEC3	vcT;

		pObj->GetRotation((FLOAT*)&vcR);
		pObj->GetScaling((FLOAT*)&vcS);
		pObj->GetPosition((FLOAT*)&vcT);

		SetDlgItemFlt(m_hWnd, IDC_OBJ_R_X, vcR.x);
		SetDlgItemFlt(m_hWnd, IDC_OBJ_R_Y, vcR.y);
		SetDlgItemFlt(m_hWnd, IDC_OBJ_R_Z, vcR.z);

		SetDlgItemFlt(m_hWnd, IDC_OBJ_S_X, vcS.x);
		SetDlgItemFlt(m_hWnd, IDC_OBJ_S_Y, vcS.y);
		SetDlgItemFlt(m_hWnd, IDC_OBJ_S_Z, vcS.z);

		SetDlgItemFlt(m_hWnd, IDC_OBJ_T_X, vcT.x);
		SetDlgItemFlt(m_hWnd, IDC_OBJ_T_Y, vcT.y);
		SetDlgItemFlt(m_hWnd, IDC_OBJ_T_Z, vcT.z);
	}
}




void CWndObj::SetVal()
{
	Lcm::ILcmMdl* pObj = (Lcm::ILcmMdl*)m_pObj;

	if(pObj)
	{
		pObj->SetRotation((FLOAT*)&m_vcR);
		pObj->SetScaling( m_vcS);
		pObj->SetPosition((FLOAT*)&m_vcT);
	}
}