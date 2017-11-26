// Implementation of the ILcTex class.
//
////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#include <Lc/LcType.h>

#include "ILcTex.h"
#include "LcTexBase.h"
#include "LcTexMng.h"


ILcTex::TxLayer::TxLayer()
{
	for(int i=0; i<ILcTex::MAX_TEXTURE_LAYER; ++i)
	{
		p[i] = NULL;
	}
}



INT LcTex_CreateTexMng(char* sCmd
					, ILcTexMng** pData	// Texture Manager Instance
					, void* p1			// Device
					, void* p2			// No Use
					, void* p3			// No Use
					, void* p4			// No Use
					)
{

//	PDEV		pd3dDev = (PDEV)p1;
	CLcTexMng*	pObj	= NULL;

	*pData = NULL;

	pObj = new CLcTexMng;

	if(pObj->Create(p1, p2, p3, p4))
	{
		delete pObj;
		return -1;
	}

	*pData = pObj;
	
	return 0;
}