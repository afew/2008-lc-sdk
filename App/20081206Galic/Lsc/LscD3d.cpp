// Implementation of the LscD3d class.
//
////////////////////////////////////////////////////////////////////////////////


#include <windows.h>
#include <windowsx.h>
#include <tchar.h>

#include <d3d9.h>
#include <d3dx9.h>

#include <Lua/lua.h>
#include <Lua/lualib.h>
#include <Lua/lauxlib.h>

#include <Lc/LcEuclid.h>
#include <Lc/ILcTex.h>
#include <Lc/LcUtil.h>
#include <Lc/ILcDev.h>

#include <Lc/ILcLsc.h>

#include <Lc/LcType.h>

#include "LscD3d.h"



namespace	LcLua
{

ILcDev* LscD3d::m_pDevSys	= NULL;
void*	LscD3d::m_pDevD3D	= NULL;


INT LscD3d::Create(ILcDev* pDev)
{
	LscD3d::m_pDevSys	= pDev;
	LscD3d::m_pDevD3D	= LcDev_GetD3Device(&m_pDevSys);

	if(NULL == LscD3d::m_pDevD3D)
		return -1;

	return 0;
}



struct LcD3DType
{
	const char * sName;
	DWORD		 dType;
};



static const luaL_reg regLscD3d[] =
{
	{"SetRenderState"		,	LscD3d::Lsc_SetRenderState			},
	{"SetTextureStageState"	,	LscD3d::Lsc_SetTextureStageState	},
	{"SetSamplerState"		,	LscD3d::Lsc_SetSamplerState			},
	{"SetTransform"			,	LscD3d::Lsc_SetTransform			},
	{"SetTransformI"		,	LscD3d::Lsc_SetTransformI			},

	{"SetLight"				,	LscD3d::Lsc_SetLight				},
	{"SetClearColor"		,	LscD3d::Lsc_SetClearColor			},
	{"BackbufferClear"		,	LscD3d::Lsc_BackbufferClear			},
	{"SetMaterial"			,	LscD3d::Lsc_SetMaterial				},
	{"SetTexture"			,	LscD3d::Lsc_SetTexture				},
	
	{"DrawPrimitive"		,	LscD3d::Lsc_DrawPrimitive			},

	{NULL, NULL},
};


INT LscD3d::OpenLib(lua_State *pL)
{
	luaL_openlib(pL, "Ld3d", regLscD3d, 0);
	return 1;
}


// Implementations La API.
//
////////////////////////////////////////////////////////////////////////////////


static const LcD3DType D3DTypeRs[] =
{
	{	"ZENABLE"			,	D3DRS_ZENABLE			},
	{	"FILLMODE"			,	D3DRS_FILLMODE			},
	{	"SHADEMODE"			,	D3DRS_SHADEMODE			},
	{	"ZWRITEENABLE"		,	D3DRS_ZWRITEENABLE		},
	{	"ALPHATESTENABLE"	,	D3DRS_ALPHATESTENABLE	},
	{	"LASTPIXEL"			,	D3DRS_LASTPIXEL			},
	{	"SRCBLEND"			,	D3DRS_SRCBLEND			},
	{	"DESTBLEND"			,	D3DRS_DESTBLEND			},
	{	"CULLMODE"			,	D3DRS_CULLMODE			},
	{	"ZFUNC"				,	D3DRS_ZFUNC				},
	{	"ALPHAREF"			,	D3DRS_ALPHAREF			},
	{	"ALPHAFUNC"			,	D3DRS_ALPHAFUNC			},
	{	"DITHERENABLE"		,	D3DRS_DITHERENABLE		},
	{	"ALPHABLENDENABLE"	,	D3DRS_ALPHABLENDENABLE	},
	{	"FOGENABLE"			,	D3DRS_FOGENABLE			},
	{	"SPECULARENABLE"	,	D3DRS_SPECULARENABLE	},
	{	"FOGCOLOR"			,	D3DRS_FOGCOLOR			},
	{	"FOGTABLEMODE"		,	D3DRS_FOGTABLEMODE		},
	{	"FOGSTART"			,	D3DRS_FOGSTART			},
	{	"FOGEND"			,	D3DRS_FOGEND			},
	{	"FOGDENSITY"		,	D3DRS_FOGDENSITY		},
	{	"RANGEFOGENABLE"	,	D3DRS_RANGEFOGENABLE	},
	{	"STENCILENABLE"		,	D3DRS_STENCILENABLE		},
	{	"STENCILFAIL"		,	D3DRS_STENCILFAIL		},
	{	"STENCILZFAIL"		,	D3DRS_STENCILZFAIL		},
	{	"STENCILPASS"		,	D3DRS_STENCILPASS		},
	{	"STENCILFUNC"		,	D3DRS_STENCILFUNC		},
	{	"STENCILREF"		,	D3DRS_STENCILREF		},
	{	"STENCILMASK"		,	D3DRS_STENCILMASK		},
	{	"STENCILWRITEMASK"	,	D3DRS_STENCILWRITEMASK	},
	{	"TEXTUREFACTOR"		,	D3DRS_TEXTUREFACTOR		},
	{	"CLIPPING"			,	D3DRS_CLIPPING			},
	{	"LIGHTING"			,	D3DRS_LIGHTING			},
	{	"AMBIENT"			,	D3DRS_AMBIENT			},
	{	"FOGVERTEXMODE"		,	D3DRS_FOGVERTEXMODE		},
	{	"COLORVERTEX"		,	D3DRS_COLORVERTEX		},
	{	"LOCALVIEWER"		,	D3DRS_LOCALVIEWER		},
	{	"NORMALIZENORMALS"	,	D3DRS_NORMALIZENORMALS	},

	{	"DIFFUSEMATERIALSOURCE"		,	D3DRS_DIFFUSEMATERIALSOURCE		},
	{	"SPECULARMATERIALSOURCE"	,	D3DRS_SPECULARMATERIALSOURCE	},
	{	"AMBIENTMATERIALSOURCE"		,	D3DRS_AMBIENTMATERIALSOURCE		},
	{	"EMISSIVEMATERIALSOURCE"	,	D3DRS_EMISSIVEMATERIALSOURCE	},

	{	"VERTEXBLEND"			,	D3DRS_VERTEXBLEND		},
	{	"CLIPPLANEENABLE"		,	D3DRS_CLIPPLANEENABLE	},
	{	"POINTSIZE"				,	D3DRS_POINTSIZE			},
	{	"POINTSIZE_MIN"			,	D3DRS_POINTSIZE_MIN		},
	{	"POINTSPRITEENABLE"		,	D3DRS_POINTSPRITEENABLE	},
	{	"POINTSCALEENABLE"		,	D3DRS_POINTSCALEENABLE	},
	{	"POINTSCALE_A"			,	D3DRS_POINTSCALE_A		},
	{	"POINTSCALE_B"			,	D3DRS_POINTSCALE_B		},
	{	"POINTSCALE_C"			,	D3DRS_POINTSCALE_C		},
	{	"MULTISAMPLEANTIALIAS"	,	D3DRS_MULTISAMPLEANTIALIAS	},
	{	"MULTISAMPLEMASK"		,	D3DRS_MULTISAMPLEMASK		},

	{	"PATCHEDGESTYLE"		,	D3DRS_PATCHEDGESTYLE	},
	{	"DEBUGMONITORTOKEN"		,	D3DRS_DEBUGMONITORTOKEN	},
	{	"POINTSIZE_MAX"			,	D3DRS_POINTSIZE_MAX	},

	{	"INDEXEDVERTEXBLENDENABLE"	,	D3DRS_INDEXEDVERTEXBLENDENABLE	},

	{	"COLORWRITEENABLE"	,	D3DRS_COLORWRITEENABLE	},
	{	"TWEENFACTOR"		,	D3DRS_TWEENFACTOR		},
	{	"BLENDOP"			,	D3DRS_BLENDOP			},
	{	"POSITIONDEGREE"	,	D3DRS_POSITIONDEGREE	},
	{	"NORMALDEGREE"		,	D3DRS_NORMALDEGREE		},
	{	"SCISSORTESTENABLE"	,	D3DRS_SCISSORTESTENABLE	},

	{	"SLOPESCALEDEPTHBIAS"	,	D3DRS_SLOPESCALEDEPTHBIAS	},
	{	"ANTIALIASEDLINEENABLE"	,	D3DRS_ANTIALIASEDLINEENABLE	},
	{	"MINTESSELLATIONLEVEL"	,	D3DRS_MINTESSELLATIONLEVEL	},
	{	"MAXTESSELLATIONLEVEL"	,	D3DRS_MAXTESSELLATIONLEVEL	},

	{	"ADAPTIVETESS_X"	,	D3DRS_ADAPTIVETESS_X	},
	{	"ADAPTIVETESS_Y"	,	D3DRS_ADAPTIVETESS_Y	},
	{	"ADAPTIVETESS_Z"	,	D3DRS_ADAPTIVETESS_Z	},
	{	"ADAPTIVETESS_W"	,	D3DRS_ADAPTIVETESS_W	},

	{	"ENABLEADAPTIVETESSELLATION"	,	D3DRS_ENABLEADAPTIVETESSELLATION	},

	{	"TWOSIDEDSTENCILMODE",	D3DRS_TWOSIDEDSTENCILMODE	},
	{	"CCW_STENCILFAIL"	,	D3DRS_CCW_STENCILFAIL		},
	{	"CCW_STENCILZFAIL"	,	D3DRS_CCW_STENCILZFAIL		},
	{	"CCW_STENCILPASS"	,	D3DRS_CCW_STENCILPASS		},
	{	"CCW_STENCILFUNC"	,	D3DRS_CCW_STENCILFUNC		},
	{	"COLORWRITEENABLE1"	,	D3DRS_COLORWRITEENABLE1		},
	{	"COLORWRITEENABLE2"	,	D3DRS_COLORWRITEENABLE2		},
	{	"COLORWRITEENABLE3"	,	D3DRS_COLORWRITEENABLE3		},
	{	"BLENDFACTOR"		,	D3DRS_BLENDFACTOR			},
	{	"SRGBWRITEENABLE"	,	D3DRS_SRGBWRITEENABLE		},
	{	"DEPTHBIAS"			,	D3DRS_DEPTHBIAS			},

	{	"SEPARATEALPHABLENDENABLE"	,	D3DRS_SEPARATEALPHABLENDENABLE		},

	{	"SRCBLENDALPHA"		,	D3DRS_SRCBLENDALPHA		},
	{	"DESTBLENDALPHA"	,	D3DRS_DESTBLENDALPHA	},
	{	"BLENDOPALPHA"		,	D3DRS_BLENDOPALPHA		},

	{NULL, NULL},
};


static const LcD3DType D3DTypeShadeMode[] =
{
    {	"FLAT"		,	D3DSHADE_FLAT		},
	{	"GOURAUD"	,	D3DSHADE_GOURAUD	},
	{	"PHONG"		,	D3DSHADE_PHONG		},

	{NULL, NULL},
};



static const LcD3DType D3DTypeLight[] =
{
	{	"POINT"			,	D3DLIGHT_POINT			},
	{	"SPOT"			,	D3DLIGHT_SPOT			},
	{	"DIRECTIONAL"	,	D3DLIGHT_DIRECTIONAL	},

	{NULL, NULL},
};


static const LcD3DType D3DTypeFillMode[] =
{
	{	"POINT"			,	D3DFILL_POINT			},
	{	"WIREFRAME"		,	D3DFILL_WIREFRAME		},
	{	"SOLID"			,	D3DFILL_SOLID			},

	{NULL, NULL},
};


static const LcD3DType D3DTypeBlend[] =
{
	{	"ZERO"				,	D3DBLEND_ZERO			},
	{	"ONE"				,	D3DBLEND_ONE			},
	{	"SRCCOLOR"			,	D3DBLEND_SRCCOLOR		},
	{	"INVSRCCOLOR"		,	D3DBLEND_INVSRCCOLOR	},
	{	"SRCALPHA"			,	D3DBLEND_SRCALPHA		},
	{	"INVSRCALPHA"		,	D3DBLEND_INVSRCALPHA	},
	{	"DESTALPHA"			,	D3DBLEND_DESTALPHA		},
	{	"INVDESTALPHA"		,	D3DBLEND_INVDESTALPHA	},
	{	"DESTCOLOR"			,	D3DBLEND_DESTCOLOR		},
	{	"INVDESTCOLOR"		,	D3DBLEND_INVDESTCOLOR	},
	{	"SRCALPHASAT"		,	D3DBLEND_SRCALPHASAT	},
	{	"BOTHSRCALPHA"		,	D3DBLEND_BOTHSRCALPHA	},
	{	"BOTHINVSRCALPHA"	,	D3DBLEND_BOTHINVSRCALPHA},
	{	"BLENDFACTOR"		,	D3DBLEND_BLENDFACTOR	},
	{	"INVBLENDFACTOR"	,	D3DBLEND_INVBLENDFACTOR	},

	{NULL, NULL},
};


static const LcD3DType D3DTypeBlendOp[] =
{
	{	"ADD"			,	D3DBLENDOP_ADD			},
	{	"SUBTRACT"		,	D3DBLENDOP_SUBTRACT		},
	{	"REVSUBTRACT"	,	D3DBLENDOP_REVSUBTRACT	},
	{	"MIN"			,	D3DBLENDOP_MIN			},
	{	"MAX"			,	D3DBLENDOP_MAX			},

	{NULL, NULL},
};


static const LcD3DType D3DTypeTAddress[] =
{
	{	"WRAP"			,	D3DTADDRESS_WRAP		},
	{	"MIRROR"		,	D3DTADDRESS_MIRROR		},
	{	"CLAMP"			,	D3DTADDRESS_CLAMP		},
	{	"BORDER"		,	D3DTADDRESS_BORDER		},
	{	"MIRRORONCE"	,	D3DTADDRESS_MIRRORONCE	},
	{	"FORCE_DWORD"	,	D3DTADDRESS_FORCE_DWORD	},

	{NULL, NULL},
};


static const LcD3DType D3DTypeCull[] =
{
	{	"NONE"	,	D3DCULL_NONE	},
	{	"CW"	,	D3DCULL_CW		},
	{	"CCW"	,	D3DCULL_CCW		},

	{NULL, NULL},
};



static const LcD3DType D3DTypeCmpFnc[] =
{
	{	"NEVER"			,	D3DCMP_NEVER		},
	{	"LESS"			,	D3DCMP_LESS			},
	{	"EQUAL"			,	D3DCMP_EQUAL		},
	{	"LESSEQUAL"		,	D3DCMP_LESSEQUAL	},
	{	"GREATER"		,	D3DCMP_GREATER		},
	{	"NOTEQUAL"		,	D3DCMP_NOTEQUAL		},
	{	"GREATEREQUAL"	,	D3DCMP_GREATEREQUAL	},
	{	"ALWAYS"		,	D3DCMP_ALWAYS		},

	{NULL, NULL},
};



static const LcD3DType D3DTypeStencilOp[] =
{
	{	"KEEP"		,	D3DSTENCILOP_KEEP		},
	{	"ZERO"		,	D3DSTENCILOP_ZERO		},
	{	"REPLACE"	,	D3DSTENCILOP_REPLACE	},
	{	"INCRSAT"	,	D3DSTENCILOP_INCRSAT	},
	{	"DECRSAT"	,	D3DSTENCILOP_DECRSAT	},
	{	"INVERT"	,	D3DSTENCILOP_INVERT		},
	{	"INCR"		,	D3DSTENCILOP_INCR		},
	{	"DECR"		,	D3DSTENCILOP_DECR		},

	{NULL, NULL},
};



static const LcD3DType D3DTypeFog[] =
{
	{	"NONE"		,	D3DFOG_NONE		},
	{	"EXP"		,	D3DFOG_EXP		},
	{	"EXP2"		,	D3DFOG_EXP2		},
	{	"LINEAR"	,	D3DFOG_LINEAR	},

	{NULL, NULL},
};



static const LcD3DType D3DTypeZB[] =
{
	{	"FALSE"	,	D3DZB_FALSE	},
	{	"TRUE"	,	D3DZB_TRUE	},
	{	"USEW"	,	D3DZB_USEW	},

	{NULL, NULL},
};



static const LcD3DType D3DTypePt[] =
{
	{	"POINTLIST"		,	D3DPT_POINTLIST		},
	{	"LINELIST"		,	D3DPT_LINELIST		},
	{	"LINESTRIP"		,	D3DPT_LINESTRIP		},
	{	"TRIANGLELIST"	,	D3DPT_TRIANGLELIST	},
	{	"TRIANGLESTRIP"	,	D3DPT_TRIANGLESTRIP	},
	{	"TRIANGLEFAN"	,	D3DPT_TRIANGLEFAN	},

	{NULL, NULL},
};



static const LcD3DType D3DTypeTransform[] =
{
	{	"WORLD"		,	D3DTS_WORLD		},
	{	"WORLD0"	,	D3DTS_WORLD1	},
	{	"WORLD1"	,	D3DTS_WORLD2	},
	{	"WORLD2"	,	D3DTS_WORLD3	},

	{	"VIEW"		,	D3DTS_VIEW		},
	{	"PROJECTION",	D3DTS_PROJECTION},
	{	"TEXTURE0"	,	D3DTS_TEXTURE0	},
	{	"TEXTURE1"	,	D3DTS_TEXTURE1	},
	{	"TEXTURE2"	,	D3DTS_TEXTURE2	},
	{	"TEXTURE3"	,	D3DTS_TEXTURE3	},
	{	"TEXTURE4"	,	D3DTS_TEXTURE4	},
	{	"TEXTURE5"	,	D3DTS_TEXTURE5	},
	{	"TEXTURE6"	,	D3DTS_TEXTURE6	},
	{	"TEXTURE7"	,	D3DTS_TEXTURE7	},

	{NULL, NULL},
};



static const LcD3DType D3DTypeTss[] =
{
	{	"COLOROP"		,	D3DTSS_COLOROP			},
	{	"COLORARG1"		,	D3DTSS_COLORARG1		},
	{	"COLORARG2"		,	D3DTSS_COLORARG2		},
	{	"ALPHAOP"		,	D3DTSS_ALPHAOP			},
	{	"ALPHAARG1"		,	D3DTSS_ALPHAARG1		},
	{	"ALPHAARG2"		,	D3DTSS_ALPHAARG2		},
	{	"BUMPENVMAT00"	,	D3DTSS_BUMPENVMAT00		},
	{	"BUMPENVMAT01"	,	D3DTSS_BUMPENVMAT01		},
	{	"BUMPENVMAT10"	,	D3DTSS_BUMPENVMAT10		},
	{	"BUMPENVMAT11"	,	D3DTSS_BUMPENVMAT11		},
	{	"TEXCOORDINDEX"	,	D3DTSS_TEXCOORDINDEX	},
	{	"BUMPENVLSCALE"	,	D3DTSS_BUMPENVLSCALE	},
	{	"BUMPENVLOFFSET",	D3DTSS_BUMPENVLOFFSET	},
	{	"COLORARG0"		,	D3DTSS_COLORARG0		},
	{	"ALPHAARG0"		,	D3DTSS_ALPHAARG0		},
	{	"RESULTARG"		,	D3DTSS_RESULTARG		},
	{	"CONSTANT"		,	D3DTSS_CONSTANT			},
	{	"TEXTURETRANSFORMFLAGS"	,	D3DTSS_TEXTURETRANSFORMFLAGS	},

	{NULL, NULL},
};



static const LcD3DType D3DTypeSamp[] =
{
	{	"ADDRESSU"		,	D3DSAMP_ADDRESSU		},
	{	"ADDRESSV"		,	D3DSAMP_ADDRESSV		},
	{	"ADDRESSW"		,	D3DSAMP_ADDRESSW		},
	{	"BORDERCOLOR"	,	D3DSAMP_BORDERCOLOR		},
	{	"MAGFILTER"		,	D3DSAMP_MAGFILTER		},
	{	"MINFILTER"		,	D3DSAMP_MINFILTER		},
	{	"MIPFILTER"		,	D3DSAMP_MIPFILTER		},
	{	"MIPMAPLODBIAS"	,	D3DSAMP_MIPMAPLODBIAS	},
	{	"MAXMIPLEVEL"	,	D3DSAMP_MAXMIPLEVEL		},
	{	"MAXANISOTROPY"	,	D3DSAMP_MAXANISOTROPY	},
	{	"SRGBTEXTURE"	,	D3DSAMP_SRGBTEXTURE		},
	{	"ELEMENTINDEX"	,	D3DSAMP_ELEMENTINDEX	},
	{	"DMAPOFFSET"	,	D3DSAMP_DMAPOFFSET		},

	{NULL, NULL},
};


static const LcD3DType D3DTypeTextureOp[] =
{
	// Texture Blending Operation
	{	"DISABLE"				,	D3DTOP_DISABLE		},
	{	"SELECTARG1"			,	D3DTOP_SELECTARG1	},
	{	"SELECTARG2"			,	D3DTOP_SELECTARG2	},
	{	"MODULATE"				,	D3DTOP_MODULATE		},
	{	"MODULATE2X"			,	D3DTOP_MODULATE2X	},
	{	"MODULATE4X"			,	D3DTOP_MODULATE4X	},
	{	"ADD"					,	D3DTOP_ADD			},
	{	"ADDSIGNED"				,	D3DTOP_ADDSIGNED	},
	{	"ADDSIGNED2X"			,	D3DTOP_ADDSIGNED2X	},
	{	"SUBTRACT"				,	D3DTOP_SUBTRACT		},
	{	"ADDSMOOTH"				,	D3DTOP_ADDSMOOTH	},
	{	"BLENDDIFFUSEALPHA"		,	D3DTOP_BLENDDIFFUSEALPHA	},
	{	"BLENDTEXTUREALPHA"		,	D3DTOP_BLENDTEXTUREALPHA	},
	{	"BLENDFACTORALPHA"		,	D3DTOP_BLENDFACTORALPHA		},
	{	"BLENDTEXTUREALPHAPM"	,	D3DTOP_BLENDTEXTUREALPHAPM	},
	{	"BLENDCURRENTALPHA"		,	D3DTOP_BLENDCURRENTALPHA	},
	{	"PREMODULATE"			,	D3DTOP_PREMODULATE			},

	{	"MODULATEALPHA_ADDCOLOR"	,	D3DTOP_MODULATEALPHA_ADDCOLOR		},
	{	"MODULATECOLOR_ADDALPHA"	,	D3DTOP_MODULATECOLOR_ADDALPHA		},
	{	"MODULATEINVALPHA_ADDCOLOR"	,	D3DTOP_MODULATEINVALPHA_ADDCOLOR	},
	{	"MODULATEINVCOLOR_ADDALPHA"	,	D3DTOP_MODULATEINVCOLOR_ADDALPHA	},

	{	"BUMPENVMAP"			,	D3DTOP_BUMPENVMAP			},
	{	"BUMPENVMAPLUMINANCE"	,	D3DTOP_BUMPENVMAPLUMINANCE	},
	{	"DOTPRODUCT3"			,	D3DTOP_DOTPRODUCT3			},
	{	"MULTIPLYADD"			,	D3DTOP_MULTIPLYADD			},
	{	"LERP"					,	D3DTOP_LERP					},


	// Values for COLORARG0,1,2, ALPHAARG0,1,2, and RESULTARG texture blending
	// operations set in texture processing stage controls in D3DRENDERSTATE.

	{	"SELECTMASK"	,	D3DTA_SELECTMASK	},
	{	"DIFFUSE"		,	D3DTA_DIFFUSE		},
	{	"CURRENT"		,	D3DTA_CURRENT		},
	{	"TEXTURE"		,	D3DTA_TEXTURE		},
	{	"TFACTOR"		,	D3DTA_TFACTOR		},
	{	"SPECULAR"		,	D3DTA_SPECULAR		},
	{	"TEMP"			,	D3DTA_TEMP			},
	{	"CONSTANT"		,	D3DTA_CONSTANT		},
	{	"COMPLEMENT"	,	D3DTA_COMPLEMENT	},
	{	"ALPHAREPLICATE",	D3DTA_ALPHAREPLICATE},

	//Values for D3DSAMP_***FILTER texture stage states

	{	"NONE"			,	D3DTEXF_NONE	},
	{	"POINT"			,	D3DTEXF_POINT	},
	{	"LINEAR"		,	D3DTEXF_LINEAR	},
	{	"ANISOTROPIC"	,	D3DTEXF_ANISOTROPIC	},
	{	"PYRAMIDALQUAD"	,	D3DTEXF_PYRAMIDALQUAD	},
	{	"GAUSSIANQUAD"	,	D3DTEXF_GAUSSIANQUAD	},
	{	"FORCE_DWORD"	,	D3DTEXF_FORCE_DWORD	},
	{NULL, NULL},
};



static const LcD3DType D3DTypeEtc[] =
{
	{	"TRUE"		,	TRUE	},
	{	"FALSE"		,	FALSE	},

	{NULL, NULL},
};





DWORD LscD3d::FindD3DTypeRs(char* sName)
{
	DWORD dValue=0xFFFFFFFF;

	INT nIdx =0;

	while( D3DTypeRs[nIdx].sName )
	{
		if(0 == _stricmp( D3DTypeRs[nIdx].sName, sName))
		{
			dValue = D3DTypeRs[nIdx].dType;
			break;
		}

		++nIdx;
	}

	return dValue;
}


DWORD LscD3d::FindD3DTypeShadeMode(char* sName)
{
	DWORD dValue=0xFFFFFFFF;

	INT nIdx =0;

	while( D3DTypeShadeMode[nIdx].sName )
	{
		if(0 == _stricmp( D3DTypeShadeMode[nIdx].sName, sName))
		{
			dValue = D3DTypeShadeMode[nIdx].dType;
			break;
		}

		++nIdx;
	}

	return dValue;
}


DWORD LscD3d::FindD3DTypeLight(char* sName)
{
	DWORD dValue=0xFFFFFFFF;

	INT nIdx =0;

	while( D3DTypeLight[nIdx].sName )
	{
		if(0 == _stricmp( D3DTypeLight[nIdx].sName, sName))
		{
			dValue = D3DTypeLight[nIdx].dType;
			break;
		}

		++nIdx;
	}

	return dValue;
}


DWORD LscD3d::FindD3DTypeFillMode(char* sName)
{
	DWORD dValue=0xFFFFFFFF;

	INT nIdx =0;

	while( D3DTypeFillMode[nIdx].sName )
	{
		if(0 == _stricmp( D3DTypeFillMode[nIdx].sName, sName))
		{
			dValue = D3DTypeFillMode[nIdx].dType;
			break;
		}

		++nIdx;
	}

	return dValue;
}



DWORD LscD3d::FindD3DTypeBlend(char* sName)
{
	DWORD dValue=0xFFFFFFFF;

	INT nIdx =0;

	while( D3DTypeBlend[nIdx].sName )
	{
		if(0 == _stricmp( D3DTypeBlend[nIdx].sName, sName))
		{
			dValue = D3DTypeBlend[nIdx].dType;
			break;
		}

		++nIdx;
	}

	return dValue;
}




DWORD LscD3d::FindD3DTypeBlendOp(char* sName)
{
	DWORD dValue=0xFFFFFFFF;

	INT nIdx =0;

	while( D3DTypeBlendOp[nIdx].sName )
	{
		if(0 == _stricmp( D3DTypeBlendOp[nIdx].sName, sName))
		{
			dValue = D3DTypeBlendOp[nIdx].dType;
			break;
		}

		++nIdx;
	}

	return dValue;
}




DWORD LscD3d::FindD3DTypeTAddress(char* sName)
{
	DWORD dValue=0xFFFFFFFF;

	INT nIdx =0;

	while( D3DTypeTAddress[nIdx].sName )
	{
		if(0 == _stricmp( D3DTypeTAddress[nIdx].sName, sName))
		{
			dValue = D3DTypeTAddress[nIdx].dType;
			break;
		}

		++nIdx;
	}

	return dValue;
}




DWORD LscD3d::FindD3DTypeCull(char* sName)
{
	DWORD dValue=0xFFFFFFFF;

	INT nIdx =0;

	while( D3DTypeCull[nIdx].sName )
	{
		if(0 == _stricmp( D3DTypeCull[nIdx].sName, sName))
		{
			dValue = D3DTypeCull[nIdx].dType;
			break;
		}

		++nIdx;
	}

	return dValue;
}




DWORD LscD3d::FindD3DTypeCmpFnc(char* sName)
{
	DWORD dValue=0xFFFFFFFF;

	INT nIdx =0;

	while( D3DTypeCmpFnc[nIdx].sName )
	{
		if(0 == _stricmp( D3DTypeCmpFnc[nIdx].sName, sName))
		{
			dValue = D3DTypeCmpFnc[nIdx].dType;
			break;
		}

		++nIdx;
	}

	return dValue;
}




DWORD LscD3d::FindD3DTypeStencilOp(char* sName)
{
	DWORD dValue=0xFFFFFFFF;

	INT nIdx =0;

	while( D3DTypeStencilOp[nIdx].sName )
	{
		if(0 == _stricmp( D3DTypeStencilOp[nIdx].sName, sName))
		{
			dValue = D3DTypeStencilOp[nIdx].dType;
			break;
		}

		++nIdx;
	}

	return dValue;
}




DWORD LscD3d::FindD3DTypeFog(char* sName)
{
	DWORD dValue=0xFFFFFFFF;

	INT nIdx =0;

	while( D3DTypeFog[nIdx].sName )
	{
		if(0 == _stricmp( D3DTypeFog[nIdx].sName, sName))
		{
			dValue = D3DTypeFog[nIdx].dType;
			break;
		}

		++nIdx;
	}

	return dValue;
}




DWORD LscD3d::FindD3DTypeZB(char* sName)
{
	DWORD dValue=0xFFFFFFFF;

	INT nIdx =0;

	while( D3DTypeZB[nIdx].sName )
	{
		if(0 == _stricmp( D3DTypeZB[nIdx].sName, sName))
		{
			dValue = D3DTypeZB[nIdx].dType;
			break;
		}

		++nIdx;
	}

	return dValue;
}




DWORD LscD3d::FindD3DTypePt(char* sName)
{
	DWORD dValue=0xFFFFFFFF;

	INT nIdx =0;

	while( D3DTypePt[nIdx].sName )
	{
		if(0 == _stricmp( D3DTypePt[nIdx].sName, sName))
		{
			dValue = D3DTypePt[nIdx].dType;
			break;
		}

		++nIdx;
	}

	return dValue;
}




DWORD LscD3d::FindD3DTypeTransform(char* sName)
{
	DWORD dValue=0xFFFFFFFF;

	INT nIdx =0;

	while( D3DTypeTransform[nIdx].sName )
	{
		if(0 == _stricmp( D3DTypeTransform[nIdx].sName, sName))
		{
			dValue = D3DTypeTransform[nIdx].dType;
			break;
		}

		++nIdx;
	}

	return dValue;
}




DWORD LscD3d::FindD3DTypeTss(char* sName)
{
	DWORD dValue=0xFFFFFFFF;

	INT nIdx =0;

	while( D3DTypeTss[nIdx].sName )
	{
		if(0 == _stricmp( D3DTypeTss[nIdx].sName, sName))
		{
			dValue = D3DTypeTss[nIdx].dType;
			break;
		}

		++nIdx;
	}

	return dValue;
}




DWORD LscD3d::FindD3DTypeSamp(char* sName)
{
	DWORD dValue=0xFFFFFFFF;

	INT nIdx =0;

	while( D3DTypeSamp[nIdx].sName )
	{
		if(0 == _stricmp( D3DTypeSamp[nIdx].sName, sName))
		{
			dValue = D3DTypeSamp[nIdx].dType;
			break;
		}

		++nIdx;
	}

	return dValue;
}




DWORD LscD3d::FindD3DTypeTextureOp(char* sName)
{
	DWORD dValue=0xFFFFFFFF;

	INT nIdx =0;

	while( D3DTypeTextureOp[nIdx].sName )
	{
		if(0 == _stricmp( D3DTypeTextureOp[nIdx].sName, sName))
		{
			dValue = D3DTypeTextureOp[nIdx].dType;
			break;
		}

		++nIdx;
	}

	return dValue;
}




DWORD LscD3d::FindD3DTypeEtc(char* sName)
{
	DWORD dValue=0xFFFFFFFF;

	INT nIdx =0;

	while( D3DTypeEtc[nIdx].sName )
	{
		if(0 == _stricmp( D3DTypeEtc[nIdx].sName, sName))
		{
			dValue = D3DTypeEtc[nIdx].dType;
			break;
		}

		++nIdx;
	}

	return dValue;
}












INT LscD3d::Lsc_SetRenderState(lua_State *pL)
{
	INT		hr=-1;
	INT		n = lua_gettop(pL);
	
	if(n<2)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	char	sState[64]={0};
	char	sValue[64]={0};
	DWORD	dState = 0;
	DWORD	dValue = 0;
	PDEV	pDev	= (PDEV)(LscD3d::m_pDevD3D);

	strcpy(sState, (char*)lua_tostring(pL, 1) );
	strcpy(sValue, (char*)lua_tostring(pL, 2) );

	dState = LscD3d::FindD3DTypeRs(sState);

	if(0xFFFFFFFF == dState)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}


