// Implementation of the LsgApp class.
//
////////////////////////////////////////////////////////////////////////////////


#include <windows.h>
#include <windowsx.h>
#include <tchar.h>

#include <d3dx9.h>

#include <Lua/lua.h>
#include <Lua/lualib.h>
#include <Lua/lauxlib.h>

#include <Lc/LcEuclid.h>
#include <Lc/ILcTex.h>
#include <Lc/LcUtil.h>
#include <Lc/ILcDev.h>

#include <Lc/LcUtil.h>
#include <Lc/LcString.h>
#include <Lc/ILcxCam.h>

#include <Lc/ILcmMdl.h>
#include <Lc/ILcxmFld.h>

#include "LsgApp.h"

#include "../Segment/IGameData.h"
#include "../Segment/IGameSgmnt.h"


namespace	LcLua
{
	
ILcDev* LsgApp::m_pDevSys	= NULL;
void*	LsgApp::m_pDevD3D	= NULL;

deque<LsgApp::Tcommand>*
		LsgApp::m_vCommand	= NULL;

void*	LsgApp::m_pMainField= NULL;
void*	LsgApp::m_pGameData	= NULL;
void*	LsgApp::m_pGameSgmnt= NULL;


INT LsgApp::Create(ILcDev* pDev)
{
	INT		hr=-1;
	
	LsgApp::m_pDevSys	= pDev;
	LsgApp::m_pDevD3D	= LcDev_GetD3Device(&m_pDevSys);

	if(NULL == LsgApp::m_pDevD3D)
		return -1;


	deque<Tcommand>* pCmd	= new deque<Tcommand>;
	LsgApp::m_vCommand	= pCmd;

	hr = LcDev_SetUserCommand(&m_pDevSys, pCmd);

	if(FAILED(hr))
		return -1;


	m_pGameData = LcDev_GetUserData1(&m_pDevSys);

	if(NULL == m_pGameData)
		return 0;

	return 0;
}


INT LsgApp::Destroy()
{
	if(LsgApp::m_vCommand)
	{
		LsgApp::m_vCommand->clear();
		delete LsgApp::m_vCommand;
		LsgApp::m_vCommand = NULL;

		LcDev_SetUserCommand(&m_pDevSys, NULL);
	}

	if(m_pGameSgmnt)
	{
		IGameSgmnt*	pSgmnt = (IGameSgmnt*)m_pGameSgmnt;
		delete pSgmnt;
		m_pGameSgmnt = NULL;
	}
	
	return 0;
}


INT LsgApp::ContainerRestore()
{
	IGameSgmnt*	pSgmnt = (IGameSgmnt*)m_pGameSgmnt;
	if(pSgmnt)
	{
		pSgmnt->Restore();
	}

	return 0;
}

INT LsgApp::ContainerInvalidate()
{
	IGameSgmnt*	pSgmnt = (IGameSgmnt*)m_pGameSgmnt;
	if(pSgmnt)
	{
		pSgmnt->Invalidate();
	}

	return 0;
}



static const luaL_reg regLsgApp[] =
{
	{"SetCommand"		,	LsgApp::Lsc_SetCommand		},
	{"GetCommand"		,	LsgApp::Lsc_GetCommand		},

	// Network Application Module
	{"GetServerName"	,	LsgApp::Lsc_GetServerName	},
	{"GetCharNameMan"	,	LsgApp::Lsc_GetCharNameMan	},
	{"GetCharNameLady"	,	LsgApp::Lsc_GetCharNameLady	},


	{"FieldLoad"		,	LsgApp::Lsc_FieldLoad		},
	{"FieldCameraSet"	,	LsgApp::Lsc_FieldCameraSet	},
	{"FieldViewRange"	,	LsgApp::Lsc_FieldViewRange	},

	{"SegmentLoad"		,	LsgApp::Lsc_SegmentLoad		},
	{"SegmentFrameMove"	,	LsgApp::Lsc_SegmentFrameMove},
	{"SegmentRender"	,	LsgApp::Lsc_SegmentRender	},

	{"MateModel"		,	LsgApp::Lsc_MateModel		},
	{"MateActive"		,	LsgApp::Lsc_MateActive		},
	{"MateRotation"		,	LsgApp::Lsc_MateRotation	},
	{"MatePosition"		,	LsgApp::Lsc_MatePosition	},

	{NULL, NULL},
};


INT LsgApp::OpenLib(lua_State *pL)
{
	luaL_openlib(pL, "Lapp", regLsgApp, 0);
	return 1;
}


// Implementations La API.
//
////////////////////////////////////////////////////////////////////////////////



// 비형식 Message and Command 명령어
INT LsgApp::Lsc_SetCommand(lua_State *pL)
{
	char sCmd[512]={0};
	char sVal[512]={0};
	
	INT n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	sprintf(sCmd, "%s", lua_tostring(pL, 1));
	
	if(n>1)
		sprintf(sVal, "%s", lua_tostring(pL, 2));


	LsgApp::Tcommand	tCmd(sCmd, sVal);

	if(m_vCommand)
		m_vCommand->push_back(tCmd);


	lua_pushnumber(pL, 0);
	return 1;
}

// 비형식 Message and Command 명령어
INT LsgApp::Lsc_GetCommand(lua_State *pL)
{
	char sCmd[512]={0};
	char sVal[512]={0};

	if(!m_vCommand || m_vCommand->empty() )
	{
		lua_pushstring(pL, "<<No Command>>");
		lua_pushstring(pL, "<<No Message>>");
		return 2;
	}

	deque<Tcommand>::iterator it = m_vCommand->begin();

	strcpy(sCmd, it->sCmd);
	strcpy(sVal, it->sVal);

	lua_pushstring(pL, sCmd);
	lua_pushstring(pL, sVal);

	m_vCommand->pop_front();

	return 2;
}


INT LsgApp::Lsc_GetServerName(lua_State *pL)
{
	IGameData* pGameData = (IGameData*)m_pGameData;
	char	sSelectedSvr[64]={0};

	if(pGameData)
		pGameData->Query("Selected Server", sSelectedSvr);
	else
		strcpy(sSelectedSvr, "<<No Selected Server>>" );
	
	lua_pushstring(pL, sSelectedSvr);
	return 1;
}


INT LsgApp::Lsc_GetCharNameMan(lua_State *pL)
{
	IGameData* pGameData = (IGameData*)m_pGameData;
	char	sSelectedChar[64]={0};

	if(pGameData)
		pGameData->Query("Selected Man Character", sSelectedChar);
	else
		strcpy(sSelectedChar, "<<No Selected Server>>" );
	
	lua_pushstring(pL, sSelectedChar);
	return 1;
}


INT LsgApp::Lsc_GetCharNameLady(lua_State *pL)
{
	IGameData* pGameData = (IGameData*)m_pGameData;
	char	sSelectedChar[64]={0};

	if(pGameData)
		pGameData->Query("Selected Lady Character", sSelectedChar);
	else
		strcpy(sSelectedChar, "<<No Selected Server>>" );
	
	lua_pushstring(pL, sSelectedChar);
	return 1;
}


INT LsgApp::Lsc_FieldLoad(lua_State *pL)
{
	INT			hr = -1;
	char		sField[160] ={0};
	char		sHlsl[160] ={0};
	IGameSgmnt*	pSgmnt = (IGameSgmnt*)m_pGameSgmnt;
	ILcxmFld*	pField = NULL;

	INT		n = lua_gettop(pL);
	
	if(n<2 || NULL == pSgmnt)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	strcpy(sField, lua_tostring(pL, 1));
	strcpy(sHlsl, lua_tostring(pL, 2));
	
	hr = pSgmnt->Query("Set HLSL", sHlsl);

	if(FAILED(hr))
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	hr = pSgmnt->Query("Load Field", sField);

	if(FAILED(hr))
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	hr = pSgmnt->Query("Get Field", &pField);
	m_pMainField	= pField;

	lua_pushnumber(pL, hr);
	return 1;
}

INT LsgApp::Lsc_SegmentLoad(lua_State *pL)
{
	IGameSgmnt*	pSgmnt = NULL;
	if(FAILED(LcxApp_CreateGameSegment(NULL, &pSgmnt, m_pDevSys)))			// Game Play
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	if(FAILED(LcDev_SetUserData2(&m_pDevSys, pSgmnt)))
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	m_pGameSgmnt = pSgmnt;

	lua_pushnumber(pL, 0);
	return 1;
}

INT LsgApp::Lsc_SegmentFrameMove(lua_State *pL)
{
	IGameSgmnt*	pSgmnt = (IGameSgmnt*)m_pGameSgmnt;
	if(pSgmnt)
	{
		pSgmnt->FrameMove();
	}

	return 0;
}

INT LsgApp::Lsc_SegmentRender(lua_State *pL)
{
	IGameSgmnt*	pSgmnt = (IGameSgmnt*)m_pGameSgmnt;
	if(pSgmnt)
	{
		pSgmnt->Render();
	}

	return 0;
}


INT LsgApp::Lsc_FieldCameraSet(lua_State *pL)
{
	ILcxCamM*	p		= NULL;
	ILcxCam*	pCam	= NULL;
	IGameSgmnt*	pSgmnt	= (IGameSgmnt*)m_pGameSgmnt;

	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	INT nCam = lua_tonumber(pL, 1);

	if(nCam>16 || nCam<0)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}


