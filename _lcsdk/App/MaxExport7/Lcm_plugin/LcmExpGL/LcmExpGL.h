// Interface for the LcmExpGL class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcmExpGL_H_
#define _LcmExpGL_H_


#pragma warning(disable: 4786)
#pragma warning(disable: 4530)

#include <vector>
#include <set>
#include <map>

#include <windows.h>
#include <direct.h>
#include <commdlg.h>

#include <d3d9.h>
#include <d3dx9.h>


#define LcmExpGL_CLASS_ID	Class_ID(0x96a3eba, 0x6af407f3)

extern TCHAR*			GetString(int id);
extern HINSTANCE		GetHinstance();


typedef D3DXVECTOR2			VEC2;
typedef D3DXVECTOR3			VEC3;
typedef D3DXVECTOR4			VEC4;
typedef D3DXQUATERNION		QUAT;

typedef D3DXMATRIX			MATA;
typedef D3DMATERIAL9		DMTL;
typedef LPDIRECT3DTEXTURE9	PDTX;
typedef	LPDIRECT3DDEVICE9	PDEV;



#pragma warning(disable : 4530)
#include <vector>

#include <d3dx9.h>

// format version
#define MRC_VERSION 201
static const DWORD mrc_version = (('m') | ('r'<<8) | ('c'<<16) | (MRC_VERSION<<24));



class LcmExpGLClassDesc : public ClassDesc2 
{
public:
	int 			IsPublic();
	void*			Create(BOOL loading = FALSE);
	const TCHAR*	ClassName();
	SClass_ID		SuperClassID();
	Class_ID		ClassID();
	const TCHAR* 	Category();
	const TCHAR*	InternalName();
	HINSTANCE		HInstance();
};


class LcmExpGL : public SceneExport
{
protected:
	ExpInterface*	m_pE;						//
	Interface*		m_pI;						//
	BOOL			m_bS;						//
	DWORD			m_dO;						//

public:
	LcmExpGL();
	virtual ~LcmExpGL();

public:
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
	int				DoExport(const TCHAR*, ExpInterface*, Interface*, BOOL s=FALSE, DWORD o=0);
	static BOOL CALLBACK DlgProc(HWND,UINT,WPARAM,LPARAM);

public:
	// Object header 
	struct MrcObj
	{
		INT		boneCnt;
		INT		boneOfs;

		DWORD	nFrmE;
		UINT	nFrmP;
		INT		keyCnt;
	};

	// Mesh header
	struct MrcMesh
	{
		INT		vertCnt;
		INT		vertOfs;

		INT		normCnt;
		INT		normOfs;

		INT		faceCnt;
		INT		faceOfs;
	};

	// Face header
	struct MrcFace
	{
		INT		vert[3];
		INT		norm[3];
	};

	//	Bone header
	struct MrcBone
	{
		INT		vertexCnt;

		INT		parentIdx;	//-1 if root
		INT		childCnt;
		MATA	inverseOrientationTM;

		INT		boneWeightsOfs;
		INT		boneKeysOfs;
	};

	//	weight data 
	struct MrcWgt
	{
		INT		vertIdx;
		FLOAT	weight;
	};

	//	Bone animation header 
	struct MrcKey
	{
		DWORD	t;
		VEC3	p;
		QUAT	q;
	};
		

	typedef std::vector<INT>		lsMrcChild;
	typedef std::vector<MrcWgt>		lsMrcWgt;
	typedef std::vector<MrcKey>		lsMrcKey;

	struct MrcLink 
	{
		MrcBone		Hdr;

		lsMrcChild	childsVect;
		lsMrcWgt	weightsVect;
		lsMrcKey	keysVect;

		MrcLink()
		{
			memset(&Hdr,0,sizeof(MrcBone));
			childsVect.clear();
			weightsVect.clear();
			keysVect.clear();		
		}
		~MrcLink()
		{
			childsVect.clear();
			weightsVect.clear();
			keysVect.clear();
		}

	};

// Application Code
protected:
	HWND	m_hWnd;
	TCHAR	m_sN[MAX_PATH*2];			// Export File Name
	TCHAR	m_sP[MAX_PATH*2];			// Export Path
	ULONG	sampleDelta;

public:
	void	setSampleDelta (ULONG delta) { sampleDelta=delta; }
	BOOL	Write(FILE*);
	void	DeleteThis(){ delete this; }


	BOOL	ExportBoneData(MrcLink *BD,INT boneCnt, INT keyCnt, FILE *out);
	BOOL	ExportMesh (INode* pNode, FILE *out);
	INT		ProcessBoneStruct(INode *pNode, INode *pRoot,INT parentIdx, MrcLink* BD);
	BOOL	ProcessBoneWeights (INode *pNode,INode *pRoot, MrcLink* BD);
	INT		ProcessBoneAnim (INode *pRoot, Interval range, ULONG sampleD, MrcLink* BD);

	BOOL	IsMesh(INode *pNode);
	BOOL	IsEditTriMesh(INode *pNode);
	BOOL	IsBone(INode *pNode);
	INT		CountBones(INode* pNode);

	// Get the number of direct child bones of a node
	INT		GetChildBoneCount(INode *pNode);

	// Get an index from a node pointer
	INT		GetBoneIndex(INode *pRoot, INode *pNodeTest);

	// Get bone pointer from an index
	INode*	GetBoneByIndex(INode *pRoot,INT index);
	BOOL	GetPhysiqueWeights(INode *pNode, INode *pRoot, Modifier *pMod, MrcLink *BD);


	void	MAXtoGL(Matrix3 &tm);
	void	MAXtoGL(Point3 &pnt);
	void	MAXtoGL(const Matrix3 &tm, FLOAT glTM[16]);
	void	EulerToQuat(const Point3 &euler, Quat &quat);


	INT		TicksToFPS(TimeValue t);
	#define TicksToMilliSec(ticks) (((FLOAT)(ticks)/(FLOAT)TIME_TICKSPERSEC) * 1000.f)
	#define MilliSecToTicks(msec)  (((FLOAT)msec / 1000.f) * TIME_TICKSPERSEC)

	BOOL	BuildIter (INode* pnode, INode** const Iterator, int& currIdx);
	Modifier* GetPhysiqueMod(INode *pNode);
	Modifier* GetSkinMod(INode *pNode);

	Matrix3	GetBoneTM(INode *pNode, TimeValue t);
	int		RecursiveGetBoneIndex(INode *pRoot, INode *pNodeTest, int &boneCount);
};


// dummy view used internally by WriteMesh
class NullView : public View
{
public:
	Point2 ViewToScreen(Point3 p)
	{ 
		return Point2(p.x,p.y); 
	}

	NullView()
	{
		worldToView.IdentityMatrix();
		screenW=800.0f; screenH = 600.0f;
	}

};



#endif
