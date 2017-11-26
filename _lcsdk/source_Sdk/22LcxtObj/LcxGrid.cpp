// Implementation of the CLcxGrid class.
//
////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <D3D9.h>
#include <d3dx9.h>

#include "ILcxtObj.h"
#include "LcxtObj.h"

#include "LcxGrid.h"


CLcxGrid::CLcxGrid()
{
	strcpy(m_sType, "XYZ Grid");
}

CLcxGrid::~CLcxGrid()
{
	Destroy();
}



void CLcxGrid::Destroy()
{
}



INT CLcxGrid::Create(void* p1, void* p2, void* p3, void* p4)
{
	INT		i, j;
	FLOAT	fMax = 10000;

	m_pDev	= (LPDIRECT3DDEVICE9)p1;
	
	// m_pLine = new CLcxGrid::VtxD[ 6*2  + 8* 8];

	m_pLine[ 0] = CLcxGrid::VtxD(-fMax,     0,     0, 0xFF770000);
	m_pLine[ 1] = CLcxGrid::VtxD(    0,     0,     0, 0xFF770000);
	m_pLine[ 2] = CLcxGrid::VtxD(    0,     0,     0, 0xFFFF0000);
	m_pLine[ 3] = CLcxGrid::VtxD( fMax,     0,     0, 0xFFFF0000);
	
	m_pLine[ 4] = CLcxGrid::VtxD(    0, -fMax,     0, 0xFF007700);
	m_pLine[ 5] = CLcxGrid::VtxD(    0,     0,     0, 0xFF007700);
	m_pLine[ 6] = CLcxGrid::VtxD(    0,     0,     0, 0xFF00FF00);
	m_pLine[ 7] = CLcxGrid::VtxD(    0,  fMax,     0, 0xFF00FF00);
	
	m_pLine[ 8] = CLcxGrid::VtxD(    0,     0, -fMax, 0xFF000077);
	m_pLine[ 9] = CLcxGrid::VtxD(    0,     0,     0, 0xFF000077);
	m_pLine[10] = CLcxGrid::VtxD(    0,     0,     0, 0xFF0000FF);
	m_pLine[11] = CLcxGrid::VtxD(    0,     0,  fMax, 0xFF0000FF);

	j =6 * 2;

	for(i=0; i<8; ++i)
	{
		m_pLine[j + 8*i +0 ] = CLcxGrid::VtxD(-128.f, 1.f,  16.f* (i+1), (i%2)? 0xFF999999 : 0xFF666666);
		m_pLine[j + 8*i +1 ] = CLcxGrid::VtxD( 128.f, 1.f,  16.f* (i+1), (i%2)? 0xFF999999 : 0xFF666666);
		m_pLine[j + 8*i +2 ] = CLcxGrid::VtxD(-128.f, 1.f, -16.f* (i+1), (i%2)? 0xFF999999 : 0xFF666666);
		m_pLine[j + 8*i +3 ] = CLcxGrid::VtxD( 128.f, 1.f, -16.f* (i+1), (i%2)? 0xFF999999 : 0xFF666666);

		m_pLine[j + 8*i +4 ] = CLcxGrid::VtxD( 16.f* (i+1), 1.f,-128.f, (i%2)? 0xFF999999 : 0xFF666666);
		m_pLine[j + 8*i +5 ] = CLcxGrid::VtxD( 16.f* (i+1), 1.f, 128.f, (i%2)? 0xFF999999 : 0xFF666666);
		m_pLine[j + 8*i +6 ] = CLcxGrid::VtxD(-16.f* (i+1), 1.f,-128.f, (i%2)? 0xFF999999 : 0xFF666666);
		m_pLine[j + 8*i +7 ] = CLcxGrid::VtxD(-16.f* (i+1), 1.f, 128.f, (i%2)? 0xFF999999 : 0xFF666666);
	}

	return 0;
}



INT CLcxGrid::FrameMove()
{
	return 0;
}



void CLcxGrid::Render()
{
	// Render Lines
	DWORD dMnLgt;
	DWORD dMnFog;

	DWORD dMnAblend;
	DWORD dMnAtest;

	m_pDev->GetRenderState( D3DRS_LIGHTING, &dMnLgt);
	m_pDev->GetRenderState( D3DRS_FOGENABLE, &dMnFog);
	m_pDev->GetRenderState( D3DRS_ALPHABLENDENABLE,  &dMnAblend);
	m_pDev->GetRenderState( D3DRS_ALPHATESTENABLE,  &dMnAtest);

	m_pDev->SetRenderState( D3DRS_FOGENABLE, FALSE);
	m_pDev->SetRenderState( D3DRS_LIGHTING, FALSE);
	m_pDev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDev->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE);

	m_pDev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pDev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	m_pDev->SetSamplerState(0, D3DSAMP_MINFILTER , D3DTEXF_LINEAR);
	m_pDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pDev->SetSamplerState(0, D3DSAMP_MIPFILTER , D3DTEXF_LINEAR);

	
	m_pDev->SetTexture(0, 0);
	m_pDev->SetFVF(CLcxGrid::VtxD::FVF);
	m_pDev->DrawPrimitiveUP(D3DPT_LINELIST, 6 + 32, m_pLine, sizeof(CLcxGrid::VtxD));

	m_pDev->SetRenderState( D3DRS_LIGHTING, dMnLgt);
	m_pDev->SetRenderState( D3DRS_FOGENABLE, dMnFog);
	m_pDev->SetRenderState( D3DRS_ALPHABLENDENABLE, dMnAblend);
	m_pDev->SetRenderState( D3DRS_ALPHATESTENABLE, dMnAtest);
}



INT CLcxGrid::Query(char* sCmd, void* pData)
{
	return CLcxtObj::Query(sCmd, pData);
}


