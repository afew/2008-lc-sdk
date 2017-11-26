// Implementation of the CLcmSolid class.
//
////////////////////////////////////////////////////////////////////////////////


#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <d3d9.h>
#include <d3dx9.h>

#include <Lc/LcType.h>
#include <Lc/LcUtil.h>
#include <Lc/LcUtilDx.h>

#include <Lc/ILcxCam.h>

#include "ILcmMdl.h"
#include "ILcmMdlX.h"

#include "LcxMdl.h"																// Model Object
#include "LcmSolid.h"															// Model Object


namespace Lcm
{

CLcmBill::CLcmBill()
{
}



CLcmBill::~CLcmBill()
{
	Destroy();
}



INT CLcmBill::Create(void* p1, void* p2, void* p3, void* p4)
{
	m_pDev = (PDEV)p1;

	m_pMsS = (ILcmMesh*)p2;
	m_pMsD = p3;

	if(NULL == m_pMsD)
	{
		ILcmMesh*	pMshS = (ILcmMesh*)m_pMsS;
		ILcmMesh* pMshDst = NULL;

		if(FAILED(MdlX_CreateMsh(NULL, &pMshDst, m_pDev, pMshS)))
			return -1;

		m_pMsD = pMshDst;
	}

	return 0;
}


void CLcmBill::Destroy()
{
}



INT CLcmBill::FrameMove()
{
	if(!m_pMsS)
		return 0;

	int	 i =0;

	ILcmMesh*		pMshS = (ILcmMesh*)m_pMsS;
	ILcmMesh*		pMshD = (ILcmMesh*)m_pMsD;
	ILcxCam*		pCam  = (ILcxCam*)m_pCam;	

	Lcm::TmshInf*	pGeo = (Lcm::TmshInf*)pMshS->GetGeometry();

	int		iSize	= pGeo->GetNumVtx() * pGeo->GetZeroStrd();
	void*	pMshDst = pMshD->GetBufVtx();
	void*	pMshSrc = pMshS->GetBufVtx();

	m_mtRot  = *pCam->GetMatrixViwI();
	VEC3	vcCam= VEC3(m_mtRot._41, m_mtRot._42, m_mtRot._43);
	VEC3	vcZ	 = VEC3(m_mtRot._31, m_mtRot._32, m_mtRot._33);
	VEC3	vcTmp;

	m_mtRot._41 =0.f;
	m_mtRot._42 =0.f;
	m_mtRot._43 =0.f;

	m_mtWld = m_mtScl * m_mtRot;
	
	m_mtWld._41 = m_vcTrn.x;
	m_mtWld._42 = m_vcTrn.y;
	m_mtWld._43 = m_vcTrn.z;

	vcTmp = m_vcTrn - vcCam;
	m_fStlSrtR = D3DXVec3Dot(&vcZ, &vcTmp);

	memcpy(pMshDst, pMshSrc, iSize);

	Lcm::VtxDUV1* pVtx = (Lcm::VtxDUV1*)(pMshD->GetBufVtx());

	m_vcEps.y = -pVtx[2].p.y*.9f;

	SetColor( D3DXCOLOR( 1, 1, 1,  (1000.f)/fabsf(m_fStlSrtR)  ));

	for(i=0; i<4; ++i)
	{
		pVtx[i].d = m_dC;
		
		D3DXVec3TransformCoord(&pVtx[i].p, &pVtx[i].p, &m_mtWld);
		pVtx[i].p += m_vcEps;
	}

	//Transform Bound Box
	BndBoxTransform();


	return 0;
}

void CLcmBill::Render()
{
	if(!m_pMsD)
		return;

	DWORD dMnLgt;
	m_pDev->GetRenderState( D3DRS_LIGHTING, &dMnLgt);

	m_pDev->SetRenderState( D3DRS_LIGHTING, FALSE);
	m_pDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDev->SetRenderState(D3DRS_ALPHAREF, 0x30);
	m_pDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	m_pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
	m_pDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	
	m_pDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	m_pDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	m_pDev->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	
	m_pDev->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
	m_pDev->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	m_pDev->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );

	ILcmMesh* pMsD = (ILcmMesh*)m_pMsD;
	pMsD->Render();


	m_pDev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDev->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE);
	m_pDev->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
	m_pDev->SetRenderState( D3DRS_LIGHTING, dMnLgt);
}



INT CLcmBill::GetType()
{
	return LCX_MDL_BILL;
}










CLcmSolid::CLcmSolid()
{
}



CLcmSolid::~CLcmSolid()
{
	Destroy();
}



INT CLcmSolid::Create(void* p1, void* p2, void* p3, void* p4)
{
	m_pDev = (PDEV)p1;
	m_pMsS = (ILcmMesh*)p2;

	FLOAT	fRotY = rand()%360;
	FLOAT	fRotZ = -7 + rand()%15;
	FLOAT	fRotX = -7 + rand()%15;

	SetRotation( (FLOAT*)&VEC3(fRotX, fRotY, fRotZ));
	
	return 0;
}


void CLcmSolid::Destroy()
{
}


INT CLcmSolid::FrameMove()
{
	ILcmMesh*		pMshS = (ILcmMesh*)m_pMsS;
	ILcxCam*		pCam  = (ILcxCam*)m_pCam;

	MATA	mtViwI	= *pCam->GetMatrixViwI();
	
	VEC3 vcCam	= VEC3(mtViwI._41, mtViwI._42, mtViwI._43);
	VEC3 vcZ	= VEC3(mtViwI._31, mtViwI._32, mtViwI._33);
	VEC3 vcTmp	= m_vcTrn - vcCam;

	m_fStlSrtR = D3DXVec3Dot(&vcZ, &vcTmp);


	//Transform Bound Box
	BndBoxTransform();

	return 0;
}


void CLcmSolid::Render()
{
	if(!m_pMsS)
		return;

	DWORD dMnLgt;
	m_pDev->GetRenderState( D3DRS_LIGHTING, &dMnLgt);

	m_pDev->SetTransform(D3DTS_WORLD, &m_mtWld);

//	m_pDev->SetRenderState(D3DRS_LIGHTING, FALSE);
//	m_pDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	
	
	m_pDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDev->SetRenderState(D3DRS_ALPHAREF, 0x80);
	m_pDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	

	m_pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	m_pDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	m_pDev->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

	m_pDev->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
	m_pDev->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	m_pDev->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	
	ILcmMesh* pMsS = (ILcmMesh*)m_pMsS;
	pMsS->Render();

	LcD3D_SetWorldIdentity(m_pDev);
	m_pDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pDev->SetRenderState( D3DRS_LIGHTING, dMnLgt);
}


INT CLcmSolid::GetType()
{
	return LCX_MDL_SOLID;
}


}// namespace Lcm


