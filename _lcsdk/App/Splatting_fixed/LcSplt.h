// Interface for the CLcSplt class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcSplt_H_
#define _LcSplt_H_


typedef D3DXVECTOR3							VEC3;
typedef D3DXVECTOR4							VEC4;
typedef FLOAT								VEC6[6];

typedef LPDIRECT3DDEVICE9					PDEV;
typedef LPDIRECT3DTEXTURE9					PDTX;

typedef LPDIRECT3DVERTEXDECLARATION9		PDVD;
typedef LPDIRECT3DPIXELSHADER9				PDPS;


class CLcSplt
{
public:
	struct VtxIdx
	{
		union	{	struct	{	WORD a;	WORD b;	WORD c;	};	WORD m[3];	};

		VtxIdx()						{	a = 0;	  b = 1;	c = 2;		}
		VtxIdx(WORD A, WORD B, WORD C)	{	a = A;	  b = B;	c = C;		}
		VtxIdx(WORD* R)					{	a = R[0]; b = R[1];	c = R[2];	}

		operator WORD* ()				{	return (WORD *) &a;				}
		operator CONST WORD* () const	{	return (CONST WORD *) &a;		}
	};

	struct TexWgt
	{
		PDTX	pTexB;

		TexWgt() : pTexB(0)
		{
		}

		~TexWgt()
		{
			if(pTexB){	pTexB->Release();	pTexB=NULL;	}
		}
	};


	struct VtxSplt
	{
		VEC3	p;
		VEC3	n;
		DWORD	d;
		FLOAT	u, v;
		VEC6	ts;

		VtxSplt()	: p(0,0,0),n(0,0,0),u(0),v(0),d(0xFFFFFFFF)
		{
			ts[0] = 0;
			ts[1] = 0;
			ts[2] = 0;
			ts[3] = 0;
			ts[4] = 0;
			ts[5] = 0;
		}
		VtxSplt(FLOAT X, FLOAT Y, FLOAT Z
				, FLOAT _X, FLOAT _Y, FLOAT _Z
				, FLOAT U, FLOAT V
				, DWORD D=0xFFFFFFFF)	: p(X,Y,Z),n(_X,_Y,_Z),u(U),v(V),d(D)
		{
			ts[0] = 0;
			ts[1] = 0;
			ts[2] = 0;
			ts[3] = 0;
			ts[4] = 0;
			ts[5] = 0;
		}

		enum {	FVF= (D3DFVF_XYZ|D3DFVF_NORMAL|\
					D3DFVF_DIFFUSE|D3DFVF_TEX3|\
					D3DFVF_TEXCOORDSIZE2(0)|\
					D3DFVF_TEXCOORDSIZE4(1)|\
					D3DFVF_TEXCOORDSIZE2(2)
					)
				};
	};


protected:
	PDEV		m_pDev;

	INT			m_nVtx;															// Number of Vertices
	INT			m_nFce;															// Number of Indices

	VtxIdx*		m_pFce;
	VtxSplt*	m_pVtx;

	INT			m_iTile;														// Number of tile for X
	FLOAT		m_fWidth;														// Width of tile for Cell

	INT			m_nTex;															// Layer Number
	TexWgt*		m_pTex;															// Texture Layer


public:
	CLcSplt();
	virtual ~CLcSplt();

	INT		Create(PDEV pDev);
	void	Destroy();

	INT		FrameMove();
	void	Render();

	INT		Restore();
	void	Invalidate();

protected:
	void	CalculateMap();
	void	CalculateMapTile(int nTx);
	void	SetupNormal();
	void	CalculateNormal(VEC3* pOut, const VEC3* v0, const VEC3* v1, const VEC3* v2);
	INT		TextureLoad(char* sFileName
						, PDTX& texture
						, DWORD _color=0x00FFFFFF
						, D3DXIMAGE_INFO *pSrcInfo=NULL
						, DWORD Filter= (D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR)
						, DWORD MipFilter= (D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR)
						, D3DFORMAT d3dFormat = D3DFMT_UNKNOWN);

};

#endif
