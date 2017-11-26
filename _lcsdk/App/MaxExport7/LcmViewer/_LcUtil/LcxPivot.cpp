// Implementation of the CLcxPivot class.
//
////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "LcxPivot.h"


#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }


CLcxPivot::CLcxPivot()
{
	m_pX	= NULL;
	m_pY	= NULL;
	m_pZ	= NULL;
	m_pI	= NULL;

	m_mtP	= NULL;
}

CLcxPivot::~CLcxPivot()
{
	Destroy();
}



INT CLcxPivot::Create(void* pDev)
{
	m_pDev	= pDev;

	m_fAH	= 3.f;
	m_fAW	= 0.2f;

	D3DXMatrixIdentity(&m_mtW);
	D3DXMatrixIdentity(&m_mtI);

	D3DXMatrixRotationY(&m_mtX,  D3DXToRadian(90) );
	D3DXMatrixRotationX(&m_mtY, -D3DXToRadian(90) );
	D3DXMatrixIdentity(&m_mtZ);

	m_pX	= new VtxD[ 5];
	m_pY	= new VtxD[ 6];
	m_pZ	= new VtxD[ 6];
	m_pI	= new VtxIdx[8];

	m_pX[0]	= VtxD( m_fAH, 0.0F, 0.0F, 0xFFFF0000);
	m_pX[1]	= VtxD( 0.0F, m_fAW,-m_fAW, 0xFFFF0000);
	m_pX[2]	= VtxD( 0.0F, m_fAW, m_fAW, 0xFFFF0000);
	m_pX[3]	= VtxD( 0.0F,-m_fAW, m_fAW, 0xFFFF0000);
	m_pX[4]	= VtxD( 0.0F,-m_fAW,-m_fAW, 0xFFFF0000);

	m_pY[0]	= VtxD( 0.0F, m_fAH, 0.0F, 0xFF00FF00);
	m_pY[1]	= VtxD( m_fAW, 0.0F,-m_fAW, 0xFF00FF00);
	m_pY[2]	= VtxD(-m_fAW, 0.0F,-m_fAW, 0xFF00FF00);
	m_pY[3]	= VtxD(-m_fAW, 0.0F, m_fAW, 0xFF00FF00);
	m_pY[4]	= VtxD( m_fAW, 0.0F, m_fAW, 0xFF00FF00);


	m_pZ[0]	= VtxD( 0.0F, 0.0F, m_fAH, 0xFF0000FF);
	m_pZ[1]	= VtxD( m_fAW, m_fAW, 0.0F, 0xFF0000FF);
	m_pZ[2]	= VtxD(-m_fAW, m_fAW, 0.0F, 0xFF0000FF);
	m_pZ[3]	= VtxD(-m_fAW,-m_fAW, 0.0F, 0xFF0000FF);
	m_pZ[4]	= VtxD( m_fAW,-m_fAW, 0.0F, 0xFF0000FF);

	m_pI[0]= VtxIdx( 0, 1, 2);
	m_pI[1]= VtxIdx( 0, 2, 3);
	m_pI[2]= VtxIdx( 0, 3, 4);
	m_pI[3]= VtxIdx( 0, 4, 1);
	m_pI[4]= VtxIdx( 2, 1, 3);
	m_pI[5]= VtxIdx( 4, 3, 1);

	return 0;
}

void CLcxPivot::Destroy()
{
	SAFE_DELETE_ARRAY(	m_pX	);
	SAFE_DELETE_ARRAY(	m_pY	);
	SAFE_DELETE_ARRAY(	m_pZ	);
	SAFE_DELETE_ARRAY(	m_pI	);
}

INT CLcxPivot::FrameMove()
{
	return 0;
}

void CLcxPivot::Render()
{
	HRESULT hr=-1;

	static D3DXMATRIX mtI(1,0,0,0,  0,1,0,0,  0,0,1,0,  0,0,0,1);

	LPDIRECT3DDEVICE9 pDev	= (LPDIRECT3DDEVICE9)m_pDev;
	
	DWORD	dLight	=0;
	DWORD	dFill	=0;

	pDev->GetRenderState(D3DRS_LIGHTING, &dLight);
	pDev->GetRenderState(D3DRS_FILLMODE, &dFill);

	pDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	if(NULL == m_mtP)
		pDev->SetTransform(D3DTS_WORLD, &m_mtW);
	else
		pDev->SetTransform(D3DTS_WORLD, m_mtP);

	pDev->SetTexture(0,0);
	pDev->SetFVF(VtxD::FVF);
	pDev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 5, 6, m_pI, D3DFMT_INDEX16, m_pX, sizeof(VtxD));
	pDev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 5, 6, m_pI, D3DFMT_INDEX16, m_pY, sizeof(VtxD));
	pDev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 5, 6, m_pI, D3DFMT_INDEX16, m_pZ, sizeof(VtxD));


	pDev->SetRenderState(D3DRS_LIGHTING, dLight);
	pDev->SetRenderState(D3DRS_FILLMODE, dFill);

	pDev->SetTransform(D3DTS_WORLD, &mtI);
}



void CLcxPivot::SetWorldMatrix(D3DXMATRIX* mtW)
{
	m_mtW = *mtW;
}

void CLcxPivot::SetTMPointer(D3DXMATRIX* mtW)
{
	m_mtP = mtW;
}