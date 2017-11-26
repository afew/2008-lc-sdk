// Interface for the CLcGrid class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcGrid_H_
#define _LcGrid_H_

class CLcGrid
{
public:
	struct VtxD
	{
		VEC3	p;
		DWORD	d;
		
		VtxD()								{	p.x=p.y=p.z=0.f;d=0xFFFFFFFF;	}
		VtxD(FLOAT X,FLOAT Y,FLOAT Z,DWORD D){	p.x=X; p.y=Y; p.z=Z; d=D;		}
		enum	{FVF = (D3DFVF_XYZ|D3DFVF_DIFFUSE),};
	};

protected:
	LPDIRECT3DDEVICE9	m_pDev;
	VtxD*		m_pLine;
	
public:
	CLcGrid();
	~CLcGrid();

	INT		Create(LPDIRECT3DDEVICE9 pDev);
	void	Destroy();

	INT		FrameMove();
	void	Render();
};

#endif