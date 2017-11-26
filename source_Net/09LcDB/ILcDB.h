// ILcDB.h: interface for the ILcDB class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _ILcDB_H_
#define _ILcDB_H_


#ifndef interface
#define interface struct
#endif


interface ILcDB
{
	virtual INT		Create(void* p1=NULL, void* p2=NULL, void* p3=NULL, void* p4=NULL)=0;
	virtual void	Destroy()=0;
	virtual	INT		Query(char* sCmd, void* pData)=0;


	virtual INT		Connect(void* pDataType, void* p1=NULL, void* p2=NULL, void* p3=NULL)=0;
	virtual void	Close()=0;

	virtual INT		SqlBind(char* sSQL, char*** sOutputBufc,INT** nDataBufc, INT nBufSize)=0;
	virtual INT		SqlExec()=0;
	virtual INT		SqlClose()=0;	
};



INT		LnDB_Create(char* sCmd, ILcDB** pData);

INT		LnDB_CreateAndConnect(char* sCmd				// "ODBC" or "OLEDB"
							  , ILcDB** pData		// In-out Data
							  , void* p1				// DataBase Type: "MDB", "excel", "DSN", ...
							  , void* p2=NULL			// DataBase Name
							  , void* p3=NULL			// User Id
							  , void* p4=NULL			// Password
							  , void* p5=NULL			// Host Computer
							  );


#ifdef _DEBUG
	#pragma comment(lib, "LcDB_.lib")
#else
	#pragma comment(lib, "LcDB.lib")
#endif



#endif

