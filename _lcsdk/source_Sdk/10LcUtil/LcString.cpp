// Implementation of Lc String functions.
//
////////////////////////////////////////////////////////////////////////////////


#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include <malloc.h>
#include <string.h>


#include "LcString.h"


char*	LcStr_Format(const char *format, ...)
{
	static char sDst[8192];
	char sOrg[8192];

	va_list ap;

	va_start(ap, format);
	vsprintf(sOrg, format, ap);
	va_end(ap);

	strcpy(sDst, sOrg);

	return sDst;
}


char* LcStr_GetWord(char* s, int nIdx)
{
	static char sWord[4];
	char*	p;
	int		nCnt =0;
	BOOL	b2B= FALSE;

	memset(sWord, 0, sizeof sWord );
	p = s;

	while(1)
	{
		// 2����Ʈ�� 0���� �۴�.
		b2B = ((*p)<0);

		if(nCnt >= nIdx)
		{
			sWord[0]= *p;

			if(b2B)
				sWord[1]= *(p+1);

			if(0== sWord[0] && 0== sWord[1])
				return NULL;

			break;
		}

		++nCnt;
		++p;

		if(b2B)
			++p;

	}

	return sWord;
}



void LcStr_ReadLineQuot(char *strOut, char *strIn, INT iC)
{
	INT iB;
	INT iE;
	INT	i;

	// Search forward
	
	INT	iLen= strlen(strIn);

	for(i=0;i<iLen; ++i)
	{
		if(iC == strIn[i])
		{
			iB = i;
			break;
		}
	}

	for(i=iLen-1;i>=0; --i)
	{
		if(iC == strIn[i])
		{
			iE = i;
			break;
		}
	}

	if(iB == (iE-1))															// ���۰� �����̿� ���ڰ� ����.
	{
		strOut[0]=0;
		return;
	}

	strncpy(strOut, strIn+iB+1, iE-iB-1);
}



char* LcStr_DWtoStr(DWORD dwA, BOOL bAdd0x)
{
	static char	buf[32];
	char	s[32];
	char*	pDst;

	memset(buf, 0, sizeof(buf));
	memset(s, 0, sizeof(s));
	sprintf(s, "0000000000%X", dwA);
	pDst = s + strlen(s) - 8;

	if(bAdd0x)
		sprintf(buf,"0x%s", pDst);
	else
		strcpy(buf, pDst);

	return buf;
}




void LcStr_StrList(std::vector<std::string>& vStr, char* sIn, char* sSpes)
{
	char *token = NULL;

	if(!strlen(sIn))
		return;

	token = strtok( sIn, sSpes );

	if(!token || !strlen(token))
		return;

	vStr.push_back(token);

	while( token != NULL )
	{
		token = strtok( NULL, sSpes );

		if(token && strlen(token))
			vStr.push_back(token);
	}
}




char* LcStr_EnumValueQuotation(char* sVal, char* s, int ch)
{
	int n = -1;
	int i=0;

	s = strchr(s, '\"');

	for( ; 0 != *s ; ++s)
	{
		if(ch == (*s))
		{
			++n;

			if(1 == n)
			{
				++s;
				break;
			}

			continue;
		}

		sVal[i] = (*s);
		++i;
	}

	return s;
}



void LcStr_Replace(char* sInOut, int origin, int dest)
{
	char* s = sInOut;

	for(; 0 != *s; ++s)
	{
		if(origin == (*s))
			(*s) = dest;
	}
}


void LcStr_Replaces(char* sInOut, const char* origin, int count, int dest)
{
	char* s = sInOut;

	for(; 0 != *s; ++s)
	{
		for(int j=0; j<count; ++j)
		{
			if((*s) == origin[j])
			{
				(*s) = dest;
				break;
			}
		}
	}
}



INT LcStr_Sgets(const char* sSrc, char* sLine, int iMax, BOOL bFillEndLineZero)
{
	int		i=0;
	INT		nTotal=0;
	
	for(i=0; i<iMax; ++i)
	{
		sLine[i] = sSrc[i];
		++nTotal;

		// End of Line
//		if('\n' == sLine[i] || '\r' == sLine[i] || '\n\r' == sLine[i] || '\r\n' == sLine[i])
		// window�� �ؽ�Ʈ ������ '\r\n'���� ��������...
		if('\n' == sLine[i])
		{
			sLine[i+1] =0;
			break;
		}		
	}
	
	if(bFillEndLineZero)
	{
		if(i>0)
		{
			if('\r' == sLine[i-1])
				sLine[i-1] =0;
			else
				sLine[i] =0;
		}
	}
	
	return nTotal;
}



