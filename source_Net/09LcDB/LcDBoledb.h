// Interface for the CLcDBoledb class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcDBoledb_H_
#define _LcDBoledb_H_


class CLcDBoledb : public CLcDB
{
protected:
	_ConnectionPtr	m_pCon;
	_RecordsetPtr	m_pRsc;

	SHORT			m_nField;
	char**			m_pDataBuf;
	INT*			m_nDataBuf;

public:	
	CLcDBoledb();
	virtual ~CLcDBoledb();

	virtual INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0);
	virtual void	Destroy();
	virtual	INT		Query(char* sCmd, void* pData);

	
	virtual INT		Connect(void* pDataType, void* p1=0, void* p2=0, void* p3=0);
	virtual void	Close();

	virtual INT		SqlBind(char* sSQL, char*** sOutputBufc,INT** nDataBufc, INT nBufSize);
	virtual INT		SqlExec();
	virtual INT		SqlClose();

protected:
	INT		ConnectSql(char* sDataBase, char* sUID, char* sPWD);
	INT 	ConnectExcel(char* sFile);
	INT 	ConnectDsnOdbc(char* sDSN, char* sUID, char* sPWD);
	INT 	ConnectDsnOledb(char* sDSN, char* sUID, char* sPWD);
	INT 	ConnectDsnInd(char* sDSN, char* sUID, char* sPWD);

public:
	INT		Test();
};


#endif


