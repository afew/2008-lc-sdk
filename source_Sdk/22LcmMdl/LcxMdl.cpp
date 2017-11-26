// Implementation of the CLcxMdl class.
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

#include <Lc/ILcxCam.h>

#include "ILcmMdl.h"
#include "ILcmMdlX.h"

#include "LcxMdl.h"


namespace Lcm
{

CLcxMdl::CLcxMdl()
{
	m_pDev	= NULL;
	m_pCam	= NULL;

	m_pMsD	= NULL;
	m_pMsS	= NULL;

	m_BndInf.SetColor(0xFF666666);
	m_BndInf.SetOwner(this);

	m_pMscl1	= NULL;					// miscellany	Field
	m_pMscl2	= NULL;					// miscellany	Local
	m_pMscl3	= NULL;

	m_fDstCam	= 0;
	m_fStlSrtR	= 0;

	m_vcEps		= VEC3(0,0,0);

	m_vcRot		= VEC3(0,0,0);
	m_vcScl		= VEC3(1,1,1);
	m_vcTrn		= VEC3(0,0,0);

	m_mtRot		= MATA(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
	m_mtScl		= MATA(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
	m_mtWld		= MATA(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
}

CLcxMdl::~CLcxMdl()
{
	if(m_pMsD)
	{
		ILcmMesh* pMshDst = (ILcmMesh*)m_pMsD;
		delete pMshDst;
		m_pMsD = NULL;
	}
}


INT CLcxMdl::Create(void* p1, void* p2, void* p3, void* p4)
{
	return -1;
}

void CLcxMdl::Destroy()
{
}

INT CLcxMdl::FrameMove()
{
	return -1;
}

void CLcxMdl::Render()
{
}



INT CLcxMdl::Query(char* sCmd, void* pData)
{
	if(0==_stricmp("set bound", sCmd))
	{
		this->SetBndInf(pData);
	
		return 0;
	}

	else if(0==_stricmp("get bound", sCmd))
	{
		TbndAB* pBnd = GetBndInf();
		INT nAddress = ((INT)pBnd);
		*((INT*)pData) = nAddress;

		return 0;
	}

	else if(0==_stricmp("set local", sCmd))
	{
		m_pMscl2	= pData;
	
		return 0;
	}

	else if(0==_stricmp("get local", sCmd))
	{
		INT nAddress = ((INT)m_pMscl2);
		*((INT*)pData) = nAddress;
		
		return 0;
	}

	else if(0==_stricmp("set field", sCmd))
	{
		m_pMscl1	= pData;
		
		return 0;
	}

	else if(0==_stricmp("get field", sCmd))
	{
		INT nAddress = ((INT)m_pMscl1);
		*((INT*)pData) = nAddress;
		
		return 0;
	}

	else if(0==_stricmp("set package", sCmd))
	{
		m_pMscl3	= pData;
		
		return 0;
	}

	else if(0==_stricmp("get package", sCmd))
	{
		INT nAddress = ((INT)m_pMscl3);
		*((INT*)pData) = nAddress;
		
		return 0;
	}

	else if(0==_stricmp("set ids", sCmd))
	{
		WORD*	wIds = (WORD*)pData;

		this->SetId(wIds[0], wIds[1], wIds[2], wIds[3]);
		
		return 0;
	}

	else if(0==_stricmp("get ids", sCmd))
	{
		WORD nI0 = this->GetId1();
		WORD nI1 = this->GetId2();
		WORD nI2 = this->GetId3();
		WORD nI3 = this->GetId4();

		*((WORD*)(pData)+0) = nI0;
		*((WORD*)(pData)+1) = nI1;
		*((WORD*)(pData)+2) = nI2;
		*((WORD*)(pData)+3) = nI3;
		
		return 0;
	}

	else if(0==_stricmp("bound box render", sCmd))
	{
		this->BndBoxRender();
		return 0;
	}


	else if(0==_stricmp("Set Mesh Source", sCmd))
	{
		m_pMsS = (ILcmMesh*)pData;
	}

	else if(0==_stricmp("Get Mesh Source", sCmd))
	{
		*((ILcmMesh**)pData)= (ILcmMesh*)m_pMsS;
		return 0;
	}

	else if(0==_stricmp("Set Mesh Dest", sCmd))
	{
		m_pMsD = (ILcmMesh*)pData;
	}

	else if(0==_stricmp("Get Mesh Dest", sCmd))
	{
		*((ILcmMesh**)pData)= (ILcmMesh*)m_pMsD;
		return 0;
	}




	else if(0==_stricmp("Set Camera", sCmd))
	{
		m_pCam	= pData;
		return 0;
	}

	else if(0==_stricmp("Get Camera", sCmd))
	{
		*((ILcxCam**)pData)= (ILcxCam*)m_pCam;
		return 0;
	}
	
	else if(0==_stricmp("Set Device", sCmd))
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



void CLcxMdl::BndBoxTransform()
{
	//Transform Bound Box
	ILcmMesh*	pMshS = (ILcmMesh*)m_pMsS;
	
	for(int i=0; i<8; ++i)
	{
		TbndAB*	pBndInf= (TbndAB*)pMshS->GetBndInf();
		D3DXVECTOR3  vcSrc = pBndInf->vcE[i].p;
		D3DXVECTOR3* vcDst = &(m_BndInf.vcE[i].p);
		
		D3DXVec3TransformCoord(vcDst, &vcSrc, &m_mtWld);
		*vcDst += m_vcEps;
	}

	m_BndInf.vcC = m_BndInf.vcE[0].p + m_BndInf.vcE[7].p;
	m_BndInf.vcC *=0.5f;

	m_BndInf.fR = D3DXVec3Length( &(m_BndInf.vcC - m_BndInf.vcE[0].p));
}

void CLcxMdl::BndBoxRender()
{
	DWORD dMnLgt;
	m_pDev->GetRenderState( D3DRS_LIGHTING, &dMnLgt);
	
	// Render BoundBox
	m_pDev->SetRenderState( D3DRS_LIGHTING, FALSE);
	m_pDev->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE);
	m_pDev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDev->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);

	m_BndInf.RenderBox(m_pDev);

	m_pDev->SetRenderState( D3DRS_LIGHTING, dMnLgt);
}


FLOAT CLcxMdl::GetSortValue()
{
	return m_fStlSrtR;
}



INT CLcxMdl::SetPosition(FLOAT* float3)
{
	m_vcTrn = *((VEC3*)float3);

	WorldMatrixSetup();

	return 0;
}


INT CLcxMdl::GetPosition(FLOAT* float3)
{
	*((VEC3*)float3)= m_vcTrn;

	return 0;
}

INT CLcxMdl::SetRotation(FLOAT* float3)
{
	m_vcRot = *((VEC3*)float3);

	MATA	mtY;
	MATA	mtZ;
	MATA	mtX;

	D3DXMatrixRotationY(&mtY, D3DXToRadian(m_vcRot.y));
	D3DXMatrixRotationZ(&mtZ, D3DXToRadian(m_vcRot.z));
	D3DXMatrixRotationX(&mtX, D3DXToRadian(m_vcRot.x));

	m_mtRot = mtY * mtZ * mtX;

	WorldMatrixSetup();

	return 0;
}

INT CLcxMdl::GetRotation(FLOAT* float3)
{
	*((VEC3*)float3)= m_vcRot;
	return 0;
}


INT CLcxMdl::SetScaling(FLOAT* float3)
{
	m_vcScl = *((VEC3*)float3);
	
	m_mtScl._11 = m_vcScl.x;
	m_mtScl._22 = m_vcScl.y;
	m_mtScl._33 = m_vcScl.z;

	WorldMatrixSetup();

	return 0;
}

INT CLcxMdl::GetScaling(FLOAT* float3)
{
	*((VEC3*)float3)= m_vcScl;
	return 0;
}




void CLcxMdl::WorldMatrixSetup()
{
	m_mtWld = m_mtScl * m_mtRot;
	m_mtWld._41 = m_vcTrn.x;
	m_mtWld._42 = m_vcTrn.y;
	m_mtWld._43 = m_vcTrn.z;
}



void CLcxMdl::SetBndInf(void* pBnd)
{
	memcpy(&m_BndInf, pBnd, sizeof(TbndAB));

	m_BndInf.SetColor(0xFF666666);
	m_BndInf.SetOwner(this);
}


TbndAB* CLcxMdl::GetBndInf() const
{
	return (TbndAB*)&m_BndInf;
}


}// namespace Lcm

