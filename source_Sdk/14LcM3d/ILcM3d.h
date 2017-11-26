// Interface for the 3D classes.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _ILcM3d_H_
#define _ILcM3d_H_


#ifndef interface
#define interface struct
#endif


#ifndef LC_CLASS_DESTROYER
#define LC_CLASS_DESTROYER( CLASS_NAME )	\
virtual ~CLASS_NAME(){}
#endif



// Index Buffer Structure
namespace LcIdx
{
	struct Idx16																// 16bit Index structure
	{
		union	{	struct	{	WORD a;	WORD b;	WORD c;	};	WORD m[3];	};

		Idx16()						: a(0),b(1),c(2){}
		Idx16(WORD A,WORD B,WORD C)	: a(A),b(B),c(C){}
		Idx16(WORD* R)					: a(R[0]),b(R[1]),c(R[2]){}

		operator WORD* ()				{	return (WORD *) &a;			}
		operator const WORD* () const	{	return (const WORD *) &a;	}
	};

	struct Idx32																// 32bit Index structure
	{
		union	{	struct	{	DWORD a; DWORD b; DWORD c; }; DWORD m[3]; };

		Idx32()						: a(0),b(1),c(2){}
		Idx32(DWORD A,DWORD B,DWORD C)	: a(A),b(B),c(C){}
		Idx32(DWORD* R)					: a(R[0]),b(R[1]),c(R[2]){}

		operator DWORD* ()				{	return (DWORD *) &a;			}
		operator const DWORD* () const	{	return (const DWORD *) &a;	}
	};
};



// Vertex Buffer Structure
namespace LcVtx
{
	// Diffuse
	struct VtxD
	{
		FLOAT	p[3];
		DWORD	d;

		VtxD();
		VtxD(FLOAT X,FLOAT Y,FLOAT Z,DWORD D=0xFFFFFFFF);

		enum { FVF = (D3DFVF_XYZ|D3DFVF_DIFFUSE) };
	};

	// Diffuse + Texture 1
	struct VtxDUV1
	{
		FLOAT	p[3];
		DWORD	d;
		FLOAT	u, v;
		
		VtxDUV1();
		VtxDUV1(FLOAT X,FLOAT Y,FLOAT Z,FLOAT U,FLOAT V, DWORD D=0xFFFFFFFF);
		
		enum { FVF = (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1) };
	};


	// Normal
	struct VtxN
	{
		FLOAT	p[3];
		FLOAT	n[3];

		VtxN();
		VtxN(FLOAT X,FLOAT Y,FLOAT Z,FLOAT nX,FLOAT nY,FLOAT nZ);

		enum { FVF = (D3DFVF_XYZ|D3DFVF_NORMAL) };
	};


	// Texture1
	struct VtxUV1
	{
		FLOAT	p[3];
		FLOAT	u,v;

		VtxUV1();
		VtxUV1(FLOAT X,FLOAT Y,FLOAT Z, FLOAT U,FLOAT V);

		enum { FVF = (D3DFVF_XYZ|D3DFVF_TEX1) };
	};



	// Normal + Texture1
	struct VtxNUV1
	{
		FLOAT	p[3];
		FLOAT	n[3];
		FLOAT	u,v;

		VtxNUV1();
		VtxNUV1(FLOAT X,FLOAT Y,FLOAT Z,FLOAT nX,FLOAT nY,FLOAT nZ, FLOAT U,FLOAT V);

		enum { FVF = (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1) };
	};

	// Normal + Diffuse + Texture1
	struct VtxNDUV1
	{
		FLOAT	p[3];
		FLOAT	n[3];
		DWORD	d;
		FLOAT	u,v;

		VtxNDUV1();
		VtxNDUV1(FLOAT X,FLOAT Y,FLOAT Z
			,	 FLOAT nX,FLOAT nY,FLOAT nZ
			,	 FLOAT U,FLOAT V
			,	 DWORD D=0xFFFFFFFF);

		enum { FVF = (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1) };
	};


	struct VtxNDUV2																	// Normal + Diffuse + Texture2
	{
		FLOAT	p[3];
		FLOAT	n[3];
		DWORD	d;
		FLOAT	u0,v0;
		FLOAT	u1,v1;

		VtxNDUV2();
		VtxNDUV2(FLOAT X,FLOAT Y,FLOAT Z
			,	 FLOAT nX,FLOAT nY,FLOAT nZ
			,	 FLOAT U0,FLOAT V0, FLOAT U1,FLOAT V1
			,	 DWORD D=0xFFFFFFFF);

		enum { FVF = (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX2) };
	};


