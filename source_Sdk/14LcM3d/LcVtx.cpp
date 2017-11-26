// Implementation of the CLcVtx classes.
// PC Game Vertex
////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <stdio.h>
#include <string.h>

#include <d3d9.h>
#include <d3dx9.h>

#include "ILcM3d.h"
#include "LcVtx.h"


CLcVtx::CLcVtx()
{
	m_pDev	= NULL;

	m_dFVF		= 0;
	m_dStrd		= 0;
	m_nNum		= 0;

	m_pVB		= NULL;
	m_dPtType	= D3DPT_TRIANGLELIST;
	m_dPtNum	= 0;

	m_pTx[0]	= NULL;	m_pTx[4]	= NULL;
	m_pTx[1]	= NULL;	m_pTx[5]	= NULL;
	m_pTx[2]	= NULL;	m_pTx[6]	= NULL;
	m_pTx[3]	= NULL;	m_pTx[7]	= NULL;
}


CLcVtx::~CLcVtx()
{
	CLcVtx::Destroy();
}


void CLcVtx::Destroy()
{
	if(m_pVB)
	{
		free(m_pVB);
		m_pVB	= NULL;
	}
}


INT CLcVtx::Create(void* p1, void* p2, void* p3, void* p4)
{
	if(FAILED(SetupDevice(p1)))
		return -1;

	m_nNum			= (INT)p2;
	void*	pVtxS	= p3;
	INT		iSize	= (INT)p4;

	m_pVB = malloc(m_nNum * m_dStrd);

	if(pVtxS)
	{
		memcpy(m_pVB, pVtxS, iSize);
	}

	return 0;
}


void CLcVtx::Render()
{
	switch(m_dFVF)
	{
		case LcVtx::VtxRHWD::FVF :
			m_pDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
			m_pDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );

			m_pDev->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
			m_pDev->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE );
			break;

		case LcVtx::VtxD::FVF :
			m_pDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
			m_pDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );

			m_pDev->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
			m_pDev->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE );
			break;

		case LcVtx::VtxDUV1::FVF :
			m_pDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
			m_pDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
			m_pDev->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

			m_pDev->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
			m_pDev->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			m_pDev->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
			break;


		default:
			m_pDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
			m_pDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
			m_pDev->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

			m_pDev->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
			m_pDev->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			m_pDev->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
			break;
	}

	for(int i=0; i<4; ++i)
	{
		if(m_pTx[i])
			m_pDev->SetTexture( i, m_pTx[i]);
	}

	m_pDev->SetFVF(m_dFVF);
	m_pDev->DrawPrimitiveUP( (D3DPRIMITIVETYPE)m_dPtType, m_dPtNum, m_pVB, m_dStrd);
}


