// Interface for the LscLgt class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LscLgt_H_
#define _LscLgt_H_


#pragma warning(disable : 4786)
#include <vector>


namespace	LcLua
{
	
struct LscLgt
{
	struct ILcCont
	{
		std::vector<ILcLsc::LscH* >	vLscH;
		
		virtual ~ILcCont();

		INT			Alloc(DWORD* pType
					   , FLOAT* pAmb, FLOAT* pDff, FLOAT* pSpc
					   , FLOAT* pPos, FLOAT* pDir
					   , FLOAT* fEtc);
		
		INT			Release(INT nKey);
		ILcLsc::LscH* Find(INT nKey);
	};


	static	ILcDev*		m_pDevSys	;
	static	void*		m_pDevD3D	;											// d3d Device
	static	INT			Create(ILcDev* pDev);
	
	static	ILcCont*	m_pContAuto	;											// 지역 변수
	static	ILcCont*	m_pContStat	;											// 정적 변수
	

	static INT	Lsc_Alloc(lua_State *pL);										// Lgt 로딩
	static INT	Lsc_Release(lua_State *pL);										// Lgt 해제

	static INT	Lsc_SetLight(lua_State *pL);									// Set	Light
	static INT	Lsc_SetLightEnable(lua_State *pL);								// Set	LightEnable

	static INT	Lsc_SetLgtType(lua_State *pL);									// Set	Type

	static INT	Lsc_SetLgtAmbient(lua_State *pL);								// Set	Ambient
	static INT	Lsc_SetLgtDiffuse(lua_State *pL);								// Set	Diffuse
	static INT	Lsc_SetLgtSpecular(lua_State *pL);								// Set	Specular

	static INT	Lsc_SetLgtPosition(lua_State *pL);								// Set	Position
	static INT	Lsc_SetLgtDirection(lua_State *pL);								// Set	Direction

	static INT	Lsc_SetLgtRange(lua_State *pL);									// Set	Range
	static INT	Lsc_SetLgtFalloff(lua_State *pL);								// Set	Falloff

	static INT	Lsc_SetLgtAttenuation0(lua_State *pL);							// Set	Attenuation0
	static INT	Lsc_SetLgtAttenuation1(lua_State *pL);							// Set	Attenuation1
	static INT	Lsc_SetLgtAttenuation2(lua_State *pL);							// Set	Attenuation2
	static INT	Lsc_SetLgtTheta(lua_State *pL);									// Set	Theta
	static INT	Lsc_SetLgtPhi(lua_State *pL);									// Set	Phi


	static INT	Lsc_GetLgtType(lua_State *pL);									// Get	Type

	static INT	Lsc_GetLgtAmbient(lua_State *pL);								// Get	Ambient
	static INT	Lsc_GetLgtDiffuse(lua_State *pL);								// Get	Diffuse
	static INT	Lsc_GetLgtSpecular(lua_State *pL);								// Get	Specular

	static INT	Lsc_GetLgtPosition(lua_State *pL);								// Get	Position
	static INT	Lsc_GetLgtDirection(lua_State *pL);								// Get	Direction

	static INT	Lsc_GetLgtRange(lua_State *pL);									// Get	Range
	static INT	Lsc_GetLgtFalloff(lua_State *pL);								// Get	Falloff

	static INT	Lsc_GetLgtAttenuation0(lua_State *pL);							// Get	Attenuation0
	static INT	Lsc_GetLgtAttenuation1(lua_State *pL);							// Get	Attenuation1
	static INT	Lsc_GetLgtAttenuation2(lua_State *pL);							// Get	Attenuation2
	static INT	Lsc_GetLgtTheta(lua_State *pL);									// Get	Theta
	static INT	Lsc_GetLgtPhi(lua_State *pL);									// Get	Phi
	

	static INT	OpenLib(lua_State *pL);


	static INT	ContainerCreate();
	static INT	ContainerDestroy();

	static INT	StaticContainerCreate();
	static INT	StaticContainerDestroy();
};

}// namespace Lua


#endif

