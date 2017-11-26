//
// Copyleft (c) All rights Not reserved.
//

#include "_StdAfx.H"



// File version
#define GS_DX_VER		0x00090002

// File version
#define GS_FL_VER		0x00000101

// Using Mutex
#define EXC_MTX_ON		1
#define EXC_MTX			"GalicSoftPC"



#if EXC_MTX_ON
	#pragma data_seg("GalicSoftPC")
	long g_nDataSeg = 0;
	#pragma data_seg()
	#pragma comment(linker, "/section:GalicSoftPC, rws")

#endif


HANDLE g_hMutex = OpenMutex(SYNCHRONIZE, TRUE, EXC_MTX);


CMain*	g_pApp = NULL;


INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE,LPTSTR, INT)
{

#if EXC_MTX_ON
	if(0 < g_nDataSeg++)
		return 0;

	if(g_hMutex)
		return 0;
#endif

	
	if(!g_hMutex)
		g_hMutex = CreateMutex(NULL, TRUE, EXC_MTX);


	DWORD dDXVer = LcD3D_DXVersion();											// DX 버전 확인

	if(GS_DX_VER > dDXVer)
	{
		MessageBox(0
					, "DirectX Version Error.\n\n"
					  "We need above the DirectX 9.0c version."
					, "Err"
					, MB_OK | MB_ICONERROR);
		return -1;
	}


//	if(GS_FL_VER > 파일 버전 체크)
//	{
//		MessageBox(0
//					, "File version Error.\n\n"
//					  "Please Update your files."
//					, "Err"
//					, MB_OK | MB_ICONERROR);
//		return -1;
//	}



	HRESULT hr=0;
	CMain	d3dMain;
	g_pApp = &d3dMain;

	LcUtil_ControlInit();

	InitCommonControls();

	if(FAILED(d3dMain.Create(hInst)))
		return -1;

	hr = d3dMain.Run();


	if(g_hMutex)
		CloseHandle(g_hMutex);

	g_hMutex	= NULL;

	return hr;
}

