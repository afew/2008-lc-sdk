// ILcDB.cpp: implementation of the ILcDB class.
//
////////////////////////////////////////////////////////////////////////////////


#pragma warning(disable : 4786)

#include <vector>
#include <string>

using namespace std;


#import "C:\Program Files\Common Files\System\ADO\msado15.dll" \
no_namespace rename("EOF", "EndOfFile")


#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <stdio.h>

#include "ILcDB.h"
#include "LcDB.h"
#include "LcDBodbc.h"
#include "LcDBoledb.h"


INT LnDB_Create(char* sCmd, ILcDB** pData)
{
	*pData = NULL;
	
	if(0==_stricmp("ODBC", sCmd))
	{
		CLcDBodbc * pDB=NULL;

		pDB = new CLcDBodbc;

		if(FAILED(pDB->Create()))
		{
			delete pDB;
			return -1;
		}

		*pData = pDB;
		return 0;
	}

	else if(0==_stricmp("OLDEDB", sCmd))
	{
		CLcDBoledb * pDB=NULL;

		pDB = new CLcDBoledb;

		if(FAILED(pDB->Create()))
		{
			delete pDB;
			return -1;
		}

		*pData = pDB;
		return 0;
	}

	return -1;
}


INT LnDB_CreateAndConnect(char* sCmd, ILcDB** pData, void* p1, void* p2, void* p3, void* p4, void* p5)
{
	*pData = NULL;
	
	if(0==_stricmp("ODBC", sCmd))
	{
		CLcDBodbc * pDB=NULL;

		pDB = new CLcDBodbc;

		if(FAILED(pDB->Create(p1, p2, p3, p4)))
		{
			pDB->Close();
			delete pDB;
			return -1;
		}

		*pData = pDB;
		return 0;
	}

	else if(0==_stricmp("OLEDB", sCmd))
	{
		CLcDBoledb * pDB=NULL;

		pDB = new CLcDBoledb;

		if(p5)
			pDB->Query("Set Host", p5);

		if(FAILED(pDB->Create(p1, p2, p3, p4)))
		{
			pDB->Close();
			delete pDB;
			return -1;
		}

		*pData = pDB;
		return 0;
	}

	return -1;
}
