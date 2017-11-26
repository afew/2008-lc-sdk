// Implementation of the ILcDev class.
//
////////////////////////////////////////////////////////////////////////////////

#if _MSC_VER >= 1200
#pragma warning(disable : 4996)
#endif

#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <string.h>
#include <tchar.h>

#include <d3d9.h>
#include <d3dx9.h>

#include <Lc/ILcTex.h>

#include "ILcDev.h"
#include "LcDevBase.h"



#include <windows.h>
#include <windowsx.h>

#include <tchar.h>
#include <stdio.h>

#include <D3D9.h>
#include <d3dx9.h>
#include <dxerr9.h>

#include "DXUtil.h"
#include "D3DEnum.h"

#include "LcDevD3D.h"
#include "LcDevSprite.h"



LcDev::LcDev()
{
	PosX	= 0;
	PosY	= 0;
	ScnX	= 0;
	ScnY	= 0;

	hInst	= NULL;
	hWnd	= NULL;
	memset(sCls, 0, sizeof sCls);
}


LcDev::LcDev(INT _PosX, INT _PosY, INT _Wx, INT _Wy, const TCHAR* _sCls, HINSTANCE _hInst, HWND _hWnd)
{
	PosX	= _PosX;
	PosY	= _PosY;
	ScnX	= _Wx;
	ScnY	= _Wy;

	hInst	= _hInst;
	hWnd	= _hWnd;
	memset(sCls, 0, sizeof sCls);

	if(_sCls && _tcslen(_sCls))
		_tcscpy(sCls, _sCls);
}




INT		LcDev_CreateWindowInstance(char* sCmd
					, ILcDev** pData
					, void* p1
					, void* p2
					, void* p3
					, void* p4
					)
{
	(*pData) = NULL;


	InitCommonControls();


	if(0==_stricmp("Direct 3D PC", sCmd))
	{
		CLcDevD3D* pObj = NULL;

		pObj = new CLcDevD3D;

		(*pData) = pObj;
		
		return 0;
	}
	

	return -1;
}




INT LcDev_CreateDevice(char* sCmd
					, ILcDev** pData
					, void* p1
					, void* p2
					, void* p3
					, void* p4
					)
{
	if(NULL == (*pData))
		return -1;

	if(0==_stricmp("Direct 3D PC", sCmd))
	{
		CLcDevD3D* pObj = (CLcDevD3D*)(*pData);

		if(FAILED(pObj->Create(p1, p2, p3, p4)))
			return -1;

		return 0;
	}

	return -1;
}




void*	LcDev_GetHinst(ILcDev** pData)
{
	if(!pData)
		return NULL;

	CLcDevD3D* pDev = (CLcDevD3D*)(*pData);
	return pDev->GetHinst();
}


void* LcDev_GetHwnd(ILcDev** pData)
{
	if(!pData)
		return NULL;

	CLcDevD3D* pDev = (CLcDevD3D*)(*pData);
	return pDev->GetHwnd();
}


void* LcDev_GetD3Device(ILcDev** pData)
{
	if(!pData)
		return NULL;


	CLcDevD3D* pDev = (CLcDevD3D*)(*pData);
	
	return pDev->GetDevice();
}


void* LcDev_GetBackBufferSurface(ILcDev** pData)
{
	if(!pData)
		return NULL;


	CLcDevD3D* pDev = (CLcDevD3D*)(*pData);
	
	return pDev->GetBufferSurface();
}



INT	LcDev_CreateSprite(char* sCmd	
					, ILcDevSprite** pData	
					, void* p1			// Device	
					, void* p2			// No Use
					, void* p3			// No Use
					, void* p4			// No Use
					)
{
	(*pData) = NULL;

	if(NULL == sCmd)
		return -1;

	if(0==_stricmp("PC Sprite", sCmd))
	{
		CLcDevSprite* pObj = NULL;

		pObj = new CLcDevSprite;

		if(FAILED(pObj->Create(p1, p2, p3, p4)))
		{
			// Return Error Notification
			delete pObj;
			return -1;
		}

		(*pData) = pObj;
		
		return 0;
	}

	return -1;
}


