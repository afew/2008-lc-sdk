// Interface for the CLcField class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcField_H_
#define _LcField_H_

class CLcField
{
public:
	struct VtxDUV1
	{
		VEC3	p;
		DWORD	d;
		FLOAT	u,v;
		

		VtxDUV1()	{}
		VtxDUV1(FLOAT X,FLOAT Y,FLOAT Z
			,FLOAT U,FLOAT V
			, DWORD D=0xFFFFFFFF):p(X,Y,Z)
				,u(U),v(V)
				,d(D){}
		enum	{FVF = (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1),};
	};


protected:
	LPDIRECT3DDEVICE9	m_pDev;
	VtxDUV1		m_pVtx[4];
	PDTX		m_pTx;
	
public:
	CLcField();
	~CLcField();

	INT		Create(LPDIRECT3DDEVICE9 pDev);
	void	Destroy();

	INT		FrameMove();
	void	Render();
};

#endif