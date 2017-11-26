// Interface for the ILcTex class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _ILcTex_H_
#define _ILcTex_H_

#if _MSC_VER > 1000
#pragma once
#endif


#ifndef D3DMX_FILTER_NONE
#define D3DMX_FILTER_NONE	(1<<0)
#endif

#ifndef interface
#define interface struct
#endif


#ifndef LC_CLASS_DESTROYER
#define LC_CLASS_DESTROYER( CLASS_NAME )	\
virtual ~CLASS_NAME(){}
#endif


interface ILcTex
{
	enum	{	MAX_TEXTURE_LAYER	=	8,	};

	struct TxLayer
	{
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

		TxLayer();
	};


	LC_CLASS_DESTROYER(	ILcTex	);
	
	virtual INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0)=0;
	virtual void	Destroy()=0;
	virtual INT		Query(char* sCmd, void* pData)=0;

	virtual DWORD	GetWidth()=0;				// Get Width
	virtual DWORD	GetHeight()=0;				// Get Height
	virtual DWORD	GetDepth()=0;				// Get Depth
	virtual DWORD	GetMipLevel()=0;			// Get Creation MipLevel
	virtual DWORD	GetFormat()=0;				// Get enumeration from D3DFORMAT
	virtual DWORD	GetResourceType()=0;		// Get enumeration from D3DRESOURCETYPE
	virtual DWORD	GetFileFormat()=0;			// Get enumeration from D3DMXIMAGE_FILEFORMAT

	virtual TCHAR*	GetSourceName()=0;			// Get Source Name or File Name
	virtual void*	GetPointer()=0;				// Get D3DTexture Pointer
	
	virtual DWORD	GetColorKey()=0;			// Get Creation color Key
	virtual DWORD	GetFilter()=0;				// Get Creation Filter

	virtual void	GetRECT(RECT* rc)=0;		// Get RECT
};


typedef ILcTex*		PLNTEX;

// LcTex_CreateD3DTexture함수 사용 예제
// LcTex_CreateTexture("File"			, &pData, pd3dDevice, "Test.jpg");
// LcTex_CreateTexture("ResourceBmp"	, &pData, pd3dDevice, &ResourceId);
// LcTex_CreateTexture("ResourceCustom"	, &pData, pd3dDevice, &ResourceId, "UserCustomDefineType");
// LcTex_CreateTexture("ResourceCustom"	, &pData, pd3dDevice, &ResourceId, "PNG");
// LcTex_CreateTexture("Memory"			, &pData, pd3dDevice, pSrcData, &dSourceSize);


INT		LcTex_CreateTexture(char* sCmd
						   , ILcTex** pData
						   , void* pD3Device
						   , void* pVal1
						   , void* pVal2 = NULL
						   , DWORD dColorKey=0x00FFFFFF
						   , DWORD dFilter= D3DX_FILTER_POINT
						   , DWORD Pool= D3DPOOL_MANAGED);






INT		LcTex_TextureLoadFromFile(void** pData		// Out put data(LPDIRECT3DTEXTURE9)
							, void*  pSrcInf	// D3DXIMAGE_INFO
							, void*	 pd3dDev	// LPDIRECT3DDEVICE9
							, TCHAR* sFile		// File Name
							, DWORD dColor		=0x00FFFFFF
							, DWORD Filter		= (D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR)
							, DWORD MipFilter	= (D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR)
							, DWORD d3dFormat	= D3DFMT_UNKNOWN
							, DWORD Pool		= D3DPOOL_MANAGED);


interface ILcTexMng
{
	LC_CLASS_DESTROYER(	ILcTexMng	);
	
	virtual INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0)=0;
	virtual void	Destroy()=0;

	virtual INT		Query(char* sCmd, void* pData)=0;


	// Management Interface
	virtual INT		Find(char* sName, ILcTex** pData)=0;
	virtual INT		FindCreate(char* sCmd
								, ILcTex** pData
								, void* pVal1
								, void* pVal2 = NULL
								, DWORD dColorKey=0x00FFFFFF
								, DWORD dFilter= D3DX_FILTER_POINT
								, DWORD Pool= D3DPOOL_MANAGED)=0;
};


INT LcTex_CreateTexMng(char* sCmd
					, ILcTexMng** pData	// Texture Manager Instance
					, void* p1			// Device
					, void* p2=0		// No Use
					, void* p3=0		// No Use
					, void* p4=0		// No Use
					);




#ifdef _DEBUG
	#pragma comment(lib, "LcTex_.lib")
#else
	#pragma comment(lib, "LcTex.lib")
#endif


#endif

