// Implementation of the CLcVtx classes.
//
////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <stdio.h>
#include <string.h>

#include <d3d9.h>
#include <d3dx9.h>

#include "ILcM3d.h"
#include "LcIdx.h"


CLcIdx::CLcIdx()
{
	m_pDev	= NULL;

	m_pIB	= NULL;
	m_dFMT	= 0;
}


CLcIdx::~CLcIdx()
{
	CLcIdx::Destroy();
}

void CLcIdx::Destroy()
{
	if(m_pIB)
	{
		free(m_pIB);
		m_pIB = NULL;
	}
}


INT CLcIdx::Create(void* p1, void* p2, void* p3, void* p4)
{
	return -1;
}


DWORD CLcIdx::GetFMT()
{
	return m_dFMT;
}


void* CLcIdx::GetIndex()
{
	return m_pIB;
}



INT CLcIdx::SetupDevice(void* p)
{
	m_pDev	= (LPDIRECT3DDEVICE9)p;

	return 0;
}




