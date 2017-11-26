// Interface for the IMdBase class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _ILcxDXFile_H_
#define _ILcxDXFile_H_


#pragma warning(disable : 4786)
#include <vector>


#ifndef interface
#define interface struct
#endif


#ifndef LC_CLASS_DESTROYER
#define LC_CLASS_DESTROYER( CLASS_NAME )	\
virtual ~CLASS_NAME(){}
#endif

// Model Type
#ifndef LCX_MDL_NONE
#define LCX_MDL_NONE	0x00000000
#define LCX_MDL_BILL	0x00000001
#define LCX_MDL_SOLID	0x00000002
#define LCX_MDL_ACM		0x00000004
#define LCX_MDL_XFILE	0x00010000
#endif

interface ILcxDXFile
{
	struct XTrack
	{
		FLOAT	fTrackPosition;
		FLOAT	fTrackSpeed;
	};

	LC_CLASS_DESTROYER(	ILcxDXFile	);

	virtual	INT		Create(void* =0, void* =0, void* =0, void* =0)=0;
	virtual	void	Destroy()=0;
	virtual	INT		FrameMove()=0;
	virtual	void	Render()=0;

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


typedef std::vector<ILcxDXFile* >		lsLcxDXFile;
typedef lsLcxDXFile::iterator			itLcxDXFile;


INT LcxDXFile_Create(char* sCmd		// "Origin", "Instance": "Origin"으로 먼저 생성하고 Instance는 Origin에서 애니메이션 컨트롤러를 복사 해서 사용.
				 , ILcxDXFile** pData
				 , void* p1			// "Origin"일 경우 LPDIRECT3DDEVICE9. "Instance"일 경우 "Origin"으로 생성한 ILcxXFile 데이터 포인터
				 , void* p2			// "Origin"일 경우 Source X file Name. "Instance"일 경우 XmshInst 데이터.
				 , void* p3=NULL	// "Origin"일 경우 ID3DXEffect포인터. "Instance"일 경우 사용안함
				 , void* p4=NULL	// No Use
				 );


//		ex)
//		LcxDXFile_Create("Origin", &m_pMdlOrg, pd3dDevice, strMeshPath, pEffect);
//		LcxDXFile_Create("Instance", &pInstance, m_pMdlOrg, &Env);



#ifdef _DEBUG
	#pragma comment(lib, "LcxDXFile_.lib")
#else
	#pragma comment(lib, "LcxDXFile.lib")
#endif


#endif


