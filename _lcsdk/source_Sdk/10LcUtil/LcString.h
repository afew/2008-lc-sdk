// Interface for the Lc String functions.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcString_H_
#define _LcString_H_


#pragma warning(disable : 4786)
#pragma warning(disable : 4996)

#include <string>
#include <vector>


#if _MSC_VER < 2200
	// 이부분은 6.0과 구분해서 사용해야함
	#include <string>
#else
	#include <string>
#endif


typedef	std::vector<std::string>	lsStr;
typedef	lsStr::iterator				itStr;


char*	LcStr_Format(const char *format, ...);									// String Formating. Like a sprintf

void	LcStr_ReadLineQuot(char *strOut, char *strIn, INT iC='\"');
char*	LcStr_GetWord(char* s, int nIdx);										// Get Word from 1 or 2 Byte code
char*	LcStr_DWtoStr(DWORD dwA, BOOL bAdd0x=TRUE);

void	LcStr_StrList(															// Token Seperator
					  std::vector<std::string>& vStr
					  , char* sIn
					  , char* sSpes=" ,\t/\'\"\\\n");


char*	LcStr_EnumValueQuotation(char* sVal, char* s, int ch='\"');

void	LcStr_Replace(char* sInOut, int origin, int dest);						// To Replace origin to dest in string sInOut.
void	LcStr_Replaces(char* sInOut, const char* origin, int count, int dest);	// To Replace origin to dest in string sInOut.


INT		LcStr_Sgets(const char* sSrc, char* sLine, int iMax, BOOL bFillEndLineZero=TRUE);

INT		LcStr_AnsiToUnicode(WCHAR* sDst, const char* sSrc, INT iLen );			// ANSI to UNICODE
INT		LcStr_UnicodeToAnsi(char* sDst, const WCHAR* sSrc, INT iLen );			// UNICODE to ANSI

int		vsscanf(const char* sBuf, const char* format, va_list ap);
int		LcStr_Scanf(char* sBuf, char* format, ...);								// if Failed then returned -1. succeeded then returned Argument count.




#endif _LcString_H_

