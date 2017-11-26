// Implementation of the LscFont class.
//
////////////////////////////////////////////////////////////////////////////////


#include <windows.h>
#include <d3dx9.h>

#include <Lua/lua.h>
#include <Lua/lualib.h>
#include <Lua/lauxlib.h>

#include <Lc/ILcFont.h>
#include <Lc/LcEuclid.h>

#include <Lc/ILcDev.h>

#include <Lc/ILcLsc.h>


#include "LscFont.h"															// Lua Font Class


namespace	LcLua
{
	
// For Container
////////////////////////////////////////////////////////////////////////////////

LscFont::ILcCont::~ILcCont()
{
	if(!vLscH.empty())
	{
		INT iSize = vLscH.size();

		for(INT i=0; i<iSize; ++i)
		{
			ILcFont* pDst = (ILcFont*)(vLscH[i]->pIns);
			delete pDst;

			delete vLscH[i];
		}

		vLscH.clear();
	}
}



INT LscFont::ILcCont::Alloc(void**	pOut
						,	void*	pDev		// LPDIRECT3DDEVICE9
						,	char*	sName
						,	INT		lHeight
						,	INT		lWeight
						,	INT		lItalic
						,	DWORD	dS
						,	DWORD	dB
						,	INT		iThckX
						,	INT		iThckY
						)
{
	INT			nKey	=-1;
	
	INT			hr		=-1;
	ILcFont*	pFont	= NULL	;

	TLcFnt		Tfont(sName, lHeight, lWeight, lItalic, iThckX, iThckY, dS, dB);

	hr = LcFont_Create("PC Font", &pFont, pDev, &Tfont);

	if(FAILED(hr))
		return -1;

	ILcLsc::LscH*	pLH	= new ILcLsc::LscH;

	pLH->pIns	= pFont;

	nKey = vLscH.size();
	vLscH.push_back(pLH);

	return nKey;
}

INT LscFont::ILcCont::Release(INT nKey)
{
	INT iSize = vLscH.size();

	if(iSize<=nKey || nKey<0)
		return -1;

	ILcLsc::LscH** pLsc  = vLscH.begin() + nKey;
	ILcFont* pDst = (ILcFont*)( (*pLsc)->pIns);

	if(pDst)
		delete pDst;

	vLscH.erase( pLsc);

	return vLscH.size();
}

ILcLsc::LscH* LscFont::ILcCont::Find(INT nKey)
{
	INT iSize = vLscH.size();

	if(iSize<=nKey || nKey<0)
		return NULL;

	return vLscH[nKey];
}



////////////////////////////////////////////////////////////////////////////////
// Script

ILcDev*				LscFont::m_pDevSys		= NULL;
void*				LscFont::m_pDevD3D		= NULL;
LscFont::ILcCont*	LscFont::m_pContAuto	= NULL;
LscFont::ILcCont*	LscFont::m_pContStat	= NULL;


INT LscFont::Create(ILcDev* pDev)
{
	LscFont::m_pDevSys	= pDev;
	LscFont::m_pDevD3D	= LcDev_GetD3Device(&m_pDevSys);

	if(NULL == LscFont::m_pDevD3D)
		return -1;

	return 0;
}




static const luaL_reg regLscFont[] =
{
	{"New"		,	LscFont::Lsc_Alloc			},
	{"Release"	,	LscFont::Lsc_Release		},

	{"Draw"		,	LscFont::Lsc_FontDraw		},
	{"ColorFore",	LscFont::Lsc_FontColorFore	},
	{"ColorBack",	LscFont::Lsc_FontColorBack	},
	{NULL, NULL},
};


INT LscFont::OpenLib(lua_State *pL)
{
	luaL_openlib(pL, "Lfont", regLscFont, 0);
	return 1;
}


#define MTFNT_DX	0
#define MTFNT_LN	1


// Font
// Name, Height, Font Weight , Italic
INT	LscFont::Lsc_Alloc(lua_State *pL)
{
	INT		nKey=-1;

	char	sName[64]={0};
	DWORD	lH = 12;
	DWORD	lW = FW_NORMAL;
	DWORD	iL = 0;

	DWORD	dColorStr = 0xFFFFFFFF;
	DWORD	dColorBck = 0xFFFFFFFF;
	INT		iThckX = 0;
	INT		iThckY = 0;

	INT		n = lua_gettop(pL);
	
	if(n<3)
	{
		lua_pushnumber(pL, -1);
//		LcUtil_ErrMsgBox("Font Load Error");
		return 1;
	}
	
	strcpy(sName, lua_tostring(pL, 1));
	
	lH = (DWORD)lua_tonumber(pL, 2);
	lW = (DWORD)lua_tonumber(pL, 3);

	if(4 <=n)
		iL = (DWORD)lua_tonumber(pL, 4);

	if(5 <=n && 8>=n)
	{
		char	sColor[32];
		strcpy(sColor, lua_tostring(pL, 5));
		sscanf(sColor,"%x", &dColorStr);

		strcpy(sColor, lua_tostring(pL, 6));
		sscanf(sColor,"%x", &dColorBck);

		iThckX = (INT)lua_tonumber(pL, 7);
		iThckY = (INT)lua_tonumber(pL, 8);
	}

	DWORD lWeight = FW_NORMAL;

	if(0 == lWeight)
		lWeight = FW_THIN;
	else if(1 == lWeight)
		lWeight = FW_NORMAL;
	else 
		lWeight = FW_BOLD;



	nKey = LscFont::m_pContAuto->Alloc(NULL, m_pDevD3D, sName, lH, lWeight, iL, dColorStr, dColorBck, iThckX, iThckY);
	
	lua_pushnumber(pL, nKey);
	
	return 1;
}




INT LscFont::Lsc_Release(lua_State *pL)
{
	INT		nKey=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey = (INT)lua_tonumber(pL, 1);
	
	INT iSize = LscFont::m_pContAuto->Release(nKey);
	
	// 남아 있는 사이즈를 돌려준다.
	lua_pushnumber(pL, iSize);
	
	return 1;
}



INT LscFont::Lsc_FontDraw(lua_State *pL)
{
	INT		nKey=-1;
	
	char	sMsg[512]={0};
	RECT	pRc;
	DWORD	fontColor=LCXCOLOR(1,1,1,1);
	char	sColor[32]={0};
	DWORD	vAlign=0;
	
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	nKey =	(INT)lua_tonumber(pL, 1);
	char* s=(char*)lua_tostring(pL, 2);
	
	if(NULL== s || strlen(s)<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	

	strcpy(sMsg, s);
	
	pRc.left	= (LONG)lua_tonumber(pL, 3);
	pRc.top		= (LONG)lua_tonumber(pL, 4);
	
	strcpy(sColor, lua_tostring(pL, 5));
	sscanf(sColor,"%x", &fontColor);
	
	if(n>5)
	{
		vAlign = (DWORD)lua_tonumber(pL, 6);
	}
	
	
	ILcLsc::LscH* p = LscFont::m_pContAuto->Find(nKey);
	
	
	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	

	ILcFont*	pFnt = (ILcFont*)p->pIns;
	FLOAT vcPos[2] ={FLOAT(pRc.left), FLOAT(pRc.top)};

	LCXCOLOR dColor = fontColor;

	pFnt->SetString(sMsg);
	pFnt->SetPos(vcPos);
	pFnt->SetPosAlign(vAlign);
	pFnt->SetColorDiffuse((FLOAT*)&dColor);
	pFnt->DrawTxt();
	

	lua_pushnumber(pL, 0);
	return 1;
}



INT	LscFont::Lsc_FontColorFore(lua_State *pL)
{
	INT		hr=-1;
	INT		nKey=-1;
	DWORD	dColor=LCXCOLOR(1,1,1,1);
	char	sColor[32]={0};
	
	INT		n = lua_gettop(pL);
	
	if(n<2)
	{
		lua_pushnumber(pL, hr);
		return 1;
	}
	
	nKey =	(INT)lua_tonumber(pL, 1);

	strcpy(sColor, lua_tostring(pL, 2));
	sscanf(sColor,"%x", &dColor);

	ILcLsc::LscH* p = LscFont::m_pContAuto->Find(nKey);
	
	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, hr);
		return 1;
	}

