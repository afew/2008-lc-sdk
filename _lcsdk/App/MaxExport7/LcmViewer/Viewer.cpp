

#include "_StdAfx.h"


CMain* g_pApp  = NULL;


void LcUtil_SetCurrentDirectory()
{
	char	s1[1024];
	char	s2[1024];

	memset(s1, 0, sizeof s1);
	memset(s2, 0, sizeof s2);

	strcpy(s1, GetCommandLine());

	INT	iLen = strlen(s1);

	for(INT i=0, k=0; i<iLen; ++i)
	{
		if('"' == s1[i])
		{
			++k;
			continue;
		}

		if(2==k)
			break;

		s2[i-k] = s1[i];
		if('\\' == s2[i-k])
			s2[i-k] ='/';
	}

	char*	p = strrchr(s2, '/');
	INT		n = p - s2;

	s2[n] =0;

	::SetCurrentDirectory(s2);
}


INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
	CMain AppMain;

	g_pApp  = &AppMain;

	LcUtil_SetCurrentDirectory();
	
	InitCommonControls();
	if( FAILED( AppMain.Create( hInst ) ) )
		return 0;
	
	return AppMain.Run();
}

