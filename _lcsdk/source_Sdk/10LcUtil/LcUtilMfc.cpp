// Implementation of the LcUtil MFC Files.
//
////////////////////////////////////////////////////////////////////////////////


#include <direct.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "LcUtilMfc.h"


void GetProjectFullName(char* sName, IBuildProject* pProject)
{
	CComBSTR bszStr;
	
	pProject->get_FullName(&bszStr);
	CString projectName = bszStr;
	strcpy(sName, projectName);
}

void GetProjectName(char* sName, IBuildProject* pProject)
{
	CComBSTR bszStr;

	pProject->get_Name(&bszStr);
	CString projectName = bszStr;
	strcpy(sName, projectName);
}