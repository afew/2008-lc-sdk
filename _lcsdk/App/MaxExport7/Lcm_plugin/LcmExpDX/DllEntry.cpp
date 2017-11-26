// DllMain.
//
////////////////////////////////////////////////////////////////////////////////

#include "Max.h"
#include "iparamb2.h"
#include "resource.h"

#include "LcmExpDX.h"


static CLcmExpDXClassDesc LcmExpDXDesc;

static HINSTANCE	g_hInst;
int					controlsInit = FALSE;

ClassDesc2* GetLcmExpDXDesc() { return &LcmExpDXDesc; }




BOOL WINAPI DllMain(HINSTANCE hinstDLL,ULONG,LPVOID)
{
	g_hInst = hinstDLL;					// Hang on to this DLL's instance handle.

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
		case 0:
			return GetLcmExpDXDesc();

		default:
			return 0;
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


HINSTANCE	GetHinstance()
{
	return g_hInst;
}



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
	
	MessageBox(GetActiveWindow() , s, "Msg", MB_OK);
}

