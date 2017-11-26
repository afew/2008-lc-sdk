// Implementation of the LscxMapi class.
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
#include <Lc/ILcM3d.h>
#include <Lc/ILcDev.h>

#include <Lc/LcUtil.h>
#include <Lc/ILcxM3d.h>
#include <Lc/ILcxCam.h>
#include <Lc/ILcLsc.h>

#include <Lc/ILcxMdl.h>
#include <Lc/ILcxTbl.h>

#include <Lc/ILcxmFld.h>
#include <Lc/ILcxmT.h>

#include "LscxUtil.h"
#include "LscxMapi.h"



namespace	LcLua
{
	
////////////////////////////////////////////////////////////////////////////////
// Script

ILcDev*			LscxMapi::m_pDevSys	= NULL;
void*			LscxMapi::m_pDevD3D	= NULL;


void*			LscxMapi::m_pTbTex	= NULL;
void*			LscxMapi::m_pTbMdl	= NULL;
void*			LscxMapi::m_pFld	= NULL;


INT LscxMapi::Create(ILcDev* pDev)
{
	LscxMapi::m_pDevSys	= pDev;
	LscxMapi::m_pDevD3D	= LcDev_GetD3Device(&m_pDevSys);

	if(NULL == LscxMapi::m_pDevD3D)
		return -1;

	return 0;
}



static const luaL_reg regLcxMapi[] =
{
	{"New"		,	LscxMapi::Lsc_Alloc		},
	{"ReAlloc"	,	LscxMapi::Lsc_ReAlloc	},
	{"Release"	,	LscxMapi::Lsc_Release	},
	{"FrameMove",	LscxMapi::Lsc_FrameMove	},
	{"Render"	,	LscxMapi::Lsc_Render	},

	{NULL, NULL},
};



INT LscxMapi::OpenLib(lua_State *pL)
{
	luaL_openlib(pL, "LxMapi", regLcxMapi, 0);
	return 1;
}


INT	LscxMapi::Lsc_Alloc(lua_State *pL)
{
	INT		nIdx =0;
	
	char	sFileMap[160]={0};
	char	sFileTex[160]={0};
	char	sFileMdl[160]={0};

	INT		n = lua_gettop(pL);
	
	if(n<3)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	strcpy(sFileMap, lua_tostring(pL, 1));
	strcpy(sFileTex, lua_tostring(pL, 2));
	strcpy(sFileMdl, lua_tostring(pL, 3));
	
	
	ILcxTbl*		pTbTex	;
	ILcxTbl*		pTbMdl	;
	ILcxmFld*		pFld	;


	if(FAILED(LcxTbl_Create("Texture", &pTbTex, sFileTex, LscxMapi::m_pDevD3D)))
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	m_pTbTex = pTbTex;
	
	if(FAILED(LcxTbl_Create("Model", &pTbMdl, sFileMdl, LscxMapi::m_pDevD3D, m_pTbTex)))
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	m_pTbMdl	= pTbMdl;

	if(FAILED(LcMap_CreateOutdoor("Out Door", &pFld, LscxMapi::m_pDevD3D, m_pTbTex, m_pTbMdl	)))
	{
		lua_pushnumber(pL, -1);
		return 1;
	}


	m_pFld	= pFld	;

	
	ILcxCamM*	p	= (ILcxCamM*)LcDev_GetUserCamera(&m_pDevSys);
	ILcxCam*	pCam= p->GetCameraPointer(0);
	
	if(FAILED(pFld->Query("Set Camera", pCam)))
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	if(FAILED(pFld->Query("File Load", sFileMap)))
	{
		lua_pushnumber(pL, -1);
		return 1;
	}



	FLOAT	fCull		= 5000;
	pFld->Query("Set Culling Range", &fCull);
	
	
	lua_pushnumber(pL, 0);
	return 1;
}


INT	LscxMapi::Lsc_ReAlloc(lua_State *pL)
{
	// 지형 재 생성

	return 0;
}


INT	LscxMapi::Lsc_Release(lua_State *pL)
{
	INT		nIdx =0;
	INT		n = lua_gettop(pL);

	// 지형 해제
	SAFE_DELETE(	m_pFld		);

	lua_pushnumber(pL, 0);
	return 1;
}


INT LscxMapi::Lsc_FrameMove(lua_State *pL)
{
	INT		nIdx =0;
	
	// 지형 갱신
	if(m_pFld)
	{
		ILcxmFld* pFld = (ILcxmFld*)m_pFld;

		if(FAILED(pFld->FrameMove()))
		{
			lua_pushnumber(pL, -1);
			return 1;
		}
	}
	
	lua_pushnumber(pL, 0);
	return 1;
}


INT LscxMapi::Lsc_Render(lua_State *pL)
{
	INT		nIdx =0;
	PDEV	pDev = (PDEV)(LscxMapi::m_pDevD3D);

	pDev->SetRenderState( D3DRS_LIGHTING,	TRUE);
	pDev->SetRenderState(D3DRS_FOGENABLE,	TRUE);
	pDev->SetRenderState( D3DRS_FILLMODE,	D3DFILL_SOLID);
	
	// 지형 렌더링
	ILcxmFld*		pFld = (ILcxmFld*)m_pFld;

	SAFE_RENDER(	pFld	);
	
	lua_pushnumber(pL, 0);
	return 1;
}


INT	LscxMapi::ContainerCreate()
{
	return 0;
}


INT	LscxMapi::ContainerDestroy()
{
	if(m_pTbTex)
	{
		ILcxTbl* p = (ILcxTbl*)m_pTbTex;
		delete	 p;
		m_pTbTex = NULL;
	}

	if(m_pTbMdl)
	{
		ILcxTbl* p = (ILcxTbl*)m_pTbMdl;
		delete	 p;
		m_pTbMdl = NULL;
	}

	if(m_pFld)
	{
		ILcxmFld* p = (ILcxmFld*)m_pFld;
		delete	   p;
		m_pFld = NULL;
	}

	return 0;
}



INT	LscxMapi::StaticContainerCreate()
{
	return 0;
}


INT	LscxMapi::StaticContainerDestroy()
{
	return 0;
}


}// namespace Lua