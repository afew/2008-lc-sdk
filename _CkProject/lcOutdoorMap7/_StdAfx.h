// All Header files.
//
////////////////////////////////////////////////////////////////////////////////


#ifndef __STDAFX_H_
#define __STDAFX_H_

#define STRICT
#define WIN32_LEAN_AND_MEAN														// Exclude rarely-used stuff from Windows headers
#define _WIN32_WINNT			0x0500
#define	WINVER					0x0400
#define DIRECTINPUT_VERSION		0x0800

#pragma once

#pragma warning( disable : 4018)
#pragma warning( disable : 4100)
#pragma warning( disable : 4238)
#pragma warning( disable : 4245)
#pragma warning( disable : 4503)
#pragma warning( disable : 4663)
#pragma warning( disable : 4786)


//#pragma comment(linker, "/NODEFAULTLIB:LIBCMTD.lib")
//#pragma comment (linker, "/SUBSYSTEM:WINDOWS") 
//#pragma comment(linker, "/NODEFAULTLIB:LIBCMTD.lib")
//#pragma comment(linker, "/nodefaultlib:oldnames.lib")

// Static Library
#pragma comment(lib, "comctl32.lib"	)
#pragma comment(lib, "dinput8.lib"	)											// for Direct Input

#pragma comment(lib, "dsound.lib"	)	// Sound
#pragma comment(lib, "dxguid.lib"	)	//
#pragma comment(lib, "dxerr9.lib"	)	//

#pragma comment(lib, "d3d9.lib"		)
#pragma comment(lib, "d3dx9.lib"	)
#pragma comment(lib, "d3dxof.lib"	)

#pragma comment(lib, "winmm.lib"	)
#pragma comment(lib, "comctl32.lib"	)
#pragma comment(lib, "Version.lib"	)



#define		USE_LC_SDK_LIB


#ifdef _DEBUG
#pragma comment(lib, "./LnD3D_.lib"	)											// DX Library
#else
#pragma comment(lib, "./LnD3D.lib"	)											// DX Library
#endif

// STL
#include <vector>
#include <algorithm>


// Basic
#include <windows.h>

#include <comdef.h>
#include <commctrl.h>
#include <commdlg.h>
#include <malloc.h>
#include <math.h>
#include <mmsystem.h>
#include <stdio.h>
#include <tchar.h>
#include <time.h>


#include <D3D9.h>
#include <d3dx9.h>
#include <dinput.h>

#include "resource.h"


// MapHF File version
#define BL_DX_VER		0x00090002

// MapHF File version
#define BL_FL_VER		0x00000101


#define EXC_TITLE	"LCX_MAP"
#define EXC_VER		"0.10.01"
#define EXC_MTX		EXC_TITLE"_"EXC_VER
#define EXC_MTX_ON	1



// include files
#include <Lc/LcEuclid.h>
#include <Lc/LcMath.h>

#include <Lc/LcType.h>
#include <Lc/LcString.h>
#include <Lc/LcUtil.h>
#include <Lc/LcUtilFile.h>
#include <Lc/LcUtilDx.h>

#include <Lc/ILcInput.h>
#include <Lc/ILcM3d.h>
#include <Lc/ILcXmsh.h>

#include <Lc/ILcxCam.h>
#include <Lc/ILcmMdl.h>
#include <Lc/ILcmMdlX.h>
#include <Lc/ILcxtObj.h>
#include <Lc/ILcxTbl.h>

#include "LcxMap/ILcxmFld.h"
#include "LcxMap/ILcxmFldX.h"

#include "include/D3DApp.h"


// Work Window class
#include "Main/WndFld.h"
#include "Main/WndLcl.h"
#include "Main/WndMtrl.h"
#include "Main/WndLght.h"
#include "Main/WndFog.h"
#include "Main/WndLyr.h"
#include "Main/WndObj.h"

#include "Main/WndTool.h"
#include "Main/WndWrk.h"


// Main Window class
#include "Main/Main.h"


#endif