	ILcFont*	pFnt = (ILcFont*)p->pIns;

	if(pFnt && ILcFont::ELcFont::LPFNT_CE == pFnt->GetType() )
	{
		LCXCOLOR xColor = dColor;

		hr = 1;
		pFnt->SetColorFore((FLOAT*)&xColor);
	}
	
	lua_pushnumber(pL, hr);
	return 1;
}


INT	LscFont::Lsc_FontColorBack(lua_State *pL)
{
	INT		hr=-1;
	INT		nKey=-1;
	DWORD	dColor=LCXCOLOR(1,1,1,1);
	char	sColor[32]={0};
	
	INT		n = lua_gettop(pL);
	
	if(n<2)
	{
		lua_pushnumber(pL, hr);
		return 1;
	}
	
	nKey =	(INT)lua_tonumber(pL, 1);

	strcpy(sColor, lua_tostring(pL, 2));
	sscanf(sColor,"%x", &dColor);

	ILcLsc::LscH* p = LscFont::m_pContAuto->Find(nKey);
	
	if(NULL == p || NULL == p->pIns)
	{
		lua_pushnumber(pL, hr);
		return 1;
	}

	ILcFont*	pFnt = (ILcFont*)p->pIns;

	if(pFnt && ILcFont::ELcFont::LPFNT_CE == pFnt->GetType() )
	{
		LCXCOLOR xColor = dColor;

		hr = 1;
		pFnt->SetColorBack((FLOAT*)&xColor);
	}
	
	lua_pushnumber(pL, hr);
	return 1;
}



INT	LscFont::ContainerCreate()
{
	LscFont::m_pContAuto = new LscFont::ILcCont;

	return 0;
}

INT	LscFont::ContainerDestroy()
{
	if(LscFont::m_pContAuto)
	{
		delete LscFont::m_pContAuto;
		LscFont::m_pContAuto = NULL;
	}

	return 0;
}


INT	LscFont::StaticContainerCreate()
{
	if(NULL == LscFont::m_pContStat)
		LscFont::m_pContStat = new LscFont::ILcCont;

	return 0;
}

INT	LscFont::StaticContainerDestroy()
{
	if(LscFont::m_pContStat)
	{
		delete LscFont::m_pContStat;
		LscFont::m_pContStat = NULL;
	}

	return 0;
}


}// namespace Lua