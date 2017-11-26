// Interface for the CLcDevSprite class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcDevSprite_H_
#define _LcDevSprite_H_


class CLcDevSprite : public ILcDevSprite
{
protected:
	struct VtxDRHW
	{
		D3DXVECTOR4		p;
		DWORD			d;
		FLOAT			u0,v0;
		FLOAT			u1,v1;

		VtxDRHW(): d(0xFFFFFFFF){};

		enum	{	FVF = (D3DFVF_XYZRHW| D3DFVF_DIFFUSE| D3DFVF_TEX2),	};
	};

protected:
	LPDIRECT3DDEVICE9	m_pDev;
	VtxDRHW				m_pVtx[4];

public:
	CLcDevSprite();
	virtual ~CLcDevSprite();

	virtual	INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0);
	virtual void	Destroy();

	virtual	INT		Begin(DWORD=0);
	virtual	INT		End(DWORD=0);

	virtual	INT		Draw( void* pTx0				// 0 Stage Texture(<==ILcTex)
						, void* pTx1				// 1 Stage Texture(<==ILcTex)
						, const RECT* pRC0			// Draw Region 0 State Texture
						, const RECT* pRC1			// Draw Region 1 State Texture
						, void* pScl=NULL			// Scaling
						, void* pPos=NULL			// Position
						, DWORD dColor =0xFFFFFFFF	// color
						, DWORD dFilter=0x0			// Filter
						);

protected:
	inline	INT LcMath_MakePower2(INT a);
};

#endif


