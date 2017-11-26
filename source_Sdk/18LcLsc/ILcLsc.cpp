// Implementation of the ILcLsc class.
//
////////////////////////////////////////////////////////////////////////////////


#include <windows.h>

#include "ILcLsc.h"																// Lua App Class


ILcLsc::LscH::LscH()
{
	nId		= 0xFFFFFFFF;
	pIns	= NULL;
	pMsc	= NULL;

	memset(sName, 0, sizeof sName);
}

