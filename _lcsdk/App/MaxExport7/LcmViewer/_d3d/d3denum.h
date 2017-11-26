
// File: D3DEnum.h
//
// Enumerates D3D adapters, devices, modes, etc.


#ifndef D3DENUM_H
#define D3DENUM_H



// class CD3DEnum
// Enumerates available D3D adapters, devices, modes, etc.

class CD3DEnum
{
public:

	// ArrayListType
	// Indicates how data should be stored in a CArrayList

	enum ArrayListType
	{
		AL_VALUE,       // entry data is copied into the list
		AL_REFERENCE,   // entry pointers are copied into the list
	};

	// CArrayList
	// A growable array

	class CArrayList
	{
	protected:
		ArrayListType m_ArrayListType;
		void* m_pData;
		UINT m_BytesPerEntry;
		UINT m_NumEntries;
		UINT m_NumEntriesAllocated;

	public:
		CArrayList( ArrayListType Type, UINT BytesPerEntry = 0 );
		~CArrayList();
		HRESULT Add( void* pEntry );
		void Remove( UINT Entry );
		void* GetPtr( UINT Entry );
		UINT Count() { return m_NumEntries; }
		bool Contains( void* pEntryData );
		void Clear() { m_NumEntries = 0; }
	};

	
	// enum VertexProcessingType
	// Enumeration of all possible D3D vertex processing types.
	
	enum VertexProcessingType
	{
		SOFTWARE_VP,
			MIXED_VP,
			HARDWARE_VP,
			PURE_HARDWARE_VP
	};
	
	
	
	// struct D3DAdapterInfo
	// Info about a display adapter.
	
	struct D3DAdapterInfo
	{
		int AdapterOrdinal;
		D3DADAPTER_IDENTIFIER9 AdapterIdentifier;
		CArrayList* pDisplayModeList; // List of D3DDISPLAYMODEs
		CArrayList* pDeviceInfoList; // List of D3DDeviceInfo pointers
		~D3DAdapterInfo();
	};
	
	
	
	// struct D3DDeviceInfo
	// Info about a D3D device, including a list of D3DDeviceCombos (see below) 
	//       that work with the device.
	
	struct D3DDeviceInfo
	{
		int AdapterOrdinal;
		D3DDEVTYPE DevType;
		D3DCAPS9 Caps;
		CArrayList* pDeviceComboList; // List of D3DDeviceCombo pointers
		~D3DDeviceInfo();
	};
	
	
	
	// struct D3DDSMSConflict
	// A depth/stencil buffer format that is incompatible with a multisample type.
	
	struct D3DDSMSConflict
	{
		D3DFORMAT DSFormat;
		D3DMULTISAMPLE_TYPE MSType;
	};
	
	
	
	// struct D3DDeviceCombo
	// A combination of adapter format, back buffer format, and windowed/fullscreen 
	//       that is compatible with a particular D3D device (and the app).
	
	struct D3DDeviceCombo
	{
		int AdapterOrdinal;
		D3DDEVTYPE DevType;
		D3DFORMAT AdapterFormat;
		D3DFORMAT BackBufferFormat;
		bool IsWindowed;
		CArrayList* pDepthStencilFormatList; // List of D3DFORMATs
		CArrayList* pMultiSampleTypeList; // List of D3DMULTISAMPLE_TYPEs
		CArrayList* pMultiSampleQualityList; // List of DWORDs (number of quality 
		// levels for each multisample type)
		CArrayList* pDSMSConflictList; // List of D3DDSMSConflicts
		CArrayList* pVertexProcessingTypeList; // List of VertexProcessingTypes
		CArrayList* pPresentIntervalList; // List of D3DPRESENT_INTERVALs
		
		~D3DDeviceCombo();
	};
	

	typedef bool(* CONFIRMDEVICECALLBACK)( D3DCAPS9* pCaps, DWORD vertexProcessingType, DWORD backBufferFormat );

	
	
private:
	IDirect3D9* m_pD3D;
	
private:
	HRESULT EnumerateDevices( D3DAdapterInfo* pAdapterInfo, CArrayList* pAdapterFormatList );
	HRESULT EnumerateDeviceCombos( D3DDeviceInfo* pDeviceInfo, CArrayList* pAdapterFormatList );
	void BuildDepthStencilFormatList( D3DDeviceCombo* pDeviceCombo );
	void BuildMultiSampleTypeList( D3DDeviceCombo* pDeviceCombo );
	void BuildDSMSConflictList( D3DDeviceCombo* pDeviceCombo );
	void BuildVertexProcessingTypeList( D3DDeviceInfo* pDeviceInfo, D3DDeviceCombo* pDeviceCombo );
	void BuildPresentIntervalList( D3DDeviceInfo* pDeviceInfo, D3DDeviceCombo* pDeviceCombo );
	
public:
	CArrayList* m_pAdapterInfoList;
	// The following variables can be used to limit what modes, formats, 
	// etc. are enumerated.  Set them to the values you want before calling
	// Enumerate().
	CONFIRMDEVICECALLBACK ConfirmDeviceCallback;
	UINT AppMinFullscreenWidth;
	UINT AppMinFullscreenHeight;
	UINT AppMinColorChannelBits; // min color bits per channel in adapter format
	UINT AppMinAlphaChannelBits; // min alpha bits per pixel in back buffer format
	UINT AppMinDepthBits;
	UINT AppMinStencilBits;
	bool AppUsesDepthBuffer;
	bool AppUsesMixedVP; // whether app can take advantage of mixed vp mode
	bool AppRequiresWindowed;
	bool AppRequiresFullscreen;
	CArrayList* m_pAllowedAdapterFormatList; // list of D3DFORMATs
	
	CD3DEnum();
	~CD3DEnum();
	void SetD3D(IDirect3D9* pD3D) { m_pD3D = pD3D; }
	HRESULT Enumerate();
};

#endif