	struct VtxNDUV3
	{
		FLOAT	p[3];
		FLOAT	n[3];
		DWORD	d;
		FLOAT	u0,v0;
		FLOAT	u1,v1;
		FLOAT	u2,v2;

		VtxNDUV3();
		VtxNDUV3(FLOAT X,FLOAT Y,FLOAT Z
			,	 FLOAT nX,FLOAT nY,FLOAT nZ
			,	 FLOAT U0,FLOAT V0
			,	 FLOAT U1,FLOAT V1
			,	 FLOAT U2,FLOAT V2
			,	 DWORD D=0xFFFFFFFF);

		enum { FVF = (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX3) };
	};



	struct VtxRHWD
	{
		FLOAT p[4];
		DWORD d;

		VtxRHWD();
		VtxRHWD(FLOAT X,FLOAT Y,FLOAT Z,DWORD D=0xFFFFFFFF);

		enum {FVF =(D3DFVF_XYZRHW | D3DFVF_DIFFUSE), };
	};


	struct VtxRHWDUV1															// RHW + Diffuse + Texture 1
	{
		FLOAT p[4];
		DWORD d;
		FLOAT u,v;

		VtxRHWDUV1();
		VtxRHWDUV1(FLOAT X,FLOAT Y,FLOAT Z,FLOAT U,FLOAT V, DWORD D=0xFFFFFFFF);

		enum { FVF = (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1) };
	};

};





// Interface
interface ILcVtx
{
	LC_CLASS_DESTROYER(	ILcVtx	);

	virtual INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0)=0;
	virtual void	Destroy()=0;
	virtual void	Render()=0;

	virtual void*	GetVertex()=0;
	virtual DWORD	GetFVF()=0;
	virtual UINT	GetStrid()=0;
	virtual INT		GetVtxNum()=0;

	virtual INT		Lock(UINT OffsetToLock, UINT SizeToLock, void** ppbData, DWORD Flags)=0;
	virtual INT		Unlock()=0;

	virtual	void	SetTexture(INT nStage, void* pTx)=0;
	virtual	void	DrawPrimitive(DWORD PrimitiveType, UINT StartVertex, UINT PrimitiveCount)=0;

	virtual	void	SetPtType(DWORD dPtType)=0;
	virtual	void	SetPtNum(UINT iPtNum)=0;


	// Vertex SetUp( if Lock ppbdata is Null.
	virtual	void	SetPos		(INT nIdx, FLOAT X, FLOAT Y, FLOAT Z, FLOAT W=1.F)=0;
	virtual	void	SetNormal	(INT nIdx, FLOAT X, FLOAT Y, FLOAT Z)=0;

	/* rgba is 0~ 1.f */
	virtual	void	SetDiffuse	(INT nIdx, FLOAT R, FLOAT G, FLOAT B, FLOAT A)=0;
	virtual	void	SetUV		(INT nIdx, INT nUV, FLOAT U, FLOAT V)=0;
};


// Vertex Buffer
// Command Ex) "Diffuse", "Diffuse Texture1", "Normal Texture1", "Normal Diffuse Texture1"
// Primitive Type: "TRIANGLELIST", "TRIANGLESTRIP"
INT	LcM3d_CreateVB(char* sCmd			// Command
				, ILcVtx** pData		//
				, void* p1				// Need D3D Device
				, void* p2				// Primitive Type
				, void* p3				// Primitive Count
				, void* p4				// Total Vertices
				, void* p5=0			// Vertex Memory Copy Source
				, void* p6=0			// Source Total Size
				, void* p7=0			// No Use
				, void* p8=0			// No Use
				, void* p9=0			// No Use
				);



// Index
interface ILcIdx
{
	LC_CLASS_DESTROYER(	ILcIdx	);

	virtual INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0) = 0;
	virtual void	Destroy()=0;

	virtual void*	GetIndex()=0;
	virtual DWORD	GetFMT()=0;
};


// Index buffer
// Command Ex) "Index 16", "Index 32"
INT	LcM3d_CreateIB(char* sCmd		// Command
				, ILcIdx** pData		//
				, void* p1				// Need D3D Device
				, void* p2				// Total Indices
				, void* p3=0			// Index memory copy source
				, void* p4=0			// No Use
				);



//Material
interface ILcMtl
{
	LC_CLASS_DESTROYER(	ILcMtl	);

