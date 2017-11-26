// Implementation of the CLcTex class.
//
//////////////////////////////////////////////////////////////////////

#if _MSC_VER >= 1200
#pragma warning(disable : 4996)
#endif

#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#include <Lc/LcType.h>

#include "ILcTex.h"
#include "LcTexBase.h"


CLcTex::TLcTex::TLcTex()
{
	m_pTx	= 0;
	m_dC	= 0x00FFFFFF;
	m_dF	= D3DMX_FILTER_NONE;
	m_dP	= D3DPOOL_MANAGED;

	memset(m_sF, 0, sizeof m_sF);
	memset(&m_Inf, 0, sizeof m_Inf);
}


void CLcTex::TLcTex::Release()
{
	if(m_pTx)
	{
		m_pTx->Release();
		m_pTx = NULL;
	}
}


CLcTex::CLcTex()
{
	m_nId	= -1;
}

CLcTex::CLcTex(INT _nId, PDTX _pTx, DWORD _dC, DWORD _dF, DWORD _dPool, const DIMG* _Inf,  const TCHAR* _sFile)
{
	m_nId		= _nId;
	m_LcTx.m_pTx= _pTx;
	m_LcTx.m_dC	= _dC;
	m_LcTx.m_dF	= _dF;
	m_LcTx.m_dP	= _dPool;

	memcpy(&m_LcTx.m_Inf, _Inf, sizeof m_LcTx.m_Inf);

	memset(m_LcTx.m_sF, 0, sizeof m_LcTx.m_sF);
	_tcscpy(m_LcTx.m_sF, _sFile);
}




CLcTex::~CLcTex()
{
	CLcTex::Destroy();
}


INT CLcTex::Create(void* p1, void* p2, void* p3, void* p4)
{
//	printf("CLcTex Create\n");
	return 0;
}

void CLcTex::Destroy()
{
	m_LcTx.Release();	
}

INT CLcTex::Query(char* sCmd, void* pData)
{
//	printf("CLcTex Query:%s\n", sCmd);
	return 0;
}



DWORD	CLcTex::GetWidth()
{
	return m_LcTx.m_Inf.Width;
}

DWORD	CLcTex::GetHeight()
{
	return m_LcTx.m_Inf.Height;
}

DWORD	CLcTex::GetDepth()
{
	return m_LcTx.m_Inf.Depth;
}

DWORD	CLcTex::GetMipLevel()
{
	return m_LcTx.m_Inf.MipLevels;
}

DWORD	CLcTex::GetFormat()
{
	return m_LcTx.m_Inf.Format;
}

DWORD	CLcTex::GetResourceType()
{
	return 0;
}

DWORD	CLcTex::GetFileFormat()
{
	return 0;
}

TCHAR*	CLcTex::GetSourceName()
{
	return m_LcTx.m_sF;
}

void*	CLcTex::GetPointer()
{
	return m_LcTx.m_pTx;
}

DWORD	CLcTex::GetColorKey()
{
	return m_LcTx.m_dC;
}


DWORD	CLcTex::GetFilter()
{
	return m_LcTx.m_dF;
}


void CLcTex::GetRECT(RECT* rc)
{
	rc->left=0;
	rc->top=0;
	rc->right=m_LcTx.m_Inf.Width;
	rc->bottom=m_LcTx.m_Inf.Height;
}




void CLcTex::SetId(INT nId)
{
	m_nId = nId;
}

void CLcTex::SetTx(void* pTx)
{
	m_LcTx.m_pTx = (PDTX)pTx;
}

void CLcTex::SetColorKey(DWORD dColorKey)
{
	m_LcTx.m_dC = dColorKey;
}

void CLcTex::SetFilter(DWORD dFilter)
{
	m_LcTx.m_dF = dFilter;
}

void CLcTex::SetInfo(DIMG* pInf)
{
	memcpy(&m_LcTx.m_Inf, pInf, sizeof( m_LcTx.m_Inf));
}

void CLcTex::SetFileName(TCHAR* sFile)
{
	if(sFile)
	{
		_tcscpy(m_LcTx.m_sF, sFile);
	}
}


//void GetModulePath(HINSTANCE hInst, TCHAR* stFile, int MaxPath)
//{
//	GetModuleFileName(hInst, stFile, MaxPath);
//	TCHAR* p = _tcsrchr(stFile, L'\\');
//	if (p)
//		*p = NULL;
//}

// LcTex_CreateTexture함수 사용 예제
// LcTex_CreateTexture("File"			, &pData, pd3dDevice, "Test.jpg");
// LcTex_CreateTexture("ResourceBmp"	, &pData, pd3dDevice, &ResourceId);
// LcTex_CreateTexture("ResourceCustom"	, &pData, pd3dDevice, &ResourceId, "UserCustomDefineType");
// LcTex_CreateTexture("ResourceCustom"	, &pData, pd3dDevice, &ResourceId, "PNG");
// LcTex_CreateTexture("Memory"			, &pData, pd3dDevice, pSrcData, &dSourceSize);

