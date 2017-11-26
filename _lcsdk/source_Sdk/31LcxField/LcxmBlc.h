// Interface for the CLcxmBlc class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcxmBlc_H_
#define _LcxmBlc_H_


class CLcxmBlc
{
public:
	char			m_sN[64];				// Map Name

	D3DXMATRIX		m_mtW;					// World Matrix
	D3DXVECTOR3		m_vcP;					// Position
	
	INT				m_iNx;					// Mesh: Number of tile for Width
	FLOAT			m_fWx;					// Mesh: Width of tile for x;
	FLOAT			m_fUV;					// Mesh: UV Width

	INT				m_iVs;					// Vertex Size
	DWORD			m_FVF;
	INT				m_iNi;					// Index Number
	INT				m_iNv;					// Vertex Number

	Lcxmo::VtxIdx*	m_pFce;
	Lcxmo::VtxSpl*	m_pVtx;
	INT*			m_pVtt;
	
	Lcxmo::lsMpTex	m_vTex;					// Tiling Texture Name and Pointer List

	Lcm::ILcmPack*	m_pMdb2D;				// 2D Model Pack
	Lcm::ILcmPack*	m_pMdb3D;				// 3D Model Pack

	BOOL			m_bRend	;				// Rendering Enable

protected:
	PDEV			m_pDev	;				// LPDIRECT3DDEVICE9
	void*			m_pDst	;				// Parent Manager Local Field
	
public:
	CLcxmBlc();
	virtual ~CLcxmBlc();
	
	INT		Create(void* p1, void* p2, void* p3, void* p4);
	void	Destroy();
	
	INT		FrameMove();
	
	void	RenderBlc();
	void	RenderObj();
	
	void	RenderTline();														// Show Block
	
	FLOAT	GetHeight(D3DXVECTOR3& pos);
	
	void	CreateMesh(INT iNx, FLOAT fWx, FLOAT fUV);
	void	LayerSetup(Lcxmo::lsMpTex* pTxLst);
	void	LayerAdd(Lcxmo::MpTex* pTex);

	void	SetNormal();
	void	SetIndex();
	D3DXVECTOR3	NormalVec(int z, int x);
	
	void	SetDiffuse(int nIdx=-1);
	void	SetPos(D3DXVECTOR3	pos)	{	m_vcP = pos	;	}
	D3DXVECTOR3	GetPos()				{	return m_vcP;	}
	INT		GetTileN()					{	return m_iNx;	}					// Get Tile Number of X or Z
	FLOAT	GetTileW()					{	return m_fWx;	}					// Get Tile Width



	void	CalculateMap();
	void	CalculateMapTile(int nTx);
};


typedef std::vector<CLcxmBlc* >	lsLcl;
typedef lsLcl::iterator			itLcl;

#endif


