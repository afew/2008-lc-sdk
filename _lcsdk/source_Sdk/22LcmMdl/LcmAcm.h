// Interface for the ILcmAcm class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcmAcm_H_
#define _LcmAcm_H_


typedef D3DXVECTOR2						VEC2;
typedef D3DXVECTOR3						VEC3;
typedef D3DXVECTOR4						VEC4;

typedef D3DXMATRIX						MATA;
typedef D3DXQUATERNION					QUAT;
typedef D3DXCOLOR						DCLR;

typedef D3DMATERIAL9					DMTL;
typedef LPDIRECT3DTEXTURE9				PDTX;
typedef	D3DXIMAGE_INFO					DIMG;

typedef	LPDIRECT3DDEVICE9				PDEV;
typedef LPDIRECT3DVERTEXDECLARATION9	PDEC;
typedef LPD3DXEFFECT					PDEF;

#pragma warning(disable : 4530)
#pragma warning(disable : 4786)
#pragma warning(disable : 4996)


#include <vector>
#include <set>
#include <map>

#include <string>


using namespace	std;


#define LCM_TX_NAME			128
#define	LCM_HEADER_SIZE		256


namespace Lcm
{

class CLcmAcm : public CLcmMdl
{
public:
	struct VtxIdx
	{
		WORD	a, b, c;
		VtxIdx(){	a=0; b=0; c=0;	}
		VtxIdx(WORD A, WORD B, WORD C) : a(A), b(B), c(C){}
		enum	{FVF = (D3DFMT_INDEX16),};
	};


	struct VtxD
	{
		VEC3	p;
		DWORD	d;
		
		VtxD() : p(0,0,0), d(0xFFFFFFFF){}
		VtxD(FLOAT X,FLOAT Y,FLOAT Z,DWORD D=0xFFFFFFFF) : p(X,Y,Z), d(D){}
		enum	{FVF = (D3DFVF_XYZ|D3DFVF_DIFFUSE),};
	};


	struct VtxNDUV1
	{
		VEC3	p;
		VEC3	n;
		DWORD	d;
		FLOAT	u,v;

		VtxNDUV1() : p(0,0,0), n(0,0,0), u(0), v(0), d(0xFFFFFFFF){}
		VtxNDUV1(FLOAT X,FLOAT Y,FLOAT Z,FLOAT U,FLOAT V, DWORD D=0xFFFFFFFF):p(X,Y,Z),u(U),v(V),d(D){}
		enum	{FVF = (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1),};
	};

	struct VtxNDUV1i
	{
		VEC3  p;						// Position

		VEC3	n;						// Normal
		DWORD	d;						// Diffuse
		FLOAT	u,v;					// Texture Coordinate

		VEC4	w;						// blend weight
		VEC4	g;						// blend index

		VtxNDUV1i()	: p(0,0,0)
					, n(0,0,0)
					, u(0), v(0)
					, w(0, 0, 0, 0)
					, g(0, 0, 0, 0)
					, d(0xFFFFFFFF){}

		enum	{
				FVF = (	D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|
						D3DFVF_TEX3|				//
						D3DFVF_TEXCOORDSIZE2(0)|	// texture
						D3DFVF_TEXCOORDSIZE4(1)|	// blend weight
						D3DFVF_TEXCOORDSIZE4(2)		// blend index
						),};
	};


	struct TaniRot
	{
		INT		nF;						// Frame
		QUAT	vT;						// Rotation is Quatanion
	};

	struct TaniScl
	{
		INT		nF;						// Frame
		VEC3	vT;						// Rotation is Quatanion
	};

	struct TaniTrn
	{
		INT		nF;						// Frame
		VEC3	vT;						// Rotation is Quatanion
	};


	struct TboneW
	{
		INT		n;						// Bone Index
		FLOAT	w;						// Weight

		TboneW() : n(-1), w(0){}
		TboneW(INT nBone, FLOAT fW) : n(nBone), w(fW) {}
	};

	struct LcmBoneW
	{
		vector<TboneW>	vB;				// Bone Index
	};

	struct LcmHeader
	{
		char		sVer[16];			// Version
		INT			iNmtl;				// Merterial Num
		INT			iNgeo;				// Geometry Num

		INT			nFrmB;				// Begin Frame
		INT			nFrmE;				// End Frame
		INT			nFrmP;				// Frame Rate(FPS)
		INT			nFrmT;				// Tick Frame
		
		LcmHeader();
	};



	struct TaniIfl
	{
		INT			nF;					// Frame
		PDTX		pT;					// Texture
		TaniIfl();
		TaniIfl(INT, PDTX);
	};

	struct LcMtl								// 머티리얼 자료구조
	{
		INT				nIdx	;				// Material 인덱스
		DMTL			dMtl	;
		char			sDif[LCM_TX_NAME];		// Diffuse Texture File
		char			sNor[LCM_TX_NAME];		// Normal Texture File
		char			sSpc[LCM_TX_NAME];		// Specular Texture File
		char			sEms[LCM_TX_NAME];		// Emissive Texture File

