// Implementation of the IGameData class.
//
////////////////////////////////////////////////////////////////////////////////


#include <windows.h>

#include <d3d9.h>
#include <d3dx9.h>

#include <Lc/ILcDev.h>

#include "../OcFSM/ILopAi.h"
#include "../OcFSM/ocAmun.h"

#include "IGameData.h"
#include "GameData.h"



INT LcxApp_CreateGameData(char* sCmd
				 , IGameData** pData
				 , void* p1				// CMain Instance Pointer
				 , void* p2				// No Use
				 , void* p3				// No Use
				 , void* p4				// No Use
				 )
{
	(*pData) = NULL;

	CGameData*	pObj = NULL;
	pObj = new CGameData;

	if(FAILED(pObj->Create(p1, p2, p3, p4)))
	{
		// Return Error Notification
		delete pObj;
		return -1;
	}

	(*pData) = pObj;
		
	return 0;
}
