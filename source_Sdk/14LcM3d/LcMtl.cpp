// Implementation of the CLcMtl classes.
//
////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <stdio.h>
#include <string.h>

#include <d3d9.h>
#include <d3dx9.h>

#include "ILcM3d.h"
#include "LcMtl.h"


CLcMtl::CLcMtl()
{
	m_pDev	= NULL;
	memset(&m_D3Mtl, 0, sizeof m_D3Mtl);
}


CLcMtl::~CLcMtl()
{
	CLcMtl::Destroy();
}


void CLcMtl::Destroy()
{
}


INT CLcMtl::Create(void* p1, void* p2, void* p3, void* p4)
{
	m_pDev	= (LPDIRECT3DDEVICE9)p1;

	return 0;
}


void CLcMtl::SetMaterial()
{
	m_pDev->SetMaterial(&m_D3Mtl);
}



void CLcMtl::SetDiffuse(FLOAT* pVal/*D3DXCOLOR*/ )
{
	D3DXCOLOR Color(pVal);
	m_D3Mtl.Diffuse = Color;
}


void CLcMtl::SetAmbient(FLOAT* pVal/*D3DXCOLOR*/ )
{
	D3DXCOLOR Color(pVal);
	m_D3Mtl.Ambient = Color;
}


void CLcMtl::SetSpecular(FLOAT* pVal/*D3DXCOLOR*/ )
{
	D3DXCOLOR Color(pVal);
	m_D3Mtl.Specular = Color;
}


void CLcMtl::SetEmissive(FLOAT* pVal/*D3DXCOLOR*/ )
{
	D3DXCOLOR Color(pVal);
	m_D3Mtl.Emissive = Color;
}


void CLcMtl::SetPower(FLOAT pVal)
{
	m_D3Mtl.Power = pVal;
}



FLOAT* CLcMtl::GetDiffuse()		{	return (FLOAT*)&m_D3Mtl.Diffuse.r;		}
FLOAT* CLcMtl::GetAmbient()		{	return (FLOAT*)&m_D3Mtl.Ambient.r;		}
FLOAT* CLcMtl::GetSpecular()	{	return (FLOAT*)&m_D3Mtl.Specular.r;		}
FLOAT* CLcMtl::GetEmissive()	{	return (FLOAT*)&m_D3Mtl.Emissive.r;		}

FLOAT CLcMtl::GetPower()		{	return m_D3Mtl.Power;		}

