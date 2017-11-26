// Interface for the CD3DEnum class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _D3DENUM_H_
#define _D3DENUM_H_


class CD3DEnum
{
public:
	typedef BOOL(*  LPCONFIRMDEVICECALLBACK)( D3DCAPS9* pCaps, INT, void*);

	// Indicates how data should be stored in a CArrayList
	enum ArrayListType
	{
		AL_VALUE,       // entry data is copied into the list
		AL_REFERENCE,   // entry pointers are copied into the list
	};
	
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
		virtual ~CArrayList();

		HRESULT Add( void* pEntry );
		void Remove( UINT Entry );
		void* GetPtr( UINT Entry );
		UINT Count() { return m_NumEntries; }
		BOOL Contains( void* pEntryData );
		void Clear() { m_NumEntries = 0; }
	};
	
	// Enumeration of all possible D3D vertex processing types.
	enum VertexProcessingType
	{
		SOFTWARE_VP,
		MIXED_VP,
		HARDWARE_VP,
		PURE_HARDWARE_VP
	};
	
	// Info about a display adapter.
	struct D3DAdapterInfo
	{
		int			AdapterOrdinal;
		D3DADAPTER_IDENTIFIER9 AdapterIdentifier;
		CArrayList* pDisplayModeList; // List of D3DDISPLAYMODEs
		CArrayList* pDeviceInfoList; // List of D3DDeviceInfo pointers
		
		virtual ~D3DAdapterInfo();
	};
	
	
	
	// Info about a D3D device, including a list of D3DDeviceCombos (see below) 
	//       that work with the device.
	
	struct D3DDeviceInfo
	{
		int AdapterOrdinal;
		D3DDEVTYPE DevType;
		D3DCAPS9 Caps;
		CArrayList* pDeviceComboList; // List of D3DDeviceCombo pointers
		
		virtual ~D3DDeviceInfo();
	};
	
	
	
	// A depth/stencil buffer format that is incompatible with a
	//       multisample type.
	
	struct D3DDSMSConflict
	{
		D3DFORMAT DSFormat;
		D3DMULTISAMPLE_TYPE MSType;
	};
	
	
	
	// A combination of adapter format, back buffer format, and windowed/fullscreen 
	//       that is compatible with a particular D3D device (and the app).
	
	struct D3DDeviceCombo
	{
		int AdapterOrdinal;
		D3DDEVTYPE DevType;
		D3DFORMAT AdapterFormat;
		D3DFORMAT BackBufferFormat;
		BOOL IsWindowed;
		CArrayList* pDepthStencilFormatList; // List of D3DFORMATs
		CArrayList* pMultiSampleTypeList; // List of D3DMULTISAMPLE_TYPEs
		CArrayList* pMultiSampleQualityList; // List of DWORDs (number of quality 
		// levels for each multisample type)
		CArrayList* pDSMSConflictList; // List of D3DDSMSConflicts
		CArrayList* pVertexProcessingTypeList; // List of VertexProcessingTypes
		CArrayList* pPresentIntervalList; // List of D3DPRESENT_INTERVALs
		
		virtual ~D3DDeviceCombo();
	};
	
private:
	IDirect3D9* m_pD3D;
	
private:
	HRESULT EnumerateDevices( CD3DEnum::D3DAdapterInfo* pAdapterInfo, CArrayList* pAdapterFormatList );
	HRESULT EnumerateDeviceCombos( CD3DEnum::D3DDeviceInfo* pDeviceInfo, CArrayList* pAdapterFormatList );
	void BuildDepthStencilFormatList( CD3DEnum::D3DDeviceCombo* pDeviceCombo );
	void BuildMultiSampleTypeList( CD3DEnum::D3DDeviceCombo* pDeviceCombo );
	void BuildDSMSConflictList( CD3DEnum::D3DDeviceCombo* pDeviceCombo );
	void BuildVertexProcessingTypeList( CD3DEnum::D3DDeviceInfo* pDeviceInfo, CD3DEnum::D3DDeviceCombo* pDeviceCombo );
	void BuildPresentIntervalList( CD3DEnum::D3DDeviceInfo* pDeviceInfo, CD3DEnum::D3DDeviceCombo* pDeviceCombo );
	
public:
	CArrayList* m_pAdapterInfoList;
	// The following variables can be used to limit what modes, formats, 
	// etc. are enumerated.  Set them to the values you want before calling
	// Enumerate().
	LPCONFIRMDEVICECALLBACK ConfirmDeviceCallback;
	UINT AppMinFullscreenWidth;
	UINT AppMinFullscreenHeight;
	UINT AppMinColorChannelBits; // min color bits per channel in adapter format
	UINT AppMinAlphaChannelBits; // min alpha bits per pixel in back buffer format
	UINT AppMinDepthBits;
	UINT AppMinStencilBits;
	BOOL AppUsesDepthBuffer;
	BOOL AppUsesMixedVP; // whether app can take advantage of mixed vp mode
	BOOL AppRequiresWindowed;
	BOOL AppRequiresFullscreen;
	CArrayList* m_pAllowedAdapterFormatList; // list of D3DFORMATs
	
	CD3DEnum();
	virtual ~CD3DEnum();

	void SetD3D(IDirect3D9* pD3D) { m_pD3D = pD3D; }
	HRESULT Enumerate();
};

#endif
