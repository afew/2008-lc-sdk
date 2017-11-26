// Implementation of the CLcmMdl class.
//
////////////////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <windows.h>

#include "ILcmMdl.h"
#include "LcmMdl.h"


namespace Lcm
{

CLcmMdl::CLcmMdl()				{				}
CLcmMdl::~CLcmMdl()				{				}

INT	CLcmMdl::Create(void*, void*, void*, void*)	{	return -1;	}

void CLcmMdl::Destroy()			{				}
INT  CLcmMdl::FrameMove()		{	return -1;	}
void CLcmMdl::Render()			{				}

INT  CLcmMdl::Query(char*,void*){	return -1;	}

}// namespace Lcm


