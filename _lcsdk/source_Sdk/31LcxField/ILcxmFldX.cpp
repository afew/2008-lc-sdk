// Implementation of the ILcxmFldX class.
//
////////////////////////////////////////////////////////////////////////////////


#include <windows.h>
#include <d3d9.h>
#include <d3dX9.h>


#include "ILcxmFldX.h"


namespace Lcxmo
{

MpTex::_Tex::_Tex()
{
	pTex	= NULL;
	memset(&Image, 0, sizeof Image);
	memset(sName, 0, sizeof sName);
}

MpTex::MpTex()
{
}
		

MpInf::MpInf()
{
	nI = -1;
	memset(sN, 0, sizeof(sN));
}


MpFog::MpFog()
{
	m_dFt	= D3DFOG_LINEAR;
	m_dFc	= 0;														// Fog Color
	m_fFb	= 0;														// Fog Begin
	m_fFe	= 0;														// Fog End
	m_fFd	= 1.f;	
}

inline DWORD	__F2DW( FLOAT f )	{ return *((DWORD*)&f); }

void MpFog::SetFog(void* _pDev)
{
	LPDIRECT3DDEVICE9	pDev = (LPDIRECT3DDEVICE9)_pDev;

//	pDev->SetRenderState(D3DRS_FOGENABLE, TRUE);
	pDev->SetRenderState(D3DRS_FOGTABLEMODE, m_dFt);
	pDev->SetRenderState(D3DRS_FOGCOLOR,	 m_dFc);					// Set Fog
	pDev->SetRenderState(D3DRS_FOGSTART,	__F2DW(m_fFb));
	pDev->SetRenderState(D3DRS_FOGEND,		__F2DW(m_fFe));
	pDev->SetRenderState(D3DRS_FOGDENSITY,	__F2DW(m_fFd));
}


MpEnv::MpEnv()
{
	m_dAb	= 0;														// Ambient color
	memset(&m_Mtl, 0, sizeof(m_Mtl));									// Material	
	m_iLg	= 0;														// Lighting Num

	memset(m_pLg, 0, sizeof(m_pLg));
}


MpVec3T::MpVec3T()
: iZ(-1)
, iX(-1)
, iK(-1)
, p(0,0,0)
{
}


MpVec3T::MpVec3T(INT Z, INT X, INT K, const D3DXVECTOR3& P)
: iZ(Z)
, iX(X)
, iK(K)
, p(P)
{
}


MpPckT::MpPckT()
: vcPk(0,0,0)
, p0(0,0,0)
, p1(0,0,0)
, p2(0,0,0)
, fStlSrtR(FLT_MAX){}

MpPckT::MpPckT(D3DXVECTOR3& vcP, D3DXVECTOR3& P0, D3DXVECTOR3& P1, D3DXVECTOR3& P2, FLOAT& _fD)
: vcPk(vcP)
, p0(P0)
, p1(P1)
, p2(P2)
, fStlSrtR(_fD)
{
}


}// namespace Lcxmo


