//
//
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////


#ifndef __STDAFX_H_
#define __STDAFX_H_

#pragma warning(disable : 4099)
#pragma warning(disable : 4996)


#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define USE_LC_SDK_LIB

//#pragma comment (linker, "/SUBSYSTEM:WINDOWS") 
//#pragma comment(linker, "/NODEFAULTLIB:LIBCMTD.lib")
//#pragma comment(linker, "/nodefaultlib:oldnames.lib")

#pragma comment(lib, "dinput8.lib"	)	// Direct Input
#pragma comment(lib, "dsound.lib"	)	// Sound
#pragma comment(lib, "dxguid.lib"	)	//
#pragma comment(lib, "dxerr9.lib"	)	//

#pragma comment(lib, "d3d9.lib"		)
#pragma comment(lib, "d3dx9.lib"	)
#pragma comment(lib, "d3dxof.lib"	)

#pragma comment(lib, "winmm.lib"	)
#pragma comment(lib, "comctl32.lib"	)
#pragma comment(lib, "Version.lib"	)


#include <WINSOCK2.H>

#include <dmusicc.h>
#include <dmusici.h>

#include <windows.h>

#include <stdio.h>
#include <math.h>
#include <tchar.h>

#include <d3d9.h>
#include <d3dx9.h>

#include <Lua/lua.h>
#include <Lua/lualib.h>
#include <Lua/lauxlib.h>

#include <Lc/LcEuclid.h>
#include <Lc/LcMath.h>

#include <Lc/LcType.h>
#include <Lc/LcUtil.h>
#include <Lc/LcString.h>
#include <Lc/LcUtilDx.h>

#include <Lc/ILcTex.h>
#include <Lc/ILcFont.h>
#include <Lc/ILcDev.h>

#include <Lc/ILcSmd.h>
#include <Lc/MpDShow.h>
#include <Lc/ILcInput.h>
#include <Lc/ILcIme.h>

#include <Lc/ILcmMdl.h>
#include <Lc/ILcmMdlX.h>

#include <Lc/ILcM3d.h>

#include <Lc/ILcXmsh.h>
#include <Lc/ILcLsc.h>

#include <Lc/ILcxCam.h>

#include <Lc/ILcxTbl.h>

#include <Lc/ILcxmFld.h>
#include <Lc/ILcxmFldX.h>

#include <LcNet/ILcNet.h>

#include "resource.h"

// script
#include "Lsc/LscSys.h"															// Lua System
#include "Lsc/LscInput.h"														// Lua Input Class
#include "Lsc/LscD3d.h"															// Lua D3D
#include "Lsc/LscD3x.h"															// Lua D3DX
#include "Lsc/LscUtil.h"														// Lua Utilities
#include "Lsc/LscIme.h"															// Lua Ime Class
#include "Lsc/LscNet.h"															// Lua Network Class
#include "Lsc/LscSmd.h"															// Lua Sound Class

#include "Lsc/LscFont.h"														// Lua Font Class
#include "Lsc/LscTex.h"															// Lua Texture Class
#include "Lsc/LscVtx.h"															// Lua Vertex Class
#include "Lsc/LscMtl.h"															// Lua Material Class
#include "Lsc/LscLgt.h"															// Lua Lighting Class

#include "Lsc/LscMdl.h"															// Lua Lcx Mdl Animation Class
#include "Lsc/LscMshX.h"														// Lua X-File Class

#include "Lsc/LscxBill.h"														// Lua Billboard Class
#include "Lsc/LscxUtil.h"														// Lua Lcx Utilities
#include "Lsc/LscxMapi.h"														// Lua Lcx Outdoor Map
#include "Lsc/LscxMshX.h"														// Lua Lcx X-File Skinning Animation Class

#include "Lsc/LsgApp.h"															// Lua Game Application

#include "Lsc/LcLsc.h"


#include "OcFSM/ILopAi.h"
#include "OcFSM/ocAmun.h"


#include "Segment/IGameData.h"
#include "Segment/IGameSgmnt.h"

#include "Segment/GameData.h"
#include "Segment/GameSgmnt.h"

#include "Main/App3D.h"
#include "Main/AppPhase.h"

#include "Main/Main.h"


#endif

