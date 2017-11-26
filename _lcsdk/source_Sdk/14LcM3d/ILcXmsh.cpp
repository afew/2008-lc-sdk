// Implementation of the ILcXmsh class.
//
////////////////////////////////////////////////////////////////////////////////


#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include <d3d9.h>
#include <d3dx9.h>

#include "ILcXmsh.h"

#include "LcXmsh.h"



INT LcXmsh_Create(char* sCmd
				 , ILcXmsh** pData
				 , void* p1			// LPDIRECT3DDEVICE9
				 , void* p2			// X-file Name
				 , void* p3			// No Use
				 , void* p4			// No Use
				 )
{

	(*pData) = NULL;

	if(0==_stricmp("PC Mesh", sCmd))
	{
		CLcXmsh* pObj = new CLcXmsh;

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