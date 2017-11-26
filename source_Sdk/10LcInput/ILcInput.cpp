// Implementation of the ILcInputBase class.
//
////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <stdio.h>

#include <d3d9.h>
#include <d3dx9.h>


#define DIRECTINPUT_VERSION		0x0800
#include <dinput.h>


#include "ILcInput.h"
#include "LcInput.h"
#include "LcInputA.h"
#include "LcInputD.h"


INT LcInput_Create(char* sCmd
				 , ILcInput** pData
				 , void* p1			// HINSTANCE
				 , void* p2			// HWND
				 , void* p3			// LPDIRECT3DDEVICE9
				 , void* p4			// No Use
				 )
{
	(*pData) = NULL;

	if(0==_stricmp("API Input", sCmd))
	{
//		HINSTANCE	hInst= (HINSTANCE)p1;
//		HWND		hWnd = (HWND)p2;
//		
		CLcInputA*	pObj = NULL;
		pObj = new CLcInputA;

		if(FAILED(pObj->Create(p1, p2, p3, p4)))
		{
			// Return Error Notification
			delete pObj;
			return -1;
		}

		(*pData) = pObj;
		
		return 0;
	}

	if(0==_stricmp("Direct Input", sCmd))
	{
//		HINSTANCE			hInst= (HINSTANCE)p1;
//		HWND				hWnd = (HWND)p2;
//		LPDIRECT3DEVICE9	pDev = (LPDIRECT3DEVICE9)p3 ;

		CLcInputD*	pObj = NULL;

		pObj = new CLcInputD;

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
