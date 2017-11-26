// Implementation of the CLcxXfile class.
//
////////////////////////////////////////////////////////////////////////////////


#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <d3d9.h>
#include <d3dx9.h>

#include <Lc/LcType.h>
#include <Lc/LcUtil.h>

#include "ILcxDXFile.h"
#include "LcxDXFile.h"

#include "XmshAlloc.h"
#include "XmshAniS.h"
#include "XmshInst.h"



INT LcxDXFile_Create(char* sCmd, ILcxDXFile** pData, void* p1, void* p2, void* p3, void* p4)
{
	(*pData) = NULL;

	if(0==_stricmp("Origin", sCmd))
	{
		CXmshAniS* pMdB = NULL;

		pMdB = new CXmshAniS;


		if(FAILED(pMdB->Create(p1, p2, p3, p4)))
		{
			// Return Error Notification
			delete pMdB;
			return -1;
		}

		(*pData) = pMdB;

		return 0;
	}

	else if(0==_stricmp("Instance", sCmd))
	{
		CXmshInst* pMdB = NULL;

		pMdB = new CXmshInst;


		if(FAILED(pMdB->Create(p1, p2, p3, p4)))
		{
			// Return Error Notification
			delete pMdB;
			return -1;
		}

		(*pData) = pMdB;

		return 0;
	}

	return -1;
}




