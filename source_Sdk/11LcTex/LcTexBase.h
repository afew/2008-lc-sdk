// Interface for the CLcTex class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcTex_H_
#define _LcTex_H_


class CLcTex : public ILcTex
{
public:
	struct TLcTex
	{
		PDTX		m_pTx;					// D3DTexture Pointer
		DWORD		m_dC;					// Creation color Key
		DWORD		m_dF;					// Creation Filter

		DWORD		m_dP;					// Pool Types

		DIMG		m_Inf;
		TCHAR		m_sF[MAX_PATH];			// File Name

		TLcTex();
		void Release();
	};


protected:
	INT			m_nId;
	TLcTex		m_LcTx;

public:
	CLcTex();
	CLcTex(	INT _nId
			, PDTX _pTx
			, DWORD _dC
			, DWORD _dF
			, DWORD _dPool
			, const DIMG* _Inf
			, const TCHAR* _sFile);

	virtual ~CLcTex();
	
	virtual INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0);
	virtual void	Destroy();
	virtual INT		Query(char* sCmd, void* pData);


	virtual DWORD	GetWidth();					// Get Width
	virtual DWORD	GetHeight();				// Get Height
	virtual DWORD	GetDepth();					// Get Depth
	virtual DWORD	GetMipLevel();				// Get Creation MipLevel
	virtual DWORD	GetFormat();				// Get enumeration from D3DFORMAT
	virtual DWORD	GetResourceType();			// Get enumeration from D3DRESOURCETYPE
	virtual DWORD	GetFileFormat();			// Get enumeration from D3DXIMAGE_FILEFORMAT

	virtual TCHAR*	GetSourceName();			// Get Source Name or File Name
	virtual void*	GetPointer();				// Get D3DTexture Pointer

	virtual DWORD	GetColorKey();				// Get Creation color Key
	virtual DWORD	GetFilter();				// Get Creation Filter
	virtual void	GetRECT(RECT* rc);			// Get RECT


	void	SetId(INT nId);
	void	SetTx(void* pTx);
	void	SetColorKey(DWORD dColorKey);
	void	SetFilter(DWORD dFilter);
	void	SetInfo(DIMG* pInf);
	void	SetFileName(TCHAR* sFile);
};


INT		LcTex_TextureLoadFile(PDEV pDev, TCHAR* sFile
							, PDTX& pTexture
							, DWORD dColor		=0x00FFFFFF
							, DIMG* pSrcInf		=NULL
							, DWORD Filter		= (D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR)
							, DWORD MipFilter	= (D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR)
							, D3DFORMAT d3Fmt	= D3DFMT_UNKNOWN
							, DWORD Pool		= D3DPOOL_MANAGED);


INT		LcTex_TextureLoadRscBmp(	//Resource의 비트맵만 가능
							PDEV pDev, INT nResourceId
							, PDTX& pTexture
							, DWORD dColor=0x00FFFFFF
							, DIMG* pSrcInf=NULL
							, DWORD Filter		= (D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR)
							, DWORD MipFilter	= (D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR)
							, D3DFORMAT d3Fmt	= D3DFMT_UNKNOWN
							, DWORD Pool		= D3DPOOL_MANAGED);


INT		LcTex_TextureFill(PDEV pDev, PDTX pTx, DWORD dColor= 0x0);


#endif

