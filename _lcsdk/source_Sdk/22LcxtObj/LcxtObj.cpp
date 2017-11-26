// Implementation of the CLcxtObj classes.
//
////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <stdio.h>
#include <string.h>

#include <d3d9.h>
#include <d3dx9.h>

#include "ILcxtObj.h"
#include "LcxtObj.h"


CLcxtObj::CLcxtObj()
{
	m_pDev	= NULL;
	memset(m_sType, 0, sizeof(m_sType));
}


CLcxtObj::~CLcxtObj()
{
}


INT	CLcxtObj::Query(char* sCmd, void* pData)
{
	if(0==_stricmp(sCmd, "Get Type"))
	{
		strcpy((char*)pData, m_sType);
		return 0;
	}

	return -1;
}