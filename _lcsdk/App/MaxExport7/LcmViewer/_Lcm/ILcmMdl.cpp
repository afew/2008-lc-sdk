// Implementation of the ILcmMdl class.
//
////////////////////////////////////////////////////////////////////////////////

#if _MSC_VER >= 1200
#pragma warning(disable : 4996)
#endif

#include <stdio.h>
#include <windows.h>

#include <d3d9.h>
#include <d3dx9.h>

#include "ILcmMdl.h"
#include "LcmMdl.h"
#include "LcmAcm.h"


namespace Lcm
{

_Tframe::_Tframe()
: nB(0)
, nE(0)
{
	memset(sN, 0, sizeof sN);
}


_Tframe::_Tframe(char* s, INT B, INT E)
: nB(B)
, nE(E)
{
	memset(sN, 0, sizeof sN);
	strcpy(sN, s);
}



INT Mdl_Create(char* sCmd
				 , ILcmMdl** pData
				 , void* p1			// Original Source: Original Source가 NULL이면 새로 만듦
				 , void* p2			// Model File Name
				 , void* p3			// No Use
				 , void* p4			// No Use
				 )
{

	(*pData) = NULL;

	if(0==_stricmp("Acm", sCmd))
	{
		CLcmAcm*	pObj = NULL;
		pObj = new CLcmAcm;

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

}// namespace Lcm
