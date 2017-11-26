// Implementation of the LcDxUtil functions and classes.
//
////////////////////////////////////////////////////////////////////////////////

#if _MSC_VER >= 1200
#pragma warning(disable : 4996)
#endif

#pragma warning(disable : 4786)

#include <map>
#include <string>


#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <dxerr9.h>

#include <Lc/LcType.h>

#include "LcUtilDx.h"

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)	{	if(p){(p)->Release();p = NULL;	}}
#endif


void LcD3D_SetWorldIdentity(PDEV pDev, DWORD val)
{
	static MATA	mtW(	1,0,0,0
					,	0,1,0,0
					,	0,0,1,0
					,	0,0,0,1);

	pDev->SetTransform((D3DTRANSFORMSTATETYPE)(val), &mtW);
}



INT LcD3D_DrawTextBackbuffer(PDEV pDev, INT X, INT Y, LPCTSTR Text, DWORD _color)
{
	HDC hDC=0;
	LPDIRECT3DSURFACE9 Surface;

	if(FAILED(pDev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &Surface)))
		return -1;

	Surface->GetDC(&hDC);

	if(NULL != hDC)
	{
		SetBkMode(hDC, TRANSPARENT);
		SetTextColor(hDC, _color);
		TextOut(hDC, X, Y, Text, strlen(Text));

		Surface->ReleaseDC(hDC);
	}

	Surface->Release(); //�����Ѵ�.


	return 0;
}


