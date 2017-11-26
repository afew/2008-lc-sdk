// Implementation of the CLcField class.
//
////////////////////////////////////////////////////////////////////////////////


#include "../_StdAfx.h"


CLcField::CLcField()
{
	m_pDev	= NULL;
	m_pTx	= NULL;
}


CLcField::~CLcField()
{
	Destroy();
}


INT CLcField::Create(LPDIRECT3DDEVICE9 pDev)
{
	m_pDev	= pDev;


	m_pVtx[0] = VtxDUV1(-512, -80, -512, 0, 8, 0xFFFFFFFF);
	m_pVtx[1] = VtxDUV1(-512, -80,  512, 0, 0, 0xFFFFFFFF);
	m_pVtx[2] = VtxDUV1( 512, -80, -512, 8, 8, 0xFFFFFFFF);
	m_pVtx[3] = VtxDUV1( 512, -80,  512, 8, 0, 0xFFFFFFFF);

	HINSTANCE	hInst = GetModuleHandle(NULL);
	HRSRC		hRsc = FindResource( hInst, MAKEINTRESOURCE(IDR_DDS_LAWN), "dds");
	DWORD		dSize= SizeofResource(hInst,hRsc);
	HGLOBAL		hMem = LoadResource(hInst, hRsc);
	LPVOID		pSrc = LockResource(hMem);

	D3DXCreateTextureFromFileInMemory(m_pDev, pSrc, dSize, &m_pTx);

//	UnlockResource(hMem);
	FreeResource(hMem);

	return 0;
}


void CLcField::Destroy()
{
	SAFE_RELEASE(	m_pTx		);
}


INT	CLcField::FrameMove()
{
	return 0;
}

void CLcField::Render()
{
	// Render Lines
	m_pDev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pDev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	m_pDev->SetSamplerState(0, D3DSAMP_MINFILTER , D3DTEXF_LINEAR);
	m_pDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pDev->SetSamplerState(0, D3DSAMP_MIPFILTER , D3DTEXF_LINEAR);

	m_pDev->SetRenderState( D3DRS_LIGHTING,  FALSE);
	m_pDev->SetRenderState( D3DRS_ALPHABLENDENABLE,  FALSE);
	m_pDev->SetRenderState( D3DRS_ALPHATESTENABLE,  FALSE);
	
	m_pDev->SetTexture(0, m_pTx);
	m_pDev->SetFVF(VtxDUV1::FVF);
	m_pDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_pVtx, sizeof(VtxDUV1));
	m_pDev->SetTexture(0, 0);
}