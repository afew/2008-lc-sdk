// Interface for the CLcFileSys class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcUtilMfc_H_
#define _LcUtilMfc_H_

#pragma warning(disable : 4786)

#include <string>
#include <vector>

#include <windows.h>
#include <io.h>
#include <atlbase.h>




void GetProjectFullName(char* sName, IBuildProject* pProject);
void GetProjectName(char* sName, IBuildProject* pProject);

#endif