	virtual INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0) = 0;
	virtual void	Destroy()=0;

	virtual void	SetMaterial()=0;

	virtual void	SetAmbient(FLOAT* pVal/*D3DXCOLOR*/ )=0;
	virtual void	SetDiffuse(FLOAT* pVal/*D3DXCOLOR*/ )=0;
    virtual void	SetSpecular(FLOAT* pVal/*D3DXCOLOR*/ )=0;
    virtual void	SetEmissive(FLOAT* pVal/*D3DXCOLOR*/ )=0;
    virtual void	SetPower(FLOAT pVal)=0;				// Sharpness if specular highlight

	virtual FLOAT*	GetDiffuse()=0;
    virtual FLOAT*	GetAmbient()=0;
    virtual FLOAT*	GetSpecular()=0;
    virtual FLOAT*	GetEmissive()=0;
    virtual FLOAT	GetPower()=0;
};

// Command Ex) "PC Material"
INT	LcM3d_CreateMaterial(char* sCmd		// Command
					, ILcMtl** pData		//
					, void* p1				// d3dDevice
					, void* p2				// Ambient
					, void* p3				// Diffuse
					, void* p4				// Specular
					, void* p5				// Emissive
					, void* p6				// Power
					);


//Lighting
interface ILcLgt
{
	LC_CLASS_DESTROYER(	ILcLgt	);

	virtual INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0) = 0;
	virtual void	Destroy()=0;

	virtual void	SetLight(INT nIdx)=0;										// pDev->SetLight( nIdx,....)
	virtual void	SetLightEnable(BOOL nEnable)=0;								// pDev->LightEnable( nIdx,....)

	virtual void	SetLgtType(DWORD dType)=0;
	
	virtual void	SetLgtAmbient(FLOAT* pVal/*D3DXCOLOR*/ )=0;
	virtual void	SetLgtDiffuse(FLOAT* pVal/*D3DXCOLOR*/ )=0;
    virtual void	SetLgtSpecular(FLOAT* pVal/*D3DXCOLOR*/ )=0;

    virtual void	SetLgtPosition(FLOAT* pVal/*D3DXVECTOR3*/ )=0;
	virtual void	SetLgtDirection(FLOAT* pVal/*D3DXVECTOR3*/ )=0;

	virtual void	SetLgtRange(FLOAT pVal)=0;									// Cutoff range
    virtual void	SetLgtFalloff(FLOAT pVal)=0;								// Falloff

	virtual void	SetLgtAttenuation0(FLOAT pVal)=0;							// Constant attenuation
	virtual void	SetLgtAttenuation1(FLOAT pVal)=0;							// Linear attenuation
	virtual void	SetLgtAttenuation2(FLOAT pVal)=0;							// Quadratic attenuation
	virtual void	SetLgtTheta(FLOAT pVal)=0;									// Inner angle of spotlight cone
	virtual void	SetLgtPhi(FLOAT pVal)=0;									// Outer angle of spotlight cone


	virtual DWORD	GetLgtType()=0;
	
	virtual FLOAT*	GetLgtAmbient()=0;
	virtual FLOAT*	GetLgtDiffuse()=0;
    virtual FLOAT*	GetLgtSpecular()=0;

    virtual FLOAT*	GetLgtPosition()=0;
	virtual FLOAT*	GetLgtDirection()=0;

	virtual FLOAT	GetLgtRange()=0;											// Cutoff range
    virtual FLOAT	GetLgtFalloff()=0;											// Falloff

	virtual FLOAT	GetLgtAttenuation0()=0;										// Constant attenuation
	virtual FLOAT	GetLgtAttenuation1()=0;										// Linear attenuation
	virtual FLOAT	GetLgtAttenuation2()=0;										// Quadratic attenuation
	virtual FLOAT	GetLgtTheta()=0;											// Inner angle of spotlight cone
	virtual FLOAT	GetLgtPhi()=0;												// Outer angle of spotlight cone
};


// Command Ex) "PC Lighting"
INT	LcM3d_CreateLight(char* sCmd			// Command
					, ILcLgt** pData		//
					, void* p1				// d3dDevice
					, void* p2				// Type
					, void* p3				// Ambient
					, void* p4				// Diffuse
					, void* p5				// Specular
					, void* p6				// Position
					, void* p7				// Direction
					, void* p8				// Etc array. {Range, Falloff, Attenuation0, Attenuation1, Attenuation2, Theta, Phi, }
					);




//#pragma comment(lib, "d3dm.lib")
//#pragma comment(lib, "d3dmx.lib")
//#pragma comment(lib, "d3dmguid.lib")

#ifdef _DEBUG
	#pragma comment(lib, "LcM3d_.lib")
#else
	#pragma comment(lib, "LcM3d.lib")
#endif


#endif