		PDTX			pDif	;
		PDTX			pNor	;
		PDTX			pSpc	;
		PDTX			pEms	;

		vector<TaniIfl>* pvIfl	;

		LcMtl();
	};


	struct TaniTx
	{
		INT				nF	;					// Frame
		LcMtl*			pM	;					// Material Pointer
		PDTX			pT	;					// Texture Pointer
		TaniTx();
		TaniTx(INT, LcMtl*);
	};


	struct LcFceR
	{
		int				nBgn	;				// Start of Face
		int				nNum	;				// Number of Face
		int				nMtl	;				// Index of Face
		
		LcFceR() : nBgn(0), nNum(0), nMtl(0){}
		LcFceR(INT _B, INT _N, INT _M) : nBgn(_B), nNum(_N), nMtl(_M){}
	};
	

	struct LcGeo								// Geometry 자료구조
	{
		char			sName[32]	;			// Node Name
		char			sPrnt[32]	;			// Parent Name
		INT				nIdx	;				// Index;
		INT				nPrn	;				// Parent Index
		INT				nMtl	;				// Material index
		INT				nMtlS	;				// Material = 0, Face = 1

		MATA			mtLcl	;				// Local 행렬
		MATA			mtWld	;				// World 행렬
		MATA			mtOrn	;				// Orient Direction World Matrix
		MATA			mtOrI	;				// Orient Direction World Matrix Inverse
		MATA			mtPvt	;				// Pivot Matrix

		DWORD			dFVF	;				// FVF
		INT				iNfce	;				// Face Number
		INT				iNvtx	;				// Vertex Number

		VtxIdx*			pFce	;
		VtxD*			pVxD	;				// Diffuse
		VtxNDUV1i*		pVxA	;				// Rendering with HLSL

		INT				nBsrc	;				// Bone에 영향받는 버텍스 수
		LcmBoneW*		pBdst	;				// Dest

		vector<QUAT>*	vAniRot	;
		vector<VEC3>*	vAniTrn	;
		vector<LcFceR>*	vFceRnd	;

		BOOL		nBlend		;				// Animation Type
		
		LcGeo();
	};

protected:
	LcmHeader*		m_pLcHead	;
	LcMtl*			m_pLcMtl	;
	LcGeo*			m_pLcGeo	;
	vector<_Tframe>* m_pLcFrame	;
	CLcmAcm*		m_pLcOrg	;
	map<string, PDTX>*	m_mpTxOrg;

	PDEV			m_pDev		;

	DWORD			m_dTimeB	;				// Time Begin
	FLOAT			m_nFrmS		;				// Animation Speed
	INT				m_nFrmI		;				// Initial Begin Frame
	INT				m_nFrmC		;				// Current Frame
	INT				m_nActM		;				// Action Master Index Current
	INT				m_nActO		;				// Action Master Index Old

	VEC4			m_vcRot		;				// Axis and angle
	VEC3			m_vcScl		;
	VEC3			m_vcTrn		;
	MATA			m_mtRot		;
	MATA			m_mtWld		;

	MATA*			m_mtPrn		;				// Parent World Matrix

	MATA*			m_pmtWld	;
	PDEF			m_pEffct	;				// 이펙트
	D3DXHANDLE		m_hdTech	;				// 테크닉

	DWORD			m_dIflTime	;				// Time Start
	DWORD			m_dIflIntv	;				// Time Interval
	vector<TaniTx>	m_vIfl		;


	FLOAT			m_fStlSrtR	;				// Z Direction Value for Sort...

public:
	CLcmAcm();
	virtual ~CLcmAcm();

	virtual INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0);
	virtual void	Destroy();
	virtual INT		FrameMove();
	virtual void	Render();

	virtual INT		Query(char* sCmd, void* pData);

	virtual	INT		GetType();

	virtual INT		SetPosition(FLOAT* float3);
	virtual INT		GetPosition(FLOAT* float3);

	virtual INT		SetRotation(FLOAT* float4);
	virtual INT		GetRotation(FLOAT* float4);

	virtual INT		SetScaling(FLOAT* float3);
	virtual INT		GetScaling(FLOAT* float3);

	virtual	FLOAT	GetSortValue();

protected:
	INT		Clone(ILcmMdl*);
	void	RederingHlsl();
	void	RederingBone();

protected:
	INT		LoadMdl(char* sFile);
	INT		LoadAtc(char* sFile);
	INT		TextureLoad(char* sFile, LPDIRECT3DTEXTURE9*, DWORD dFault=0xFFFFFFFF);

	DWORD	TimeGetTime();
	INT		TextureLoadIfl(LcMtl* pMtl, char* sDriv, char* sDirc, char* sName);

protected:
	INT		m_bTex;						// for Viewer: App Texture
	INT		m_bBone;					// for Viewer: Show Bone
	INT		m_bModel;					// for Viewer: Show Model
	INT		m_bAnima;					// for Viewer: Animation
};

}// namespace Lcm

#endif

