// Implementation of the CLcGrid class.
//
////////////////////////////////////////////////////////////////////////////////


#include "../_StdAfx.h"


CLcGrid::CLcGrid()
{
	m_pDev	= NULL;
	m_pLine	= NULL;
}

CLcGrid::~CLcGrid()
{
	Destroy();
}


INT CLcGrid::Create(LPDIRECT3DDEVICE9 pDev)
{
	m_pDev	= pDev;

	INT		i;
	INT		j;
	FLOAT	fMax;

	fMax = 10000;
	m_pLine = new VtxD[ (6 + 20*4 ) * 2];

	m_pLine[ 0] = VtxD(-fMax,     0,     0, 0xFFFF0000);
	m_pLine[ 1] = VtxD(    0,     0,     0, 0xFFFF0000);
	m_pLine[ 2] = VtxD(    0,     0,     0, 0xFFFF0000);
	m_pLine[ 3] = VtxD( fMax,     0,     0, 0xFFFF0000);
	
	m_pLine[ 4] = VtxD(    0, -fMax,     0, 0xFF00FF00);
	m_pLine[ 5] = VtxD(    0,     0,     0, 0xFF00FF00);
	m_pLine[ 6] = VtxD(    0,     0,     0, 0xFF00FF00);
	m_pLine[ 7] = VtxD(    0,  fMax,     0, 0xFF00FF00);
	
	m_pLine[ 8] = VtxD(    0,     0, -fMax, 0xFF0000FF);
	m_pLine[ 9] = VtxD(    0,     0,     0, 0xFF0000FF);
	m_pLine[10] = VtxD(    0,     0,     0, 0xFF0000FF);
	m_pLine[11] = VtxD(    0,     0,  fMax, 0xFF0000FF);

	j =6 * 2;

	DWORD	d1 = 0xFF999999;
	DWORD	d2 = 0xFF555555;

	VtxD*	pLine = &m_pLine[j];

	for(i=0; i<20; ++i)
	{
		pLine[8*i +0 ] = VtxD(-100, 0,  5* (i+1), (i%2)? d1 : d2);
		pLine[8*i +1 ] = VtxD( 100, 0,  5* (i+1), (i%2)? d1 : d2);
		pLine[8*i +2 ] = VtxD(-100, 0, -5* (i+1), (i%2)? d1 : d2);
		pLine[8*i +3 ] = VtxD( 100, 0, -5* (i+1), (i%2)? d1 : d2);

		pLine[8*i +4 ] = VtxD(  5* (i+1), 0,-100, (i%2)? d1 : d2);
		pLine[8*i +5 ] = VtxD(  5* (i+1), 0, 100, (i%2)? d1 : d2);
		pLine[8*i +6 ] = VtxD( -5* (i+1), 0,-100, (i%2)? d1 : d2);
		pLine[8*i +7 ] = VtxD( -5* (i+1), 0, 100, (i%2)? d1 : d2);
	}


	return 0;
}


void CLcGrid::Destroy()
{
	SAFE_DELETE_ARRAY(		m_pLine		);
}


INT	CLcGrid::FrameMove()
{
	return 0;
}

void CLcGrid::Render()
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
	
	m_pDev->SetTexture(0, 0);
	m_pDev->SetFVF(VtxD::FVF);
	m_pDev->DrawPrimitiveUP(D3DPT_LINELIST, 6 + 4*20, m_pLine, sizeof(VtxD));
}