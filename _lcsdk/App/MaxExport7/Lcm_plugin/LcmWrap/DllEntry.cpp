//
//	DESCRIPTION:	Max Plug-in Wrapper
//
//	CREATED BY: Tom Hudson
//	Updated By: Heesung Oh
//
//	HISTORY:
//	1. created 4 September 1998(Tom Hudson)
//	2. Updated 1. 2006.(Heesung Oh)
//	2. Updated 1. 2008.(Heesung Oh)
//
//	Copyright (c) 2000, All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////


#include "LcmWrap.h"


HINSTANCE	g_hInst;
int			controlsInit = FALSE;
HWND		g_hWnd;


void MsgBox(char *format,...)
{
	va_list ap;
	char s[2048];
	
	if (format == NULL)
		return;
	
	va_start(ap, format);
	vsprintf((char *)s, format, ap);
	va_end(ap);
	
	if (s == NULL)
		return;
	
	MessageBox(0, s, "Msg", MB_OK);
}


BOOL WINAPI DllMain(HINSTANCE hinstDLL,ULONG fdwReason,LPVOID lpvReserved)
{
	g_hInst = hinstDLL;
	g_hWnd = GetActiveWindow();

	if (!controlsInit)
	{
		controlsInit = TRUE;
		InitCustomControls(g_hInst);	// Initialize MAX's custom controls
		InitCommonControls();			// Initialize Win95 controls
	}
			
	return (TRUE);
}

__declspec( dllexport ) const TCHAR* LibDescription()
{
	return GetString(IDS_LIBDESCRIPTION);
}


__declspec( dllexport ) int LibNumberClasses()
{
	return 1;
}

__declspec( dllexport ) ClassDesc* LibClassDesc(int i)
{
	switch(i)
	{
		case 0: return GetLcmWrapDesc();
		default: return 0;
	}
}

__declspec( dllexport ) ULONG LibVersion()
{
	return VERSION_3DSMAX;
}

TCHAR *GetString(int id)
{
	static TCHAR buf[256];

	if (g_hInst)
		return LoadString(g_hInst, id, buf, sizeof(buf)) ? buf : NULL;
	
	return NULL;
}

