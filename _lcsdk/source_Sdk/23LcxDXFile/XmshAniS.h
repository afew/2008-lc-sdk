// Interface for the CXmshAniS class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _XmshAniS_H_
#define _XmshAniS_H_


#pragma warning(disable : 4786)
#pragma warning(disable : 4788)
#include <vector>


class CXmshAniS : public CLcxDXFile
{
public:
	static	D3DCAPS9			m_d3dCaps;
	static	LPDIRECT3DDEVICE9	m_pDev;					// LPDIRECT3DDEVICE9

protected:
	LPD3DXFRAME                 m_pFrameRoot;
	LPD3DXANIMATIONCONTROLLER   m_pAnimController;

	INT							m_SkinningMethod;   // Current skinning method
	D3DXMATRIX*					m_pBoneMatrices;
	UINT						m_NumBoneMatricesMax;

	LPDIRECT3DVERTEXSHADER9		m_pIndexedVertexShader[4];
	LPD3DXEFFECT				m_pEffect;


	D3DXVECTOR3					m_vObjectCenter;    // Center of bounding sphere of object
	FLOAT						m_fObjectRadius;    // Radius of bounding sphere of object


	
	FLOAT						m_fElapse;			// Elapsed Time


public:
	struct _Tex
	{
		TCHAR		sTxFile[MAX_PATH];
		TexLayer*	pLtx;

		_Tex()	{	pLtx = NULL;	}
	};

protected:
	std::vector<_Tex* >		m_vTex;


public:
	CXmshAniS();
	virtual ~CXmshAniS();

	virtual INT		Create(void* =0, void* =0, void* =0, void* =0);
	virtual	void	Destroy();
	virtual INT		FrameMove();
	virtual	void	Render();
	virtual INT		Query(char* sCmd, void* pData);
	virtual void*	CreateNewInstance();

public:
	LPD3DXEFFECT	GetEffect()		{	return m_pEffect;			}
	D3DXVECTOR3		GetObjCenter()	{	return m_vObjectCenter;		}
	FLOAT			GetObjRadius()	{	return m_fObjectRadius;		}
	FLOAT			GetElpase()		{	return m_fElapse;			}

	void			NewSkinMethod(INT nMethod);
	void			SetVtxShader(INT nIdx, LPDIRECT3DVERTEXSHADER9 pshd)	{	m_pIndexedVertexShader[nIdx] = pshd;	}

	void			PushTexture(TCHAR*	sTxFile, TexLayer* pTx);
	INT				GetTextureNum();
	const TCHAR*	GetTextureName(INT nIdx);
	void			SetTexturePointer(INT nIdx, TexLayer* pTx);

	void*			GetAniController()	{	return	m_pAnimController;	}
	void*			GetFrameRoot()		{	return	m_pFrameRoot;		}


public:
	void DrawMeshContainer( LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase );
	void DrawFrame( LPD3DXFRAME pFrame );

	HRESULT SetupBoneMatrixPointersOnMesh( LPD3DXMESHCONTAINER pMeshContainer );
	HRESULT SetupBoneMatrixPointers( LPD3DXFRAME pFrame );
	void UpdateFrameMatrices( LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix );
	void UpdateSkinningMethod( LPD3DXFRAME pFrameBase );
	HRESULT GenerateSkinnedMesh( D3DXMESHCONTAINER_DERIVED *pMeshContainer );
};


#endif


