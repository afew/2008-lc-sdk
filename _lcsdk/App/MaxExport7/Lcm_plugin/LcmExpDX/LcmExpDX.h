// Interface for the LcmExpDX class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcmExpDX_H_
#define _LcmExpDX_H_


#pragma warning(disable: 4530)
#pragma warning(disable: 4786)

#include <vector>
#include <set>
#include <map>
#include <string>

using namespace	std;

#include <d3d9.h>
#include <d3dx9.h>

#define LcmExpDX_CLASS_ID	Class_ID(0x46732508, 0x3519f75c)


extern TCHAR*			GetString(int id);
extern HINSTANCE		GetHinstance();
extern void				MsgBox(char *format,...);


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


#define LCM_TX_NAME			128
#define	LCM_HEADER_SIZE		256


#define ANI_TYPE_NONE	0
#define ANI_TYPE_LINEAR	1
#define ANI_TYPE_TCB	2
#define ANI_TYPE_BEZIER	3


#define PHYSQ_CLASSID Class_ID(PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B)

#ifndef SKIN_CLASSID
#define SKIN_CLASSID Class_ID(9815843,87654)
#endif

class CLcmExpDXClassDesc:public ClassDesc2
{
public:
	int 			IsPublic();
	void*			Create(BOOL loading = FALSE);
	const TCHAR*	ClassName();
	SClass_ID		SuperClassID();
	Class_ID		ClassID();
	const TCHAR*	Category();
	
	const TCHAR*	InternalName();
	HINSTANCE		HInstance();
};


class CLcmExpDX : public SceneExport
{
// Basic Code
protected:
	ExpInterface*	m_pE;						//
	Interface*		m_pI;						//
	BOOL			m_bS;						//
	DWORD			m_dO;						//

public:
	CLcmExpDX();
	virtual ~CLcmExpDX();
	
	int				ExtCount();					// Number of extensions supported
	const TCHAR*	Ext(int n);					// Extension #n (i.e. "3DS")
	const TCHAR*	LongDesc();					// Long ASCII description (i.e. "Autodesk 3D Studio File")
	const TCHAR*	ShortDesc();				// Short ASCII description (i.e. "3D Studio")
	const TCHAR*	AuthorName();				// ASCII Author name
	const TCHAR*	CopyrightMessage();			// ASCII Copyright message
	const TCHAR*	OtherMessage1();			// Other message #1
	const TCHAR*	OtherMessage2();			// Other message #2
	unsigned int	Version();					// Version number * 100 (i.e. v3.01 = 301)
	void			ShowAbout(HWND hWnd);		// Show DLL's "About..." box
	
	BOOL			SupportsOptions(int ext, DWORD options);
	int				DoExport(const TCHAR*, ExpInterface*, Interface*, BOOL sPrompts=FALSE, DWORD options=0);
	int				ExportAcm();
	static BOOL CALLBACK DlgProc(HWND,UINT,WPARAM,LPARAM);

// Application Code
protected:
	BOOL			m_bLog;						// IDC_EXP_LOG
	BOOL			m_bAni;						// IDC_EXP_ANI
	BOOL			m_bTxt;						// IDC_EXP_TXT

	HWND			m_hWnd;
	TCHAR			m_sN[MAX_PATH*2];			// Export File Name
	TCHAR			m_sP[MAX_PATH*2];			// Export Path


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
		
