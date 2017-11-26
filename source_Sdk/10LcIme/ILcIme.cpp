// Implementation of the ILcImeBase class.
//
////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <stdio.h>

#include "ILcIme.h"
#include "LcHangul.h"


INT LcIme_Create(char* sCmd
				 , ILcIme** pData
				 , void* p1			// String Table Back space Id
				 , void* p2			// No Use
				 , void* p3			// No Use
				 , void* p4			// No Use
				 )
{
	(*pData) = NULL;

	if(0==_stricmp("Hangul", sCmd))
	{
		CLcHangul*	pObj = NULL;

		pObj = new CLcHangul;

		if(FAILED(pObj->Create(p1, p2, p3, p4)))
		{
			delete pObj;
			return -1;
		}

		(*pData) = pObj;
		
		return 0;
	}

	return -1;
}
