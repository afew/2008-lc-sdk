// Implementation of the CLcmMesh class.
//
////////////////////////////////////////////////////////////////////////////////


#include <windows.h>
#include <stdio.h>

#include <d3d9.h>
#include <d3dx9.h>

#include <Lc/LcType.h>
#include <Lc/LcString.h>
#include <Lc/LcUtil.h>

#include "ILcmMdl.h"
#include "ILcmMdlX.h"

#include "LcmMesh.h"


namespace Lcm
{

CLcmMesh::CLcmMesh()
{
	m_pLcOrg	= NULL;
	m_pGeo		= NULL;

	m_pDev		= NULL;

	m_BndInf.SetColor(0xFF666666);
}


CLcmMesh::~CLcmMesh()
{
	Destroy();
}

void CLcmMesh::Destroy()
{
	if(!m_pLcOrg)
	{
		SAFE_DELETE(	m_pGeo		);
		m_pVtx	= NULL;
	}

	else
	{
		SAFE_FREE(	m_pVtx	);
	}

	m_pVtx	= NULL;
}


INT CLcmMesh::Create(void* p1, void* p2, void* p3, void* p4)
{
	m_pDev = (PDEV)p1;

	if(p2)
		Clone(p2);

	else
	{
		struct _T
		{
			char*	sF;		// Model Full Path
			char*	sN;		// Model Name
		};

		_T* t = (_T*)p3;

		m_pGeo = new TmshInf;

		m_pGeo->SetMdlName(t->sN);
		Load(t->sF);
	}

	return 0;
}


INT CLcmMesh::FrameMove()
{
	return 0;
}


void CLcmMesh::Render()
{
	TmshInf*	pGeo = m_pGeo;

	if(m_pLcOrg)
		pGeo = m_pLcOrg->m_pGeo;

	m_pDev->SetTexture(0, (PDTX)pGeo->pTex);
	
	m_pDev->SetFVF(pGeo->dFVF);
	m_pDev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0
									, pGeo->iNvx
									, pGeo->iNix
									, pGeo->pIdx
									, D3DFMT_INDEX16
									, m_pVtx
									, pGeo->iVxS
									);

//	m_pDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
//	m_pDev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);
//	m_pDev->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE);
//	m_pDev->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
//
//	m_BndInf.RenderBox(m_pDev);
}



INT CLcmMesh::Query(char* sCmd, void* pData)
{
	if(0==_stricmp("Set Device", sCmd))
	{
		m_pDev = (PDEV)pData;

		return 0;
	}

	else if(0==_stricmp("Get Device", sCmd))
	{
		*((PDEV*)pData)= m_pDev;
		return 0;
	}

	return -1;
}



INT CLcmMesh::GetType()
{
	TmshInf*	pGeo = m_pGeo;

	if(m_pLcOrg)
		pGeo = m_pLcOrg->m_pGeo;

	return pGeo->nType;
}

void* CLcmMesh::GetGeometry()
{
	return m_pGeo;
}


void* CLcmMesh::GetBufVtx()
{
	return m_pVtx;
}


INT CLcmMesh::Load(char* sFile)
{
	INT		i=0;
	char	sTmp[512];

	GetPrivateProfileString("Header", "nType", NULL, sTmp, sizeof(sTmp), sFile);
	m_pGeo->nType = atoi(sTmp);

	GetPrivateProfileString("Header", "dFvf", NULL, sTmp, sizeof(sTmp), sFile);
	m_pGeo->dFVF = atol(sTmp);

	GetPrivateProfileString("Header", "iVcS", NULL, sTmp, sizeof(sTmp), sFile);
	m_pGeo->iVxS= atoi(sTmp);

	GetPrivateProfileString("Header", "iNfce", NULL, sTmp, sizeof(sTmp), sFile);
	m_pGeo->iNix = atoi(sTmp);

	GetPrivateProfileString("Header", "iNvtx", NULL, sTmp, sizeof(sTmp), sFile);
	m_pGeo->iNvx = atoi(sTmp);


	GetPrivateProfileString("Header", "Texture", NULL, sTmp, sizeof(sTmp), sFile);
	strcpy(m_pGeo->sTex, sTmp);

	if(0 == m_pGeo->iNix || 0 == m_pGeo->iNvx)
		return -1;

	m_pGeo->pIdx = (Lcm::Idx16*) malloc(m_pGeo->iNix * sizeof(Lcm::Idx16));
	m_pGeo->pVtx = malloc(m_pGeo->iNvx * m_pGeo->iVxS);

	for(i=0; i<m_pGeo->iNix; ++i)
	{
		Lcm::Idx16	idx;
		GetPrivateProfileString("Idx", LcStr_Format("%d", i), NULL, sTmp, sizeof(sTmp), sFile);
		sscanf(sTmp, "%d %d %d", &idx.a, &idx.b, &idx.c);

		m_pGeo->pIdx[i] = idx;
	}

	if( Lcm::VtxDUV1::FVF == m_pGeo->dFVF)
	{
		for(i=0; i<m_pGeo->iNvx; ++i)
		{
			Lcm::VtxDUV1	tVtx;
			GetPrivateProfileString("Vtx", LcStr_Format("%d", i), NULL, sTmp, sizeof(sTmp), sFile);

			sscanf(sTmp, "%f %f %f   %f %f %x"
				, &tVtx.p.x, &tVtx.p.y, &tVtx.p.z
				, &tVtx.u, &tVtx.v
				, &tVtx.d
				);

			((Lcm::VtxDUV1*)m_pGeo->pVtx)[i] = tVtx;
		}
	}

	else if(Lcm::VtxNDUV1::FVF == m_pGeo->dFVF)
	{
		for(i=0; i<m_pGeo->iNvx; ++i)
		{
			Lcm::VtxNDUV1	tVtx;
			GetPrivateProfileString("Vtx", LcStr_Format("%d", i), NULL, sTmp, sizeof(sTmp), sFile);

			sscanf(sTmp, "%f %f %f   %f %f %f  %f %f %x"
				, &tVtx.p.x, &tVtx.p.y, &tVtx.p.z
				, &tVtx.n.x, &tVtx.n.y, &tVtx.n.z
				, &tVtx.u, &tVtx.v
				, &tVtx.d
				);

			tVtx.p *= 0.4f;
			((Lcm::VtxNDUV1*)m_pGeo->pVtx)[i] = tVtx;
		}
	}


	D3DXVECTOR3 vcMin;
	D3DXVECTOR3 vcMax;

	// Set Max and Min vertex Position
	D3DXComputeBoundingBox(	(D3DXVECTOR3*)m_pGeo->pVtx
							, m_pGeo->iNvx
							, D3DXGetFVFVertexSize( m_pGeo->dFVF)
							, &vcMin
							, &vcMax);

	m_BndInf.Set(vcMin, vcMax);


	m_pVtx	= m_pGeo->pVtx;

	return 0;
}



void CLcmMesh::Clone(void* pRhs)
{
	m_pLcOrg	= (CLcmMesh*)pRhs;
	m_pGeo		= m_pLcOrg->m_pGeo;

	memcpy(&this->m_BndInf, m_pLcOrg->GetBndInf(), sizeof m_BndInf);

	INT		iVtxSize = m_pGeo->iNvx * m_pGeo->iVxS;

	m_pVtx	= malloc(iVtxSize);
	memcpy(m_pVtx, m_pLcOrg->m_pVtx, iVtxSize);
}



void* CLcmMesh::GetBndInf()
{
	return (void*)&m_BndInf;
}


}// namespace Lcm

