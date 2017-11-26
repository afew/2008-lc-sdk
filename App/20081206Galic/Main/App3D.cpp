// Implementation of the CApp3D class.
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <stdio.h>

#include "App3D.h"


static CApp3D*	g_pApp;

INT		CApp3D::pFuncInit()			{	return g_pApp->Init();		}
void	CApp3D::pFuncDestroy()		{	g_pApp->Destroy();			}
INT		CApp3D::pFuncRestore()		{	return g_pApp->Restore();	}
void	CApp3D::pFuncInvalidate()	{	g_pApp->Invalidate();		}
INT		CApp3D::pFuncFrameMove()	{	return g_pApp->FrameMove();	}
void	CApp3D::pFuncRender()		{	g_pApp->Render();			}

INT		CApp3D::pFuncWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return g_pApp->MsgProc(hWnd, uMsg, wParam, lParam);
}


CApp3D::CApp3D()
{
	g_pApp = this;
}

CApp3D::~CApp3D()
{
}



