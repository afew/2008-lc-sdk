// Interface for the CLcxDXFile class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcxDXFile_H_
#define _LcxDXFile_H_


struct TexLayer
{
	enum	{	MAX_TEXTURE_LAYER	=	8,	};

	union
	{
		struct
		{
			LPDIRECT3DTEXTURE9	p0;
			LPDIRECT3DTEXTURE9	p1;
			LPDIRECT3DTEXTURE9	p2;
			LPDIRECT3DTEXTURE9	p3;
			LPDIRECT3DTEXTURE9	p4;
			LPDIRECT3DTEXTURE9	p5;
			LPDIRECT3DTEXTURE9	p6;
			LPDIRECT3DTEXTURE9	p7;
		};

		LPDIRECT3DTEXTURE9 p[MAX_TEXTURE_LAYER];
	};

	TexLayer();
};


// enum for various skinning modes possible
enum ESkinType
{
	SKINNING_NONINDEXED,
	SKINNING_INDEXED,
	SKINNING_SOFTWARE,
	SKINNING_INDEXEDVS,
	SKINNING_INDEXEDHLSL,
};

class CLcxDXFile : public ILcxDXFile
{
protected:
	FLOAT		m_fStlSrtR	;
	FLOAT		m_fElapse	;			// Elapsed Time
	DOUBLE		m_dTimeCur	;			// Current Time

	D3DXVECTOR3	m_vcRot		;			// X, Y, Z Rotation
	D3DXVECTOR3	m_vcScl		;
	D3DXVECTOR3	m_vcTrn		;
	D3DXMATRIX	m_mtRot		;
	D3DXMATRIX	m_mtWld		;			// World Matrix

public:
	CLcxDXFile();
	virtual ~CLcxDXFile();

	virtual INT		Query(char* sCmd, void* pData);

	virtual	INT		GetType();

	virtual INT		SetPosition(FLOAT* float3);
	virtual INT		GetPosition(FLOAT* float3);
	virtual INT		SetRotation(FLOAT* float4);		// if Type is LCX_MDL_ACM: Axis(x,y,z) and radian w.
	virtual INT		GetRotation(FLOAT* float4);		// else x, y, z radian for RotMatrixY(y) * RotMatrixZ(z) *  else X, Y, Z radian for RotMatrixY(y) * RotMatrixX(x)
	virtual INT		SetScaling(FLOAT* float3);
	virtual INT		GetScaling(FLOAT* float3);

	virtual	FLOAT	GetSortValue();
};



#endif


