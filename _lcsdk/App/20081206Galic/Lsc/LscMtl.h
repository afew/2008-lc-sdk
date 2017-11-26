// Interface for the LscMtl class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LscMtl_H_
#define _LscMtl_H_


#pragma warning(disable : 4786)
#include <vector>

namespace	LcLua
{

struct LscMtl
{
	struct ILcCont
	{
		std::vector<ILcLsc::LscH* >	vLscH;
		
		virtual ~ILcCont();

		INT			Alloc(FLOAT* pAmb, FLOAT* pDif, FLOAT* pSpc, FLOAT* pEms, FLOAT fPow);
		INT			Release(INT nKey);
		ILcLsc::LscH* Find(INT nKey);
	};


	static	ILcDev*		m_pDevSys	;
	static	void*		m_pDevD3D	;											// d3d Device
	static	INT			Create(ILcDev* pDev);
	
	static	ILcCont*	m_pContAuto	;											// 지역 변수
	static	ILcCont*	m_pContStat	;											// 정적 변수
	

	static INT	Lsc_Alloc(lua_State *pL);										// Mtl 로딩
	static INT	Lsc_Release(lua_State *pL);										// Mtl 해제

	static INT	Lsc_MtlDiffuseSet(lua_State *pL);								// Set Mtl Diffuse
	static INT	Lsc_MtlAmbientSet(lua_State *pL);								// Set Mtl Ambient
	static INT	Lsc_MtlSpecularSet(lua_State *pL);								// Set Mtl Specular
	static INT	Lsc_MtlEmissiveSet(lua_State *pL);								// Set Mtl Emissive
	static INT	Lsc_MtlPowerSet(lua_State *pL);									// Set Mtl Power

	static INT	Lsc_MtlDiffuseGet(lua_State *pL);								// Get Mtl Diffuse
	static INT	Lsc_MtlAmbientGet(lua_State *pL);								// Get Mtl Ambient
	static INT	Lsc_MtlSpecularGet(lua_State *pL);								// Get Mtl Specular
	static INT	Lsc_MtlEmissiveGet(lua_State *pL);								// Get Mtl Emissive
	static INT	Lsc_MtlPowerGet(lua_State *pL);									// Get Mtl Power

	static INT	Lsc_SetMaterial(lua_State *pL);									// pDev->SetMaterial

	static INT	OpenLib(lua_State *pL);


	static INT	ContainerCreate();
	static INT	ContainerDestroy();

	static INT	StaticContainerCreate();
	static INT	StaticContainerDestroy();
};

}// namespace Lua

#endif