void*	LcDev_GetUserSprite	(ILcDev** ppDev)			{	if(!ppDev)	return 0;	CLcDevD3D* pDev = (CLcDevD3D*)(*ppDev);	return pDev->GetSprite()	;	}
void*	LcDev_GetUserInput	(ILcDev** ppDev)			{	if(!ppDev)	return 0;	CLcDevD3D* pDev = (CLcDevD3D*)(*ppDev);	return pDev->GetInput()		;	}
void*	LcDev_GetUserCamera	(ILcDev** ppDev)			{	if(!ppDev)	return 0;	CLcDevD3D* pDev = (CLcDevD3D*)(*ppDev);	return pDev->GetCamera()	;	}
void*	LcDev_GetUserNetwork(ILcDev** ppDev)			{	if(!ppDev)	return 0;	CLcDevD3D* pDev = (CLcDevD3D*)(*ppDev);	return pDev->GetNetwork()	;	}
void*	LcDev_GetUserIme	(ILcDev** ppDev)			{	if(!ppDev)	return 0;	CLcDevD3D* pDev = (CLcDevD3D*)(*ppDev);	return pDev->GetIme()		;	}
void*	LcDev_GetUserCommand(ILcDev** ppDev)			{	if(!ppDev)	return 0;	CLcDevD3D* pDev = (CLcDevD3D*)(*ppDev);	return pDev->GetCommand()	;	}

void*	LcDev_GetUserData1	(ILcDev** ppDev)			{	if(!ppDev)	return 0;	CLcDevD3D* pDev = (CLcDevD3D*)(*ppDev);	return pDev->GetUserData1()	;	}
void*	LcDev_GetUserData2	(ILcDev** ppDev)			{	if(!ppDev)	return 0;	CLcDevD3D* pDev = (CLcDevD3D*)(*ppDev);	return pDev->GetUserData2()	;	}
void*	LcDev_GetUserData3	(ILcDev** ppDev)			{	if(!ppDev)	return 0;	CLcDevD3D* pDev = (CLcDevD3D*)(*ppDev);	return pDev->GetUserData3()	;	}
void*	LcDev_GetUserData4	(ILcDev** ppDev)			{	if(!ppDev)	return 0;	CLcDevD3D* pDev = (CLcDevD3D*)(*ppDev);	return pDev->GetUserData4()	;	}

INT		LcDev_SetUserSprite	(ILcDev** ppDev, void* p)	{	if(!ppDev)	return -1;	CLcDevD3D* pDev = (CLcDevD3D*)(*ppDev);	return pDev->SetSprite(p)	;	}
INT		LcDev_SetUserInput	(ILcDev** ppDev, void* p)	{	if(!ppDev)	return -1;	CLcDevD3D* pDev = (CLcDevD3D*)(*ppDev);	return pDev->SetInput(p)	;	}
INT		LcDev_SetUserCamera	(ILcDev** ppDev, void* p)	{	if(!ppDev)	return -1;	CLcDevD3D* pDev = (CLcDevD3D*)(*ppDev);	return pDev->SetCamera(p)	;	}
INT		LcDev_SetUserNetwork(ILcDev** ppDev, void* p)	{	if(!ppDev)	return -1;	CLcDevD3D* pDev = (CLcDevD3D*)(*ppDev);	return pDev->SetNetwork(p)	;	}
INT		LcDev_SetUserIme	(ILcDev** ppDev, void* p)	{	if(!ppDev)	return -1;	CLcDevD3D* pDev = (CLcDevD3D*)(*ppDev);	return pDev->SetIme(p)		;	}
INT		LcDev_SetUserCommand(ILcDev** ppDev, void* p)	{	if(!ppDev)	return -1;	CLcDevD3D* pDev = (CLcDevD3D*)(*ppDev);	return pDev->SetCommand(p)	;	}

INT		LcDev_SetUserData1	(ILcDev** ppDev, void* p)	{	if(!ppDev)	return -1;	CLcDevD3D* pDev = (CLcDevD3D*)(*ppDev);	return pDev->SetUserData1(p);	}
INT		LcDev_SetUserData2	(ILcDev** ppDev, void* p)	{	if(!ppDev)	return -1;	CLcDevD3D* pDev = (CLcDevD3D*)(*ppDev);	return pDev->SetUserData2(p);	}
INT		LcDev_SetUserData3	(ILcDev** ppDev, void* p)	{	if(!ppDev)	return -1;	CLcDevD3D* pDev = (CLcDevD3D*)(*ppDev);	return pDev->SetUserData3(p);	}
INT		LcDev_SetUserData4	(ILcDev** ppDev, void* p)	{	if(!ppDev)	return -1;	CLcDevD3D* pDev = (CLcDevD3D*)(*ppDev);	return pDev->SetUserData4(p);	}