	char*	sEnable = NULL;
	sEnable = strstr(sState, "ENABLE");

	if(sEnable)
		dValue = FindD3DTypeEtc(sValue);
	
	else if(D3DRS_LIGHTING == dState)
		dValue = FindD3DTypeEtc(sValue);

	else if(D3DRS_SHADEMODE == dState)
		dValue = FindD3DTypeShadeMode(sValue);
	
	else if(D3DRS_FILLMODE == dState)
		dValue = FindD3DTypeFillMode(sValue);

	else if(D3DRS_CULLMODE == dState)
		dValue = FindD3DTypeCull(sValue);

	else if(D3DRS_SRCBLEND == dState || D3DRS_DESTBLEND == dState)
		dValue = FindD3DTypeBlend(sValue);

	else if(D3DRS_BLENDOP == dState)
		dValue = FindD3DTypeBlendOp(sValue);

	else if(D3DRS_ALPHAFUNC == dState
			|| D3DRS_ZFUNC == dState
			|| D3DRS_STENCILFUNC == dState
			|| D3DRS_CCW_STENCILFUNC == dState)
		dValue = FindD3DTypeCmpFnc(sValue);

	else if(D3DRS_FOGTABLEMODE == dState || D3DRS_FOGVERTEXMODE == dState)
		dValue = FindD3DTypeFog(sValue);

