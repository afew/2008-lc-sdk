// Implementation of the LscxUtil class.
//
////////////////////////////////////////////////////////////////////////////////


#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include <Lua/lua.h>
#include <Lua/lualib.h>
#include <Lua/lauxlib.h>

#include <Lc/LcEuclid.h>
#include <Lc/LcType.h>
#include <Lc/LcUtil.h>

#include <Lc/ILcM3d.h>
#include <Lc/ILcDev.h>

#include <Lc/ILcxtObj.h>
#include <Lc/ILcxCam.h>
#include <Lc/ILcLsc.h>

#include "LscxUtil.h"



////////////////////////////////////////////////////////////////////////////////
// Script

namespace	LcLua
{

ILcDev*			LscxUtil::m_pDevSys	= NULL;
void*			LscxUtil::m_pDevD3D	= NULL;

void*			LscxUtil::m_pGrid	= NULL;


INT LscxUtil::Create(ILcDev* pDev)
{
	LscxUtil::m_pDevSys	= pDev;
	LscxUtil::m_pDevD3D	= LcDev_GetD3Device(&m_pDevSys);

	if(NULL == LscxUtil::m_pDevD3D)
		return -1;

	return 0;
}



static const luaL_reg regLcxCam[] =
{
	{"SetType"		,	LscxUtil::Lscx_CamSetType	},

	{"FrameMove"	,	LscxUtil::Lscx_CamFrameMove	},
	{"ViewParam"	,	LscxUtil::Lscx_CamViewParam	},
	{"SetEye"		,	LscxUtil::Lscx_CamSetEye	},
	{"GetEye"		,	LscxUtil::Lscx_CamGetEye	},
	{"SetLook"		,	LscxUtil::Lscx_CamSetLook	},
	{"GetLook"		,	LscxUtil::Lscx_CamGetLook	},
	{"SetUp"		,	LscxUtil::Lscx_CamSetUp		},
	{"SetBasis"		,	LscxUtil::Lscx_CamSetBasis	},
	{"SetEpsilonY"	,	LscxUtil::Lscx_CamSetEpsilonY},

	{"ProjParam"	,	LscxUtil::Lscx_CamProjParam	},
	{"SetFov"		,	LscxUtil::Lscx_CamSetFov	},
	{"SetScreen"	,	LscxUtil::Lscx_CamSetScreen	},
	{"SetNear"		,	LscxUtil::Lscx_CamSetNear	},
	{"SetFar"		,	LscxUtil::Lscx_CamSetFar	},

	{"SetSpeed"		,	LscxUtil::Lscx_CamSetSpeed		},
	{"SetControlKey",	LscxUtil::Lscx_CamSetControl	},

	{"MoveForward"	,	LscxUtil::Lscx_CamMoveForward	},
	{"MoveSideward"	,	LscxUtil::Lscx_CamMoveSideward	},
	
	{"RotateYaw"	,	LscxUtil::Lscx_CamRotateYaw		},
	{"RotatePitch"	,	LscxUtil::Lscx_CamRotatePitch	},
	{"Zoom"			,	LscxUtil::Lscx_CamZoom			},

	{"SetTransformVP",	LscxUtil::Lscx_CamTransformVP	 },
	{"SetTransformView", LscxUtil::Lscx_CamTransformView },
	{"SetTransformProj", LscxUtil::Lscx_CamTransformProj },

	{NULL, NULL},
};

static const luaL_reg regLcxUtil[] =
{
	{"Command"	,	LscxUtil::Lscx_Command	},
	{NULL, NULL},
};



INT LscxUtil::OpenLib(lua_State *pL)
{
	luaL_openlib(pL, "LxCam", regLcxCam, 0);
	luaL_openlib(pL, "LxUtil", regLcxUtil, 0);
	
	return 1;
}



INT	LscxUtil::Lscx_CamSetType(lua_State *pL)
{
	INT		n	= lua_gettop(pL);
	INT		nIdx= 0;
	INT		Val	=0;
	
	if(n<2)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nIdx	= (INT  )lua_tonumber(pL, 1);
	Val		= (INT  )lua_tonumber(pL, 2);


	// Setup Camera
	ILcxCamM*	p	= (ILcxCamM*)LcDev_GetUserCamera(&m_pDevSys);
	p->SetType(nIdx, Val);

	lua_pushnumber(pL, 0);
	return 1;
}


INT	LscxUtil::Lscx_CamFrameMove(lua_State *pL)
{
	ILcxCamM*	p	= (ILcxCamM*)LcDev_GetUserCamera(&m_pDevSys);
	if(p)
		p->FrameMove();

	return 0;
}


INT	LscxUtil::Lscx_CamViewParam(lua_State *pL)
{
	INT		n	= lua_gettop(pL);
	INT		nIdx=0;

	VEC3	vcEye (0, 0,-1);
	VEC3	vcLook(0, 0, 0);
	VEC3	vcUp  (0, 1, 0);

	if(n<7)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nIdx	= (INT  )lua_tonumber(pL, 1);
	vcEye.x	= (FLOAT)lua_tonumber(pL, 2);
	vcEye.y	= (FLOAT)lua_tonumber(pL, 3);
	vcEye.z	= (FLOAT)lua_tonumber(pL, 4);
	vcLook.x= (FLOAT)lua_tonumber(pL, 5);
	vcLook.y= (FLOAT)lua_tonumber(pL, 6);
	vcLook.z= (FLOAT)lua_tonumber(pL, 7);

	if(n>7)
	{
		vcUp.x	= (FLOAT)lua_tonumber(pL, 8);
		vcUp.y	= (FLOAT)lua_tonumber(pL, 9);
		vcUp.z	= (FLOAT)lua_tonumber(pL,10);
	}


	// Setup Camera
	ILcxCamM*	p	= (ILcxCamM*)LcDev_GetUserCamera(&m_pDevSys);
	p->SetParamView(nIdx, &vcEye, &vcLook, &vcUp);

	lua_pushnumber(pL, 0);
	return 1;
}


INT LscxUtil::Lscx_CamSetEye(lua_State *pL)
{
	INT		n	= lua_gettop(pL);
	INT		nIdx=0;	
	VEC3	vcEye(0, 0,-1);

	if(n<4)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nIdx	= (INT  )lua_tonumber(pL, 1);
	vcEye.x	= (FLOAT)lua_tonumber(pL, 2);
	vcEye.y	= (FLOAT)lua_tonumber(pL, 3);
	vcEye.z	= (FLOAT)lua_tonumber(pL, 4);


	// Setup Camera
	ILcxCamM*	p	= (ILcxCamM*)LcDev_GetUserCamera(&m_pDevSys);
	p->SetEye(nIdx, &vcEye);

	lua_pushnumber(pL, 0);
	return 1;
}


INT LscxUtil::Lscx_CamGetEye(lua_State *pL)
{
	INT		n	= lua_gettop(pL);
	INT		nIdx= 0;

	nIdx = (INT  )lua_tonumber(pL, 1);

	// Setup Camera
	ILcxCamM*	p	= (ILcxCamM*)LcDev_GetUserCamera(&m_pDevSys);
	D3DXVECTOR3 vVal = *p->GetEye(nIdx);

	lua_pushnumber(pL, vVal.x);
	lua_pushnumber(pL, vVal.y);
	lua_pushnumber(pL, vVal.z);
	
	return 3;
}


INT LscxUtil::Lscx_CamSetLook(lua_State *pL)
{
	INT		n	= lua_gettop(pL);
	INT		nIdx= 0;
	VEC3	vcLook(0, 0, 0);

	if(n<4)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nIdx	= (INT  )lua_tonumber(pL, 1);
	vcLook.x= (FLOAT)lua_tonumber(pL, 2);
	vcLook.y= (FLOAT)lua_tonumber(pL, 3);
	vcLook.z= (FLOAT)lua_tonumber(pL, 4);

	
	// Setup Camera
	ILcxCamM*	p	= (ILcxCamM*)LcDev_GetUserCamera(&m_pDevSys);
	p->SetLook(nIdx, &vcLook);

	lua_pushnumber(pL, 0);
	return 1;
}


INT LscxUtil::Lscx_CamGetLook(lua_State *pL)
{
	INT		n	= lua_gettop(pL);
	INT		nIdx= 0;

	nIdx = (INT  )lua_tonumber(pL, 1);

	// Setup Camera
	ILcxCamM*	p	= (ILcxCamM*)LcDev_GetUserCamera(&m_pDevSys);
	D3DXVECTOR3 vVal = *p->GetLook(nIdx);

	lua_pushnumber(pL, vVal.x);
	lua_pushnumber(pL, vVal.y);
	lua_pushnumber(pL, vVal.z);
	
	return 3;
}


INT LscxUtil::Lscx_CamSetUp(lua_State *pL)
{
	INT		n	= lua_gettop(pL);
	INT		nIdx= 0;
	VEC3	vcUp(0, 1, 0);

	if(n<4)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nIdx	= (INT  )lua_tonumber(pL, 1);
	vcUp.x	= (FLOAT)lua_tonumber(pL, 2);
	vcUp.y	= (FLOAT)lua_tonumber(pL, 3);
	vcUp.z	= (FLOAT)lua_tonumber(pL, 4);
	

	// Setup Camera
	ILcxCamM*	p	= (ILcxCamM*)LcDev_GetUserCamera(&m_pDevSys);
	p->SetUp(nIdx, &vcUp);

	lua_pushnumber(pL, 0);
	return 1;
}


INT LscxUtil::Lscx_CamSetBasis(lua_State *pL)
{
	INT		n	= lua_gettop(pL);
	INT		nIdx= 0;
	VEC3	vcV(0, 0, 0);

	if(n<4)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nIdx	= (INT  )lua_tonumber(pL, 1);
	vcV.x	= (FLOAT)lua_tonumber(pL, 2);
	vcV.y	= (FLOAT)lua_tonumber(pL, 3);
	vcV.z	= (FLOAT)lua_tonumber(pL, 4);
	

	// Setup Camera
	ILcxCamM*	p	= (ILcxCamM*)LcDev_GetUserCamera(&m_pDevSys);
	p->SetBasis(nIdx, &vcV);

	lua_pushnumber(pL, 0);
	return 1;
}


INT LscxUtil::Lscx_CamSetEpsilonY(lua_State *pL)
{
	INT		n	= lua_gettop(pL);
	INT		nIdx= 0;
	FLOAT	fEpY= 0;

	if(n<2)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nIdx = (INT  )lua_tonumber(pL, 1);
	fEpY = (FLOAT)lua_tonumber(pL, 2);

	ILcxCamM*	p	= (ILcxCamM*)LcDev_GetUserCamera(&m_pDevSys);
	p->SetEpsilonY(nIdx, fEpY);

	lua_pushnumber(pL, 0);
	return 1;
}


INT	LscxUtil::Lscx_CamProjParam(lua_State *pL)
{
	INT		n	= lua_gettop(pL);
	INT		nIdx= 0;

	FLOAT	fFov	= D3DX_PI/4.F;
	FLOAT	fScnW	=1024;
	FLOAT	fScnH	= 768;
	FLOAT	fNear	= 1.f;
	FLOAT	fFar	= 8000.f;
	FLOAT	fAspc	= fScnW/fScnH;

	if(n<6)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nIdx	= (INT  )lua_tonumber(pL, 1);
	fFov	= (FLOAT)lua_tonumber(pL, 2);
	fScnW	= (FLOAT)lua_tonumber(pL, 3);
	fScnH	= (FLOAT)lua_tonumber(pL, 4);
	fNear	= (FLOAT)lua_tonumber(pL, 5);
	fFar	= (FLOAT)lua_tonumber(pL, 6);
	fAspc	= fScnW/ fScnH;

	// Setup Camera
	ILcxCamM*	p	= (ILcxCamM*)LcDev_GetUserCamera(&m_pDevSys);
	p->SetParamProj(nIdx, fFov, fScnW, fScnH, fNear, fFar);

	lua_pushnumber(pL, 0);
	return 1;
}


INT	LscxUtil::Lscx_CamSetFov(lua_State *pL)
{
	INT		n	= lua_gettop(pL);
	INT		nIdx= 0;
	FLOAT	fFov= D3DX_PI/4.F;
	
	if(n<2)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nIdx	= (INT  )lua_tonumber(pL, 1);
	fFov	= (FLOAT)lua_tonumber(pL, 2);
	
	// Setup Camera
	ILcxCamM*	p	= (ILcxCamM*)LcDev_GetUserCamera(&m_pDevSys);
	p->SetFov(nIdx, fFov);

	lua_pushnumber(pL, 0);
	return 1;
}


INT	LscxUtil::Lscx_CamSetScreen(lua_State *pL)
{
	INT		n	= lua_gettop(pL);
	INT		nIdx= 0;

	FLOAT	fScnW	=1024;
	FLOAT	fScnH	= 768;
	FLOAT	fAspc	= fScnW/fScnH;

	if(n<3)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nIdx	= (INT  )lua_tonumber(pL, 1);
	fScnW	= (FLOAT)lua_tonumber(pL, 2);
	fScnH	= (FLOAT)lua_tonumber(pL, 3);
	fAspc	= fScnW/ fScnH;

	// Setup Camera
	ILcxCamM*	p	= (ILcxCamM*)LcDev_GetUserCamera(&m_pDevSys);
	p->SetScnW(nIdx, fScnW);
	p->SetScnH(nIdx, fScnH);

	lua_pushnumber(pL, 0);
	return 1;
}



INT	LscxUtil::Lscx_CamSetNear(lua_State *pL)
{
	INT		n	= lua_gettop(pL);
	INT		nIdx= 0;
	FLOAT	fNear= 1.f;

	if(n<2)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nIdx	= (INT  )lua_tonumber(pL, 1);
	fNear	= (FLOAT)lua_tonumber(pL, 2);


	// Setup Camera
	ILcxCamM*	p	= (ILcxCamM*)LcDev_GetUserCamera(&m_pDevSys);
	p->SetNear(nIdx, fNear);

	lua_pushnumber(pL, 0);
	return 1;
}



INT	LscxUtil::Lscx_CamSetFar(lua_State *pL)
{
	INT		n	= lua_gettop(pL);
	INT		nIdx= 0;
	FLOAT	fFar= 8000.f;

	if(n<2)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nIdx	= (INT  )lua_tonumber(pL, 1);
	fFar	= (FLOAT)lua_tonumber(pL, 2);
	
	// Setup Camera
	ILcxCamM*	p	= (ILcxCamM*)LcDev_GetUserCamera(&m_pDevSys);
	p->SetFar(nIdx, fFar);

	lua_pushnumber(pL, 0);
	return 1;
}



INT	LscxUtil::Lscx_CamSetSpeed(lua_State *pL)
{
	INT		n	= lua_gettop(pL);
	INT		nIdx= 0;
	FLOAT	fV	= 1.f;

	if(n<2)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nIdx	= (INT  )lua_tonumber(pL, 1);
	fV	= (FLOAT)lua_tonumber(pL, 2);

	ILcxCamM*	p	= (ILcxCamM*)LcDev_GetUserCamera(&m_pDevSys);
//	p->SetSpeed(nIdx, fV);
	lua_pushnumber(pL, 0);
	return 1;
}


INT	LscxUtil::Lscx_CamSetControl(lua_State *pL)
{
	INT		n	= lua_gettop(pL);
	INT		nIdx= 0;
	char	sControl[128]={0};			// Control Name
	INT		nControl=0;					// This Ascii code value

	if(n<3)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nIdx			=(INT  )lua_tonumber(pL, 1	);
	strcpy(sControl, (char*)lua_tostring(pL, 2)	);
	nControl		=(INT  )lua_tonumber(pL, 3	);

	// Setup Camera
	//

	lua_pushnumber(pL, 0);
	return 1;
}


INT	LscxUtil::Lscx_CamMoveForward(lua_State *pL)
{
	INT		n	= lua_gettop(pL);
	INT		nIdx= 0;
	FLOAT	fV	= 0.f;

	if(n<2)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nIdx	= (INT  )lua_tonumber(pL, 1);
	fV		= (FLOAT)lua_tonumber(pL, 2);
	
	ILcxCamM*	p	= (ILcxCamM*)LcDev_GetUserCamera(&m_pDevSys);
	p->MoveForward(nIdx, fV);

	lua_pushnumber(pL, 0);
	return 1;
}


INT	LscxUtil::Lscx_CamMoveSideward(lua_State *pL)
{
	INT		n	= lua_gettop(pL);
	INT		nIdx= 0;
	FLOAT	fV	= 0.f;

	if(n<2)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nIdx	= (INT  )lua_tonumber(pL, 1);
	fV		= (FLOAT)lua_tonumber(pL, 2);
	
	// Setup Camera
	ILcxCamM*	p	= (ILcxCamM*)LcDev_GetUserCamera(&m_pDevSys);
	p->MoveSideward(nIdx, fV);

	lua_pushnumber(pL, 0);
	return 1;
}


INT	LscxUtil::Lscx_CamRotateYaw(lua_State *pL)
{
	INT		n	= lua_gettop(pL);
	INT		nIdx= 0;
	FLOAT	fV1	= 0.f;
	FLOAT	fV2	= 0.f;

	if(n<3)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nIdx	= (INT  )lua_tonumber(pL, 1);
	fV1		= (FLOAT)lua_tonumber(pL, 2);
	fV2		= (FLOAT)lua_tonumber(pL, 3);


	ILcxCamM*	p	= (ILcxCamM*)LcDev_GetUserCamera(&m_pDevSys);
	p->RotateYaw(nIdx, fV1, fV2);

	lua_pushnumber(pL, 0);
	return 1;
}


INT	LscxUtil::Lscx_CamRotatePitch(lua_State *pL)
{
	INT		n	= lua_gettop(pL);
	INT		nIdx= 0;
	FLOAT	fV1	= 0.f;
	FLOAT	fV2	= 0.f;

	if(n<3)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nIdx	= (INT  )lua_tonumber(pL, 1);
	fV1		= (FLOAT)lua_tonumber(pL, 2);
	fV2		= (FLOAT)lua_tonumber(pL, 3);

	// Setup Camera
	ILcxCamM*	p	= (ILcxCamM*)LcDev_GetUserCamera(&m_pDevSys);
	p->RotatePitch(nIdx, fV1, fV2);

	lua_pushnumber(pL, 0);
	return 1;
}


INT	LscxUtil::Lscx_CamZoom(lua_State *pL)
{
	INT		n	= lua_gettop(pL);
	INT		nIdx= 0;
	FLOAT	fV	= 0.f;

	if(n<2)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nIdx	= (INT  )lua_tonumber(pL, 1);
	fV	= (FLOAT)lua_tonumber(pL, 2);

	ILcxCamM*	p	= (ILcxCamM*)LcDev_GetUserCamera(&m_pDevSys);
	p->Zoom(nIdx, fV);

	lua_pushnumber(pL, 0);
	return 1;
}


INT	LscxUtil::Lscx_CamTransformView(lua_State *pL)
{
	INT		n	= lua_gettop(pL);
	INT		nIdx= 0;
	
	nIdx = (INT  )lua_tonumber(pL, 1);

	ILcxCamM*	p	= (ILcxCamM*)LcDev_GetUserCamera(&m_pDevSys);
	p->SetTransformViw(nIdx);
	lua_pushnumber(pL, 0);
	return 1;
}


INT	LscxUtil::Lscx_CamTransformProj(lua_State *pL)
{
	INT		n	= lua_gettop(pL);
	INT		nIdx= 0;

	nIdx = (INT  )lua_tonumber(pL, 1);

	ILcxCamM*	p	= (ILcxCamM*)LcDev_GetUserCamera(&m_pDevSys);
	p->SetTransformPrj(nIdx);
	lua_pushnumber(pL, 0);
	return 1;
}



INT LscxUtil::Lscx_CamTransformVP(lua_State* pL)
{
	INT		n	= lua_gettop(pL);
	INT		nIdx= 0;

	nIdx = (INT  )lua_tonumber(pL, 1);

	// Setup Camera
	ILcxCamM*	p	= (ILcxCamM*)LcDev_GetUserCamera(&m_pDevSys);
	p->SetTransformViw(nIdx);
	p->SetTransformPrj(nIdx);

	lua_pushnumber(pL, 0);
	return 1;
}




INT	LscxUtil::Lscx_Command(lua_State *pL)
{
	char	sCmd[128]={0};			// Command Name
	strcpy(sCmd, (char*)lua_tostring(pL, 1));

	if(0 == _stricmp("Render Grid", sCmd))
	{
		ILcxtObj*	p = (ILcxtObj*)LscxUtil::m_pGrid;
		SAFE_RENDER(	p	);
	}

	
	return 0;
}



INT	LscxUtil::ContainerCreate()
{
	return 0;
}


INT	LscxUtil::ContainerDestroy()
{
	return 0;
}



INT	LscxUtil::StaticContainerCreate()
{
	if(NULL == LscxUtil::m_pGrid)
	{
		ILcxtObj*	p = NULL;
		if(FAILED(LcxtObj_Create("XYZ Grid", &p, LscxUtil::m_pDevD3D)))
			return -1;

		LscxUtil::m_pGrid = p;
	}
	
	return 0;
}


INT	LscxUtil::StaticContainerDestroy()
{
	if(LscxUtil::m_pGrid)
	{
		ILcxtObj*	p= (ILcxtObj*)LscxUtil::m_pGrid;
		delete p;
		LscxUtil::m_pGrid = NULL;
	}

	return 0;
}

}// namespace Lua