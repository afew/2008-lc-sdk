// Interface for the LscD3d cLscss.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LscD3d_H_
#define _LscD3d_H_


namespace	LcLua
{

	
struct LscD3d
{
	static	ILcDev*		m_pDevSys	;
	static	void*		m_pDevD3D	;
	static	INT			Create(ILcDev* pDev);
	
	
	static INT	Lsc_SetRenderState		(lua_State *pL);						// Set Device Render State
	static INT	Lsc_SetTextureStageState(lua_State *pL);						// Set Device Texture Stage State
	static INT	Lsc_SetSamplerState		(lua_State *pL);						// Set Sampler State
	static INT	Lsc_SetTransform		(lua_State *pL);						// Set Transform
	static INT	Lsc_SetTransformI		(lua_State *pL);						// Set Transform Identity

	static INT	Lsc_SetLight			(lua_State *pL);						// Set Lighting
	static INT	Lsc_BackbufferClear		(lua_State *pL);						// Backbuffer Clear
	static INT	Lsc_SetClearColor		(lua_State *pL);						// Backbuffer Clear Color
	static INT	Lsc_SetMaterial			(lua_State *pL);						// Set Material
	static INT	Lsc_SetTexture			(lua_State *pL);						// Set Texture

	static INT	Lsc_DrawPrimitive		(lua_State *pL);						// Draw Primitive

	static INT	OpenLib(lua_State *pL);


	static DWORD	FindD3DTypeRs		(char* sName);							// Render State
	static DWORD	FindD3DTypeShadeMode(char* sName);
	static DWORD	FindD3DTypeLight	(char* sName);
	static DWORD	FindD3DTypeFillMode	(char* sName);
	static DWORD	FindD3DTypeBlend	(char* sName);
	static DWORD	FindD3DTypeBlendOp	(char* sName);
	static DWORD	FindD3DTypeTAddress	(char* sName);
	static DWORD	FindD3DTypeCull		(char* sName);
	static DWORD	FindD3DTypeCmpFnc	(char* sName);
	static DWORD	FindD3DTypeStencilOp(char* sName);
	static DWORD	FindD3DTypeFog		(char* sName);
	static DWORD	FindD3DTypeZB		(char* sName);
	static DWORD	FindD3DTypePt		(char* sName);
	static DWORD	FindD3DTypeTransform(char* sName);
	static DWORD	FindD3DTypeTss		(char* sName);
	static DWORD	FindD3DTypeSamp		(char* sName);
	static DWORD	FindD3DTypeTextureOp(char* sName);

	static DWORD	FindD3DTypeEtc		(char* sName);
};

}//name space Lcx

#endif