int LcStr_AnsiToUnicode(WCHAR* sDst, const char* sSrc, INT iLen )
{
	if( sDst==NULL || sSrc==NULL || iLen < 1 )
		return -1;
	
	INT nResult = MultiByteToWideChar( CP_ACP, 0, sSrc, -1, sDst, iLen );

	sDst[iLen-1] = 0;
	
	if( 0 == nResult)
		return -1;

	return 0;
}

INT LcStr_UnicodeToAnsi(char* sDst, const WCHAR* sSrc, INT iLen )
{
	if( sDst==NULL || sSrc==NULL || iLen < 1 )
		return -1;
	
	INT nResult = WideCharToMultiByte( CP_ACP, 0, sSrc, -1, sDst, iLen*sizeof(char), NULL, NULL );
	sDst[iLen-1] = 0;
	
	if( 0 == nResult)
		return -1;

	return 0;
}






//
//	 * vsscanf for Win32
//	 *
//	 * Written 5/2003 by <mgix@mgix.com>
//	 *
//	 * This code is in the Public Domain
//	 *
//	http://www.flipcode.com/cgi-bin/fcarticles.cgi?show=4&id=64176
//

int vsscanf(const char* sBuf, const char* format, va_list ap)
{
	// Get an upper bound for the # of args
	size_t count = 0;
	const char *p = format;

	while(1)
	{
		char c = *(p++);
		if(c==0) break;
		if(c=='%' && (p[0]!='*' && p[0]!='%')) ++count;
	}    // Make a local stack
	
	size_t stackSize = (2+count)*sizeof(void*);
	void **newStack = (void**)alloca(stackSize);    // Fill local stack the way sscanf likes it
	
	newStack[0] = (void*)sBuf;
	newStack[1] = (void*)format;
	memcpy(newStack+2, ap, count*sizeof(void*));    // Warp into system sscanf with new stack
	
	int result;
	void *savedESP;
	
	_asm
	{
		mov     savedESP, esp;
		mov     esp, newStack;
		call    sscanf;
		mov     esp, savedESP;
		mov     result, eax;
	}

	if(0==result)
		return -1;
	
	return result;
}



int LcStr_Scanf(char* sBuf, char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	
	return vsscanf(sBuf, format, ap);
}


//int LcStr_Scanf(char* sBuf, char* format, ...)
//{
//	va_list ap;
//	va_start(ap, format);
//
//	
//	int		i=0;
//	
//	int		nCnt=0;
//	int		nArg=0;
//	
//	void*	pArg[64]={0};
//	char	sFmt[64][512]={0};
//
//	char	cCur;
//	char*	sCur;
//	char*	sBgn;
//	
//
//	nArg	= 0;
//	sCur	= strchr(format, '%');
//	sBgn	= sCur;
//
//	if(NULL == sCur)
//		return -1;
//
//	do
//	{
//		++sCur;
//		cCur = *sCur;
//
//		if(NULL == cCur)
//		{
//		}
//		else if( '*' == cCur)		// "%*"
//		{
//			++nCnt;
//		}
//		else if ( '%' == cCur)		// "%%"
//		{
//			++sCur;
//		}
//		else
//		{
//			pArg[nArg] = va_arg( ap, void* );
//
//			for(i=0; i<nCnt; ++i)
//			{
//				strcat(sFmt[nArg], "%*s ");
//			}
//
//			char	sFmtSmall[16]={0};
//			char*	sNxt = strchr( sCur, '%');
//
//			if(sNxt)
//			{
//				int nSize = sNxt - sBgn;
//				strncpy(sFmtSmall, sBgn, nSize);
//				strcat(sFmtSmall, " ");
//			}
//			else
//			{
//				strcpy(sFmtSmall, sBgn);
//			}
//
//
//			strcat((char*)&sFmt[nArg], sFmtSmall);
//			++nArg;
//			++nCnt;
//
//			if ( nArg >= 64 )
//				return -1;
//		}
//
//		sCur	= strchr( sCur, '%');
//		sBgn	= sCur;
//
//	} while (NULL != sCur);
//
//	va_end( ap );
//
//
//	if(0==nArg)
//		return -1;
//
//
//	for(i=0; i<nArg; ++i)
//		sscanf( sBuf, sFmt[i], pArg[i]  );
//
//	return nArg;
//}