	else if(D3DRS_FOGCOLOR == dState)
		sscanf(sValue, "%x" , &dValue);

	else if(D3DRS_FOGSTART == dState
			|| D3DRS_FOGEND == dState
			|| D3DRS_FOGDENSITY == dState)
	{
		FLOAT fT;
		sscanf(sValue, "%f" , &fT);
		dValue = *((DWORD*)&fT);
	}

	else if(D3DRS_AMBIENT == dState)
	{
		sscanf(sValue, "%x", &dValue);
	}
		
	hr = pDev->SetRenderState((D3DRENDERSTATETYPE)dState, dValue);

	if(FAILED(hr))
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	lua_pushnumber(pL, 0);
	return 1;
}




INT LscD3d::Lsc_SetTextureStageState(lua_State *pL)
{
	INT		hr=-1;
	INT		n = lua_gettop(pL);
	
	if(n<2)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	char	sState[64]={0};
	char	sValue[64]={0};
	DWORD	dStage = 0;
	DWORD	dState = 0;
	DWORD	dValue = 0;
	PDEV	pDev   = (PDEV)(LscD3d::m_pDevD3D);

	dStage = (DWORD)lua_tonumber(pL, 1);
	strcpy(sState, (char*)lua_tostring(pL, 2) );
	strcpy(sValue, (char*)lua_tostring(pL, 3) );

	dState = LscD3d::FindD3DTypeTss(sState);

	if(0xFFFFFFFF == dState)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	dValue = LscD3d::FindD3DTypeTextureOp(sValue);

	if(0xFFFFFFFF == dState)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

		
	hr = pDev->SetTextureStageState(dStage, (D3DTEXTURESTAGESTATETYPE)dState, dValue);

	if(FAILED(hr))
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	lua_pushnumber(pL, 0);
	return 1;
}



