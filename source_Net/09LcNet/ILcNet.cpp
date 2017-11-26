// Implementation of the ILcNet class.
//
////////////////////////////////////////////////////////////////////////////////


#include <WINSOCK2.H>
#include <windows.h>
#include <stdio.h>

#include "ILcNet.h"

#include "LcNetUtil.h"
#include "LcNet.h"

#include "LcNetSlct.h"
#include "LcNetSlctA.h"
#include "LcNetSlctE.h"
#include "LcNetIocp.h"


INT LcNet_WinSockCreate()
{
	return LcNet_WSAStartup();
}

void LcNet_WinsockDestroy()
{
	LcNet_WSACleanup();
}

INT LcNet_Create(char* sCmd
				 , ILcNet** pData
				 , void* p1			// IP
				 , void* p2			// Port
				 , void* p3			// TCP: "TCP" , UDP: "UDP"
				 , void* p4			// Server: "Server", Client: "Client"
				 , void* v1 = NULL	// Ex Value1
				 , void* v2 = NULL	// Ex Value2
				 );




INT LcNet_CreateTcpServer(char* sCmd
				 , ILcNet** pData
				 , void* p1			// IP
				 , void* p2			// Port
				 )
{
	return LcNet_Create(sCmd, pData, p1, p2, "TCP", "Server");
}

INT LcNet_CreateTcpClient(char* sCmd
				 , ILcNet** pData
				 , void* p1			// IP
				 , void* p2			// Port
				 )
{
	return LcNet_Create(sCmd, pData, p1, p2, "TCP", "Client");
}



INT LcNet_CreateTcpServerEx(char* sCmd
				 , ILcNet** pData
				 , void* p1			// IP
				 , void* p2			// Port
				 , void* v1			// Ex Value1
				 , void* v2			// Ex Value2
				 )
{
	return LcNet_Create(sCmd, pData, p1, p2, "TCP", "Server", v1, v2);
}

INT LcNet_CreateTcpClientEx(char* sCmd
				 , ILcNet** pData
				 , void* p1			// IP
				 , void* p2			// Port
				 , void* v1			// Ex Value1
				 , void* v2			// Ex Value2
				 )
{
	return LcNet_Create(sCmd, pData, p1, p2, "TCP", "Client", v1, v2);
}





INT LcNet_Create(char* sCmd
				 , ILcNet** pData
				 , void* p1			// IP
				 , void* p2			// Port
				 , void* p3			// TCP: "TCP" , UDP: "UDP"
				 , void* p4			// Server: "Server", Client: "Client"
				 , void* v1			// Ex Value1
				 , void* v2			// Ex Value2
				 )
{
	(*pData) = NULL;

	if(0==_stricmp("Select", sCmd))
	{
		CLcNetSlct* pObj = NULL;
		pObj = new CLcNetSlct;

		if(FAILED(pObj->Create(p1, p2, p3, p4)))
		{
			delete pObj;
			return -1;
		}

		(*pData) = pObj;
		return 0;
	}

	else if(0==_stricmp("Async Select", sCmd))
	{
		CLcNetSlctA* pObj = NULL;
		pObj = new CLcNetSlctA;

		pObj->Query("Set Window Handle", v1);
		pObj->Query("Set Message Value", v2);

		if(FAILED(pObj->Create(p1, p2, p3, p4)))
		{
			delete pObj;
			return -1;
		}

		(*pData) = pObj;
		return 0;
	}

	else if(0==_stricmp("Event Select", sCmd))
	{
		CLcNetSlctE* pObj = NULL;
		pObj = new CLcNetSlctE;

		if(FAILED(pObj->Create(p1, p2, p3, p4)))
		{
			delete pObj;
			return -1;
		}

		(*pData) = pObj;
		return 0;
	}

	else if(0==_stricmp("IOCP", sCmd))
	{
		CLcNetIocp* pObj = NULL;
		pObj = new CLcNetIocp;

		if(FAILED(pObj->Create(p1, p2, p3, p4)))
		{
			delete pObj;
			return -1;
		}

		(*pData) = pObj;
		return 0;
	}

	return -1;
}


