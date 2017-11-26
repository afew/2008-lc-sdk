// Interface for the IMdBase class.
// Geometry = Material + Mesh
// Model = Geometries.
////////////////////////////////////////////////////////////////////////////////


#ifndef _ILcmMdlX_H_
#define _ILcmMdlX_H_


#pragma warning(disable : 4786)
#include <vector>

#ifndef interface
#define interface struct
#endif

#ifndef LC_CLASS_DESTROYER
#define LC_CLASS_DESTROYER( CLASS_NAME )	\
virtual ~CLASS_NAME(){}
#endif


namespace Lcm
{

// Model: Geometry 와 지오메트리에 대응하는 행렬로 구성
INT MdlX_Create(char* sCmd
				 , ILcmMdl** pData
				 , void* p1			// LPDIRECT3DDEVICE9
				 , void* p2=NULL	// Source Mesh
				 , void* p3=NULL	// No Use
				 , void* p4=NULL	// No Use
				 );

typedef std::vector<ILcmMdl* >		lsLcxMdl;
typedef lsLcxMdl::iterator			itLcxMdl;


// Packages : 모델들 리스트 자료구조.
interface ILcmPack
{
	LC_CLASS_DESTROYER(	ILcmPack	);
	
	virtual	INT		Create(void* =0, void* =0, void* =0, void* =0)=0;
	virtual	void	Destroy()=0;
	virtual	INT		FrameMove()=0;
	virtual	void	Render()=0;
	virtual INT		Query(char* sCmd, void* pData)=0;

	virtual void	SetLcl(void* pLcl)=0;
	virtual void	AddObj(ILcmMdl* pMdB
							, D3DXVECTOR3 vcP		// Position
							, void* p1		// Camera
							, void* p2		// Field
							, void* p3		// Field Local
							, void* p4		// Parent Package
							)=0;

	// For STL
	virtual	void		Push_Back(ILcmMdl* pMdB)=0;
	virtual	INT			Size()=0;
	virtual	ILcmMdl*	GetObj(int n)=0;
	virtual	void		Erase(int n)=0;	
};


typedef std::vector<ILcmPack* >	lsLcmPack;
typedef lsLcmPack::iterator		itLcmPack;


INT MdlX_CreatePack(char* sCmd
				 , ILcmPack** pData
				 , void* p1			// LPDIRECT3DDEVICE9
				 , void* p2=NULL	// Local Field
				 , void* p3=NULL	// No Use
				 , void* p4=NULL	// No Use
				 );

// Mesh: Vertex Buffer와 Index Buffer 를 가지고 있는 모델링 단위
// Texture 이름은 하나만 있을 수 있는데 다중 텍스처를 사용하는 경우 강제로 
// 텍스처를 설정한다.
//
interface ILcmMesh
{
	LC_CLASS_DESTROYER(	ILcmMesh	);

	virtual	INT		Create(void* =0, void* =0, void* =0, void* =0)=0;
	virtual	void	Destroy()=0;

	virtual	INT		FrameMove()=0;
	virtual	void	Render()=0;

	virtual INT		Query(char* sCmd, void* pData)=0;

	virtual INT		GetType()		=0;											// Mesh Type, Billboard ,solid, etc.
	virtual void*	GetGeometry()	=0;
	virtual	void*	GetBufVtx()		=0;											// Get Vertex Pointer
	virtual void*	GetBndInf()		=0;
};


typedef std::vector<ILcmMesh* >		lsLcmMesh;
typedef lsLcmMesh::iterator			itLcmMesh;


INT MdlX_CreateMsh(char* sCmd
				 , ILcmMesh** pData
				 , void* p1			// LPDIRECT3DDEVICE9
				 , void* p2=NULL	// Source Mesh
				 , void* p3=NULL	// If Surce is NULL then Fill the File Name.
				 , void* p4=NULL	// No Use
				 );



// Cuboid Axis Aligned Bounding Box
//
//	Edges
//
//		5------------7(Max)
//	y  /|           /|
//	| / |   z      / |
//	|/  |  /      /  |
//	1------------3   |
//	|   |/       |   |
//	|   4--------|---6
//	|  /         |  /
//	| /          | /
//	|/           |/
//	0(Min)-------2----- x


struct TbndAB
{
	struct _Tvtx
	{
		D3DXVECTOR3	p;
		DWORD		d;
		enum { FVF = (D3DFVF_XYZ|D3DFVF_DIFFUSE) };

		_Tvtx() : d(0xFFFFFFFF){}

	};
	
	TbndAB::_Tvtx vcE[8];	// Edge

	D3DXVECTOR3	vcM;	// Max
	D3DXVECTOR3	vcN;	// Min
	D3DXVECTOR3	vcC;	// Center