		VtxD()		: p(0,0,0), d(0xFFFFFFFF){}
		VtxD(FLOAT X,FLOAT Y,FLOAT Z,DWORD D=0xFFFFFFFF) : p(X,Y,Z), d(D){}
		enum	{FVF = (D3DFVF_XYZ|D3DFVF_DIFFUSE),};
	};


	struct VtxNDUV1
	{
		VEC3	p;
		VEC3	n;
		DWORD	d;
		FLOAT	u,v;

		VtxNDUV1()	: p(0,0,0), n(0,0,0), u(0), v(0), d(0xFFFFFFFF){}
		VtxNDUV1(FLOAT X,FLOAT Y,FLOAT Z,FLOAT U,FLOAT V, DWORD D=0xFFFFFFFF):p(X,Y,Z),u(U),v(V),d(D){}
		enum	{FVF = (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1),};
	};

	struct TfceIdx
	{
		INT	n;
		TfceIdx() : n(0){}
		TfceIdx(INT _T) : n(_T){}
	};


	template<class T>
	struct TsrtG																	// For sort... descendent Sort
	{
		bool operator()(const T& t1,const T& t2) const	{ return t1.n < t2.n; }
	};

	typedef map<TfceIdx, INT, TsrtG<TfceIdx > >	mpTfce;
	typedef mpTfce::iterator							itTfce;



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
		map<INT, FLOAT >	vB;		// Bone Index
	};

	struct LcmHeader
	{
		char	sVer[16];				// Version
		INT		iNmtl;					// Merterial Num
		INT		iNgeo;					// Geometry Num

		INT		nFrmB;					// Begin Frame
		INT		nFrmE;					// End Frame
		INT		nFrmP;					// Frame Rate(FPS)
		INT		nFrmT;					// Tick Fram
		
		LcmHeader();
	};

	struct LcMtl						// 머티리얼 자료구조
	{
		INT			nIdx;				// Material 인덱스
		DMTL		dMtl;
		char		sDif[LCM_TX_NAME];	// Diffuse Texture File
		char		sNor[LCM_TX_NAME];	// Normal Texture File
		char		sSpc[LCM_TX_NAME];	// Specular Texture File
		char		sEms[LCM_TX_NAME];	// Emissive Texture File

		LcMtl();
	};


	
	struct LcFceR
	{
		int		nBgn;					// Start of Face
		int		nNum;					// Number of Face
		int		nMtl;					// Index of Face
		
		LcFceR() : nBgn(0), nNum(0), nMtl(0){}
		LcFceR(INT _B, INT _N, INT _M) : nBgn(_B), nNum(_N), nMtl(_M){}
	};

	struct LcGeo						// Geometry 자료구조
	{
		char		sName[32];			// Node Name
		char		sPrnt[32];			// Parent Name
		INT			nIdx;				// Index;
		INT			nPrn;				// Parent Index
		INT			nMtl;				// Material index

		MATA		mtLcl;				// Local 행렬
		MATA		mtWld;				// World 행렬
		MATA		mtWldT;				// World 행렬
		MATA		mtOrn;				// Orient Direction World Matrix

		DWORD		dFVF;				// FVF
		INT			iNfce;				// Face Number
		INT			iNvtx;				// Vertex Number

		VtxIdx*		pFce;
		void*		pVxO;				// Original

		INode*		pNode;

		BOOL		bBone;				// IsBone?
		INT			nBsrc;				// Bone에 영향받는 버텍스 수
		LcmBoneW*	pBsrc;				// Source
		LcmBoneW*	pBdst;				// Dest


		vector<QUAT>	vAniRot;
		vector<VEC3>	vAniTrn;

		vector<LcFceR>	vFceRnd;

		LcGeo();
		~LcGeo();
	};




	// Exporting Base Data
public:
	struct MtlInf
	{
		Mtl*	pM;
		INT		nM;		// Master Id
		INT		nS;		// Sub Id

		MtlInf(Mtl*	p, INT M, INT S)
		{
			pM	=	p;
			nM	=	M;
			nS	=	S;
		}
	};

	vector<INode*>	m_vMaxNode;		// 맥스의 Node 자료구조
	vector<MtlInf>	m_vMaxMtrl;		// 맥스의 Node 자료구조

	void		GatherNode(INode*);
	void		GatherMtl();


	// for Output Data
public:
	LcmHeader	m_Header;
	LcMtl*		m_pLcMtl;
	LcGeo*		m_pLcGeo;


	BOOL		IsBone(INode *pNode);
	void		DumpTexture(LcMtl*, Texmap*, INT iSubNo);

	void		GatherMesh(LcGeo*, TimeValue);


	// for Animation
	void		GatherAnimation(LcGeo*);

	// for Bone or Biped  Animation
	void		GatherBoneAni(LcGeo*);

	void		GatherBoneWeight(LcGeo*);
	void		GetPhysiqueWeights(LcGeo*, Modifier*);
	void		GetSkinWeights(LcGeo*, Modifier*);

	Modifier*	GetModifier(INode* , Class_ID /*Physique=PHYSQ_CLASSID, skin=SKIN_CLASSID*/);
	INT			FindBoneId(INode*);


	TriObject*	GetTriObjectFromNode(INode*, TimeValue, int&);

	void		WriteBin();
	void		WriteTxt();


protected:
	// For Log..
	FILE*		m_pFl;					// Log File

	void		WriteLog(char *format,...);
	void		FileRename(char* pOut, char* pIn, char* sFileName, char* sExt);
	void		MaxMatrixToD3D(D3DXMATRIX* pDst, Matrix3* pSrc, BOOL bIdentity);

};


// UV Packing용ㅇ
struct _Tpck
{
	INT		nT;		// Original Tface Index
	INT		nV;		// Vertex Index
	FLOAT	u;		// Texture U
	FLOAT	v;		// Texture V

	_Tpck()
	{
		nT=0;
		nV=0;
		u=0;
		v=0;
	}

	_Tpck(INT T, INT Vtx, FLOAT U, FLOAT V): nT(T), nV(Vtx), u(U), v(V)
	{
	}
};


#endif