INT LcTex_CreateTexture(char* sCmd, ILcTex** pData, void* pD3DMevice, void* pVal1, void* pVal2, DWORD dColorKey, DWORD dFilter, DWORD Pool)
{
	(*pData) = NULL;

	CLcTex* pObj = NULL;

	PDEV	pDev = (PDEV)pD3DMevice;
	TCHAR*	sFile=(TCHAR*)pVal1;
	PDTX	pTexture;
	DWORD	dColor = dColorKey;
	DWORD	Filter=dFilter;
	DWORD	MipFilter= dFilter;
	DIMG	pSrcInf;


	if(0==_stricmp("File", sCmd))
	{
		if(FAILED(LcTex_TextureLoadFile(pDev, sFile, pTexture, dColor, &pSrcInf, Filter, MipFilter)))
			return -1;
	}

	pObj = new CLcTex(-1, pTexture, dColor,  Filter, Pool, &pSrcInf, sFile);

	(*pData) = pObj;

	return 0;
}




INT		LcTex_TextureLoadFromFile(void** pData		// Out put data(LPDIRECT3DTEXTURE9)
							, void*  pSrcInf	// D3DXIMAGE_INFO
							, void*  pd3dDev	// LPDIRECT3DDEVICE9
							, TCHAR* sFile		// File Name
							, DWORD dColor
							, DWORD Filter
							, DWORD MipFilter
							, DWORD d3dFormat
							, DWORD Pool		)
{
	INT			hr=-1;
	PDEV		pDev  = (PDEV)pd3dDev;
	PDTX		pTex  = NULL;
	DIMG		pInf;
	D3DFORMAT	d3Fmt = (D3DFORMAT)d3dFormat;

	*pData = NULL;
	
	hr = LcTex_TextureLoadFile(pDev, sFile, pTex, dColor, &pInf, Filter, MipFilter, d3Fmt, Pool);

	if(FAILED(hr))
		return -1;

	*pData = pTex;

	if(pSrcInf)
		memcpy(pSrcInf, &pInf, sizeof(DIMG));

	return 0;
}





// Texture Load
INT LcTex_TextureLoadFile(PDEV pDev, TCHAR* sFile
						, PDTX& pTexture
						, DWORD dColor		//= 0x00FFFFFF
						, DIMG* pSrcInf		//= NULL
						, DWORD Filter		//= (D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR)
						, DWORD MipFilter	//= (D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR)
						, D3DFORMAT d3Fmt	//= D3DFMT_UNKNOWN
						, DWORD Pool		//= D3DPOOL_MANAGED
						)
{
	HRESULT hr;

	UINT uMip = D3DX_DEFAULT;

	if(D3DX_FILTER_POINT >= Filter || D3DX_FILTER_POINT >= MipFilter)
		uMip = 1;


	D3DPOOL d3dPool;
//	D3DCAPS9 caps;
//	if( FAILED( pDev->GetDeviceCaps(&caps) ) )
//		return -1;
//
//	d3dPool = (caps.SurfaceCaps & D3DSURFCAPS_VIDTEXTURE) ? D3DPOOL_VIDEOMEM : D3DPOOL_MANAGED;
	
	d3dPool = D3DPOOL_MANAGED;


	hr=D3DXCreateTextureFromFileEx(pDev
			, sFile
			, D3DX_DEFAULT, D3DX_DEFAULT
			, uMip
			, 0
			, d3Fmt
			, d3dPool	//, (D3DPOOL)Pool
			, Filter, MipFilter
			, dColor, pSrcInf
			, NULL
			, &pTexture		);

	if(FAILED(hr))
	{
		pTexture = 0;
		return hr;
	}

	return 0;
}


INT LcTex_TextureLoadRscBmp(PDEV pDev, INT nResourceId
							, PDTX& pTexture
							, DWORD dColor
							, DIMG* pSrcInf
							, DWORD Filter
							, DWORD MipFilter
							, D3DFORMAT d3Fmt
							, DWORD Pool)
{
	HRESULT		hr;
	HINSTANCE	hInst = NULL;

	UINT uMip = D3DX_DEFAULT;

	if(D3DX_FILTER_NONE == Filter || D3DX_FILTER_NONE == MipFilter)
		uMip = 1;

	hInst = GetModuleHandle(NULL);

	// DirectX Help에는 이렇게 나와 있다.
	// The resource being loaded must be of type RT_BITMAP or RT_RCDATA.
	// Resource type RT_RCDATA is used to load formats other than bitmaps (such as .tga, .jpg, and .dds).
	// 하지만 LcTex_TextureLoadRscCustom처럼 하는 것이 더 낫다.
	// 왜냐 하면 D3DXCreateTextureFromResourceEx함수는 리소스 타입을 전달할 아무런 인자가 없다.
	// 이 함수는 비트맵만 사용할 수 있다.

	hr=D3DXCreateTextureFromResourceEx(pDev, hInst
			, MAKEINTRESOURCE(nResourceId)
			, D3DX_DEFAULT
			, D3DX_DEFAULT
			, uMip
			, 0
			, d3Fmt
			, (D3DPOOL)Pool
			, Filter, MipFilter
			, dColor, pSrcInf
			, NULL
			, &pTexture		);

	if(FAILED(hr))
	{
		pTexture = 0;
		return hr;
	}

	return 0;
}



INT LcTex_TextureFill(PDEV pDev, PDTX pTx, DWORD dColor)
{
	INT hr = -1;
	PDSF pSurface = NULL;
	
	hr = pTx->GetSurfaceLevel( 0, &pSurface );

	if( SUCCEEDED(hr) )
		pDev->ColorFill( pSurface, NULL, 0x0);
	
	pSurface->Release();

	return hr;
}