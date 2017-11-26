// Implementation of the LscMdl class.
//
////////////////////////////////////////////////////////////////////////////////

#define USE_LC_SDK_LIB

#include <windows.h>
#include <tchar.h>
#include <d3d9.h>
#include <d3dx9.h>

#include <Lua/lua.h>
#include <Lua/lualib.h>
#include <Lua/lauxlib.h>

#include <Lc/LcEuclid.h>

#include <Lc/LcUtilDx.h>

#include <Lc/ILcTex.h>
#include <Lc/ILcM3d.h>
#include <Lc/ILcDev.h>

#include <Lc/ILcLsc.h>
#include <Lc/ILcmMdl.h>

#include "LscMdl.h"


#ifndef SAFE_DELETE
#	define SAFE_DELETE(p)	{	if(p){delete (p);p = NULL;	}}
#endif



namespace	LcLua
{
	
// For Container
////////////////////////////////////////////////////////////////////////////////

LscMdl::ILcCont::~ILcCont()
{
	if(!vLscH.empty())
	{
		INT iSize = vLscH.size();

		for(INT i=0; i<iSize; ++i)
		{
			Lcm::ILcmMdl* pDst = (Lcm::ILcmMdl*)(vLscH[i]->pIns);
			delete pDst;

			delete vLscH[i];
		}

		vLscH.clear();
	}


	if(!vLscHorg.empty())
	{
		INT iSize = vLscHorg.size();

		for(INT i=0; i<iSize; ++i)
		{
			Lcm::ILcmMdl* pDst = (Lcm::ILcmMdl*)(vLscHorg[i]->pIns);
			delete pDst;
			delete vLscHorg[i];
		}

		vLscHorg.clear();
	}
}


INT LscMdl::ILcCont::Alloc(char* sName, void* pLscHorg)
{
	INT				hr	=-1;
	Lcm::ILcmMdl*	pObj = NULL;

	ILcLsc::LscH*	_t = (ILcLsc::LscH*)pLscHorg;
	Lcm::ILcmMdl*	pOrg = (Lcm::ILcmMdl*)_t->pIns;
	PDEV			pDev = (PDEV)LscMdl::m_pDevD3D;

	hr = Lcm::Mdl_Create("Acm", &pObj, pOrg, sName, pDev);

	if(FAILED(hr))
		return -1;

	ILcLsc::LscH* pLH	= new ILcLsc::LscH;


	pLH->pIns	= pObj;
	hr = vLscH.size();
	vLscH.push_back(pLH);

	return hr;
}



INT LscMdl::ILcCont::Release(INT nKey)
{
	INT iSize = vLscH.size();

	if(iSize<=nKey || nKey<0)
		return -1;

	std::vector<ILcLsc::LscH* >::iterator pLa  = vLscH.begin() + nKey;
	Lcm::ILcmMdl*						  pDst = (Lcm::ILcmMdl*)( (*pLa)->pIns);

	if(pDst)
		delete pDst;

	vLscH.erase( pLa);

	return vLscH.size();
}

ILcLsc::LscH* LscMdl::ILcCont::Find(INT nKey)
{
	INT iSize = vLscH.size();

	if(iSize<=nKey || nKey<0)
		return NULL;

	return vLscH[nKey];
}



INT LscMdl::ILcCont::AllocOrg(char* sName, char* sTxFolder, char* sEft)
{
	INT		hr	=-1;
	PDEV	pDev = (PDEV)LscMdl::m_pDevD3D;

	Lcm::ILcmMdl*	pObj= NULL;
	void*			pEft = LcD3D_EffectFileBuild(pDev, sEft);
	
	if(NULL == pEft)
	{
		MessageBox( GetActiveWindow(), "Build Shader Error", "Err", MB_OK | MB_ICONERROR);
		return -1;
	}

	hr = Lcm::Mdl_Create("Acm", &pObj, NULL, sName, pDev, pEft);

	if(FAILED(hr))
		return -1;


	INT		i;
	INT		nTex = 0;

	TCHAR	sTxPath[160]={0};

	pObj->Query("Get Texture Number", &nTex);

	for(i=0; i<nTex; ++i)
	{
		char	sFile[160] = {0};
		char	sTxPath[160] = {0};

		LPDIRECT3DTEXTURE9	pTx0 = NULL;
		LPDIRECT3DTEXTURE9	pTx1 = NULL;
		LPDIRECT3DTEXTURE9	pTx2 = NULL;
		LPDIRECT3DTEXTURE9	pTx3 = NULL;



		memcpy(sFile, &i, sizeof(INT));
		pObj->Query("Get Texture Name", sFile);

		
		// Diffuse
		sprintf(sTxPath, "%s%s.png", sTxFolder, sFile);
		hr = LcTex_TextureLoadFromFile((void**)&pTx0, NULL, LscMdl::m_pDevD3D, sTxPath);

		// Normal
		sprintf(sTxPath, "%s%s_n.png", sTxFolder, sFile);
		hr = LcTex_TextureLoadFromFile((void**)&pTx1, NULL, LscMdl::m_pDevD3D, sTxPath);

		// Specular
		sprintf(sTxPath, "%s%s_s.png", sTxFolder, sFile);
		hr = LcTex_TextureLoadFromFile((void**)&pTx2, NULL, LscMdl::m_pDevD3D, sTxPath);

		// Emissional
		sprintf(sTxPath, "%s%s_e.png", sTxFolder, sFile);
		hr = LcTex_TextureLoadFromFile((void**)&pTx3, NULL, LscMdl::m_pDevD3D, sTxPath);
	


		struct _T	{	INT n;	ILcTex::TxLayer	pTx;	};
		_T t;

		t.n = i;
		t.pTx.p[0]= pTx0;
		t.pTx.p[1]= pTx1;
		t.pTx.p[2]= pTx2;
		t.pTx.p[3]= pTx3;

		pObj->Query("Set Texture Pointer", &t);
	}



	ILcLsc::LscH* pLH	= new ILcLsc::LscH;

	pLH->pIns	= pObj;					// Original Model 복사
	strcpy(pLH->sName, sName);			// 이름 복사:  찾기로 사용


	hr = vLscHorg.size();
	vLscHorg.push_back(pLH);

	return hr;
}


ILcLsc::LscH* LscMdl::ILcCont::FindOrg(char* sName)
{
	INT iSize = vLscHorg.size();

	for(int i=0; i<iSize; ++i)
	{
		ILcLsc::LscH*	pLH = vLscHorg[i];
		if(0==_stricmp(pLH->sName, sName))
			return vLscHorg[i];
	}

	return NULL;
}


void LscMdl::ILcCont::SetOrgElapseTime(FLOAT fTime)
{
	DOUBLE	dAvg=0;
	m_pDevSys->Query("Get Elapsed Average Time", &dAvg);


	INT iSize = vLscHorg.size();

	for(int i=0; i<iSize; ++i)
	{
		ILcLsc::LscH*		pLH = vLscHorg[i];
		Lcm::ILcmMdl*		pOrg= (Lcm::ILcmMdl*)pLH->pIns;

		pOrg->Query("Set Animation Speed", &dAvg);
	}
}


INT LscMdl::ILcCont::Restore()
{
//	INT	i=0;
//	INT iSize = vLscHorg.size();
//
//	for(i=0; i<iSize; ++i)
//	{
//		ILcmMdl* pDst = (ILcmMdl*)(vLscHorg[i]->pIns);
//		LPD3DXEFFECT	pEft=NULL;
//		pDst->Query("Get HLSL", &pEft);
//		pEft->OnResetDevice();
//	}

	return 0;
}

void LscMdl::ILcCont::Invalidate()
{
//	INT iSize = vLscHorg.size();
//
//	for(INT i=0; i<iSize; ++i)
//	{
//		ILcmMdl* pDst = (ILcmMdl*)(vLscHorg[i]->pIns);
//		LPD3DXEFFECT	pEft=NULL;
//		pDst->Query("Get HLSL", &pEft);
//		pEft->OnLostDevice();
//	}
}



////////////////////////////////////////////////////////////////////////////////
// Script

ILcDev*				LscMdl::m_pDevSys	= NULL;
void*				LscMdl::m_pDevD3D	= NULL;

LscMdl::ILcCont*	LscMdl::m_pContAuto	= NULL;
LscMdl::ILcCont*	LscMdl::m_pContStat	= NULL;


INT LscMdl::Create(ILcDev* pDev)
{
	LscMdl::m_pDevSys	= pDev;
	LscMdl::m_pDevD3D	= LcDev_GetD3Device(&m_pDevSys);

	if(NULL == LscMdl::m_pDevD3D)
		return -1;

	return 0;
}



static const luaL_reg regLscxMdl[] =
{
	{"New"		,	LscMdl::Lsc_Alloc		},
	{"Release"	,	LscMdl::Lsc_Release		},
	{"Render"	,	LscMdl::Lsc_Render		},
	{"FrameMove",	LscMdl::Lsc_FrameMove	},
	
	{"SetRotation"	,	LscMdl::Lsc_LcmRotation		},
	{"SetScaling"	,	LscMdl::Lsc_LcmScaling		},
	{"SetPosition"	,	LscMdl::Lsc_LcmPosition		},
	{"SetStartFrame",	LscMdl::Lsc_LcmStartFrame	},
	
	{NULL, NULL},
};


INT LscMdl::OpenLib(lua_State *pL)
{
	luaL_openlib(pL, "Lxlcm", regLscxMdl, 0);
	return 1;
}


// X-File
INT	LscMdl::Lsc_Alloc(lua_State *pL)
{
	INT		nKey=-1;
	char	sMdl[160] ={0};
	char	sTex[160] ={0};
	char	sEft[160] ={0};

	ILcLsc::LscH* pOrg	= NULL;

	INT		n = lua_gettop(pL);
	
	if(n<2)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	strcpy(sMdl, lua_tostring(pL, 1));
	strcpy(sTex, lua_tostring(pL, 2));

	if(n>=3)
		strcpy(sEft, lua_tostring(pL, 3));


	//1. Lcm을 Origin에서 찾는다.
	pOrg	= LscMdl::m_pContAuto->FindOrg(sMdl);

	//2. Lcm이 없으면 생성하고 Origin에 등록을 한다.
	if(!pOrg)
	{
		if(FAILED(LscMdl::m_pContAuto->AllocOrg(sMdl, sTex, sEft)))
		{
			lua_pushnumber(pL, -1);
			return 1;
		}

		//3. Lcm을 다시 Origin에서 찾아서 인스턴스를 생성한다.
		pOrg	= LscMdl::m_pContAuto->FindOrg(sMdl);

		if(!pOrg)
		{
			lua_pushnumber(pL, -1);
			return 1;
		}
	}

	nKey	= LscMdl::m_pContAuto->Alloc(sMdl, pOrg);
	
	// Key를 돌려 준다.
	lua_pushnumber(pL, nKey);
	return 1;
}




INT LscMdl::Lsc_Release(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	INT iSize = LscMdl::m_pContAuto->Release(nKey);
	
	// 남아 있는 사이즈를 돌려준다.
	lua_pushnumber(pL, iSize);
	return 1;
}


INT LscMdl::Lsc_FrameMove(lua_State *pL)
{
	INT		hr = -1;
	INT		nKey=-1;
	ILcLsc::LscH* p	= NULL;

	INT		n = lua_gettop(pL);

	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	

	nKey= (INT)lua_tonumber(pL, 1);
	p	= LscMdl::m_pContAuto->Find(nKey);

	if(NULL ==p || NULL ==p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	Lcm::ILcmMdl* pObj = (Lcm::ILcmMdl*)p->pIns;
	pObj->FrameMove();
	
	lua_pushnumber(pL, 0);
	return 1;
}


INT LscMdl::Lsc_Render(lua_State *pL)
{
	INT		hr = -1;
	INT		nKey=-1;
	ILcLsc::LscH* p	= NULL;

	INT		n = lua_gettop(pL);

	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey= (INT)lua_tonumber(pL, 1);
	p	= LscMdl::m_pContAuto->Find(nKey);

	if(NULL ==p || NULL ==p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	Lcm::ILcmMdl* pObj = (Lcm::ILcmMdl*)p->pIns;
	pObj->Render();
	
	lua_pushnumber(pL, 0);
	return 1;
}


INT LscMdl::Lsc_LcmRotation(lua_State *pL)
{
	INT		hr = -1;
	INT		nKey=-1;
	ILcLsc::LscH* p	= NULL;

	INT		n = lua_gettop(pL);

	if(n<4)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey= (INT)lua_tonumber(pL, 1);
	p	= LscMdl::m_pContAuto->Find(nKey);

	if(NULL ==p || NULL ==p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	D3DXVECTOR4	vcRot(0,0,0, 0);

	vcRot.x	= (FLOAT)lua_tonumber(pL, 2);
	vcRot.y	= (FLOAT)lua_tonumber(pL, 3);
	vcRot.z	= (FLOAT)lua_tonumber(pL, 4);
	vcRot.w	= (FLOAT)lua_tonumber(pL, 5);
	
	vcRot.w	= D3DXToRadian(vcRot.w);

	Lcm::ILcmMdl*	pObj = (Lcm::ILcmMdl*)p->pIns;
	pObj->SetRotation( (FLOAT*)&vcRot);

	lua_pushnumber(pL, 0);
	return 1;
}



INT LscMdl::Lsc_LcmScaling(lua_State *pL)
{
	INT		hr = -1;
	INT		nKey=-1;
	ILcLsc::LscH* p	= NULL;

	INT		n	= lua_gettop(pL);

	if(n<4)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey= (INT)lua_tonumber(pL, 1);
	p	= LscMdl::m_pContAuto->Find(nKey);

	if(NULL ==p || NULL ==p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	D3DXVECTOR3	vcScl(0,0,0);

	vcScl.x	= (FLOAT)lua_tonumber(pL, 2);
	vcScl.y	= (FLOAT)lua_tonumber(pL, 3);
	vcScl.z	= (FLOAT)lua_tonumber(pL, 4);

	Lcm::ILcmMdl*	pObj = (Lcm::ILcmMdl*)p->pIns;
	pObj->SetScaling((FLOAT*) &vcScl);

	lua_pushnumber(pL, 0);
	return 1;
}


INT LscMdl::Lsc_LcmPosition(lua_State *pL)
{
	INT		hr = -1;
	INT		nKey=-1;
	ILcLsc::LscH* p	= NULL;

	INT		n = lua_gettop(pL);

	if(n<4)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey= (INT)lua_tonumber(pL, 1);
	p	= LscMdl::m_pContAuto->Find(nKey);

	if(NULL ==p || NULL ==p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	D3DXVECTOR3	vcPos(0,0,0);

	vcPos.x	= (FLOAT)lua_tonumber(pL, 2);
	vcPos.y	= (FLOAT)lua_tonumber(pL, 3);
	vcPos.z	= (FLOAT)lua_tonumber(pL, 4);

	Lcm::ILcmMdl*	pObj = (Lcm::ILcmMdl*)p->pIns;
	pObj->SetPosition((FLOAT*)&vcPos);

	lua_pushnumber(pL, 0);
	return 1;
}



INT LscMdl::Lsc_LcmStartFrame(lua_State* pL)
{
	INT		hr = -1;
	INT		nKey=-1;
	ILcLsc::LscH* p	= NULL;

	INT		n = lua_gettop(pL);

	if(n<2)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey= (INT)lua_tonumber(pL, 1);
	p	= LscMdl::m_pContAuto->Find(nKey);

	if(NULL ==p || NULL ==p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	DWORD dStartFrame = (DWORD)lua_tonumber(pL, 2);
	
	Lcm::ILcmMdl*	pObj = (Lcm::ILcmMdl*)p->pIns;
	pObj->Query("Set Start Frame", &dStartFrame);

	lua_pushnumber(pL, 0);
	return 1;
}



INT	LscMdl::ContainerCreate()
{
	LscMdl::m_pContAuto = new LscMdl::ILcCont;

	return 0;
}


INT	LscMdl::ContainerRestore()
{
	if(FAILED(LscMdl::m_pContAuto->Restore()))
		return -1;

	if(FAILED(LscMdl::m_pContStat->Restore()))
		return -1;

	return 0;
}

INT	LscMdl::ContainerInvalidate()
{
	LscMdl::m_pContAuto->Invalidate();
	LscMdl::m_pContStat->Invalidate();

	return 0;
}


INT	LscMdl::ContainerDestroy()
{
	if(LscMdl::m_pContAuto)
	{
		delete LscMdl::m_pContAuto;
		LscMdl::m_pContAuto = NULL;
	}

	return 0;
}


INT	LscMdl::StaticContainerCreate()
{
	if(NULL == LscMdl::m_pContStat)
		LscMdl::m_pContStat = new LscMdl::ILcCont;

	return 0;
}


INT	LscMdl::StaticContainerDestroy()
{
	if(LscMdl::m_pContStat)
	{
		delete LscMdl::m_pContStat;
		LscMdl::m_pContStat = NULL;
	}

	return 0;
}


}// namespace Lua