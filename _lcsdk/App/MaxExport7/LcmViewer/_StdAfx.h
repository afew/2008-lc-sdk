#ifndef __STDAFX_H_
#define __STDAFX_H_

#pragma warning( disable : 4018)
#pragma warning( disable : 4100)
#pragma warning( disable : 4245)
#pragma warning( disable : 4503)
#pragma warning( disable : 4663)
#pragma warning( disable : 4786)


#pragma once

#pragma comment(lib, "dinput8.lib")

#define _WIN32_WINNT			0x0400
#define DIRECTINPUT_VERSION		0x0800

#define STRICT

#include <vector>
#include <list>
#include <map>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <functional>

using namespace std;


#include <windows.h>

#include <windowsx.h>
#include <mmsystem.h>

#include <commctrl.h>
#include <commdlg.h>
#include <basetsd.h>

#include <math.h>
#include <stdio.h>
#include <tchar.h>

#include <D3D9.h>
#include <d3dx9.h>
#include <dxerr9.h>

#include <dinput.h>

#include <Richedit.h>

#include "_d3d/DXUtil.h"

#include "_d3d/D3DEnum.h"
#include "_d3d/D3DSetting.h"
#include "_d3d/D3DApp.h"
#include "resource.h"


#include "_LcUtil/LcType.h"
#include "_LcUtil/LcUtil.h"

#include "_LcUtil/LcInput.h"
#include "_LcUtil/LcCam.h"
#include "_LcUtil/LcGrid.h"
#include "_LcUtil/LcField.h"
#include "_LcUtil/LcxPivot.h"

#include "_Lcm/ILcmMdl.h"


#include "Main.h"

#endif