INT LscD3d::Lsc_SetSamplerState(lua_State *pL)
{
	INT		hr=-1;
	INT		n = lua_gettop(pL);
	
	if(n<2)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	char	sState[64]={0};
	char	sValue[64]={0};
	DWORD	dStage = 0;
	DWORD	dState = 0;
	DWORD	dValue = 0;
	PDEV	pDev   = (PDEV)(LscD3d::m_pDevD3D);

	dStage = (DWORD)lua_tonumber(pL, 1);
	strcpy(sState, (char*)lua_tostring(pL, 2) );
	strcpy(sValue, (char*)lua_tostring(pL, 3) );

	dState = LscD3d::FindD3DTypeSamp(sState);

	if(0xFFFFFFFF == dState)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

	dValue = LscD3d::FindD3DTypeTextureOp(sValue);

	if(0xFFFFFFFF == dState)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}

		
	hr = pDev->SetSamplerState(dStage, (D3DSAMPLERSTATETYPE)dState, dValue);

	if(FAILED(hr))
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	lua_pushnumber(pL, 0);
	return 1;
}




INT LscD3d::Lsc_SetTransform(lua_State *pL)
{
	INT		hr=-1;
	INT		n = lua_gettop(pL);
	
	if(n<2)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	char	sType[64]	={0};
	DWORD	dType		= 0;
	FLOAT	dValue[16]	= {0};
	PDEV	pDev		= (PDEV)(LscD3d::m_pDevD3D);

	strcpy(sType,  (char*)lua_tostring(pL, 1) );

	dType = LscD3d::FindD3DTypeTransform(sType);

	if(0xFFFFFFFF == dType)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}


	D3DXMATRIX mtTs;
	D3DXMatrixIdentity(&mtTs);

	if(D3DTS_VIEW == dType)
	{
		if(n > 2)
		{
			D3DXVECTOR3	vcEye;
			D3DXVECTOR3	vcLook;
			D3DXVECTOR3	vcUp;

			vcEye.x		= (FLOAT)lua_tonumber(pL, 2);
			vcEye.y		= (FLOAT)lua_tonumber(pL, 3);
			vcEye.z		= (FLOAT)lua_tonumber(pL, 4);

			vcLook.x	= (FLOAT)lua_tonumber(pL, 5);
			vcLook.y	= (FLOAT)lua_tonumber(pL, 6);
			vcLook.z	= (FLOAT)lua_tonumber(pL, 7);

			vcUp.x		= (FLOAT)lua_tonumber(pL, 8);
			vcUp.y		= (FLOAT)lua_tonumber(pL, 9);
			vcUp.z		= (FLOAT)lua_tonumber(pL, 10);

			D3DXMatrixLookAtLH(&mtTs, &vcEye, &vcLook, &vcUp);
		}

		else
		{
			INT nIdx =-1;
			double v = -1;

			luaL_checktype(pL, 2, LUA_TTABLE);
			lua_pushnil(pL);  /* first key */

			while(++nIdx <16 )
			{
				if (lua_next(pL, 2) == 0)
					break;

				lua_pop(pL, 1);

				v = lua_tonumber(pL, 0);
				//v = lua_tonumber(pL, 1);

				mtTs.m[nIdx/4][nIdx % 4] = (FLOAT)v;
			}
		}


		hr = pDev->SetTransform(D3DTS_VIEW, &mtTs);
	}

	else if(D3DTS_PROJECTION == dType)
	{
		if(n > 2)
		{
			FLOAT fFov = 0.f;
			FLOAT	fAspect	= 1.f;
			FLOAT	fNear	= 0.f;
			FLOAT	fFar	= 1000.f;

			fFov		= (FLOAT)lua_tonumber(pL, 2);
			fAspect		= (FLOAT)lua_tonumber(pL, 3);
			fNear		= (FLOAT)lua_tonumber(pL, 4);
			fFar		= (FLOAT)lua_tonumber(pL, 5);;

			D3DXMatrixPerspectiveFovLH(&mtTs, fFov, fAspect, fNear, fFar);
		}

		else
		{
			INT nIdx =-1;
			double v = -1;

			luaL_checktype(pL, 2, LUA_TTABLE);
			lua_pushnil(pL);  /* first key */

			while(++nIdx <16 )
			{
				if (lua_next(pL, 2) == 0)
					break;

				lua_pop(pL, 1);

				v = lua_tonumber(pL, 0);
				//v = lua_tonumber(pL, 1);

				mtTs.m[nIdx/4][nIdx % 4] = (FLOAT)v;
			}
		}


		hr = pDev->SetTransform(D3DTS_PROJECTION, &mtTs);
	}

	else if(D3DTS_WORLD == dType)
	{
		if(n > 2)
		{
			mtTs._11 = (FLOAT)lua_tonumber(pL, 2+ 0);
			mtTs._12 = (FLOAT)lua_tonumber(pL, 2+ 1);
			mtTs._13 = (FLOAT)lua_tonumber(pL, 2+ 2);
			mtTs._14 = (FLOAT)lua_tonumber(pL, 2+ 3);

			mtTs._21 = (FLOAT)lua_tonumber(pL, 2+ 4);
			mtTs._22 = (FLOAT)lua_tonumber(pL, 2+ 5);
			mtTs._23 = (FLOAT)lua_tonumber(pL, 2+ 6);
			mtTs._24 = (FLOAT)lua_tonumber(pL, 2+ 7);

			mtTs._31 = (FLOAT)lua_tonumber(pL, 2+ 8);
			mtTs._32 = (FLOAT)lua_tonumber(pL, 2+ 9);
			mtTs._33 = (FLOAT)lua_tonumber(pL, 2+10);
			mtTs._34 = (FLOAT)lua_tonumber(pL, 2+11);

			mtTs._41 = (FLOAT)lua_tonumber(pL, 2+12);
			mtTs._42 = (FLOAT)lua_tonumber(pL, 2+13);
			mtTs._43 = (FLOAT)lua_tonumber(pL, 2+14);
			mtTs._44 = (FLOAT)lua_tonumber(pL, 2+15);
		}

		else
		{
			INT nIdx = -1;
			double v = -1;

			luaL_checktype(pL, 2, LUA_TTABLE);
			lua_pushnil(pL);  /* first key */

			while(++nIdx <16 )
			{
				if (lua_next(pL, 2) == 0)
					break;

				lua_pop(pL, 1);

				v = lua_tonumber(pL, 0);
				mtTs.m[nIdx/4][nIdx % 4] = (FLOAT)v;
				//v = lua_tonumber(pL, 1);
			}
		}


		hr = pDev->SetTransform(D3DTS_WORLD, &mtTs);
	}


	if(FAILED(hr))
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	lua_pushnumber(pL, 0);
	return 1;
}


