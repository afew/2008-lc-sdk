// Implementation of the CLcDB class.
//
////////////////////////////////////////////////////////////////////////////////


#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <stdio.h>

#include "ILcDB.h"
#include "LcDB.h"



CLcDB::CLcDB()
{
	m_eType		= DB_NONE;			// DataBase Type

	memset(	m_sHst, 0, sizeof m_sHst);
	memset(	m_sDsn, 0, sizeof m_sDsn);
	memset(	m_sUid, 0, sizeof m_sUid);
	memset(	m_sPwd, 0, sizeof m_sPwd);
}


CLcDB::~CLcDB()
{
	Destroy();
}


INT CLcDB::Create(void* p1, void* p2, void* p3, void* p4)
{
	return -1;
}


void CLcDB::Destroy()
{
}


INT CLcDB::Query(char* sCmd, void* pData)
{
	return -1;
}



void CLcDB::Close()
{
}



INT CLcDB::Connect(void* pDataType, void* p1, void* p2, void* p3)
{
	return -1;
}


INT CLcDB::SqlBind(char* sQuery, char*** sDataBufc,INT** nDataBufc, INT nBufSize)
{
	return -1;
}


INT CLcDB::SqlExec()
{
	return -1;
}



INT CLcDB::SqlClose()
{
	return -1;
}

