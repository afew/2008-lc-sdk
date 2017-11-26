// Implementation of the LscxMshX class.
//
////////////////////////////////////////////////////////////////////////////////


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
#include <Lc/ILcxXfile.h>

#include "LscxMshX.h"


#ifndef SAFE_DELETE
#	define SAFE_DELETE(p)	{	if(p){delete (p);p = NULL;	}}
#endif



namespace	LcLua
{
	
// For Container
////////////////////////////////////////////////////////////////////////////////

LscxMshX::ILcCont::~ILcCont()
{
	if(!vLscH.empty())
	{
		INT iSize = vLscH.size();

		for(INT i=0; i<iSize; ++i)
		{
			ILcxXfile* pDst = (ILcxXfile*)(vLscH[i]->pIns);
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
			ILcxXfile* pDst = (ILcxXfile*)(vLscHorg[i]->pIns);
			delete pDst;

			delete vLscHorg[i];
		}

		vLscHorg.clear();
	}
}


INT LscxMshX::ILcCont::Alloc(char* sName, void* pLscHorg)
{
	INT			hr	=-1;
	ILcxXfile*	pObj = NULL;
	ILcxXfile::XmshInst	Env;

	Env.fTrackPosition = 1 + rand()%10;
	Env.fTrackPosition *=100.f;

	Env.fTrackSpeed = 10 + rand()%10;
	Env.fTrackSpeed *=0.05f;

	Env.fTrackPosition	= 0.f;
	Env.fTrackSpeed		= 1.f;

	ILcLsc::LscH*	_t = (ILcLsc::LscH*)pLscHorg;
	ILcxXfile*		pOrg = (ILcxXfile*)_t->pIns;

	hr = LcxXfile_Create("Instance", &pObj, pOrg, &Env);

	if(FAILED(hr))
		return -1;

	ILcLsc::LscH* pLH	= new ILcLsc::LscH;


	pLH->pIns	= pObj;
	hr = vLscH.size();
	vLscH.push_back(pLH);

	return hr;
}



INT LscxMshX::ILcCont::Release(INT nKey)
{
	INT iSize = vLscH.size();

	if(iSize<=nKey || nKey<0)
		return -1;

	ILcLsc::LscH** pLa  = vLscH.begin() + nKey;
	ILcxXfile*  pDst = (ILcxXfile*)( (*pLa)->pIns);

	if(pDst)
		delete pDst;

	vLscH.erase( pLa);

	return vLscH.size();
}

ILcLsc::LscH* LscxMshX::ILcCont::Find(INT nKey)
{
	INT iSize = vLscH.size();

	if(iSize<=nKey || nKey<0)
		return NULL;

	return vLscH[nKey];
}



INT LscxMshX::ILcCont::AllocOrg(char* sName, char* sTxFolder, char* sEft)
{
	INT		hr	=-1;
	PDEV	pDev= (PDEV)LscxMshX::m_pDevD3D;

	ILcxXfile*	pObj= NULL;
	void*		pEft = LcD3D_EffectFileBuild(pDev, sEft);
	
	if(NULL == pEft)
	{
		MessageBox( GetActiveWindow(), "Build Shader Error", "Err", MB_OK | MB_ICONERROR);
		return -1;
	}

	hr = LcxXfile_Create("Origin", &pObj, LscxMshX::m_pDevD3D, sName, pEft);

	if(FAILED(hr))
		return -1;


	INT		i;
	INT		nTex = 0;

	
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
		hr = LcTex_TextureLoadFromFile((void**)&pTx0, NULL, LscxMshX::m_pDevD3D, sTxPath);

		// Normal
		sprintf(sTxPath, "%s%s_n.png", sTxFolder, sFile);
		hr = LcTex_TextureLoadFromFile((void**)&pTx1, NULL, LscxMshX::m_pDevD3D, sTxPath);

		// Specular
		sprintf(sTxPath, "%s%s_s.png", sTxFolder, sFile);
		hr = LcTex_TextureLoadFromFile((void**)&pTx2, NULL, LscxMshX::m_pDevD3D, sTxPath);

		// Emissional
		sprintf(sTxPath, "%s%s_e.png", sTxFolder, sFile);
		hr = LcTex_TextureLoadFromFile((void**)&pTx3, NULL, LscxMshX::m_pDevD3D, sTxPath);
	


		struct _T	{	INT n;	ILcxXfile::TxLayer	pTx;	};
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


ILcLsc::LscH* LscxMshX::ILcCont::FindOrg(char* sName)
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


void LscxMshX::ILcCont::SetOrgElapseTime(FLOAT fTime)
{
	DOUBLE	dAvg=0;
	m_pDevSys->Query("Get Elapsed Average Time", &dAvg);


	INT iSize = vLscHorg.size();

	for(int i=0; i<iSize; ++i)
	{
		ILcLsc::LscH*	pLH = vLscHorg[i];
		ILcxXfile*		pOrg= (ILcxXfile*)pLH->pIns;

		pOrg->SetElapse(dAvg);
	}
}



////////////////////////////////////////////////////////////////////////////////
// Script

ILcDev*				LscxMshX::m_pDevSys	= NULL;
void*				LscxMshX::m_pDevD3D	= NULL;

LscxMshX::ILcCont*	LscxMshX::m_pContAuto	= NULL;
LscxMshX::ILcCont*	LscxMshX::m_pContStat	= NULL;


INT LscxMshX::Create(ILcDev* pDev)
{
	LscxMshX::m_pDevSys	= pDev;
	LscxMshX::m_pDevD3D	= LcDev_GetD3Device(&m_pDevSys);

	if(NULL == LscxMshX::m_pDevD3D)
		return -1;

	return 0;
}



static const luaL_reg regLscxMshX[] =
{
	{"New"		,	LscxMshX::Lsc_Alloc			},
	{"Release"	,	LscxMshX::Lsc_Release		},
	{"Render"	,	LscxMshX::Lsc_Render		},

	{NULL, NULL},
};


INT LscxMshX::OpenLib(lua_State *pL)
{
	luaL_openlib(pL, "Lxmsx", regLscxMshX, 0);
	return 1;
}


// X-File
INT	LscxMshX::Lsc_Alloc(lua_State *pL)
{
	INT		nKey =-1;

	char	sName[160] ={0};
	char	sTxFolder[160] ={0};
	char	sEft[160] ={0};

	ILcLsc::LscH*	pOrg	= NULL;

	INT		n	= lua_gettop(pL);
	
	if(n<2)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	strcpy(sName, lua_tostring(pL, 1));
	strcpy(sTxFolder, lua_tostring(pL, 2));

	if(n>=3)
		strcpy(sEft, lua_tostring(pL, 3));


	//1. XFile을 Origin에서 찾는다.
	pOrg	= LscxMshX::m_pContAuto->FindOrg(sName);

	//2. XFile이 없으면 생성하고 Origin에 등록을 한다.
	if(!pOrg)
	{
		if(FAILED(LscxMshX::m_pContAuto->AllocOrg(sName, sTxFolder, sEft)))
		{
			lua_pushnumber(pL, -1);
			return 1;
		}

		//3. XFile을 다시 Origin에서 찾아서 인스턴스를 생성한다.
		pOrg	= LscxMshX::m_pContAuto->FindOrg(sName);

		if(!pOrg)
		{
			lua_pushnumber(pL, -1);
			return 1;
		}
	}

	nKey	= LscxMshX::m_pContAuto->Alloc(sName, pOrg);
	
	// Key를 돌려 준다.
	lua_pushnumber(pL, nKey);
	return 1;
}




INT LscxMshX::Lsc_Release(lua_State *pL)
{
	INT		nKey =-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	INT iSize = LscxMshX::m_pContAuto->Release(nKey);
	
	// 남아 있는 사이즈를 돌려준다.
	lua_pushnumber(pL, iSize);
	return 1;
}


INT LscxMshX::Lsc_Render(lua_State *pL)
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
	p	= LscxMshX::m_pContAuto->Find(nKey);

	if(NULL ==p || NULL ==p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}


	DOUBLE	dAvg=0;
	m_pDevSys->Query("Get Elapsed Average Time", &dAvg);

	ILcxXfile*	pObj = (ILcxXfile*)p->pIns;
	dAvg *=0.001f;
	pObj->SetElapse(dAvg);
	pObj->Render();
	
	lua_pushnumber(pL, 0);
	return 1;
}



INT	LscxMshX::ContainerCreate()
{
	LscxMshX::m_pContAuto = new LscxMshX::ILcCont;

	return 0;
}


INT	LscxMshX::ContainerDestroy()
{
	if(LscxMshX::m_pContAuto)
	{
		delete LscxMshX::m_pContAuto;
		LscxMshX::m_pContAuto = NULL;
	}

	return 0;
}


INT	LscxMshX::StaticContainerCreate()
{
	if(NULL == LscxMshX::m_pContStat)
		LscxMshX::m_pContStat = new LscxMshX::ILcCont;

	return 0;
}


INT	LscxMshX::StaticContainerDestroy()
{
	if(LscxMshX::m_pContStat)
	{
		delete LscxMshX::m_pContStat;
		LscxMshX::m_pContStat = NULL;
	}

	return 0;
}


}// namespace Lua