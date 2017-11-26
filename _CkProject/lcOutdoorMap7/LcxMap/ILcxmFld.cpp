// Implementation of the ILcxmFld class.
//
////////////////////////////////////////////////////////////////////////////////


#include <algorithm>

#include <windows.h>
#include <d3d9.h>
#include <d3dX9.h>

#include <Lc/LcType.h>
#include <Lc/LcUtil.h>

#include <Lc/ILcxCam.h>

#include <Lc/ILcmMdl.h>
#include <Lc/ILcmMdlX.h>

#include <Lc/ILcxTbl.h>


#include "ILcxmFld.h"
#include "ILcxmFldX.h"
#include "LcxmBlc.h"
#include "LcxmDst.h"


INT LcMap_CreateOutdoor(char* sCmd
				, ILcxmFld** pData
				, void* p1			// Device
				, void* p2			// Texture Table
				, void* p3			// Model Table
				, void* p4			// Map File Name
				, void* p5			// No Use
				, void* p6			// No Use
				, void* p7			// No Use
				)
{
	CLcxmDst*	pObj	= NULL;

	*pData = NULL;

	pObj = new CLcxmDst;

	if(pObj->Create(p1, p2, p3, p4))
	{
		delete pObj;
		return -1;
	}

	*pData = pObj;
	return 0;
}