void CLcVtx::DrawPrimitive(DWORD PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
{
	m_pDev->SetFVF(m_dFVF);
	m_pDev->DrawPrimitiveUP( (D3DPRIMITIVETYPE)m_dPtType, m_dPtNum, m_pVB, m_dStrd);
}



void* CLcVtx::GetVertex()
{
	return m_pVB;
}


DWORD CLcVtx::GetFVF()
{
	return m_dFVF;
}


UINT CLcVtx::GetStrid()
{
	return m_dStrd;
}


INT CLcVtx::GetVtxNum()
{
	return m_nNum;
}



INT CLcVtx::Lock(UINT OffsetToLock, UINT SizeToLock, void** ppbData, DWORD Flags)
{
	if(!m_pVB)
		return -1;

	if(ppbData)
		*ppbData = m_pVB;

	return 0;
}

INT CLcVtx::Unlock()
{
	if(!m_pVB)
		return -1;

	return 0;
}


void CLcVtx::SetTexture(INT nStage, void* pTx)
{
	m_pTx[nStage] = (LPDIRECT3DTEXTURE9)pTx;
}






INT CLcVtx::SetupDevice(void* p)
{
	m_pDev	= (LPDIRECT3DDEVICE9)p;

	return 0;
}



void CLcVtx::SetFVF(DWORD dFVF)
{
	m_dFVF = dFVF;
}

void CLcVtx::SetStrdSize(UINT iStrd)
{
	m_dStrd = iStrd;
}


void CLcVtx::SetPtType(DWORD dPtType)
{
	m_dPtType = dPtType;
}

void CLcVtx::SetPtNum(UINT iPtNum)
{
	m_dPtNum = iPtNum;
}




void CLcVtx::SetPos(INT nIdx, FLOAT X, FLOAT Y, FLOAT Z, FLOAT W)
{
	if(!m_pVB)
		return;

	if(LcVtx::VtxD::FVF == m_dFVF)
	{
		LcVtx::VtxD* pVtx = (LcVtx::VtxD*)m_pVB;
		pVtx[nIdx].p[0] = X;
		pVtx[nIdx].p[1] = Y;
		pVtx[nIdx].p[2] = Z;
	}

	else if(LcVtx::VtxDUV1::FVF == m_dFVF)
	{
		LcVtx::VtxDUV1* pVtx = (LcVtx::VtxDUV1*)m_pVB;
		pVtx[nIdx].p[0] = X;
		pVtx[nIdx].p[1] = Y;
		pVtx[nIdx].p[2] = Z;
	}

	else if(LcVtx::VtxN::FVF == m_dFVF)
	{
		LcVtx::VtxN* pVtx = (LcVtx::VtxN*)m_pVB;
		pVtx[nIdx].p[0] = X;
		pVtx[nIdx].p[1] = Y;
		pVtx[nIdx].p[2] = Z;
	}
	
	else if(LcVtx::VtxUV1::FVF == m_dFVF)
	{
		LcVtx::VtxUV1* pVtx = (LcVtx::VtxUV1*)m_pVB;
		pVtx[nIdx].p[0] = X;
		pVtx[nIdx].p[1] = Y;
		pVtx[nIdx].p[2] = Z;
	}

	else if(LcVtx::VtxNUV1::FVF == m_dFVF)
	{
		LcVtx::VtxNUV1* pVtx = (LcVtx::VtxNUV1*)m_pVB;
		pVtx[nIdx].p[0] = X;
		pVtx[nIdx].p[1] = Y;
		pVtx[nIdx].p[2] = Z;
	}

	else if(LcVtx::VtxNDUV1::FVF == m_dFVF)
	{
		LcVtx::VtxNDUV1* pVtx = (LcVtx::VtxNDUV1*)m_pVB;
		pVtx[nIdx].p[0] = X;
		pVtx[nIdx].p[1] = Y;
		pVtx[nIdx].p[2] = Z;
	}


	else if(LcVtx::VtxNDUV3::FVF == m_dFVF)
	{
		LcVtx::VtxNDUV3* pVtx = (LcVtx::VtxNDUV3*)m_pVB;
		pVtx[nIdx].p[0] = X;
		pVtx[nIdx].p[1] = Y;
		pVtx[nIdx].p[2] = Z;
	}


	else if(LcVtx::VtxRHWD::FVF == m_dFVF)
	{
		LcVtx::VtxRHWD* pVtx = (LcVtx::VtxRHWD*)m_pVB;
		pVtx[nIdx].p[0] = X;
		pVtx[nIdx].p[1] = Y;
		pVtx[nIdx].p[2] = Z;
		pVtx[nIdx].p[3] = W;
	}
}

void CLcVtx::SetNormal(INT nIdx, FLOAT X, FLOAT Y, FLOAT Z)
{
	if(!m_pVB)
		return;

	if(LcVtx::VtxN::FVF == m_dFVF)
	{
		LcVtx::VtxN* pVtx = (LcVtx::VtxN*)m_pVB;
		pVtx[nIdx].n[0] = X;
		pVtx[nIdx].n[1] = Y;
		pVtx[nIdx].n[2] = Z;
	}
	
	else if(LcVtx::VtxNUV1::FVF == m_dFVF)
	{
		LcVtx::VtxNUV1* pVtx = (LcVtx::VtxNUV1*)m_pVB;
		pVtx[nIdx].n[0] = X;
		pVtx[nIdx].n[1] = Y;
		pVtx[nIdx].n[2] = Z;
	}

	else if(LcVtx::VtxNDUV1::FVF == m_dFVF)
	{
		LcVtx::VtxNDUV1* pVtx = (LcVtx::VtxNDUV1*)m_pVB;
		pVtx[nIdx].n[0] = X;
		pVtx[nIdx].n[1] = Y;
		pVtx[nIdx].n[2] = Z;
	}

	else if(LcVtx::VtxNDUV3::FVF == m_dFVF)
	{
		LcVtx::VtxNDUV3* pVtx = (LcVtx::VtxNDUV3*)m_pVB;
		pVtx[nIdx].n[0] = X;
		pVtx[nIdx].n[1] = Y;
		pVtx[nIdx].n[2] = Z;
	}
}


void CLcVtx::SetDiffuse(INT nIdx, FLOAT R, FLOAT G, FLOAT B, FLOAT A)
{
	DWORD dColor = D3DXCOLOR(R,G, B, A);

	if(!m_pVB)
		return;

	if(LcVtx::VtxD::FVF == m_dFVF)
	{
		LcVtx::VtxD* pVtx = (LcVtx::VtxD*)m_pVB;
		pVtx[nIdx].d = dColor;
	}

	else if(LcVtx::VtxDUV1::FVF == m_dFVF)
	{
		LcVtx::VtxDUV1* pVtx = (LcVtx::VtxDUV1*)m_pVB;
		pVtx[nIdx].d = dColor;
	}

	else if(LcVtx::VtxNDUV1::FVF == m_dFVF)
	{
		LcVtx::VtxNDUV1* pVtx = (LcVtx::VtxNDUV1*)m_pVB;
		pVtx[nIdx].d = dColor;
	}

	else if(LcVtx::VtxNDUV3::FVF == m_dFVF)
	{
		LcVtx::VtxNDUV3* pVtx = (LcVtx::VtxNDUV3*)m_pVB;
		pVtx[nIdx].d = dColor;
	}

	else if(LcVtx::VtxRHWD::FVF == m_dFVF)
	{
		LcVtx::VtxRHWD* pVtx = (LcVtx::VtxRHWD*)m_pVB;
		pVtx[nIdx].d = dColor;
	}
}


void CLcVtx::SetUV(INT nIdx, INT nUV, FLOAT U, FLOAT V)
{
	if(!m_pVB)
		return;

	if(LcVtx::VtxDUV1::FVF == m_dFVF)
	{
		LcVtx::VtxDUV1* pVtx = (LcVtx::VtxDUV1*)m_pVB;
		pVtx[nIdx].u = U;
		pVtx[nIdx].v = V;
	}

	else if(LcVtx::VtxNDUV1::FVF == m_dFVF)
	{
		LcVtx::VtxNDUV1* pVtx = (LcVtx::VtxNDUV1*)m_pVB;
		pVtx[nIdx].u = U;
		pVtx[nIdx].v = V;
	}

	else if(LcVtx::VtxNDUV3::FVF == m_dFVF)
	{
		LcVtx::VtxNDUV3* pVtx = (LcVtx::VtxNDUV3*)m_pVB;

		if(0==nUV)
		{
			pVtx[nIdx].u0 = U;
			pVtx[nIdx].v0 = V;
		}
		else if(1==nUV)
		{
			pVtx[nIdx].u1 = U;
			pVtx[nIdx].v1 = V;
		}
		else if(2==nUV)
		{
			pVtx[nIdx].u2 = U;
			pVtx[nIdx].v2 = V;
		}
	}

}



INT	LcM3d_CreateVB_PC(
					ILcVtx** pData	//
					, void* p1		// Vertex Structure
					, void* p2		// Need D3D Device
					, void* p3		// Primitive Type
					, void* p4		// Primitive Count
					, void* p5		// Total Vertices
					, void* p6		// Vertex memory copy source.
					, void* p7		// Source Total Size
					, void* p8		// No Use
					, void* p9		// No Use
					)
{
	(*pData)= NULL;

	DWORD	dFVF	= LcVtx::VtxD::FVF;
	UINT	dStrd	= sizeof(LcVtx::VtxD);
	CLcVtx* pObj	= NULL;

	char*	sVtx	= (char*)p1;
	void*	pDev	= (void*)p2;
	char*	sPtType	= (char*)p3;
	INT		iPtNum	= (INT  )p4;

	pObj	= new CLcVtx;

	if(0==_stricmp("TRIANGLE LIST", sPtType))
		pObj->SetPtType(D3DPT_TRIANGLELIST);

	else if(0==_stricmp("TRIANGLE STRIP", sPtType))
		pObj->SetPtType(D3DPT_TRIANGLESTRIP);

	else if(0==_stricmp("POINT LIST", sPtType))
		pObj->SetPtType(D3DPT_POINTLIST);

	else if(0==_stricmp("TRIANGLE FAN", sPtType))
		pObj->SetPtType(D3DPT_TRIANGLEFAN);


	pObj->SetPtNum(iPtNum);


	if(0==_stricmp("Diffuse", sVtx))
	{
		dFVF	= LcVtx::VtxD::FVF;
		dStrd	= sizeof(LcVtx::VtxD);
	}

	else if(0==_stricmp("Diffuse Texture1", sVtx))
	{
		dFVF	= LcVtx::VtxDUV1::FVF;
		dStrd	= sizeof(LcVtx::VtxDUV1);
	}

	else if(0==_stricmp("Normal", sVtx))
	{
		dFVF	= LcVtx::VtxN::FVF;
		dStrd	= sizeof(LcVtx::VtxN);
	}
	
	else if(0==_stricmp("Texture1", sVtx))
	{
		dFVF	= LcVtx::VtxUV1::FVF;
		dStrd	= sizeof(LcVtx::VtxUV1);
	}

	else if(0==_stricmp("Normal Texture1", sVtx))
	{
		dFVF	= LcVtx::VtxNUV1::FVF;
		dStrd	= sizeof(LcVtx::VtxNUV1);
	}

	else if(0==_stricmp("Normal Diffuse Texture1", sVtx))
	{
		dFVF	= LcVtx::VtxNDUV1::FVF;
		dStrd	= sizeof(LcVtx::VtxNDUV1);
	}

	else if(0==_stricmp("Normal Diffuse Texture3", sVtx))
	{
		dFVF	= LcVtx::VtxNDUV3::FVF;
		dStrd	= sizeof(LcVtx::VtxNDUV3);
	}

	else if(0==_stricmp("RHW Diffuse", sVtx))
	{
		dFVF	= LcVtx::VtxRHWD::FVF;
		dStrd	= sizeof(LcVtx::VtxRHWD);
	}

	pObj->SetFVF(dFVF);
	pObj->SetStrdSize(dStrd);

	if(FAILED(pObj->Create(p2, p5, p6, p7)))
	{
		delete pObj;
		return -1;
	}

	(*pData) = pObj;

	return 0;
}
