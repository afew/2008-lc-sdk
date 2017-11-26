// ILcxmFldX Utilities.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _ILcxmFldX_H_
#define _ILcxmFldX_H_


#pragma warning(disable : 4786)
#include <vector>


typedef D3DXVECTOR2				VEC2;
typedef	D3DXVECTOR3				VEC3;
typedef D3DXVECTOR4				VEC4;



namespace Lcxmo							// Map(Outdoor)
{
	struct VtxIdx																	// Index structure
	{
		union	{	struct	{	WORD a;	WORD b;	WORD c;	};	WORD m[3];	};

		VtxIdx()					: a(0),b(1),c(2){}
		VtxIdx(WORD A,WORD B,WORD C): a(A),b(B),c(C){}
		VtxIdx(WORD* R)				: a(R[0]),b(R[1]),c(R[2]){}

		operator WORD* ()			{	return (WORD *) &a;			}
		operator const WORD* () const{	return (const WORD *) &a;	}
	};

	struct VtxD																		// Diffuse
	{
		D3DXVECTOR3	p;
		D3DCOLOR	d;
		
		VtxD()							: p(0,0,0),d(0XFFFFFFFF){}
		VtxD(FLOAT X,FLOAT Y,FLOAT Z
			,D3DCOLOR D=0xFFFFFFFF)		: p(X,Y,Z),d(D){}

		enum { FVF = (D3DFVF_XYZ|D3DFVF_DIFFUSE) };
	};


	typedef FLOAT	VEC6[6];

	typedef	int		SplIdx;

	struct VtxSpl
	{
		VEC3	p;
		VEC3	n;
		DWORD	d;
		FLOAT	u, v;
		VEC6	ts;

		VtxSpl()	: p(0,0,0),n(0,0,0),u(0),v(0),d(0xFFFFFFFF)
		{
			ts[0] = 0;	ts[1] = 0;	ts[2] = 0;	ts[3] = 0;	ts[4] = 0;	ts[5] = 0;
		}
		VtxSpl(FLOAT X, FLOAT Y, FLOAT Z
				, FLOAT _X, FLOAT _Y, FLOAT _Z
				, FLOAT U, FLOAT V
				, DWORD D=0xFFFFFFFF)	: p(X,Y,Z),n(_X,_Y,_Z),u(U),v(V),d(D)
		{
			ts[0] = 0;	ts[1] = 0;	ts[2] = 0;	ts[3] = 0;	ts[4] = 0;	ts[5] = 0;
		}

		enum {	FVF= (D3DFVF_XYZ|D3DFVF_NORMAL|\
					D3DFVF_DIFFUSE|D3DFVF_TEX3|\
					D3DFVF_TEXCOORDSIZE2(0)|\
					D3DFVF_TEXCOORDSIZE4(1)|\
					D3DFVF_TEXCOORDSIZE2(2)
					)
				};
	};


	struct MpTex
	{
		struct _Tex
		{
			LPDIRECT3DTEXTURE9	pTex;		// D3DTexture Pointer
			D3DXIMAGE_INFO		Image;			// Image Info
			char				sName[128];		// File Name

			_Tex();
		};

		_Tex	TexB;
		
		MpTex();
	};

	typedef std::vector<MpTex >	lsMpTex;
	typedef lsMpTex::iterator	itMpTex;
	


	struct MpInf						// Field Information
	{
		union
		{
			struct
			{
				INT		nI		;		// Index
				char	sN[128]	;		// Name
				INT		iNfX	;		// Number of Field for X Direction
				INT		iNfZ	;		// Number of Field for Z Direction
				INT		iExt	;		// Extent of Local for X Direction
				INT		iNx		;		// Number of Cells in Block for X or Z Direction
				FLOAT	fUV		;		// UV value
			};

			char		_t[1024];		// Map 크기를 맞추기 위한 Offset Value
		};

		MpInf();
	};


	struct MpFog
	{
		DWORD		m_dFt		;		// Fog Table Mode
		DWORD		m_dFc		;		// Fog Color
		FLOAT		m_fFb		;		// Fog Begin
		FLOAT		m_fFe		;		// Fog End
		FLOAT		m_fFd		;		// Fog Density

		MpFog();

		void	SetFog(void* pDev);
	};


	// Material Redefine
	union MpMtl
	{
		struct
		{
			D3DXCOLOR	dcD	;			// Diffuse color RGBA
			D3DXCOLOR	dcA	;			// Ambient color RGB
			D3DXCOLOR	dcS	;			// Specular 'shininess'
			D3DXCOLOR	dcE	;			// Emissive color RGB
			FLOAT		fPw	;			// Sharpness if specular highlight
		};

		D3DMATERIAL9 m;
	};

	union MpLgt
	{
		struct 
		{
			UINT		Type	;		// Type of light source
			D3DXCOLOR	dDif	;		// Diffuse color of light
			D3DXCOLOR	dSpc	;		// Specular color of light
			D3DXCOLOR	dAmb	;		// Ambient color of light
			D3DXVECTOR3	vPos	;		// Position in world space
			D3DXVECTOR3	vDir	;		// Direction in world space
	//		FLOAT		vEtc[7]	;
			FLOAT		Range	;		// Cutoff range
			FLOAT		Falloff	;		// Falloff
			FLOAT		Attn0	;		// constant attenuation
			FLOAT		Attn1	;		// Linear attenuation
			FLOAT		Attn2	;		// Quadratic attenuation
			FLOAT		Theta	;		// Inner angle of spotlight cone
			FLOAT		Phi		;		// Outer angle of spotlight cone
		};
		
		D3DLIGHT9		l;				// Lighting

		operator D3DLIGHT9* ()
		{
			return (D3DLIGHT9 *) &Type;
		}

		operator const D3DLIGHT9* () const
		{
			return (const D3DLIGHT9 *) &Type;
		}
	};


#define MAX_LCX_MAP_LIGHT	16

	struct MpEnv
	{
		DWORD		m_dAb	;			// Ambient color
		MpMtl		m_Mtl	;			// Material
		
		MpFog		m_Fog	;
		INT			m_iLg	;			// Lighting Num
		MpLgt		m_pLg[MAX_LCX_MAP_LIGHT];			// Lighting

		MpEnv();
	};


	struct MpVec3T
	{
		INT			iZ;
		INT			iX;
		INT			iK;
		D3DXVECTOR3	p;

		MpVec3T();
		MpVec3T(INT Z, INT X, INT K, const D3DXVECTOR3& P);
	};


	struct MpPckT																	// Picking Triangle
	{
		D3DXVECTOR3	vcPk;																// Get Position
		D3DXVECTOR3	p0;																	// Triangle
		D3DXVECTOR3	p1;																	// Triangle
		D3DXVECTOR3	p2;																	// Triangle
		FLOAT		fStlSrtR;															// Distance From Camera

		MpPckT();
		MpPckT(D3DXVECTOR3& vcP, D3DXVECTOR3& P0, D3DXVECTOR3& P1, D3DXVECTOR3& P2, FLOAT& _fD);
	};


	typedef std::vector<MpPckT >	lsTlPkT;
	typedef lsTlPkT::iterator		itTlPkT;


	struct MpHgt			// Height Filed Setup structure
	{
		INT 	iFlat;		// 0 or 1
		INT 	iRng;		// Radius or range
		INT 	bAc;		// Active Color
		FLOAT	fH;			// Increase Height Value
		FLOAT	fDH;		// Direction : 1 or -1
	};

};


#endif