INT LscD3d::Lsc_SetTransformI(lua_State *pL)
{
	INT		hr=-1;
	INT		n = lua_gettop(pL);
	
	if(n<1)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	char	sType[64]	={0};
	DWORD	dType		= 0;
	FLOAT	dValue[16]	= {0};
	PDEV	pDev		= (PDEV)(LscD3d::m_pDevD3D);

	strcpy(sType,  (char*)lua_tostring(pL, 1) );

	dType = LscD3d::FindD3DTypeTransform(sType);

	if(0xFFFFFFFF == dType)
	{
		lua_pushnumber(pL, -1);
		return 1;
	}


	static D3DXMATRIX mtTs(1, 0, 0, 0,   0, 1, 0, 0,   0, 0, 1, 0,   0, 0, 0, 1);

	if(D3DTS_WORLD == dType)
		hr = pDev->SetTransform(D3DTS_WORLD, &mtTs);

	else if(D3DTS_VIEW == dType)
		hr = pDev->SetTransform(D3DTS_VIEW, &mtTs);

	else if(D3DTS_PROJECTION == dType)
		hr = pDev->SetTransform(D3DTS_PROJECTION, &mtTs);


	if(FAILED(hr))
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	lua_pushnumber(pL, 0);
	return 1;
}


INT LscD3d::Lsc_DrawPrimitive(lua_State *pL)
{
	return 0;
}


