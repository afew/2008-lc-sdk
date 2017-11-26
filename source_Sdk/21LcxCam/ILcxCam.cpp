// Implementation of the ILcxCam class.
//
////////////////////////////////////////////////////////////////////////////////


#include <windows.h>
#include <stdio.h>
#include <d3dx9.h>

#include "ILcxCam.h"
#include "LcxCam.h"
#include "LcxCamA.h"


INT LcxCam_Create(char* sCmd
				, ILcxCam** pData
				, void* p1		// Camera name
				, void* p2		// Camera Initial Value
				, void* p3		// LPDIRECT3DDEVICE9
				, void* p4		// No Use
				, void* p5		// No Use
				, void* p6		// No Use
				)
{
	
	(*pData) = NULL;

	CLcxCamA* pObj = new CLcxCamA;

	if(FAILED(pObj->Create(p1, p2, p3, p4)))
	{
		delete pObj;
		return -1;
	}

	(*pData) = pObj;

	return 0;
}