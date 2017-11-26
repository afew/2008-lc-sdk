// interface for the LcUtil Files.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcUtilFile_H_
#define _LcUtilFile_H_

#pragma warning(disable : 4786)

#include <string>
#include <vector>

#include <windows.h>
#include <io.h>



DWORD	LcFile_Err();
void	LcFile_Err(DWORD dError);
void	LcFile_GetAllFiles(std::vector<std::string>* pOut, char* sPath);
void	LcFile_GetAllFolders(std::vector<std::string>* pOut, char* sPath);

void	LcFile_GetAllFilesAndFoldersFromSubDir(std::vector<std::string>* pOut, char* sPath);
void	LcFile_GetAllFilesFromSubDir(std::vector<std::string>* pOut, char* sPath);
void	LcFile_GetAllFoldersFromSubDir(std::vector<std::string>* pOut, char* sPath);
void	LcFile_Finds(std::vector<std::string>* pOut, char* sPath, char* sFile);

INT		LcFile_FileFind(char* sFile);
INT		LcFile_FileSize(char* sFile);
long	LcFile_FileSize(FILE* &fp);												// Get File Size

DWORD	LcFile_GetAttrib(char* sFile);

void	LcFile_GetDrive( char const *pPath, char *pDrive);
void	LcFile_GetDir(char const *pPath, char *pDir);
void	LcFile_GetName(char const *pPath, char *pName);
void	LcFile_GetExt(char const *pPath, char *pExt);
void	LcFile_GetDriveAndDir(char const *pPath, char *pDriveDir);
void	LcFile_GetNameAndExt(char const *pPath, char *pNameExt);
void	LcFile_SetExt(char const *pSrcFile, char const *pExt, char *pDestFile);


INT		LcFile_XCopy(char *pSrc, char *pDst);
INT		LcFile_RemoveAll(char *pSrc, char *pDst=NULL);
char*	LcFile_SimpleStrDateTime(char* sYear, char* sMon, char* sDay, char* sTime, time_t lTime);

INT		LcFile_CmtlFileOpen(													// Common Control File Open
								HWND hWnd										// HWND Owner
							,	char* sFileFull									// File Full Path Name
							,	char* sFilePath									// File Path
							,	char* sFileName									// File Name
							,	char* sName										// Name
							,	char* sFileExtn									// File Extension
							,	CONST char* sFilter								// Filter
							,	CONST char* sDefExt								// Default Extension
							,	CONST char* sTitle								// Common Control Title
							,	CONST char* sInitPath = NULL					// Init Path
							);




// For WinAPI File Management From C Style Functions
HANDLE	LcFile_fopen(char* sFile, char* sT);									// Fail: return INVALID_HANDLE_VALUE, append Mode일 경우 파일 포인터는 마지막에 있다.
INT		LcFile_fclose(HANDLE hFile);
DWORD	LcFile_fsize(HANDLE hFile);

INT		LcFile_fread(HANDLE hFile, void* p, INT bufSize, INT nCount=1);
INT		LcFile_fwrite(HANDLE hFile, void* p, INT bufSize, INT nCount=1);
INT		LcFile_fprintf(HANDLE hFile, char *format,...);
INT		LcFile_fgets(HANDLE hFile, char* sLine, int iMax, BOOL bFillEndLineZero=TRUE);
DWORD	LcFile_fseek(HANDLE hFile, LONG lOff, DWORD nFrom=FILE_BEGIN /*FILE_BEGIN, FILE_CURRENT, FILE_END*/);
DWORD	LcFile_fseekBegin(HANDLE hFile);
DWORD	LcFile_fseekEnd(HANDLE hFile);

void	LcFile_fsetLength(HANDLE hFile, DWORD dwNewLen);
DWORD	LcFile_fsetLength(HANDLE hFile);

DWORD	LcFile_ftell(HANDLE hFile);
BOOL	LcFile_feof(HANDLE hFile=INVALID_HANDLE_VALUE);							// return EOF is TRUE, others FALSE




struct TfileEntry
{
	DWORD	dAtt;
	
	INT		iYea;			// Year
	INT		iMon;			// Mon
	INT		iDay;			// Day

	INT		iHou;			// Hour
	INT		iMin;			// Minute
	INT		iSec;			// Second
	
	DWORD	dLen;			// Size
	char	name[MAX_PATH];	// Name
	char	path[MAX_PATH];	// Path

	BOOL	IsFolder()
	{
		return ( _A_SUBDIR & dAtt);
	}
};


INT		LcFile_GetEntry(TfileEntry* pData, char* sFullPathFile);

void	LcFile_ReadFileLine(HANDLE hFile, char* sLine);
void	LcFile_ReadFileLine(FILE *fp, char* sLine);


#endif


