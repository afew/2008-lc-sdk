// Implementation of the ILcLsc class.
//
////////////////////////////////////////////////////////////////////////////////


#include <windows.h>
#include <d3dx9.h>

#include <Lua/lua.h>
#include <Lua/lualib.h>
#include <Lua/lauxlib.h>

#include <Lc/LcUtil.h>
#include <Lc/LcString.h>

#include <Lc/ILcIme.h>
#include <Lc/ILcInput.h>
#include <Lc/ILcDev.h>

#include <Lc/ILcLsc.h>

#include <LcNet/ILcNet.h>


// script
#include "LscSys.h"																// Lua System
#include "LscD3d.h"																// Lua D3D
#include "LscD3x.h"

#include "LscUtil.h"															// Lua Utilities
#include "LscInput.h"															// Lua Input Class
#include "LscIme.h"																// Lua Ime Class
#include "LscNet.h"																// Lua Network Class

#include "LscFont.h"															// Lua Font Class
#include "LscTex.h"																// Lua Texture Class
#include "LscVtx.h"																// Lua Vertex Class
#include "LscMtl.h"																// Lua Material Class
#include "LscLgt.h"																// Lua Lighting Class
#include "LscSmd.h"																// Lua Media Class
#include "LscMshX.h"																// Lua X-File Class
#include "LscxBill.h"															// Lua Billboard Class

#include "LscxUtil.h"															// Lua LcxUtil Class
#include "LscxMshX.h"															// Lua Lcx X-File Skinning Animation Class
#include "LscxMapi.h"															// Lua Map Indoor Class
#include "LscMdl.h"																// Lua Lcx Mdl Animation Class

#include "LsgApp.h"																// Lua Game Application

#include "LcLsc.h"																// Lua App Class


