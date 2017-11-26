// Implementation of the LscTex class.
//
////////////////////////////////////////////////////////////////////////////////


#include <windows.h>
#include <d3dx9.h>

#include <Lua/lua.h>
#include <Lua/lualib.h>
#include <Lua/lauxlib.h>

#include <Lc/LcEuclid.h>
#include <Lc/ILcTex.h>
#include <Lc/ILcDev.h>
#include <Lc/LcType.h>

#include <Lc/ILcLsc.h>


#include "LscTex.h"


#ifndef SAFE_DELETE
#	define SAFE_DELETE(p)	{	if(p){delete (p);p = NULL;	}}
#endif


namespace	LcLua
{
	
// For Container
////////////////////////////////////////////////////////////////////////////////

LscTex::ILcCont::~ILcCont()
{
	if(!vLscH.empty())
	{
		INT iSize = vLscH.size();

		for(INT i=0; i<iSize; ++i)
		{
			ILcTex* pDst = (ILcTex*)(vLscH[i]->pIns);
			delete pDst;

			delete vLscH[i];
		}

		vLscH.clear();
	}
}


INT LscTex::ILcCont::Alloc(char* sFile, DWORD dc, DWORD dF)
{
	INT		nKey=-1;
	ILcLsc::LscH* p = this->Find(&nKey, sFile);

	if(NULL == p)
	{
		INT		hr=-1;
		ILcTex*	pObj = NULL;
		hr = LcTex_CreateTexture("File", &pObj, LscTex::m_pDevD3D, sFile, NULL, dc, dF);
	
		if(FAILED(hr))
			return -1;

		ILcLsc::LscH* pLH = new	ILcLsc::LscH;

		pLH->pIns	= pObj;
		strcpy(pLH->sName, sFile);

		nKey = vLscH.size();
		vLscH.push_back(pLH);
	}

	return nKey;
}


INT LscTex::ILcCont::ReAlloc(INT nKey, char* sFile, DWORD dc, DWORD dF)
{
	INT			hr	=-1;
	ILcTex*		pObj= NULL;

	ILcLsc::LscH* p	= this->Find(nKey);
	
	if(p && p->pIns)
	{
		// 있다면 이전 Object를 해제하고, 새로운 Object를 할당한다.
		hr = LcTex_CreateTexture("File", &pObj, LscTex::m_pDevD3D, sFile, NULL, dF, dc);

		if(FAILED(hr))
			return -1;

		ILcTex*	pTmp = (ILcTex*)p->pIns;
		SAFE_DELETE(	pTmp	);

		p->pIns = pTmp;
		strcpy(p->sName, sFile);
	}
	else
	{
		// 없으면 메모리를 할당한다.
		nKey = this->Alloc(sFile, dc, dF);
	}

	return nKey;
}





INT LscTex::ILcCont::Release(INT nKey)
{
	INT iSize = vLscH.size();

	if(iSize<=nKey || nKey<0)
		return -1;

	ILcLsc::LscH** pLa  = vLscH.begin() + nKey;
	ILcTex*  pDst = (ILcTex*)( (*pLa)->pIns);

	if(pDst)
		delete pDst;

	vLscH.erase( pLa);

	return vLscH.size();
}

ILcLsc::LscH* LscTex::ILcCont::Find(INT nKey)
{
	INT iSize = vLscH.size();

	if(iSize<=nKey || nKey<0)
		return NULL;

	return vLscH[nKey];
}

ILcLsc::LscH* LscTex::ILcCont::Find(INT* nIdx, char* sFile)
{
	if(nIdx)
		*nIdx = -1;

	if(NULL == sFile || strlen(sFile)<1)
		return NULL;


	INT iSize = vLscH.size();

	for(INT i=0; i<iSize; ++i)
	{
		if(0 == _stricmp(sFile, vLscH[i]->sName))
		{
			if(nIdx)
				*nIdx = i;
			
			return vLscH[i];
		}
	}

	return NULL;
}



////////////////////////////////////////////////////////////////////////////////
// Script

ILcDev*				LscTex::m_pDevSys	= NULL;
ILcDevSprite*		LscTex::m_pDev2D	= NULL;
LscTex::ILcCont*	LscTex::m_pContAuto	= NULL;
LscTex::ILcCont*	LscTex::m_pContStat	= NULL;
void*				LscTex::m_pDevD3D	= NULL;


INT LscTex::Create(ILcDev* pDev)
{
	LscTex::m_pDevSys	= pDev;
	LscTex::m_pDevD3D	= LcDev_GetD3Device(&m_pDevSys);

	if(NULL == LscTex::m_pDevD3D)
		return -1;

	void*	pData = NULL;
	if(FAILED(m_pDevSys->Query("Get 2D Sprite", &pData)))
		return -1;

	LscTex::m_pDev2D = (ILcDevSprite*)pData;
	return 0;
}



static const luaL_reg regLscTex[] =
{
	{"New"		,	LscTex::Lsc_Alloc			},
	{"ReAlloc"	,	LscTex::Lsc_ReAlloc			},
	{"Release"	,	LscTex::Lsc_Release			},

	{"Width"	,	LscTex::Lsc_TextureWidth	},
	{"Height"	,	LscTex::Lsc_TextureHeight	},
	{"Pointer"	,	LscTex::Lsc_TexturePointer	},
	{"Draw"		,	LscTex::Lsc_TextureDraw		},
	{"DrawMask"	,	LscTex::Lsc_TextureDrawMask	},

	{"SetTexture"		,	LscTex::Lsc_SetTexture		},
	{"SetTextureNull"	,	LscTex::Lsc_SetTextureNull	},

	{NULL, NULL},
};


INT LscTex::OpenLib(lua_State *pL)
{
	luaL_openlib(pL, "Ltex", regLscTex, 0);
	return 1;
}


// Texture
INT	LscTex::Lsc_Alloc(lua_State *pL)
{
	INT		nKey=-1;
	char	sFile[160]={0};
	char	sColor[32]={0};
	char	sFilter[32]={0};

	DWORD	dc=0x00FFFFFF;
	DWORD	dF=D3DX_FILTER_NONE;

	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	else if(n < 2)
	{
		strcpy(sFile, lua_tostring(pL, 1));
	}
	
	else if(n < 3)
	{
		strcpy(sFile, lua_tostring(pL, 1));
		strcpy(sColor, lua_tostring(pL, 2));
		
		sscanf(sColor, "%x", &dc);
	}
	
	else if(n < 4)
	{
		strcpy(sFile, lua_tostring(pL, 1));
		strcpy(sColor, lua_tostring(pL, 2));
		strcpy(sFilter, lua_tostring(pL, 3));
		
		sscanf(sColor, "%x", &dc);

		if(0==_stricmp(sFilter, "Linear"))
			dF = D3DX_FILTER_LINEAR;
	}

	else
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = LscTex::m_pContAuto->Alloc(sFile, dc, dF);
	
	// Key를 돌려 준다.
	lua_pushnumber(pL, nKey);
	
	return 1;
}




// Texture
INT	LscTex::Lsc_ReAlloc(lua_State *pL)
{
	INT		nKey=-1;

	char	sFile[160]={0};
	char	sColor[32]={0};
	DWORD	dc=0x00FFFFFF;
	DWORD	dF=D3DX_FILTER_NONE;
	
	INT		n = lua_gettop(pL);
	
	
	if(n<2)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	if(n < 3)
	{
		nKey = (INT)lua_tonumber(pL, 1);
		strcpy(sFile, lua_tostring(pL, 2));
	}
	
	else if(n < 4)
	{
		nKey = (INT)lua_tonumber(pL, 1);
		strcpy(sFile, lua_tostring(pL, 2));
		strcpy(sColor, lua_tostring(pL, 3));
		sscanf(sColor, "%x", &dc);
	}
	
	else
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	if(strlen(sFile)<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	
	nKey = LscTex::m_pContAuto->ReAlloc(nKey, sFile, dc, dF);
	
	// Key를 돌려 준다.
	lua_pushnumber(pL, nKey);
	return 1;
}




INT LscTex::Lsc_Release(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	INT iSize = LscTex::m_pContAuto->Release(nKey);
	
	// 남아 있는 사이즈를 돌려준다.
	lua_pushnumber(pL, iSize);
	
	return 1;
}


INT LscTex::Lsc_TextureWidth(lua_State *pL)
{
	INT		nKey=-1;
	INT		iWidth=-1;
	INT		n = lua_gettop(pL);

	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	ILcLsc::LscH* p	= LscTex::m_pContAuto->Find(nKey);

	if(NULL ==p || NULL ==p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}


	ILcTex*	pTx = (ILcTex*)p->pIns;
	iWidth = pTx->GetWidth();
	
	lua_pushnumber(pL, iWidth);

	return 1;
}


INT LscTex::Lsc_TextureHeight(lua_State *pL)
{
	INT		nKey=-1;
	INT		iHeight=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);

	ILcLsc::LscH* p	= LscTex::m_pContAuto->Find(nKey);

	if(NULL ==p || NULL ==p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	ILcTex*	pTx = (ILcTex*)p->pIns;
	iHeight = pTx->GetHeight();
	
	lua_pushnumber(pL, iHeight);

	return 1;
}


INT LscTex::Lsc_TexturePointer(lua_State *pL)
{
	INT		nKey=-1;
	DWORD	nPointer=0;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);

	ILcLsc::LscH* p	= LscTex::m_pContAuto->Find(nKey);

	if(NULL ==p || NULL ==p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	ILcTex*	pTx = (ILcTex*)p->pIns;
	nPointer	= (DWORD)pTx->GetPointer();
	
	lua_pushnumber(pL, nPointer);

	return 1;
}




INT LscTex::Lsc_TextureDraw(lua_State *pL)
{
	INT		nKey=-1;

	RECT	pRc;
	VEC2	vcPos;
	VEC2	vcScl(1,1);
	VEC2	vcRot(0,0);
	FLOAT	fAngle = 0;
	DWORD	dC=D3DXCOLOR(1,1,1,1);
	char	sC[16];

	ILcTex*	pTx = NULL;
	
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	pRc.left	= (LONG)lua_tonumber(pL, 2);
	pRc.top		= (LONG)lua_tonumber(pL, 3);
	pRc.right	= (LONG)lua_tonumber(pL, 4);
	pRc.bottom	= (LONG)lua_tonumber(pL, 5);
	
	// 위치
	vcPos.x		= (FLOAT)lua_tonumber(pL, 6);
	vcPos.y		= (FLOAT)lua_tonumber(pL, 7);
	
	// 스케일
	if(n>7)
	{
		vcScl.x		= (FLOAT)lua_tonumber(pL, 8);
		vcScl.y		= (FLOAT)lua_tonumber(pL, 9);
	}
	
	// 색상
	if(n>9)
	{
		strcpy(sC, lua_tostring(pL, 10));
		sscanf(sC, "%x", &dC);
	}
	
	
	ILcLsc::LscH* p	= LscTex::m_pContAuto->Find(nKey);
	
	if(NULL == p || NULL== p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	
	LscTex::m_pDev2D->Draw(p->pIns, NULL, &pRc, NULL, (FLOAT*)&vcScl, (FLOAT*)&vcPos, dC);

	lua_pushnumber(pL, 0);
	return 1;
}


INT LscTex::Lsc_TextureDrawMask(lua_State *pL)
{
	INT		nKey0 = -1;
	INT		nKey1 = -1;

	RECT	pRc0;
	RECT	pRc1;
	VEC2	vcPos;
	VEC2	vcScl(1,1);
	VEC2	vcRot(0,0);
	FLOAT	fAngle = 0;
	DWORD	dC=D3DXCOLOR(1,1,1,1);
	char	sC[16];

	ILcTex*	pTx = NULL;
	
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey0 = (INT)lua_tonumber(pL, 1);
	nKey1 = (INT)lua_tonumber(pL, 2);
	
	pRc0.left	= (LONG )lua_tonumber(pL,  3);
	pRc0.top	= (LONG )lua_tonumber(pL,  4);
	pRc0.right	= (LONG )lua_tonumber(pL,  5);
	pRc0.bottom	= (LONG )lua_tonumber(pL,  6);

	pRc1.left	= (LONG )lua_tonumber(pL,  7);
	pRc1.top	= (LONG )lua_tonumber(pL,  8);
	pRc1.right	= (LONG )lua_tonumber(pL,  9);
	pRc1.bottom	= (LONG )lua_tonumber(pL, 10);
	
	// 위치
	vcPos.x		= (FLOAT)lua_tonumber(pL, 11);
	vcPos.y		= (FLOAT)lua_tonumber(pL, 12);
	
	// 스케일
	if(n>12)
	{
		vcScl.x		= (FLOAT)lua_tonumber(pL, 13);
		vcScl.y		= (FLOAT)lua_tonumber(pL, 14);
	}
	
	// 색상
	if(n>14)
	{
		strcpy(sC, lua_tostring(pL, 15));
		sscanf(sC, "%x", &dC);
	}
	
	
	ILcLsc::LscH* p0	= LscTex::m_pContAuto->Find(nKey0);
	ILcLsc::LscH* p1	= LscTex::m_pContAuto->Find(nKey1);
	
	void* pTx0	= NULL;
	void* pTx1	= NULL;
	
	if(NULL == p0 || NULL== p0->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	pTx0	= p0->pIns;
	
	if(p1)
		pTx1	= p1->pIns;
	
	LscTex::m_pDev2D->Draw(pTx0, pTx1, &pRc0, &pRc1, (FLOAT*)&vcScl, (FLOAT*)&vcPos, dC);

	lua_pushnumber(pL, 0);
	return 1;
}



INT LscTex::Lsc_SetTexture(lua_State *pL)
{
	INT		hr=-1;
	INT		nKey=-1;
	INT		nStage=-1;
	INT		n = lua_gettop(pL);
		
	nKey	= (INT)lua_tonumber(pL, 1);
	nStage	= (INT)lua_tonumber(pL, 2);
	
	ILcLsc::LscH* p	= LscTex::m_pContAuto->Find(nKey);
	
	if(NULL == p || NULL== p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	PDEV	pDev	= (PDEV)(LscTex::m_pDevD3D);
	ILcTex*	pObj	= (ILcTex*)(p->pIns);
	PDTX	pTxD3d	= (PDTX)pObj->GetPointer();

	hr = pDev->SetTexture( nStage, pTxD3d);

	if(FAILED(hr))
		hr =-1;
	else
		hr =0;

	lua_pushnumber(pL, hr);
	
	return 1;
}




INT LscTex::Lsc_SetTextureNull(lua_State *pL)
{
	INT		hr=-1;
	INT		nStage=-1;
	INT		n = lua_gettop(pL);
	
	nStage = (INT)lua_tonumber(pL, 1);

	PDEV	pDev	= (PDEV)(LscTex::m_pDevD3D);
		
	hr = pDev->SetTexture( nStage, NULL);

	if(FAILED(hr))
		hr =-1;
	else
		hr =0;

	lua_pushnumber(pL, hr);
	return 1;
}



INT	LscTex::ContainerCreate()
{
	LscTex::m_pContAuto = new LscTex::ILcCont;

	return 0;
}

INT	LscTex::ContainerDestroy()
{
	if(LscTex::m_pContAuto)
	{
		delete LscTex::m_pContAuto;
		LscTex::m_pContAuto = NULL;
	}

	return 0;
}


INT	LscTex::StaticContainerCreate()
{
	if(NULL == LscTex::m_pContStat)
		LscTex::m_pContStat = new LscTex::ILcCont;

	return 0;
}

INT	LscTex::StaticContainerDestroy()
{
	if(LscTex::m_pContStat)
	{
		delete LscTex::m_pContStat;
		LscTex::m_pContStat = NULL;
	}

	return 0;
}

}// namespace Lua