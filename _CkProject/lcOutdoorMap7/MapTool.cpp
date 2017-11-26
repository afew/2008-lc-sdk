// Program start.
//
////////////////////////////////////////////////////////////////////////////////

#include "_StdAfx.h"


HANDLE		g_hMtx	= NULL;
CMain*		g_pApp	= NULL;


INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
	LcUtil_ControlInit();
	InitCommonControls();
	
	g_hMtx = OpenMutex(SYNCHRONIZE, FALSE, EXC_MTX);

#if EXC_MTX_ON
	if(g_hMtx)
		return -1;
#endif

	if(!g_hMtx)
		g_hMtx = CreateMutex(NULL, TRUE, EXC_MTX);



	INT		hr;
	CMain	d3dApp;
	g_pApp  = &d3dApp;
	
	if(FAILED( d3dApp.Create(hInst)))
        return -1;

	
	hr = d3dApp.Run();


	if(g_hMtx)
	{
		CloseHandle(g_hMtx);
		g_hMtx	= NULL;
	}


	return hr;
}





