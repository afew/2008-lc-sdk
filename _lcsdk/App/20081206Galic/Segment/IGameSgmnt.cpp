// Implementation of the IIGameSgmnt class.
//
////////////////////////////////////////////////////////////////////////////////


#include <windows.h>
#include <stdio.h>

#include <d3d9.h>
#include <d3dx9.h>

#include <Lc/ILcDev.h>
#include <Lc/ILcInput.h>
#include <Lc/ILcM3d.h>

#include <Lc/ILcmMdl.h>
#include <Lc/ILcxTbl.h>
#include <Lc/ILcxCam.h>
#include <Lc/ILcxmFld.h>
#include <Lc/ILcxmT.h>

#include <Lc/LcType.h>

#include "IGameSgmnt.h"
#include "GameSgmnt.h"



INT LcxApp_CreateGameSegment(char* sCmd
				 , IGameSgmnt** pData
				 , void* p1				// CMain Instance Pointer
				 , void* p2				// No Use
				 , void* p3				// No Use
				 , void* p4				// No Use
				 )
{
	(*pData) = NULL;

	CGameSgmnt*	pObj = NULL;
	pObj = new CGameSgmnt;

	if(FAILED(pObj->Create(p1, p2, p3, p4)))
	{
		// Return Error Notification
		delete pObj;
		return -1;
	}

	(*pData) = pObj;

	return 0;
}
