// Interface for the CLcxPivot class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcxPivot_H_
#define _LcxPivot_H_


class CLcxPivot
{
public:
	struct VtxIdx
	{
		WORD	a, b, c;
		VtxIdx(){	a=0; b=0; c=0;	}
		VtxIdx(WORD A, WORD B, WORD C) : a(A), b(B), c(C){}
		enum	{FVF = (D3DFMT_INDEX16),};
	};

	struct VtxD
	{
		D3DXVECTOR3	p;
		DWORD		d;
		
		VtxD()		: p(0,0,0), d(0xFFFFFFFF){}
		VtxD(FLOAT X,FLOAT Y,FLOAT Z,DWORD D=0xFFFFFFFF) : p(X,Y,Z), d(D){}
		enum	{FVF = (D3DFVF_XYZ|D3DFVF_DIFFUSE),};
	};

protected:
	void*		m_pDev;

	VtxD*		m_pX;
	VtxD*		m_pY;
	VtxD*		m_pZ;
	VtxIdx*		m_pI;
	
	D3DXMATRIX	m_mtW;
	D3DXMATRIX	m_mtI;
	D3DXMATRIX	m_mtX;
	D3DXMATRIX	m_mtY;
	D3DXMATRIX	m_mtZ;
	D3DXMATRIX*	m_mtP;			// Parent World Matrix Pointer

	FLOAT		m_fAH;
	FLOAT		m_fAW;

public:
	CLcxPivot();
	virtual ~CLcxPivot();

	INT		Create(void* pDev);
	void	Destroy();

	INT		FrameMove();
	void	Render();

	void	SetWorldMatrix(D3DXMATRIX* mtW);
	void	SetTMPointer(D3DXMATRIX* mtW);
};


#endif
