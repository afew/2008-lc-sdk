// Implementation of the CLcDevSprite class.
//
////////////////////////////////////////////////////////////////////////////////


#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include <Lc/ILcTex.h>

#include "ILcDev.h"
#include "LcDevSprite.h"



CLcDevSprite::CLcDevSprite()
{
	m_pDev	= NULL;
}

CLcDevSprite::~CLcDevSprite()
{
	CLcDevSprite::Destroy();
}


void CLcDevSprite::Destroy()
{
}


INT CLcDevSprite::Create(void* p1, void* p2, void* p3, void* p4)
{
	HRESULT hr=0;
	INT		i=0;

	m_pDev	= (LPDIRECT3DDEVICE9)p1;
	
	for(i=0; i<4; ++i)
	{
		m_pVtx[i].p.z = (	0.f	);
		m_pVtx[i].p.w = (	1.f	);
	}



	return 0;
}


INT CLcDevSprite::Begin(DWORD dVal)
{
	return 0;
}


INT CLcDevSprite::End(DWORD dVal)
{
	return 0;
}






INT CLcDevSprite::Draw(	void* ptTx0					// 0 Stage Texture(<==ILcTex)
					  , void* ptTx1					// 1 Stage Texture(<==ILcTex)
					  , const RECT* pRc0			// Draw Region 0 State Texture
					  , const RECT* pRc1			// Draw Region 1 State Texture
					  , void* pScl					// Scaling
					  , void* pPos					// Position
					  , DWORD dColor				// color
					  , DWORD dFilter)				// Filter
{
	if(!ptTx0)
		return -1;

	LPDIRECT3DTEXTURE9	pTx0 = NULL;
	LPDIRECT3DTEXTURE9	pTx1 = NULL;

	ILcTex* pLcTx0	= (ILcTex*)ptTx0;				// Image Information
	ILcTex* pLcTx1	= (ILcTex*)ptTx1;				// Image Information

	DWORD	dTxW0	= pLcTx0->GetWidth();			// Get Width
	DWORD	dTxH0	= pLcTx0->GetHeight();			// Get Height
	DWORD	dTxW1	= dTxW0;
	DWORD	dTxH1	= dTxH0;

	DWORD	dTxC0	= pLcTx0->GetColorKey();		// Get Creation color Key
	DWORD	dTxF0	= pLcTx0->GetFilter();			// Get Creation Filter

	void*	pTex0	= NULL;
	void*	pTex1	= NULL;


	pTex0	= pLcTx0->GetPointer();					// Get D3DTexture Pointer
	pTx0	= (LPDIRECT3DTEXTURE9)pTex0;
	
	if(pLcTx1)
	{
		pTex1	= pLcTx1->GetPointer();
		pTx1	= (LPDIRECT3DTEXTURE9)pTex1;

		dTxW1	= pLcTx1->GetWidth();
		dTxH1	= pLcTx1->GetHeight();
	}

	FLOAT	fScl[2]	={1.f, 1.f};
	FLOAT	fPos[2]	={0.f, 0.f};


	FLOAT	fX0		= FLOAT(pRc0->right - pRc0->left);
	FLOAT	fY0		= FLOAT(pRc0->bottom- pRc0->top );
	FLOAT	fX1		= 0;
	FLOAT	fY1		= 0;

	FLOAT	fMxW0	= (FLOAT) LcMath_MakePower2(dTxW0);
	FLOAT	fMxH0	= (FLOAT) LcMath_MakePower2(dTxH0);
	FLOAT	fMxW1	= 0;
	FLOAT	fMxH1	= 0;



	m_pVtx[0].u0	= (pRc0->left+0.5f)/(fMxW0);
	m_pVtx[0].v0	= (pRc0->top +0.5f)/(fMxH0);

	m_pVtx[3].u0	= (pRc0->right -0.5F)/(fMxW0);
	m_pVtx[3].v0	= (pRc0->bottom-0.5F)/(fMxH0);

	m_pVtx[1].u0	= m_pVtx[3].u0;
	m_pVtx[1].v0	= m_pVtx[0].v0;

	m_pVtx[2].u0	= m_pVtx[0].u0;
	m_pVtx[2].v0	= m_pVtx[3].v0;


	if(pScl)
	{
		FLOAT*	vScl = (FLOAT*)pScl;
		fScl[0]= vScl[0];
		fScl[1]= vScl[1];

		fX0	*= fScl[0];
		fY0	*= fScl[1];
	}

	if(pPos)
	{
		FLOAT*	vPos = (FLOAT*)pPos;
		fPos[0]= vPos[0];
		fPos[1]= vPos[1];
	}


	m_pVtx[0].p.x = fPos[0]	-0.5f;
	m_pVtx[0].p.y = fPos[1]	-0.5f;

	m_pVtx[1].p.x = m_pVtx[0].p.x + fX0	;
	m_pVtx[1].p.y = m_pVtx[0].p.y		;

	m_pVtx[2].p.x = m_pVtx[0].p.x		;
	m_pVtx[2].p.y = m_pVtx[0].p.y + fY0	;

	m_pVtx[3].p.x = m_pVtx[0].p.x + fX0	;
	m_pVtx[3].p.y = m_pVtx[0].p.y + fY0	;


	if(fX0<0.f)
	{
		FLOAT	_t = fabsf(fX0);
		for(int i=0; i<4; ++i)
		{
			m_pVtx[i].p.x += _t;
		}
	}

	if(fY0<0.f)
	{
		FLOAT	_t = fabsf(fY0);
		for(int i=0; i<4; ++i)
		{
			m_pVtx[i].p.y += _t;
		}
	}


	m_pVtx[0].d = dColor;
	m_pVtx[1].d = dColor;
	m_pVtx[2].d = dColor;
	m_pVtx[3].d = dColor;


	if(pRc1)
	{
		fX1		= FLOAT(pRc1->right - pRc1->left);
		fY1		= FLOAT(pRc1->bottom- pRc1->top );

		fMxW1	= (FLOAT) LcMath_MakePower2(dTxW1);
		fMxH1	= (FLOAT) LcMath_MakePower2(dTxH1);

		m_pVtx[0].u1	= (pRc1->left+0.5f)/(fMxW1);
		m_pVtx[0].v1	= (pRc1->top +0.5f)/(fMxH1);

		m_pVtx[3].u1	= (pRc1->right -0.5F)/(fMxW1);
		m_pVtx[3].v1	= (pRc1->bottom-0.5F)/(fMxH1);

		m_pVtx[1].u1	= m_pVtx[3].u1;
		m_pVtx[1].v1	= m_pVtx[0].v1;

		m_pVtx[2].u1	= m_pVtx[0].u1;
		m_pVtx[2].v1	= m_pVtx[3].v1;
	}


	m_pDev->SetRenderState(D3DRS_CULLMODE,			D3DCULL_NONE);
	m_pDev->SetRenderState( D3DRS_DITHERENABLE,		FALSE);
	m_pDev->SetRenderState( D3DRS_ZENABLE,			FALSE);
	m_pDev->SetRenderState(D3DRS_LIGHTING,			FALSE);
	m_pDev->SetRenderState( D3DRS_SPECULARENABLE,	FALSE);
	
	m_pDev->SetRenderState(D3DRS_ALPHATESTENABLE,	FALSE);
	m_pDev->SetRenderState(D3DRS_ALPHABLENDENABLE,	TRUE);
	
	m_pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pDev->SetSamplerState(0, D3DSAMP_MIPFILTER, dFilter);
	m_pDev->SetSamplerState(0, D3DSAMP_MAGFILTER, dFilter);
	m_pDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
	m_pDev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	m_pDev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	m_pDev->SetSamplerState(1, D3DSAMP_MIPFILTER, dFilter);
	m_pDev->SetSamplerState(1, D3DSAMP_MAGFILTER, dFilter);
	m_pDev->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_NONE);
	m_pDev->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	m_pDev->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	m_pDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	m_pDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	m_pDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	m_pDev->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
	m_pDev->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);

	m_pDev->SetTexture(0, pTx0);

	if(pTx1)
	{
		m_pDev->SetTexture(1, pTx1);

		m_pDev->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_CURRENT );
		m_pDev->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_TEXTURE );
		m_pDev->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_MODULATE);
		
		m_pDev->SetTextureStageState( 1, D3DTSS_ALPHAARG1, D3DTA_CURRENT );
		m_pDev->SetTextureStageState( 1, D3DTSS_ALPHAARG2, D3DTA_TEXTURE );
		m_pDev->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
	}


	m_pDev->SetFVF(CLcDevSprite::VtxDRHW::FVF);
	m_pDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, m_pVtx, sizeof (CLcDevSprite::VtxDRHW));


	m_pDev->SetTexture(0, NULL);
	m_pDev->SetTexture(1, NULL);

	m_pDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_pDev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pDev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	
	m_pDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	m_pDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	m_pDev->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	m_pDev->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pDev->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	m_pDev->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_CURRENT);
	m_pDev->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	m_pDev->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
	
	m_pDev->SetTextureStageState( 1, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
	m_pDev->SetTextureStageState( 1, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
	m_pDev->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);

	return 0;
}





inline INT CLcDevSprite::LcMath_MakePower2(INT a)
{
	int n=0;
	int t=a;

	while(t)
	{
		t>>=1;
		++n;
	}

	return (0x1<<(n-1) ^ a) ? 0x1<<n : 0x1<<(n-1);
};