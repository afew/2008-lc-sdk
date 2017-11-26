// Implementation of the ILcVtx class.
//
////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <stdio.h>
#include <string.h>

#include <d3d9.h>
#include <d3dx9.h>

#include "ILcxtObj.h"
#include "LcxtObj.h"

#include "LcxBillboard.h"
#include "LcxGrid.h"


////////////////////////////////////////////////////////////////////////////////

INT	LcxtObj_Create(char* sCmd				// Command
					, ILcxtObj** pData		//
					, void* p1				// Need D3D Device
					, void* p2				// Billboard Count or Etc...
					, void* p3				//
					, void* p4				//
					)

{
	(*pData)= NULL;

	if(0==_stricmp("Billboard", sCmd))
	{
		CLcxBillboard* pObj	= NULL;
		pObj	= new CLcxBillboard;

		if(FAILED(pObj->Create(p1, p2, p3, p4)))
		{
			delete pObj;
			return -1;
		}

		(*pData) = pObj;
		return 0;
	}

	if(0==_stricmp("XYZ Grid", sCmd))
	{
		CLcxGrid* pObj = NULL;
		pObj = new CLcxGrid;

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