// Texture Load
INT LcD3D_TextureLoadFile(PDEV pDev, char* sFile, PDTX& pTexture, DWORD dColor
						   , D3DXIMAGE_INFO* pSrcInf
						   , DWORD Filter, DWORD MipFilter, D3DFORMAT d3Fmt)
{
	HRESULT hr;

	UINT uMip = D3DX_DEFAULT;

	if(D3DX_FILTER_NONE == Filter || D3DX_FILTER_NONE == MipFilter)
		uMip = 1;

	hr=D3DXCreateTextureFromFileEx(pDev
			, sFile
			, D3DX_DEFAULT, D3DX_DEFAULT
			, uMip
			, 0
			, d3Fmt
			, D3DPOOL_MANAGED
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


INT LcD3D_TextureLoadRscBmp(PDEV pDev, INT nResourceId, PDTX& pTexture, DWORD dColor
						   , D3DXIMAGE_INFO* pSrcInf
						   , DWORD Filter, DWORD MipFilter, D3DFORMAT d3Fmt)
{
	HRESULT		hr;
	HINSTANCE	hInst = NULL;

	UINT uMip = D3DX_DEFAULT;

	if(D3DX_FILTER_NONE == Filter || D3DX_FILTER_NONE == MipFilter)
		uMip = 1;

	hInst = GetModuleHandle(NULL);

	// DirectX Help���� �̷��� ���� �ִ�.
	// The resource being loaded must be of type RT_BITMAP or RT_RCDATA.
	// Resource type RT_RCDATA is used to load formats other than bitmaps (such as .tga, .jpg, and .dds).
	// ������ LcD3D_TextureLoadRscCustomó�� �ϴ� ���� �� ����.
	// �ֳ� �ϸ� D3DXCreateTextureFromResourceEx�Լ��� ���ҽ� Ÿ���� ������ �ƹ��� ���ڰ� ����.
	// �� �Լ��� ��Ʈ�ʸ� ����� �� �ִ�.

	hr=D3DXCreateTextureFromResourceEx(pDev, hInst
			, MAKEINTRESOURCE(nResourceId)
			, D3DX_DEFAULT
			, D3DX_DEFAULT
			, uMip
			, 0
			, d3Fmt
			, D3DPOOL_MANAGED
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



INT LcD3D_TextureLoadRscCustom(PDEV pDev, INT nResourceId, const char* sType, PDTX& pTexture, DWORD dColor
								 , D3DXIMAGE_INFO* pSrcInf
								 , DWORD Filter, DWORD MipFilter, D3DFORMAT d3Fmt)
{
	HINSTANCE	hInst	= NULL;
	HRSRC		hRsc	= NULL;
	DWORD		dwSize	= 0;
	HGLOBAL		hMem	= NULL;
	LPVOID		pMem	= NULL;

	HRESULT hr;

	UINT uMip = D3DX_DEFAULT;

	if(D3DX_FILTER_NONE == Filter || D3DX_FILTER_NONE == MipFilter)
		uMip = 1;


	hInst = GetModuleHandle(NULL);
	hRsc = FindResource( hInst, MAKEINTRESOURCE(nResourceId), sType);

	if(NULL == hRsc)
		return -1;

	dwSize = SizeofResource(hInst,hRsc);

	if(0==dwSize)
		return -1;

	hMem = LoadResource(hInst, hRsc);

	if(NULL == hMem)
		return -1;

	pMem = LockResource(hMem);

	// �̰��� �ȵȴ�...
	//	HRESULT hr = D3DXCreateTextureFromResourceEx(
	//		pDev
	//		, NULL
	//		, MAKEINTRESOURCE(IDR_PNG1)
	//		, D3DX_DEFAULT
	//		, D3DX_DEFAULT
	//		, uMip
	//		, 0
	//		, d3Fmt
	//		, D3DPOOL_MANAGED
	//		, Filter, MipFilter
	//		, dColor, pSrcInf
	//		, NULL
	//		, &pTexture);



	// �̰����� �ؾ� �Ѵ�.

	hr = D3DXCreateTextureFromFileInMemoryEx(
		pDev
		, pMem
		, dwSize
		, D3DX_DEFAULT
		, D3DX_DEFAULT
		, uMip
		, 0
		, d3Fmt
		, D3DPOOL_MANAGED
		, Filter, MipFilter
		, dColor, pSrcInf
		, NULL
		, &pTexture);

	UnlockResource(hMem);
	FreeResource(hMem);

	if(FAILED(hr))
		return -1;

	return 0;
}


INT LcD3D_TextureLoadMemory(PDEV pDev, void* pData, UINT dSize, PDTX& pTexture, DWORD dColor
						   , D3DXIMAGE_INFO* pSrcInf
						   , DWORD Filter, DWORD MipFilter, D3DFORMAT d3Fmt)
{
	HRESULT hr;

	UINT uMip = D3DX_DEFAULT;

	if(D3DX_FILTER_NONE == Filter || D3DX_FILTER_NONE == MipFilter)
		uMip = 1;

	hr=D3DXCreateTextureFromFileInMemoryEx(pDev
			, pData
			, dSize
			, D3DX_DEFAULT
			, D3DX_DEFAULT
			, uMip
			, 0
			, d3Fmt
			, D3DPOOL_MANAGED
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





INT LcD3D_TextureFill(PDEV pDev, PDTX pTx, DWORD dColor)
{
	INT hr = -1;
	PDSF pSurface = NULL;
	
	hr = pTx->GetSurfaceLevel( 0, &pSurface );

	if( SUCCEEDED(hr) )
		pDev->ColorFill( pSurface, NULL, 0x0);
	
	pSurface->Release();

	return hr;
}












INT LcD3D_FontCreate(PDEV pDev, LPD3DXFONT& pDxFnt
						  , char*	sFntName
						  , HFONT*	pHfont
						  , INT		lfHeight
						  , INT		lfWeight
						  , DWORD	lfItalic
						  , DWORD	lfCharSet)
{
	HRESULT hr;

	if(pHfont)
	{
		*pHfont = CreateFont(lfHeight
							, 0, 0, 0
							, lfWeight, lfItalic
							, 0, 0
							, lfCharSet
							, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,	ANTIALIASED_QUALITY, FF_DONTCARE
							, sFntName);

#if 9 == D3DX_SDK_VERSION
			hr = D3DXCreateFont( pDev, *pHfont, &pDxFnt);

#elif 21 == D3DX_SDK_VERSION
			hr = D3DXCreateFont(pDev
					, lfHeight
			        , 0
			        , lfWeight
			        , 1
			        , lfItalic
			        , lfCharSet
			        , OUT_DEFAULT_PRECIS
			        , ANTIALIASED_QUALITY
			        , FF_DONTCARE
			        , sFntName
			        , &pDxFnt);

#endif

	}

	else
	{
		LOGFONT hLogFont =
		{
			lfHeight
			, 0, 0, 0
			, lfWeight, lfItalic
			, 0, 0
			, lfCharSet
			, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,	ANTIALIASED_QUALITY, FF_DONTCARE
			, "Arial"
		};

		if(sFntName && strlen(sFntName))
			strcpy(hLogFont.lfFaceName, sFntName);


#if 9 == D3DX_SDK_VERSION
			hr = D3DXCreateFont( pDev, *pHfont, &pDxFnt);

#elif 21 == D3DX_SDK_VERSION
		//	struct D3DXFONT_DESC
		//	{
		//		UINT Height;
		//		UINT Width;
		//		UINT Weight;
		//		UINT MipLevels;
		//		BOOL Italic;
		//		BYTE CharSet;
		//		BYTE OutputPrecision;
		//		BYTE Quality;
		//		BYTE PitchAndFamily;
		//		char FaceName[LF_FACESIZE];
		//	};

			D3DXFONT_DESC	hFont;
			memset(&hFont, 0, sizeof hFont);

			hFont.Height		= lfHeight;
			hFont.Weight		= lfWeight;
			hFont.MipLevels		= 1;
			hFont.Italic		= lfItalic;
			hFont.CharSet		= HANGEUL_CHARSET;
			hFont.OutputPrecision= OUT_DEFAULT_PRECIS;
			hFont.Quality		= ANTIALIASED_QUALITY;
			hFont.PitchAndFamily= FF_DONTCARE;
			strcpy(hFont.FaceName, sFntName);

		hr = D3DXCreateFontIndirect(pDev, &hFont, &pDxFnt );
#endif
	}


	if(FAILED(hr))
	{
		pDxFnt = 0;
		return hr;
	}

	return 0;
}



HRESULT LcD3D_FontBegin(ID3DXFont* pDxFnt)
{
	HRESULT hr =0;
#if 9 == D3DX_SDK_VERSION
	hr = pDxFnt->Begin();

#elif 21 == D3DX_SDK_VERSION

#endif

	return hr;
}


HRESULT LcD3D_FontDraw(ID3DXFont* pDxFnt, char* pString, LPRECT pRect, DWORD Format, D3DCOLOR Color, INT Count)
{
	HRESULT hr =0;
#if 9 == D3DX_SDK_VERSION
	hr = pDxFnt->DrawText(pString, Count, pRect, Format, Color);

#elif 21 == D3DX_SDK_VERSION
	hr = pDxFnt->DrawText(NULL, pString, Count, pRect, Format, Color);
#endif

	return hr;
}


HRESULT LcD3D_FontEnd(ID3DXFont* pDxFnt)
{
HRESULT hr =0;

#if 9 == D3DX_SDK_VERSION
	hr = pDxFnt->End();

#elif 21 == D3DX_SDK_VERSION

#endif

	return hr;
}







HWND LcD3D_FocusHwndFromDevice(PDEV pDev)
{
	HWND hWnd = NULL;
	D3DDEVICE_CREATION_PARAMETERS d3dParam;

	if(SUCCEEDED(pDev->GetCreationParameters(&d3dParam)))
	{
		hWnd = d3dParam.hFocusWindow;
	}

	return hWnd;
}


PDEV LcD3D_DeviceFromDxSprite(PDSP pSp)
{
	PDEV pDev = NULL;
	PDEV pDevT = NULL;

	if(FAILED(pSp->GetDevice(&pDev)))
	{
		return NULL;
	}

	// �ּ� ����
	pDevT = pDev;
	pDev->Release();

	return pDevT;
}

INT LcD3D_DeviceFormat(PDEV pDev, DWORD* FMTColor, DWORD* FMTDepthStencil, UINT* Width, UINT* Height)
{
	PDSF pSf1		= NULL;
	PDSF pSf2		= NULL;

	D3DSURFACE_DESC	dsc1;
	D3DSURFACE_DESC	dsc2;

	if(FAILED(pDev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pSf1)))
		return -1;

	if(FAILED(pSf1->GetDesc(&dsc1)))
		return -1;

	pSf1->Release();


	if(FAILED(pDev->GetDepthStencilSurface(&pSf2)))
		return -1;

	if(FAILED(pSf2->GetDesc(&dsc2)))
		return -1;

	pSf2->Release();

	if(FMTColor)
		*FMTColor = dsc1.Format;

	if(FMTDepthStencil)
		*FMTDepthStencil = dsc2.Format;

	if(Width)
		*Width = dsc1.Width;

	if(Height)
		*Height = dsc1.Height;

	return 0;
}






void LcD3D_VBCreate(PDEV pDev,PDVB& pVB, INT nSize, DWORD fvf, void* pVtx, D3DPOOL usage)
{
	HRESULT hr;

	hr = pDev->CreateVertexBuffer(nSize,
		0,
		fvf,
		usage,
		&pVB, NULL );

	if( FAILED(hr) )
	{
		::MessageBox(GetActiveWindow(), "Vertex create failed", "Err", MB_ICONERROR);
		return;
	}

	if(!pVtx)
		return;

	LcD3D_VBLock(pVB, nSize, pVtx);
}


void LcD3D_VBLock(PDVB& pVB, INT nSize, void* pVtx)
{
	void* p;

	if( FAILED( pVB->Lock( 0, 0, &p, 0 )))
		return;

	memcpy( p, pVtx, nSize);
	pVB->Unlock();
}


void LcD3D_IBCreate(PDEV pDev, PDIB& pIB, INT nSize, void* pIdx, D3DFORMAT fmt, D3DPOOL usage)
{
	HRESULT hr;

	hr = pDev->CreateIndexBuffer( nSize, 0, fmt, usage, &pIB, NULL);

	if( FAILED(hr) )
	{
		::MessageBox(GetActiveWindow(), "Index buffer create failed", "Err", MB_ICONERROR);
		return;
	}

	if(!pIdx)
		return;

	LcD3D_IBLock(pIB, nSize, pIdx);
}


void LcD3D_IBLock(PDIB & pIB, INT nSize, void* pIdx)
{
	void* p;

	if( FAILED( pIB->Lock( 0, 0, &p, 0 )))
		return;

	memcpy(p, pIdx, nSize);
	pIB->Unlock();
}
















void LcD3D_ScreenCapture(PDSF pSf)
{
	SYSTEMTIME st;
	char	sFile[512]="\0";
	char	sFile2[512]="\0";

	GetLocalTime(&st);
	GetCurrentDirectory(sizeof(sFile2), sFile2);

	sprintf(sFile, "%s/%04d_%02d_%02d_%02d_%02d_%02d.bmp", sFile2,
		(int)st.wYear, (int)st.wMonth, (int)st.wDay,
		(int)st.wHour, (int)st.wMinute, (int)st.wSecond);

	D3DXSaveSurfaceToFile( sFile, D3DXIFF_BMP, pSf, NULL, NULL);
}


void LcD3D_ScreenCapture(PDEV pDev)
{
	SYSTEMTIME st;
	char	sFile[512]="\0";
	char	sFile2[512]="\0";

	D3DDISPLAYMODE	d3ddm;
	PDSF			pSf= NULL;

	GetLocalTime(&st);
	GetCurrentDirectory(sizeof(sFile2), sFile2);

	sprintf(sFile, "%s/%04d_%02d_%02d_%02d_%02d_%02d.bmp", sFile2,
		(int)st.wYear, (int)st.wMonth, (int)st.wDay,
		(int)st.wHour, (int)st.wMinute, (int)st.wSecond);


	pDev->GetDisplayMode( 0, &d3ddm );
	pDev->CreateOffscreenPlainSurface( d3ddm.Width, d3ddm.Height, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &pSf, NULL );
	pDev->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pSf );

	D3DXSaveSurfaceToFile( sFile, D3DXIFF_BMP, pSf, NULL, NULL);

	SAFE_RELEASE( pSf );
}









DWORD	LcD3D_GetVertexShaderVersion(PDEV pDev)
{
	D3DCAPS9 caps;
	DWORD	Version;
	pDev->GetDeviceCaps(&caps);

	Version = caps.VertexShaderVersion;
	return Version;
}

DWORD	LcD3D_GetVertexShaderVersionMajor(PDEV pDev)
{
	D3DCAPS9 caps;
	DWORD	Version;
	pDev->GetDeviceCaps(&caps);

	Version = caps.VertexShaderVersion;
	return D3DSHADER_VERSION_MAJOR(Version);
}


DWORD	LcD3D_GetVertexShaderVersionMinor(PDEV pDev)
{
	D3DCAPS9 caps;
	DWORD	Version;
	pDev->GetDeviceCaps(&caps);

	Version = caps.VertexShaderVersion;
	return D3DSHADER_VERSION_MINOR(Version);
}


DWORD	LcD3D_GetPixelShaderVersion(PDEV pDev)
{
	D3DCAPS9 caps;
	DWORD	Version;
	pDev->GetDeviceCaps(&caps);

	Version = caps.PixelShaderVersion;
	return Version;
}


DWORD	LcD3D_GetPixelShaderVersionMajor(PDEV pDev)
{
	D3DCAPS9 caps;
	DWORD	Version;
	pDev->GetDeviceCaps(&caps);

	Version = caps.PixelShaderVersion;
	return D3DSHADER_VERSION_MAJOR(Version);
}


DWORD	LcD3D_GetPixelShaderVersionMinor(PDEV pDev)
{
	D3DCAPS9 caps;
	DWORD	Version;
	pDev->GetDeviceCaps(&caps);

	Version = caps.PixelShaderVersion;
	return D3DSHADER_VERSION_MINOR(Version);
}







PDVD LcD3D_GetVertexDeclarator(PDEV pDev, DWORD fvf)
{
	PDVD pFVF = NULL;

	D3DVERTEXELEMENT9 pDcl[MAX_FVF_DECL_SIZE]={0};
	D3DXDeclaratorFromFVF(fvf, pDcl);

	if(FAILED(pDev->CreateVertexDeclaration( pDcl, &pFVF )))
		return NULL;

	return pFVF;
}


void* LcD3D_BuildShader(PDEV pDev, char* sStrAssem, int iLen, char* sShader)
{
	HRESULT	hr;

	DWORD dFlag = 0;
	#if defined( _DEBUG ) || defined( DEBUG )
	dFlag |= D3DXSHADER_DEBUG;
	#endif

	LPD3DXBUFFER	pAsm = NULL;
	LPD3DXBUFFER	pErr = NULL;
	void*			pShd = NULL;

	hr = D3DXAssembleShader(
			sStrAssem
		,	iLen
		,	NULL
		,	NULL
		,	dFlag
		,	&pAsm
		,	&pErr);

	if( FAILED(hr) )
	{
		if(pErr)
		{
			char* sErr = (char*)pErr->GetBufferPointer();
			LcD3D_GetDxError(hr, sErr);
			pErr->Release();
		}
		else
			LcD3D_GetDxError(hr);

		return NULL;
	}

	if( 0 == _strnicmp(sShader, "vs", 2))
	{
		PDVS pVS = NULL;
		hr = pDev->CreateVertexShader( (DWORD*)pAsm->GetBufferPointer() , &pVS);
		pShd = pVS;
	}

	else if( 0 == _strnicmp(sShader, "ps", 2))
	{
		PDPS pPS	= NULL;
		hr = pDev->CreatePixelShader( (DWORD*)pAsm->GetBufferPointer() , &pPS);
		pShd = pPS;
	}

	pAsm->Release();

	if( FAILED(hr) )
	{
		LcD3D_GetDxError(hr);
		return NULL;
	}

	return pShd;
}


void* LcD3D_BuildShaderFromFile(PDEV pDev, char* sStrFile, char* sShader)
{
	HRESULT	hr;

	DWORD dFlag = 0;
	#if defined( _DEBUG ) || defined( DEBUG )
	dFlag |= D3DXSHADER_DEBUG;
	#endif

	LPD3DXBUFFER	pAsm = NULL;
	LPD3DXBUFFER	pErr = NULL;
	void*			pShd = NULL;


	hr = D3DXAssembleShaderFromFile(
			sStrFile
		,	NULL
		,	NULL
		,	dFlag
		,	&pAsm
		,	&pErr);

	if( FAILED(hr) )
	{
		if(pErr)
		{
			char* sErr = (char*)pErr->GetBufferPointer();
			LcD3D_GetDxError(hr, sErr);
			pErr->Release();
		}
		else
			LcD3D_GetDxError(hr);

		return NULL;
	}

	if( 0 == _strnicmp(sShader, "vs", 2))
	{
		PDVS pVS = NULL;
		hr = pDev->CreateVertexShader( (DWORD*)pAsm->GetBufferPointer() , &pVS);
		pShd = pVS;
	}
	else if( 0 == _strnicmp(sShader, "ps", 2))
	{
		PDPS pPS	= NULL;
		hr = pDev->CreatePixelShader( (DWORD*)pAsm->GetBufferPointer() , &pPS);
		pShd = pPS;
	}

	pAsm->Release();
	
	if( FAILED(hr) )
	{
		LcD3D_GetDxError(hr);
		return NULL;
	}

	return pShd;
}


void* LcD3D_BuildHLSL(PDEV pDev, char* sHLSL, int iLen, char* sFunction, char* sShader, PDCT* pTbl/*Out*/)
{
	HRESULT	hr;

	DWORD dFlag = 0;
	#if defined( _DEBUG ) || defined( DEBUG )
	dFlag |= D3DXSHADER_DEBUG;
	#endif

	LPD3DXBUFFER	pAsm = NULL;
	LPD3DXBUFFER	pErr = NULL;
	void*			pShd = NULL;

	hr = D3DXCompileShader(
			sHLSL
		,	iLen
		,	NULL
		,	NULL
		,	sFunction
		,	sShader
		,	dFlag
		,	&pAsm
		,	&pErr
		,	pTbl);


	if( FAILED(hr) )
	{
		if(pErr)
		{
			char* sErr = (char*)pErr->GetBufferPointer();
			LcD3D_GetDxError(hr, sErr);
			pErr->Release();
		}
		else
			LcD3D_GetDxError(hr);

		return NULL;
	}

	if( 0 == _strnicmp(sShader, "vs", 2))
	{
		PDVS pVS = NULL;
		hr = pDev->CreateVertexShader( (DWORD*)pAsm->GetBufferPointer() , &pVS);
		pShd = pVS;
	}
	else if( 0 == _strnicmp(sShader, "ps", 2))
	{
		PDPS pPS	= NULL;
		hr = pDev->CreatePixelShader( (DWORD*)pAsm->GetBufferPointer() , &pPS);
		pShd = pPS;
	}

	pAsm->Release();
	
	if( FAILED(hr) )
	{
		LcD3D_GetDxError(hr);
		return NULL;
	}

	return pShd;
}


void* LcD3D_BuildHLSLFromFile(PDEV pDev, char* sStrFile, char* sFunction, char* sShader, PDCT* pTbl/*Out*/)
{
	HRESULT	hr;

	DWORD dFlag = 0;
	#if defined( _DEBUG ) || defined( DEBUG )
	dFlag |= D3DXSHADER_DEBUG;
	#endif

	LPD3DXBUFFER	pAsm = NULL;
	LPD3DXBUFFER	pErr = NULL;
	void*			pShd = NULL;

	hr = D3DXCompileShaderFromFile(
			sStrFile
		,	NULL
		,	NULL
		,	sFunction
		,	sShader
		,	dFlag
		,	&pAsm
		,	&pErr
		,	pTbl);


	if( FAILED(hr) )
	{
		if(pErr)
		{
			char* sErr = (char*)pErr->GetBufferPointer();
			LcD3D_GetDxError(hr, sErr);
			pErr->Release();
		}
		else
			LcD3D_GetDxError(hr);

		return NULL;
	}

	if( 0 == _strnicmp(sShader, "vs", 2))
	{
		PDVS pVS = NULL;
		hr = pDev->CreateVertexShader( (DWORD*)pAsm->GetBufferPointer() , &pVS);
		pShd = pVS;
	}

	else if( 0 == _strnicmp(sShader, "ps", 2))
	{
		PDPS pPS	= NULL;
		hr = pDev->CreatePixelShader( (DWORD*)pAsm->GetBufferPointer() , &pPS);
		pShd = pPS;
	}

	pAsm->Release();

	if( FAILED(hr) )
	{
		LcD3D_GetDxError(hr);
		return NULL;
	}

	return pShd;
}



void LcD3D_SetVertexShaderConstant(PDEV pDev, UINT uReg , MATA* v)
{
	MATA	trans;
	D3DXMatrixTranspose( &trans , v);
	pDev->SetVertexShaderConstantF( uReg , (FLOAT*)&trans , 4);
}


void LcD3D_SetVertexShaderConstant(PDEV pDev, UINT uReg , DCLR* v)
{
	pDev->SetVertexShaderConstantF(uReg , (FLOAT*)v, 1);
}

void LcD3D_SetVertexShaderConstant(PDEV pDev, UINT uReg , VEC4* v)
{
	pDev->SetVertexShaderConstantF(uReg , (FLOAT*)v, 1);
}

void LcD3D_SetVertexShaderConstant(PDEV pDev, UINT uReg , FLOAT* v)
{
	pDev->SetVertexShaderConstantF(uReg , v, 1);
}


void LcD3D_SetPixelShaderConstant(PDEV pDev, UINT uReg , DCLR* v)
{
	pDev->SetPixelShaderConstantF(uReg , (FLOAT*)v, 1);
}


void LcD3D_SetPixelShaderConstant(PDEV pDev, UINT uReg , FLOAT* v)
{
	pDev->SetPixelShaderConstantF(uReg , v, 1);
}

void LcD3D_SetPixelShaderConstant(PDEV pDev, UINT uReg , INT* v)
{
	pDev->SetPixelShaderConstantI(uReg , v, 1);
}




void LcD3D_SetHlslTableConstant(PDCT& pTbl, PDEV pDev, char* sValue, MATA* v)
{
	if(pTbl)
		pTbl->SetMatrix(pDev, (D3DXHANDLE)sValue, v);
}

void LcD3D_SetHlslVector(PDCT& pTbl, PDEV pDev, char* sValue, VEC4* v)
{
	if(pTbl)
		pTbl->SetVector(pDev, sValue, v);
}





class TmngD3DXEffect
{
public:
	typedef std::map<std::string, PDEF	>	mpLst;
	typedef mpLst::iterator					itLst;

protected:
	PDEV	m_pDev;
	mpLst	mpEft;


public:
	TmngD3DXEffect(PDEV pDev)
	{
		m_pDev = pDev;
	}


	virtual ~TmngD3DXEffect()
	{
		Destroy();
	}



	void* Find(char* sStrFile)
	{
		char	sFile[160]={0};
		strcpy(sFile, sStrFile);
		strlwr(sFile);

		itLst	itEft = mpEft.find(sFile);

		if(itEft != mpEft.end())
			return (*itEft).second;


		HRESULT	hr;

		DWORD dFlag = 0;
		#if defined( _DEBUG ) || defined( DEBUG )
		dFlag |= D3DXSHADER_DEBUG;
		#endif

		LPD3DXEFFECT	pEft = NULL;
		LPD3DXBUFFER	pErr = NULL;

		hr = D3DXCreateEffectFromFile(	m_pDev
									,	sFile
									,	NULL
									,	NULL
									,	dFlag
									,	NULL
									,	&pEft
									,	&pErr);

		if(FAILED(hr))
		{
			pEft	= NULL;

			if(pErr)
			{
				char* sErr = (char*)pErr->GetBufferPointer();
				LcD3D_GetDxError(hr, sErr);
				pErr->Release();
			}
			else
				LcD3D_GetDxError(hr);
		}

		else
			mpEft.insert(	mpLst::value_type(sFile, pEft) );

		return pEft;
	}

	INT LostDevice()
	{
		itLst	_F = mpEft.begin();
		itLst	_L = mpEft.end();

		for(; _F != _L; ++_F)
		{
			LPD3DXEFFECT pEft = _F->second;

			if(FAILED(pEft->OnLostDevice()))
				return -1;
		}

		return 0;
	}

	INT ResetDevice()
	{
		itLst	_F = mpEft.begin();
		itLst	_L = mpEft.end();

		for(; _F != _L; ++_F)
		{
			LPD3DXEFFECT pEft = _F->second;

			if(FAILED(pEft->OnResetDevice()))
				return -1;
		}

		return 0;
	}

	void Destroy()
	{
		itLst	_F = mpEft.begin();
		itLst	_L = mpEft.end();

		for(; _F != _L; ++_F)
		{
			LPD3DXEFFECT pEft = _F->second;
			pEft->Release();
		}

		mpEft.clear();
	}
};



static TmngD3DXEffect*	g_pLcD3DXEffect_Manager	= NULL;




void* LcD3D_EffectStringBuild(PDEV pDev, char* sStr, int iLen)
{
	HRESULT	hr;

	DWORD dFlag = 0;
	#if defined( _DEBUG ) || defined( DEBUG )
	dFlag |= D3DXSHADER_DEBUG;
	#endif

	PDEF			pEft = NULL;
	LPD3DXBUFFER	pErr = NULL;

	hr = D3DXCreateEffect(	pDev
		,	sStr
		,	iLen
		,	NULL
		,	NULL
		,	dFlag
		,	NULL
		,	&pEft
		,	&pErr);

	if(FAILED(hr))
	{
		pEft	= NULL;

		if(pErr)
		{
			char* sErr = (char*)pErr->GetBufferPointer();
			LcD3D_GetDxError(hr, sErr);
			pErr->Release();
		}
		else
			LcD3D_GetDxError(hr);
	}

	return pEft;
}



void* LcD3D_EffectFileBuild(PDEV pDev, char* sStrFile)
{
	if(NULL == g_pLcD3DXEffect_Manager)
		g_pLcD3DXEffect_Manager = new TmngD3DXEffect(pDev);

	return g_pLcD3DXEffect_Manager->Find(sStrFile);
}

void LcD3D_EffectFileDestroy()
{
	if(NULL == g_pLcD3DXEffect_Manager)
		return;

	delete g_pLcD3DXEffect_Manager;
	g_pLcD3DXEffect_Manager = NULL;
}



void LcD3D_EffectFileLostDevice()
{
	if(NULL == g_pLcD3DXEffect_Manager)
		return;

	g_pLcD3DXEffect_Manager->LostDevice();
}


void LcD3D_EffectFileResetDevice()
{
	if(NULL == g_pLcD3DXEffect_Manager)
		return;

	g_pLcD3DXEffect_Manager->ResetDevice();
}



void LcD3D_GetDxError(HRESULT hr, char* pBufferPointer)
{
	char* s = (char*) malloc(2048);

	if(pBufferPointer)
	{
		sprintf(s
			,	"File:%s\n"
				"Error Line:%d\n"
				"Error Msg: %s\n"
				"Error Desc:%s\n"
				"Error Pointer: %s\n"
				,	__FILE__
				,	__LINE__
				,	DXGetErrorString9(hr)
				,	DXGetErrorDescription9(hr)
				,	pBufferPointer );
	}

	else
	{
		sprintf(s
			,	"File:%s\n"
				"Error Line:%d\n"
				"Error Msg: %s\n"
				"Error Desc:%s\n"
				,	__FILE__
				,	__LINE__
				,	DXGetErrorString9(hr)
				,	DXGetErrorDescription9(hr));

	}

	OutputDebugString( s );
	MessageBox(0, s, "Err", MB_OK | MB_ICONERROR);
	free(s);
}





void LcD3D_InitMaterial( D3DMATERIAL9& mtrl, FLOAT r, FLOAT g, FLOAT b, FLOAT a)
{
    ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
    mtrl.Diffuse.r = mtrl.Ambient.r = r;
    mtrl.Diffuse.g = mtrl.Ambient.g = g;
    mtrl.Diffuse.b = mtrl.Ambient.b = b;
    mtrl.Diffuse.a = mtrl.Ambient.a = a;
}



void LcD3D_InitLight( D3DLIGHT9& light, D3DLIGHTTYPE ltType, FLOAT x, FLOAT y, FLOAT z )
{
    D3DXVECTOR3 vecLightDirUnnormalized(x, y, z);
    ZeroMemory( &light, sizeof(D3DLIGHT9) );
    light.Type        = ltType;
    light.Diffuse.r   = 1.0f;
    light.Diffuse.g   = 1.0f;
    light.Diffuse.b   = 1.0f;
    D3DXVec3Normalize( (D3DXVECTOR3*)&light.Direction, &vecLightDirUnnormalized );
    light.Position.x   = x;
    light.Position.y   = y;
    light.Position.z   = z;
    light.Range        = 1000.0f;
}




////////////////////////////////////////////////////////////////////////////////
//
// Implementation of the TswpWn class.
//
////////////////////////////////////////////////////////////////////////////////

TswpWn::TswpWn()
: pC (0)
, pB (0)
, pS (0)
, hW (0)

, pD (0)
, pOT(0)
, pOS(0)
, dCM(D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL)
, dCc(0x00005588)
{
}

TswpWn::~TswpWn()
{
	Release();
}

INT TswpWn::Create(PDEV pDev, HWND hWnd, DWORD dClearMode)
{
	pD = pDev;
	hW = hWnd;
	dCM= dClearMode;

	return 0;
}

INT TswpWn::OnResetDevice()
{
	if(pC)
		return 0;

	D3DPRESENT_PARAMETERS	param;
	memset(&param, 0, sizeof(D3DPRESENT_PARAMETERS));

	param.SwapEffect = D3DSWAPEFFECT_DISCARD;
	param.Windowed = TRUE;
	param.BackBufferHeight= 0;
	param.BackBufferWidth = 0;
	param.hDeviceWindow = hW;

	// Create addtional swap chain
	if( FAILED(pD->CreateAdditionalSwapChain(&param, &pC)) )
		return -1;

	// Get back buffer Failed
	if( FAILED(pC->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pB)) )
		return -1;


	// ����̽��� ���� ���ٽ��� ������ �˾ƿ´�.
	D3DSURFACE_DESC desc;
	PDSF psf = NULL;

	if(FAILED(pD->GetDepthStencilSurface(&psf)))
		return -1;

	psf->GetDesc(&desc);

	SAFE_RELEASE(psf);

	// Device�� ���� DepthStencil Surface�� �����.
	if ( FAILED(pD->CreateDepthStencilSurface(
			param.BackBufferWidth
		,	param.BackBufferHeight
		,	desc.Format
		,	D3DMULTISAMPLE_NONE, 0, 0, &pS, NULL)))

		return -1;


	pD->GetRenderTarget(0, &pOT);
	pD->GetDepthStencilSurface(&pOS);

	return 0;
}


INT TswpWn::OnLostDevice()
{
	SAFE_RELEASE(pC);
	SAFE_RELEASE(pB);
	SAFE_RELEASE(pS);

	SAFE_RELEASE(pOT);
	SAFE_RELEASE(pOS);

	return 0;
}

void TswpWn::Release()
{
	SAFE_RELEASE(pC);
	SAFE_RELEASE(pB);
	SAFE_RELEASE(pS);

	SAFE_RELEASE(pOT);
	SAFE_RELEASE(pOS);
}

void TswpWn::SetClearMode(DWORD d)
{
	dCM = d;
}

void TswpWn::SetClearColor(DWORD d)
{
	dCc = d;
}

INT TswpWn::Clear()
{
	if(FAILED(pD->Clear( 0L, NULL, dCM, dCc, 1.0f, 0L)))
		return -1;

	return 0;
}

INT TswpWn::BeginScene()
{
	if(FAILED(pD->SetRenderTarget(0, pB)))		return -1;
	if(FAILED(pD->SetDepthStencilSurface(pS)))	return -1;
	if(FAILED(pD->BeginScene()))				return -1;

	return 0;
}

INT TswpWn::EndScene()
{
	if(FAILED(pD->EndScene()))					return -1;
	if(FAILED(pC->Present(0, 0, 0, 0, 0)))		return -1;
	if(FAILED(pD->SetRenderTarget(0, pOT)))		return -1;
	if(FAILED(pD->SetDepthStencilSurface(pOS)))	return -1;

	return 0;
}


PDSF TswpWn::GetBackBuffer() const
{
	return pB;
}


PDSF TswpWn::GetDepthStencil() const
{
	return pS;
}




////////////////////////////////////////////////////////////////////////////////
//
// Implementation of the TShd class.
//
////////////////////////////////////////////////////////////////////////////////

TShd::TShd() : pShd(0), pDcl(0), pPxl(0)
{
	memset(sVtx, 0, sizeof(sVtx));
	memset(sPxl, 0, sizeof(sPxl));
}

TShd::~TShd()
{
	SAFE_RELEASE( pShd );
	SAFE_RELEASE( pDcl );
	SAFE_RELEASE( pPxl );
}

void TShd::SetFileVtx(char*	sFile)
{
	sprintf(sVtx, sFile);
}

void TShd::SetFilePxl(char*	sFile)
{
	sprintf(sPxl, sFile);
}

INT TShd::SetDeclarator(PDEV pDev, DWORD dFVF)
{
	HRESULT hr;
	D3DVERTEXELEMENT9	decl[MAX_FVF_DECL_SIZE]={0};

	if( FAILED( hr = D3DXDeclaratorFromFVF(dFVF, decl) ) )
		return hr;

	if( FAILED( hr = pDev->CreateVertexDeclaration( decl, &pDcl ) ) )
		return hr;

	return 0;
}

INT TShd::CreateVertexShader(PDEV pDev)
{
	HRESULT			hr= -1;

	DWORD	dFlag = 0;
	#if defined( _DEBUG ) || defined( DEBUG )
	dFlag |= D3DXSHADER_DEBUG;
	#endif

	LPD3DXBUFFER	pAsm = NULL;
	LPD3DXBUFFER	pErr = NULL;

	if(0==strlen(sVtx))
		return -1;

	hr = D3DXAssembleShaderFromFile(sVtx, 0, 0, dFlag, &pAsm, &pErr);

	if( FAILED(hr) )
	{
		if(pErr)
		{
			char* sErr = (char*)pErr->GetBufferPointer();
			LcD3D_GetDxError(hr, sErr);
			pErr->Release();
		}
		else
			LcD3D_GetDxError(hr);

		return hr;
	}

	hr = pDev->CreateVertexShader( (DWORD*)pAsm->GetBufferPointer(), &pShd );

	if( FAILED(hr) )
	{
		LcD3D_GetDxError(hr);
		return hr;
	}

	pAsm->Release();

	return hr;
}

INT TShd::CreatePixelShader(PDEV pDev)
{
	HRESULT			hr= -1;
	
	DWORD	dFlag = 0;
	#if defined( _DEBUG ) || defined( DEBUG )
	dFlag |= D3DXSHADER_DEBUG;
	#endif

	LPD3DXBUFFER	pAsm = NULL;
	LPD3DXBUFFER	pErr = NULL;


	if(0==strlen(sPxl))
		return -1;

	hr = D3DXAssembleShaderFromFile(sPxl, 0, 0, dFlag, &pAsm, &pErr );

	if(FAILED(hr))
	{
		if(pErr)
		{
			char* sErr = (char*)pErr->GetBufferPointer();
			LcD3D_GetDxError(hr, sErr);
			pErr->Release();
		}
		else
			LcD3D_GetDxError(hr);

		return hr;
	}

	hr = pDev->CreatePixelShader( (DWORD*)pAsm->GetBufferPointer(), &pPxl);
	pAsm->Release();
	
	if( FAILED(hr) )
	{
		LcD3D_GetDxError(hr);
		return hr;
	}

	return hr;
}




////////////////////////////////////////////////////////////////////////////////
//
// Implementation of the TwinRc class.
//
////////////////////////////////////////////////////////////////////////////////

TwinRc::TwinRc()
{
	memset(&rc, 0, sizeof(RECT));
	memset(&vp, 0, sizeof(DVWP));
}

TwinRc::TwinRc(const RECT& Rc, const DVWP& Vp)
{
	rc = Rc;
	vp = Vp;
}

TwinRc::TwinRc(LONG X1,LONG Y1,LONG X2,LONG Y2)
{
	rc.left		=X1;
	rc.top		=Y1;
	rc.right	=X2;
	rc.bottom	=Y2;

	vp.X		= X1;
	vp.Y		= Y1;
	vp.Width	= X2;
	vp.Height	= Y2;
	vp.MinZ		= 0;
	vp.MaxZ		= 1;
}

TwinRc::TwinRc(LONG X1,LONG Y1,LONG X2,LONG Y2
	, DWORD X,DWORD Y,DWORD W,DWORD H
	, FLOAT Min,FLOAT Max)
{
	rc.left		=X1;
	rc.top		=Y1;
	rc.right	=X2;
	rc.bottom	=Y2;

	vp.X = X;
	vp.Y = Y;
	vp.Width = W;
	vp.Height = H;
	vp.MinZ = Min;
	vp.MaxZ = Max;
}

RECT TwinRc::GetRect()
{
	return rc;
}





////////////////////////////////////////////////////////////////////////////////
//
// Implementation of the Render Target class.
//
////////////////////////////////////////////////////////////////////////////////

TrndSf::TrndSf()
{
	m_iW	= 0;
	m_iH	= 0;
	m_dD	= 0;

	m_pDev	= NULL;
	m_pRS	= NULL;
	m_pTx	= NULL;
	m_pSf	= NULL;
}


TrndSf::~TrndSf()
{
	Destroy();
}


INT TrndSf::Create(PDEV pDev, INT iW, INT iH)
{
	HRESULT hr=-1;

	m_pDev	= pDev;
	m_iW	= iW;
	m_iH	= iH;

	PDSF	pSf1 = NULL;
	PDSF	pSf2 = NULL;
	D3DSURFACE_DESC SfDsC;
	D3DSURFACE_DESC SfDsD;


	m_pDev->GetRenderTarget(0,&pSf1);
	m_pDev->GetDepthStencilSurface(&pSf2);

    pSf1->GetDesc(&SfDsC);
	pSf2->GetDesc(&SfDsD);

	pSf1->Release();
	pSf2->Release();

    if(FAILED(D3DXCreateRenderToSurface(m_pDev
										, m_iW, m_iH, SfDsC.Format, TRUE
										, SfDsD.Format, &m_pRS)))
        return -1;


	hr = D3DXCreateTexture(m_pDev, m_iW, m_iH, 1
						, D3DUSAGE_RENDERTARGET
						, D3DFMT_X8R8G8B8
						, D3DPOOL_DEFAULT
						, &m_pTx);

	if(FAILED(hr))
		return -1;


	hr = m_pTx->GetSurfaceLevel(0, &m_pSf);

	m_dD = (D3DFORMAT)SfDsD.Format;

	return 0;
}


void TrndSf::Destroy()
{
	SAFE_RELEASE(	m_pSf	);
	SAFE_RELEASE(	m_pTx	);
	SAFE_RELEASE(	m_pRS	);

	m_pDev	= NULL;
	m_iW	= 0;
	m_iH	= 0;
}


INT TrndSf::OnResetDevice()
{
	HRESULT hr=-1;
	PDSF	pSf1 = NULL;
	PDSF	pSf2 = NULL;
	D3DSURFACE_DESC SfDsC;
	D3DSURFACE_DESC SfDsD;

	if(NULL == m_pRS)
	{
		m_pDev->GetRenderTarget(0,&pSf1);
		m_pDev->GetDepthStencilSurface(&pSf2);

		pSf1->GetDesc(&SfDsC);
		pSf2->GetDesc(&SfDsD);

		pSf1->Release();
		pSf2->Release();

		if(FAILED(D3DXCreateRenderToSurface(m_pDev
											, m_iW, m_iH, SfDsC.Format, TRUE
											, SfDsD.Format, &m_pRS)))
        return -1;


		hr = D3DXCreateTexture(m_pDev, m_iW, m_iH, 1
						, D3DUSAGE_RENDERTARGET
						, D3DFMT_X8R8G8B8
						, D3DPOOL_DEFAULT
						, &m_pTx);

		if(FAILED(hr))
			return -1;


		hr = m_pTx->GetSurfaceLevel(0, &m_pSf);
	}

	m_dD = (D3DFORMAT)SfDsD.Format;

	return 0;
}

INT TrndSf::OnLostDevice()
{
	SAFE_RELEASE(	m_pSf	);
	SAFE_RELEASE(	m_pTx	);
	SAFE_RELEASE(	m_pRS	);

	return 0;
}


INT TrndSf::BeginScene(DWORD dClearMode, DWORD dClearColor)
{
	HRESULT hr = m_pRS->BeginScene(m_pSf, NULL);
	hr = m_pDev->Clear( 0L, NULL, dClearMode, dClearColor, 1.0f, 0L );

	return hr;
}


INT TrndSf::EndScene()
{
	HRESULT hr = m_pRS->EndScene( 0 );
	return hr;
}


INT TrndSf::GetWidth()
{
	return m_iW;
}

INT TrndSf::GetHeight()
{
	return m_iH;
}


DWORD TrndSf::GetDepth()
{
	return m_dD;
}


PDTX TrndSf::GetTexture() const
{
	return m_pTx;
}


PDSF TrndSf::GetSurface() const
{
	return m_pSf;
}






////////////////////////////////////////////////////////////////////////////////
//
// Implementation of TcolorMap class.
//
////////////////////////////////////////////////////////////////////////////////

TcolorMap::TcolorMap()
{
	m_iW			=1024	;	// Width
	m_iH			= 768	;	// Height

	m_pDev			= NULL	;
	m_pTxT			= NULL	;

	m_pTx			= NULL	;	// Rendering�� �ؽ���
	m_pSf			= NULL	;	// Rendering�� �ؽ��� surface

	m_pBckC			= NULL	;	// Back Buffer Surface
	m_pBckD			= NULL	;	// Back Buffer Depth and Stencil
}


TcolorMap::~TcolorMap()
{
	Destroy();
}


INT TcolorMap::Create(PDEV pDev, INT iW, INT iH)
{
	m_pDev	= pDev;
	m_iW	= iW;	// Width
	m_iH	= iH;	// Height

	D3DXCreateTexture(m_pDev, m_iW, m_iH, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pTx);

	m_pTx->GetSurfaceLevel(0,&m_pSf);
	m_pDev->GetRenderTarget(0,&m_pBckC);
	m_pDev->GetDepthStencilSurface(&m_pBckD);

	return 0;
}


void TcolorMap::Destroy()
{
	SAFE_RELEASE(	m_pTxT	);
	SAFE_RELEASE(	m_pTx	);
	SAFE_RELEASE(	m_pSf	);
	SAFE_RELEASE(	m_pBckC	);
	SAFE_RELEASE(	m_pBckD	);
}


INT TcolorMap::OnResetDevice()
{
	HRESULT hr;

	if(NULL == m_pTx)
	{
		hr = D3DXCreateTexture(m_pDev, m_iW, m_iH, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pTx);

		if(FAILED(hr))
			return -1;

		m_pTx->GetSurfaceLevel(0,&m_pSf);

		m_pDev->GetRenderTarget(0,&m_pBckC);
		m_pDev->GetDepthStencilSurface(&m_pBckD);

		if(FAILED(hr))
			return -1;
	}

	if(m_pTxT)
	{
		PDSF	pSrc;
		PDSF	pDst =m_pSf;
		hr = m_pTxT->GetSurfaceLevel(0,&pSrc);

		hr = D3DXLoadSurfaceFromSurface(pDst, NULL, NULL, m_pSf, NULL, NULL, D3DX_FILTER_NONE, 0x0);

		if(FAILED(hr))
			return -1;

		SAFE_RELEASE(	pSrc	);
		SAFE_RELEASE(	m_pTxT	);
	}

	return 0;
}

INT TcolorMap::OnLostDevice()
{
	HRESULT hr;
	PDSF	pSrc =m_pSf;
	PDSF	pDst;

	hr = D3DXCreateTexture(m_pDev, m_iW, m_iH, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pTxT);

	if(FAILED(hr))
		return -1;

	hr = m_pTxT->GetSurfaceLevel(0,&pDst);

	if(FAILED(hr))
		return -1;

	hr = D3DXLoadSurfaceFromSurface(pDst, NULL, NULL, m_pSf, NULL, NULL, D3DX_FILTER_NONE, 0x0);

	if(FAILED(hr))
		return -1;

	SAFE_RELEASE(	pDst	);


	SAFE_RELEASE(m_pSf);
	SAFE_RELEASE(m_pTx);

	SAFE_RELEASE(m_pBckC);
	SAFE_RELEASE(m_pBckD);

	return 0;
}


HRESULT TcolorMap::Begin(DWORD dClearMode)
{
	HRESULT hr = m_pDev->SetRenderTarget(0,m_pSf);
	hr = m_pDev->Clear( 0L, NULL, dClearMode, 0x00000000, 1.0f, 0L );

	return hr;
}


HRESULT TcolorMap::End()
{

	m_pDev->SetRenderTarget(0,m_pBckC);
	return m_pDev->SetDepthStencilSurface(m_pBckD);
}


PDTX TcolorMap::GetTexture() const
{
	return m_pTx;
}




FLOAT LcD3D_AngleFromXAxis(const VEC3* pLook, const VEC3* pEye)
{
	FLOAT	fAngle =0.f;
	VEC3	vcT = *pLook - *pEye;

	vcT.y = 0;

	D3DXVec3Normalize(&vcT, &vcT);

	if ( -0.9999f>vcT.x)
		fAngle = 3.1415926535897932f;

	else if (0.9999f<vcT.x)
		fAngle = 0.f;

	else
		fAngle = acos(vcT.x);

	if(vcT.z<0.f)
			fAngle = -fAngle;

	return fAngle;
}


