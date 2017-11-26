// Implementation of the ILcxMdl class.
//
////////////////////////////////////////////////////////////////////////////////


#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#include <d3d9.h>
#include <d3dx9.h>

#include "ILcxTbl.h"
#include "TblTex.h"
#include "TblMdl.h"


INT LcxTbl_Create(char* sCmd
				 , ILcxTbl** pData
				 , void* p1			// File Name
				 , void* p2			// LPDIRECT3DDEVICE9
				 , void* p3			// Etc
				 , void* p4			// No Use
				 )
{
	(*pData) = NULL;

	if(0==_stricmp("Texture", sCmd))
	{
		CTblTex* pObj = NULL;

		pObj = new CTblTex;

		if(FAILED(pObj->Create(p1, p2, p3, p4)))
		{
			// Return Error Notification
			delete pObj;
			return -1;
		}

		(*pData) = pObj;
		
		return 0;
	}

	else if(0==_stricmp("Model", sCmd))
	{
		CTblMdl* pObj = NULL;

		pObj = new CTblMdl;

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



