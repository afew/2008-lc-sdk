// Implementation of the CLcxDXFile class.
//
////////////////////////////////////////////////////////////////////////////////


#include <windows.h>
#include <stdio.h>

#include <d3d9.h>
#include <d3dx9.h>

#include "ILcxDXFile.h"
#include "LcxDXFile.h"


TexLayer::TexLayer()
{
	for(int i=0; i<TexLayer::MAX_TEXTURE_LAYER; ++i)
	{
		p[i] = NULL;
	}
}

CLcxDXFile::CLcxDXFile()
{
	m_fElapse	= 0;
	m_dTimeCur	= 0;

	m_vcRot	= D3DXVECTOR3(0,0,0);
	m_vcScl	= D3DXVECTOR3(1,1,1);
	m_vcTrn	= D3DXVECTOR3(0,0,0);

	D3DXMatrixIdentity(&m_mtRot);
	D3DXMatrixIdentity(&m_mtWld);
}


CLcxDXFile::~CLcxDXFile()
{
}


INT CLcxDXFile::Query(char* sCmd, void* pData)
{
	if(0==_stricmp("Set Elapsed Time", sCmd))
	{
		DOUBLE t = *((DOUBLE*)pData);
		m_fElapse = t;
		return 0;
	}

	else if(0==_stricmp("Set World Matrix", sCmd))
	{
		D3DXMATRIX* p = (D3DXMATRIX*)pData;
		m_mtWld = *p;
		return 0;
	}

	return -1;
}




INT CLcxDXFile::GetType()
{
	return LCX_MDL_XFILE;
}

INT CLcxDXFile::SetPosition(FLOAT* float3)
{
	m_vcTrn = *((D3DXVECTOR3*)float3);

	m_mtWld._41 = m_vcTrn.x;
	m_mtWld._42 = m_vcTrn.y;
	m_mtWld._43 = m_vcTrn.z;

	return 0;
}


INT CLcxDXFile::GetPosition(FLOAT* float3)
{
	*((D3DXVECTOR3*)float3)= m_vcTrn;

	return 0;
}

INT CLcxDXFile::SetRotation(FLOAT* float3)
{
	m_vcRot = *((D3DXVECTOR3*)float3);

	D3DXMATRIX	mtS;
	D3DXMATRIX	mtY;
	D3DXMATRIX	mtZ;
	D3DXMATRIX	mtX;

	D3DXMatrixScaling(&mtS, m_vcScl.x, m_vcScl.y, m_vcScl.z);

	D3DXMatrixRotationY(&mtY, D3DXToRadian(m_vcRot.y));
	D3DXMatrixRotationZ(&mtZ, D3DXToRadian(m_vcRot.z));
	D3DXMatrixRotationX(&mtX, D3DXToRadian(m_vcRot.x));

	m_mtRot = mtY * mtZ * mtX;
	m_mtWld = mtS * m_mtRot;
	m_mtWld._41 = m_vcTrn.x;
	m_mtWld._42 = m_vcTrn.y;
	m_mtWld._43 = m_vcTrn.z;

	return 0;
}

INT CLcxDXFile::GetRotation(FLOAT* float3)
{
	*((D3DXVECTOR3*)float3)= m_vcRot;
	return 0;
}


INT CLcxDXFile::SetScaling(FLOAT* float3)
{
	m_vcScl = *((D3DXVECTOR3*)float3);

	D3DXMATRIX	mtS;
	D3DXMatrixScaling(&mtS, m_vcScl.x, m_vcScl.y, m_vcScl.z);

	m_mtWld = mtS * m_mtRot;
	m_mtWld._41 = m_vcTrn.x;
	m_mtWld._42 = m_vcTrn.y;
	m_mtWld._43 = m_vcTrn.z;

	return 0;
}

INT CLcxDXFile::GetScaling(FLOAT* float3)
{
	*((D3DXVECTOR3*)float3)= m_vcScl;
	return 0;
}



FLOAT CLcxDXFile::GetSortValue()
{
	return m_fStlSrtR;
}