	// Setup Camera
	p	= (ILcxCamM*)LcDev_GetUserCamera(&m_pDevSys);
	pCam= p->GetCameraPointer(nCam);

	if(pSgmnt)
		pSgmnt->Query("Set Field Camera", pCam);

	lua_pushnumber(pL, 0);
	return 1;
}


INT LsgApp::Lsc_MateModel(lua_State *pL)
{
	INT			hr = -1;
	INT			n1 = -1;
	INT			n2 = -1;
	char		sMdl[128] ={0};
	char		sTex[128] ={0};
	char		sShd[128] ={0};
	
	IGameSgmnt*	pSgmnt = (IGameSgmnt*)m_pGameSgmnt;

	INT		n = lua_gettop(pL);
	
	if(n<2 || NULL == pSgmnt)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}	

	n1	= (INT)lua_tonumber(pL, 1);
	n2	= (INT)lua_tonumber(pL, 2);
	strcpy(sMdl, lua_tostring(pL, 3));
	strcpy(sTex, lua_tostring(pL, 4));
	strcpy(sShd, lua_tostring(pL, 5));

	struct _T{	INT	_n1; INT _n2; char* mdl; char* tex;	char* shd;	}
			t={ n1, n2, sMdl, sTex, sShd};

	hr = pSgmnt->Query("Load Character", &t);

	lua_pushnumber(pL, hr);
	return 1;
}


