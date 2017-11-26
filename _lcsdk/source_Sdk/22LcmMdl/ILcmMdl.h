// Interface for the ILcmMdl class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _ILcmMdl_H_
#define _ILcmMdl_H_


#ifndef LC_CLASS_DESTROYER
#define LC_CLASS_DESTROYER( CLASS_NAME )	\
virtual ~CLASS_NAME(){}
#endif

namespace Lcm
{

// Model Type
#ifndef LCX_MDL_NONE
#define LCX_MDL_NONE	0x00000000
#define LCX_MDL_BILL	0x00000001
#define LCX_MDL_SOLID	0x00000002
#define LCX_MDL_ACM		0x00000004
#define LCX_MDL_XFILE	0x00010000
#endif

struct _Tattach
{
	INT		nI	;					// Geometry Index
	char*	sN	;					// Name
	FLOAT*	tm	;					// Transformation Matrix Pointer
	INT		nM	;					// Material
};

struct _Tframe
{
	char	sN[64];					// Phase Name
	INT		nB	;					// Begin Frame
	INT		nE	;					// End Frame
	_Tframe();
	_Tframe(char* s, INT B, INT E);
};

struct _Tmtl
{
	void*	dMtl	;				// D3DMATERIAL9 Pointer
	char*	sDif	;				// Diffuse Texture File
	char*	sNor	;				// Normal Texture File
	char*	sSpc	;				// Specular Texture File
	char*	sEms	;				// Emissive Texture File
};


interface ILcmMdl
{
	LC_CLASS_DESTROYER(	ILcmMdl	);
	
	virtual INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0)=0;
	virtual void	Destroy()=0;
	virtual INT		FrameMove()=0;
	virtual void	Render()=0;

	virtual INT		Query(char* sCmd, void* pData)=0;

	virtual	INT		GetType()	=0;

	virtual INT		SetPosition(FLOAT* float3)=0;
	virtual INT		GetPosition(FLOAT* float3)=0;
	virtual INT		SetRotation(FLOAT* float4)=0;	// if Type is LCX_MDL_ACM: Axis(x,y,z) and radian w.
	virtual INT		GetRotation(FLOAT* float4)=0;	// else x, y, z radian for RotMatrixY(y) * RotMatrixZ(z) *  else X, Y, Z radian for RotMatrixY(y) * RotMatrixX(x)
	virtual INT		SetScaling(FLOAT* float3)=0;
	virtual INT		GetScaling(FLOAT* float3)=0;

	virtual	FLOAT	GetSortValue()	=0;
};


INT Mdl_Create(char* sCmd
				 , ILcmMdl** pData
				 , void* p1			// Original Source: Original Source가 NULL이면 새로 만듦
				 , void* p2=0		// Model File Name
				 , void* p3=0		// No Use
				 , void* p4=0		// No Use
				 );

}// namespace Lcm


#ifdef USE_LC_SDK_LIB

	#ifdef _DEBUG
		#pragma comment(lib, "LcmMdl_.lib")
	#else
		#pragma comment(lib, "LcmMdl.lib")
	#endif
#endif


#endif