INT LscD3d::Lsc_SetLight(lua_State *pL)
{
	return 0;
}




INT LscD3d::Lsc_SetClearColor(lua_State *pL)
{
	INT hr=-1;

	char	sValue[16]={0};
	DWORD	dValue	= 0xFFFFFFFF;
	PDEV	pDev	= (PDEV)(LscD3d::m_pDevD3D);

	strcpy(sValue, (char*)lua_tostring(pL, 1) );
	sscanf(sValue, "%x", &dValue);

	hr = m_pDevSys->Query("Set Clear Color" , &dValue);

	if(FAILED(hr))
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	lua_pushnumber(pL, 0);
	return 1;
}



INT LscD3d::Lsc_BackbufferClear(lua_State *pL)
{
	INT hr=-1;

	char	sValue[16]={0};
	DWORD	dValue	= 0xFFFFFFFF;
	DWORD	dClrMode=0x0;
	PDEV	pDev	= (PDEV)(LscD3d::m_pDevD3D);

	strcpy(sValue, (char*)lua_tostring(pL, 1) );
	sscanf(sValue, "%x", &dValue);

	m_pDevSys->Query("Get Clear Mode" , &dClrMode);

	hr = m_pDevSys->Clear(0, NULL, dClrMode, dValue, 1, 0L);

	if(FAILED(hr))
	{
		lua_pushnumber(pL, -1);
		return 1;
	}
	
	lua_pushnumber(pL, 0);
	return 1;
}


INT LscD3d::Lsc_SetMaterial(lua_State *pL)
{
	return 0;
}


INT LscD3d::Lsc_SetTexture(lua_State *pL)
{
	return 0;
}


}// namespace