INT LsgApp::Lsc_MateActive(lua_State *pL)
{
	INT			hr = -1;
	INT			n1 = -1;
	INT			n2 = -1;
	INT			v  = -1;
	
	IGameSgmnt*	pSgmnt = (IGameSgmnt*)m_pGameSgmnt;

	INT		n = lua_gettop(pL);
	
	if(n<2 || NULL == pSgmnt)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}	

	n1	= (INT)lua_tonumber(pL, 1);
	n2	= (INT)lua_tonumber(pL, 2);
	v	= (INT)lua_tonumber(pL, 3);

	struct _T{	INT	_n1; INT _n2; INT _v;	}
			t={ n1, n2, v};

	hr = pSgmnt->Query("Mate Activation", &t);

	lua_pushnumber(pL, hr);
	return 1;
}


INT LsgApp::Lsc_MateRotation(lua_State *pL)
{
	INT			hr = -1;
	INT			n1 = -1;
	INT			n2 = -1;
	VEC4		vcVal(0, 0, 0, 0);
	IGameSgmnt*	pSgmnt = (IGameSgmnt*)m_pGameSgmnt;

	INT		n = lua_gettop(pL);
	
	if(n<2 || NULL == pSgmnt)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	n1	= (INT)lua_tonumber(pL, 1);
	n2	= (INT)lua_tonumber(pL, 2);
	vcVal.x	= (FLOAT)lua_tonumber(pL, 3);
	vcVal.y	= (FLOAT)lua_tonumber(pL, 4);
	vcVal.z	= (FLOAT)lua_tonumber(pL, 5);
	vcVal.w	= (FLOAT)lua_tonumber(pL, 6);
	
	vcVal.w	= D3DXToRadian(vcVal.w);


	struct _T{	INT	_n1; INT _n2; VEC4* pV;	}
			t={ n1, n2, &vcVal};

	hr = pSgmnt->Query("Set Rotation", &t);

	lua_pushnumber(pL, hr);
	return 1;
}


INT LsgApp::Lsc_MatePosition(lua_State *pL)
{
	INT			hr = -1;
	INT			n1 = -1;
	INT			n2 = -1;
	VEC4		vcVal(0, 0, 0, 0);
	IGameSgmnt*	pSgmnt = (IGameSgmnt*)m_pGameSgmnt;

	INT		n = lua_gettop(pL);
	
	if(n<2 || NULL == pSgmnt)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	n1	= (INT)lua_tonumber(pL, 1);
	n2	= (INT)lua_tonumber(pL, 2);
	vcVal.x	= (FLOAT)lua_tonumber(pL, 3);
	vcVal.y	= (FLOAT)lua_tonumber(pL, 4);
	vcVal.z	= (FLOAT)lua_tonumber(pL, 5);
	
	vcVal.w	= D3DXToRadian(vcVal.w);

	struct _T{	INT	_n1; INT _n2; VEC4* pV;	}
			t={ n1, n2, &vcVal};

	hr = pSgmnt->Query("Set Position", &t);

	lua_pushnumber(pL, hr);
	return 1;
}


INT LsgApp::Lsc_FieldViewRange(lua_State *pL)
{
	INT			hr = 0;
	FLOAT		fRange = 0;
	ILcxmFld*	pField = (ILcxmFld*)m_pMainField;
	IGameSgmnt*	pSgmnt = (IGameSgmnt*)m_pGameSgmnt;

	INT		n = lua_gettop(pL);
	
	if(n<2 || NULL == pSgmnt)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	fRange = (FLOAT)lua_tonumber(pL, 2);

	hr = pField->Query("Set View Range", &fRange);

	lua_pushnumber(pL, hr);
	return 1;
}


}// namespace Lua