	FLOAT		fX;		// Width
	FLOAT		fY;		// Height
	FLOAT		fZ;		// Depth
	FLOAT		fR;		// Radius
	void*		pOwner;	// Owner of Bound Box

	TbndAB();
	TbndAB(const D3DXVECTOR3& Min, const D3DXVECTOR3& Max, void* pOwn=NULL);


	void			Set(const D3DXVECTOR3& Min, const D3DXVECTOR3& Max, void* pOwn=NULL);

	void			RenderBox(void* pDev);
	void			SetOwner(void* pOwn);
	void*			GetOwner();
	void			SetColor(DWORD dColor);

	D3DXVECTOR3*	GetMin() const;
	D3DXVECTOR3*	GetMax() const;
	D3DXVECTOR3*	GetCenter() const;
	FLOAT			GetWidth();
	FLOAT			GetHeight();
	FLOAT			GetDepth();
	FLOAT			GetRadius();
};


struct TbndAABB																	// Picking AABB
{
	D3DXVECTOR3	vPck;															// Get Position
	TbndAB*		pBnd;															// Bound Box Pointer
	FLOAT		fStlSrtR;														// Distance From Camera

	TbndAABB();
	TbndAABB(const D3DXVECTOR3& vcP, TbndAB* _pBnd, FLOAT _fD);
};

typedef std::vector<TbndAABB >	lsPkBndA;
typedef lsPkBndA::iterator		itPkBndA;


INT	LcxUtil_PickAABB(FLOAT* fDist												// Distance
					, D3DXVECTOR3* pvcOut										// In, Out: Picking Position
					, D3DXVECTOR3* pvcCamPos									// In: Camera Position
					, D3DXVECTOR3* pvcRayDir									// In: Ray Direction
					, TbndAB* pIn												// In: Bound Box for Test.
					);



struct Idx16																	// 16bit Index structure
{
	union	{	struct	{	WORD a;	WORD b;	WORD c;	};	WORD m[3];	};

	Idx16()						: a(0),b(1),c(2){}
	Idx16(WORD A,WORD B,WORD C)	: a(A),b(B),c(C){}
	Idx16(WORD* R)					: a(R[0]),b(R[1]),c(R[2]){}

	operator WORD* ()				{	return (WORD *) &a;			}
	operator const WORD* () const	{	return (const WORD *) &a;	}
};

struct VtxDUV1																	// Diffuse + Texture 1
{
	D3DXVECTOR3	p;
	DWORD		d;
	FLOAT		u, v;
	
	VtxDUV1()						: p(0,0,0),u(0),v(0),d(0xFFFFFFFF){}
	VtxDUV1(FLOAT X,FLOAT Y,FLOAT Z
		,FLOAT U,FLOAT V, DWORD D=0xFFFFFFFF)	: p(X,Y,Z),u(U),v(V),d(D){}
	
	enum { FVF = (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1) };
};


struct VtxNDUV1																	// Normal + Diffuse + Texture1
{
	D3DXVECTOR3	p;
	D3DXVECTOR3	n;
	DWORD		d;
	FLOAT		u,v;

	VtxNDUV1()						: p(0,0,0),n(0,0,0),u(0),v(0),d(0xFFFFFFFF){}
	VtxNDUV1(FLOAT X,FLOAT Y,FLOAT Z
			,FLOAT nX,FLOAT nY,FLOAT nZ
			,FLOAT U,FLOAT V,DWORD D=0xFFFFFFFF) : p(X,Y,Z),n(nX,nY,nZ),u(U),v(V),d(D){}

	enum { FVF = (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1) };
};


struct TmshInf
{
	INT			nType	;														// Model Type
	char		sMdl[128];

	INT			iNix	;														// Count Vertex Index
	INT			iNvx	;														// Count Vertex

	DWORD		dFVF	;														// FVF
	INT			iVxS	;														// vertex structure size

	Idx16*		pIdx	;														// Vertex Face Index
	void*		pVtx	;														//	VtxD, VtxUV, VtxNUV, VtxNDUV

	void*		pTex	;
	char		sTex[128];

	TmshInf();
	~TmshInf();

	INT		GetType()		;													// Mesh Type, Billboard ,solid, etc.

	char*	GetMdlName()	;													// Get Model Name
	void	SetMdlName(char*);													// Set Model Name

	INT		GetNumFce()		;													// Count Vertex Index
	INT		GetNumVtx()		;													// Count Vertex
	DWORD	GetFVF()		;													// Flexible vertex Format
	INT		GetZeroStrd()	;													// Zero Stride size

	void*	GetBufFce()		;													// Get Index Pointer
	char*	GetTexName()	;													// Get Texture Name
	void	SetTexName(char*);													// Set Texture Name

	void*	GetTex()		;													// Tet Texture Point
	void	SetTex(void*)	;													// Set Texture Point
};

}// namespace Lcm


#endif


