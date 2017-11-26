// Client Application
//
////////////////////////////////////////////////////////////////////////////////

#pragma comment(lib, "Winmm.lib")

#include <windows.h>
#include <Mmsystem.h>
#include <stdio.h>


#include <LcNet/ILcDB.h>


ILcDB*		g_pDataBase=NULL;											// DataBase Instance


int main()
{
	char	sDBtype[32];
	char	sFile[MAX_PATH];
	char	sSQL[512];
	
	strcpy(sDBtype, "excel");
	strcpy(sFile, "database/cigarette2.xls");
	strcpy(sSQL, "Select * from [Sheet1$]");


	if(FAILED(LnDB_CreateAndConnect("OLEDB", &g_pDataBase, sDBtype, sFile)))
		return -1;


	INT		nBufSize= 1024;
	char**	sDataBuf= NULL;
	INT*	nDataBuf= NULL;
	INT		iColumn	= 0;


	for(int j=0; j<3; ++j)
	{
		iColumn = g_pDataBase->SqlBind(sSQL, &sDataBuf, &nDataBuf, nBufSize);

		if(SUCCEEDED(iColumn))
		{
			printf("이름		가격		국산\n");
			printf("--------------------------------------------------\n");

			while(1)
			{
				if(FAILED(g_pDataBase->SqlExec()))
					break;

				for(int i=0; i<iColumn; ++i)
				{
					printf("%11s(%2d) ", sDataBuf[i], nDataBuf[i]);
				}

				printf("\n");
			}

			g_pDataBase->SqlClose();

			printf("--------------------------------------------------\n\n");
		}
	}


	g_pDataBase->Close();
	g_pDataBase->Destroy();

	delete g_pDataBase;
	
	return 1;
}