namespace	LcLua
{

CLcLsc::CLcLsc()
{
	m_pL		= NULL;
	memset(m_sF, 0, sizeof m_sF);

	m_pDevSys	= NULL;
}

CLcLsc::~CLcLsc()
{
	Destroy();
}


void CLcLsc::Destroy()
{
	m_pDevSys	= NULL;
	memset(m_sF, 0, sizeof m_sF);

	if(m_pL)
	{
		lua_State*	pL =(lua_State*)m_pL;
		lua_close(pL);
		m_pL = NULL;
	}
}



INT CLcLsc::Create(void* p1, void* p2, void* p3, void* p4)
{
	INT			hr =-1;
	lua_State*	pL = NULL;

	char*	sFile	= (char*)p1;

	m_pDevSys	= (ILcDev*)p2;

	if(NULL == sFile)
		return -1;

	 pL= lua_open();

	if(NULL == pL)
		return -1;

	m_pL = pL;

	luaopen_base(pL);
	luaopen_io(pL);
	luaopen_math(pL);
	luaopen_string(pL);
	luaopen_table(pL);

	LscFont	::StaticContainerCreate();
	LscTex	::StaticContainerCreate();
	LscSmd	::StaticContainerCreate();
	
	LscSys	::m_pDevSys	= m_pDevSys;
	LscSys	::OpenLib(pL);

	strcpy(m_sF, sFile);
	hr = lua_dofile(pL, m_sF);	
	
	Lua_Create();
	
	return 0;
}

INT CLcLsc::Query(char* sCmd, void* pData)
{
	return -1;
}





// Implementations Lsc Glue.
//
////////////////////////////////////////////////////////////////////////////////


INT CLcLsc::Lua_Init()
{
	INT			hr	= -1;
	lua_State*	pL	= (lua_State*)m_pL;

	// Lua 스크립트 파일에 있는 Lua_Init 함수를 호출
	lua_getglobal(pL, "Lua_Init");
	
	if (lua_pcall(pL, 0, 1, 0) != 0)
	{
		LcUtil_ErrMsgBox(LcStr_Format("Lua_Init Failed: %s",  lua_tostring(pL, -1)));
		return -1;
	}
	
	// get the result
	hr = (HRESULT)lua_tonumber(pL, -1);
	lua_pop(pL, 1);
	
	if(FAILED(hr))
		return -1;
	
	return hr;
}



INT CLcLsc::Lua_Destroy()
{
	INT			hr	= -1;
	lua_State*	pL	= (lua_State*)m_pL;
	
	// Lua 스크립트 파일에 있는 Lua_Destroy() 함수를 호출
	lua_getglobal(pL, "Lua_Destroy");
	
	if (lua_pcall(pL, 0, 1, 0) != 0)
	{
		LcUtil_ErrMsgBox(LcStr_Format("Lua_Destroy Failed: %s",  lua_tostring(pL, -1)));
		return -1;
	}
	
	hr = (INT)lua_tonumber(pL, -1);
	lua_pop(pL, 1);
	
	if(FAILED(hr))
		return -1;
	
	return hr;
}



INT	CLcLsc::Lua_FrameMove()
{
	INT			hr	= -1;
	lua_State*	pL	= (lua_State*)m_pL;
	
	// Lua 스크립트 파일에 있는 Lua_FrameMove 함수를 호출
	lua_getglobal(pL, "Lua_FrameMove");
	
	//	lua_call(pL, 0, 1);
	
	if( 0 != lua_pcall(pL, 0, 1, 0))
	{
		LcUtil_ErrMsgBox(LcStr_Format("Lua_FrameMove Failed: %s",  lua_tostring(pL, -1)));
		return -1;
	}
	
	// get the result
	hr = (INT)lua_tonumber(pL, -1);
	lua_pop(pL, 1);
	
	if(FAILED(hr))
		return -1;
	
	return hr;
}




INT CLcLsc::Lua_Render()
{
	INT			hr	= -1;
	lua_State*	pL	= (lua_State*)m_pL;
	
	// Lua 스크립트 파일에 있는 Lua_Render 함수를 호출
	lua_getglobal(pL, "Lua_Render");
	
	if (lua_pcall(pL, 0, 1, 0) != 0)
	{
		LcUtil_ErrMsgBox(LcStr_Format("Lua_Render Failed: %s",  lua_tostring(pL, -1)));
		return -1;
	}
	
	// get the result
	hr = (INT)lua_tonumber(pL, -1);
	lua_pop(pL, 1);
	
	return hr;
}






INT CLcLsc::Lua_Create()
{
	INT			hr	= -1;
	lua_State*	pL	= (lua_State*)m_pL;

	// Lua 스크립트 파일에 있는 Lua_Create 함수를 호출
	lua_getglobal(pL, "Lua_Create");
	
	if (lua_pcall(pL, 0, 1, 0) != 0)
	{
		LcUtil_ErrMsgBox(LcStr_Format("Lua_Create Failed: %s",  lua_tostring(pL, -1)));
		return -1;
	}
	
	// get the result
	hr = (INT)lua_tonumber(pL, -1);
	lua_pop(pL, 1);
	
	if(FAILED(hr))
		return -1;
	
	return hr;
}





CLcLscApp::CLcLscApp()
{
	m_pL		= NULL;

	memset(m_sF, 0, sizeof m_sF);
}

CLcLscApp::~CLcLscApp()
{
	Destroy();
}



void CLcLscApp::Destroy()
{
	ContainerDestroy();
}


INT CLcLscApp::Query(char* sCmd, void* pData)
{
	if(0==_stricmp("Create Local Containers", sCmd))
	{
		ContainerCreate();
		return 0;
	}
	else if(0==_stricmp("Destroy Local Containers", sCmd))
	{
		ContainerDestroy();
		return 0;
	}

	else if(0==_stricmp("Create Static Containers", sCmd))
	{
		StaticContainerCreate();
		return 0;
	}
	else if(0==_stricmp("Destroy Static Containers", sCmd))
	{
		StaticContainerDestroy();
		return 0;
	}

	else if(0==_stricmp("Set Clear Color", sCmd))
	{
		DWORD	dVal = *((DWORD*)pData);
		return 0;
	}

	else if(0==_stricmp("Get Clear Color", sCmd))
	{
		*((DWORD*)pData) = 0;
		return 0;
	}

	return -1;
}


void CLcLscApp::ContainerCreate()
{
	LscFont	::ContainerCreate();
	LscTex	::ContainerCreate();
	LscVtx	::ContainerCreate();
	LscMtl	::ContainerCreate();
	LscLgt	::ContainerCreate();
	LscMshX	::ContainerCreate();
	LscxBill::ContainerCreate();
	LscSmd	::ContainerCreate();

	LscxUtil::ContainerCreate();
	LscxMshX::ContainerCreate();
	LscxMapi::ContainerCreate();
	LscMdl	::ContainerCreate();
}


void CLcLscApp::ContainerDestroy()
{
	LscFont	::ContainerDestroy();
	LscTex	::ContainerDestroy();
	LscVtx	::ContainerDestroy();
	LscMtl	::ContainerDestroy();
	LscLgt	::ContainerDestroy();
	LscMshX	::ContainerDestroy();
	LscxBill::ContainerDestroy();
	LscSmd	::ContainerDestroy();

	LscxUtil::ContainerDestroy();
	LscxMshX::ContainerDestroy();
	LscxMapi::ContainerDestroy();
	LscMdl	::ContainerDestroy();
}



void CLcLscApp::StaticContainerCreate()
{
	LscFont	::StaticContainerCreate();
	LscTex	::StaticContainerCreate();
	LscVtx	::StaticContainerCreate();
	LscMtl	::StaticContainerCreate();
	LscLgt	::StaticContainerCreate();
	LscMshX	::StaticContainerCreate();
	LscxBill::StaticContainerCreate();
	LscSmd	::StaticContainerCreate();

	LscxUtil::StaticContainerCreate();										// Static LscLcx Utilities Class
	LscxMshX::StaticContainerCreate();
	LscxMapi::StaticContainerCreate();
	LscMdl	::StaticContainerCreate();
}


void CLcLscApp::StaticContainerDestroy()
{
	LscFont	::StaticContainerDestroy();
	LscTex	::StaticContainerDestroy();
	LscVtx	::StaticContainerDestroy();
	LscMtl	::StaticContainerDestroy();
	LscLgt	::StaticContainerDestroy();
	LscMshX	::StaticContainerDestroy();
	LscxBill::StaticContainerDestroy();
	LscSmd	::StaticContainerDestroy();

	LscxUtil::StaticContainerDestroy();
	LscxMshX::StaticContainerDestroy();
	LscxMapi::StaticContainerDestroy();
	LscMdl	::StaticContainerDestroy();
}



void CLcLscApp::OpenLib()
{
	lua_State*	pL	= (lua_State*)m_pL;

	LscSys	::OpenLib(pL);
	LscD3d	::OpenLib(pL);
	LscD3x	::OpenLib(pL);

	LscInput::OpenLib(pL);
	LscUtil	::OpenLib(pL);
	LscIme	::OpenLib(pL);
	LscNet	::OpenLib(pL);

	LscFont	::OpenLib(pL);
	LscTex	::OpenLib(pL);
	LscVtx	::OpenLib(pL);
	LscMtl	::OpenLib(pL);
	LscLgt	::OpenLib(pL);
	LscMshX	::OpenLib(pL);
	LscxBill::OpenLib(pL);
	LscSmd	::OpenLib(pL);

	LscxUtil::OpenLib(pL);
	LscxMshX::OpenLib(pL);
	LscxMapi::OpenLib(pL);
	LscMdl	::OpenLib(pL);


	LsgApp	::OpenLib(pL);
}


INT CLcLscApp::Create(void* p1, void* p2, void* p3, void* p4)
{
	INT			hr	= -1;
	lua_State*	pL	= NULL;

	char*	sFile		= (char*)p1;
	m_pDevSys			= (ILcDev*)p2;


	if(NULL == sFile)
		return -1;

	 pL= lua_open();

	if(NULL == pL)
		return -1;

	m_pL	= pL;

	void*	pDev2D = NULL	;				// 2D Sprite
	m_pDevSys->Query("Get 2D Sprite", &pDev2D);
	
	// load Lsc libraries
	luaopen_base(pL);
	luaopen_io(pL);
	luaopen_math(pL);
	luaopen_string(pL);
	luaopen_table(pL);


	ContainerDestroy();
	ContainerCreate();

	StaticContainerCreate();
	
	OpenLib();
	
	strcpy(m_sF, sFile);
	hr = lua_dofile(pL, m_sF);
	
	return hr;
}

}// namespace Lua