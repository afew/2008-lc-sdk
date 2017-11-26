// Interface for the ILcFont class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _ILcFont_H_
#define _ILcFont_H_


#ifndef interface
#define interface struct
#endif


#ifndef LC_CLASS_DESTROYER
#define LC_CLASS_DESTROYER( CLASS_NAME )	\
virtual ~CLASS_NAME(){}
#endif



struct TLcFnt
{
	INT		iHeight	;															// Height
	INT		iWeight	;															// Weight, Normal, Bold, thin...
	INT		iItalic	;															// Is Italic
	INT		iThckX	;															// Girth of thick X
	INT		iThckY	;															// Girth of thick Y
	DWORD	dString	;															// String color
	DWORD	dGirth	;															// Girth of Font color
	TCHAR	sName	[LF_FACESIZE];

	TLcFnt();

	TLcFnt(		TCHAR*	_sName								// Font Name
			,	INT		_iHeight=18							// Height
			,	INT		_iWeight=FW_NORMAL					// Weight, Normal, Bold, thin...
			,	INT		_iItalic=0							// Is Italic
			,	INT		_iThckX=1							// Girth of thick X
			,	INT		_iThckY=1							// Girth of thick Y
			,	DWORD	_dString=0xFFFFFFFF					// String color
			,	DWORD	_dGirth=0xFF000099					// Girth of Font color
		);
	

	const TLcFnt& operator=(const TLcFnt& r);	// r: right hand side(rhs)
	const TLcFnt& operator=(const TLcFnt* r);
};



interface ILcFont
{
	LC_CLASS_DESTROYER(	ILcFont	);

	typedef enum tagEFntAlgn														// Font Align
	{
		FNT_ALIGN_H_L	= 0x00000000,												// Horizontal Left align
		FNT_ALIGN_H_C	= 0x00000001,												// Horizontal center
		FNT_ALIGN_H_R	= 0x00000002,												// Horizontal Right align

		FNT_ALIGN_V_T	= 0x00000000,												// vertical Top
		FNT_ALIGN_V_C	= 0x00000004,												// vertical center
		FNT_ALIGN_V_B	= 0x00000008,												// vertical bottom
	}EFntAlgn;


	enum ELcFont
	{
		LPFNT_DX=0,
		LPFNT_CE=1,
	};
	
	virtual INT		Create(	  void* p1=0	// LPDIRECT3DDEVICE9
							, void* p2=0	// ID3DXSprite
							, void* p3=0	// TLcFnt
							, void* p4=0	// No Use
							)=0;

	virtual void	Destroy()=0;
	virtual void	DrawTxt()=0;

	virtual INT		Query(char* sCmd, void* pData)=0;

	virtual	void	SetString(TCHAR* sStr=NULL)=0;
	virtual	void	SetPos(FLOAT* pos=NULL/*FLOAT3 or Float2*/)=0;

	virtual	INT		SetColorDiffuse	(FLOAT* pColor=NULL /*rgba <== D3DXCOLOR*/)=0;
	virtual	INT		SetColorFore	(FLOAT* pColor=NULL /*rgba <== D3DXCOLOR*/)=0;
	virtual	INT		SetColorBack	(FLOAT* pColor=NULL /*rgba <== D3DXCOLOR*/)=0;

	virtual void	SetPosAlign(DWORD dwAlign=0)=0;

	virtual	INT		OnLostDevice()=0;
	virtual	INT		OnReset()=0;

	virtual	INT		GetType() = 0;
};


INT LcFont_Create(char* sCmd
				 , ILcFont** pData
				 , void* p1		// LPDIRECT3DDEVICE9
				 , void* p2		// TLcFnt
				 , void* p3=0	// No Use
				 , void* p4=0	// No Use
				 );


#ifdef _DEBUG
	#pragma comment(lib, "LcFont_.lib")
#else
	#pragma comment(lib, "LcFont.lib")
#endif


#endif

