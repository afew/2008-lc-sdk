// Interface for the ILcxCam class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _ILcxCam_H_
#define _ILcxCam_H_


#ifndef interface
#define interface struct
#endif


#ifndef LC_CLASS_DESTROYER
#define LC_CLASS_DESTROYER( CLASS_NAME )	\
virtual ~CLASS_NAME(){}
#endif



struct LcxCam
{
	FLOAT	fScnW;			// Screen Width
	FLOAT	fScnH;			// Screen Height
	FLOAT	fFov;			// Field of View
	FLOAT	fNear;			// Near value
	FLOAT	fFar;			// Far value

	FLOAT	vEyeX;			// Camera Initial Position X
	FLOAT	vEyeY;			// Camera Initial Position Y
	FLOAT	vEyeZ;			// Camera Initial Position Z

	FLOAT	vLookX;			// Camera Look Position X
	FLOAT	vLookY;			// Camera Look Position Y
	FLOAT	vLookZ;			// Camera Look Position Z

	FLOAT	vUpX;			// Camera Up Vector X
	FLOAT	vUpY;			// Camera Up Vector Y
	FLOAT	vUpZ;			// Camera Up Vector Z
};



////////////////////////////////////////////////////////////////////////////////
// Single Camera

interface ILcxCam
{
	LC_CLASS_DESTROYER(	ILcxCam	);
	
	enum ELcxCam
	{
		LC_CAM_NONE	=0,		//
		LC_CAM_FIRST,		// 1 ÀÎÄª
		LC_CAM_THIRD,		// 3 ÀÎÄª
		LC_CAM_ALL,			// 1~3 ÀÎÄª
	};
	

	virtual INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0)=0;
	virtual void	Destroy()=0;
	virtual INT		FrameMove()=0;
	virtual INT		Query(char* sCmd, void* pData)=0;


	virtual void	SetName(char* sN)=0;
	virtual char*	GetName()=0;
	
	virtual void	SetType(INT	eType)=0;
	virtual ELcxCam	GetType()=0;
	
	virtual const	D3DXMATRIX*  const GetMatrixViw()	const=0;
	virtual const	D3DXMATRIX*  const GetMatrixPrj()	const=0;
	virtual const	D3DXMATRIX*  const GetMatrixViwI()	const=0;
	virtual const	D3DXMATRIX*  const GetMatrixBill()	const=0;
	virtual const	D3DXMATRIX*  const GetMatrixViwPrj()const=0;
	virtual const	D3DXMATRIX*  const GetMatrixViwPrjI()const=0;
	
	virtual const	D3DXVECTOR3* const GetEye()	const=0;
	virtual const	D3DXVECTOR3* const GetLook()const=0;
	virtual const	D3DXVECTOR3* const GetUp()	const=0;

	virtual void	SetEye	(D3DXVECTOR3*)=0;
	virtual void	SetLook	(D3DXVECTOR3*)=0;
	virtual void	SetUp	(D3DXVECTOR3*)=0;
	
	virtual FLOAT	GetYaw	()=0;
	virtual FLOAT	GetPitch()=0;
	
	virtual void	SetFov	(FLOAT)=0;
	virtual void	SetScnW	(FLOAT)=0;
	virtual void	SetScnH	(FLOAT)=0;
	virtual void	SetNear	(FLOAT)=0;
	virtual void	SetFar	(FLOAT)=0;

	virtual FLOAT	GetFov	()=0;
	virtual FLOAT	GetScnW	()=0;
	virtual FLOAT	GetScnH	()=0;
	virtual FLOAT	GetAspect()=0;
	virtual FLOAT	GetNear	()=0;
	virtual FLOAT	GetFar	()=0;

	virtual const	D3DXVECTOR3* const GetAxisX()=0;
	virtual const	D3DXVECTOR3* const GetAxisY()=0;
	virtual const	D3DXVECTOR3* const GetAxisZ()=0;

	virtual const	D3DXPLANE*   const GetFrustumPlanes()=0;
	virtual const	D3DXVECTOR3* const GetFrustumPoints()=0;
		
	virtual void	SetParamView(D3DXVECTOR3* Eye, D3DXVECTOR3* Look, D3DXVECTOR3* Up)=0;
	virtual void	SetParamProj(FLOAT fFov, FLOAT fScnW, FLOAT fScnH, FLOAT fNear, FLOAT fFar)=0;
	
	virtual void	SetTransformViw()=0;
	virtual void	SetTransformPrj()=0;

	// Pick Ray direction
	virtual D3DXVECTOR3	GetDirectionOfMouse(FLOAT fMouseX, FLOAT fMouseY)=0;

	// Individual Camera
	virtual void	SetMatrixViw(D3DXMATRIX*)=0;
	virtual void	SetMatrixPrj(D3DXMATRIX*)=0;

	virtual void	MoveSideward(FLOAT fSpeed)=0;
	virtual void	MoveForward	(FLOAT fSpeed, FLOAT fY=1)=0;
	virtual void	RotateYaw	(FLOAT fDelta, FLOAT fSpeed)=0;
	virtual void	RotatePitch	(FLOAT fDelta, FLOAT fSpeed)=0;
	virtual	void	Zoom(FLOAT)	= 0;


	// For Third Person Camera
	virtual void	SetBasis(D3DXVECTOR3*) =0;
	virtual const	D3DXVECTOR3* const GetBasis() const=0;

	virtual void	SetEpsilonY(FLOAT)	=0;
	virtual FLOAT	GetEpsilonY()		=0;

	virtual void	SetGap(FLOAT)	=0;
	virtual FLOAT	GetGap()		=0;
};




INT LcxCam_Create(char* sCmd
				, ILcxCam** pData
				, void* p1=0		// Camera name
				, void* p2=0		// Camera Initial Value
				, void* p3=0		// LPDIRECT3DDEVICE9
				, void* p4=0		// No Use
				, void* p5=0		// No Use
				, void* p6=0		// No Use
				);







////////////////////////////////////////////////////////////////////////////////
// Multi Camera

#define LC_MUTI_CAMERA_MAX	16


interface ILcxCamM
{
	LC_CLASS_DESTROYER(	ILcxCamM	);
	
	virtual INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0)=0;
	virtual void	Destroy()=0;
	virtual INT		FrameMove()=0;
	virtual INT		Query(char* sCmd, void* pData)=0;


	virtual void	SetName(INT nIdx, char* sN)=0;
	virtual char*	GetName(INT nIdx)=0;
	
	virtual void	SetType(INT nIdx, INT	eType)=0;
	virtual INT		GetType(INT nIdx)=0;
	
	virtual const	D3DXMATRIX*  const GetMatrixViw		(INT nIdx) const=0;
	virtual const	D3DXMATRIX*  const GetMatrixPrj		(INT nIdx) const=0;
	virtual const	D3DXMATRIX*  const GetMatrixViwI	(INT nIdx) const=0;
	virtual const	D3DXMATRIX*  const GetMatrixBill	(INT nIdx) const=0;
	virtual const	D3DXMATRIX*  const GetMatrixViwPrj	(INT nIdx) const=0;
	virtual const	D3DXMATRIX*  const GetMatrixViwPrjI	(INT nIdx) const=0;
	
	virtual const	D3DXVECTOR3* const GetEye (INT nIdx) const=0;
	virtual const	D3DXVECTOR3* const GetLook(INT nIdx) const=0;
	virtual const	D3DXVECTOR3* const GetUp  (INT nIdx) const=0;

	virtual void	SetEye	(INT nIdx, D3DXVECTOR3*)=0;
	virtual void	SetLook	(INT nIdx, D3DXVECTOR3*)=0;
	virtual void	SetUp	(INT nIdx, D3DXVECTOR3*)=0;
	
	virtual FLOAT	GetYaw	(INT nIdx)=0;
	virtual FLOAT	GetPitch(INT nIdx)=0;
	
	virtual void	SetFov	(INT nIdx, FLOAT)=0;
	virtual void	SetScnW	(INT nIdx, FLOAT)=0;
	virtual void	SetScnH	(INT nIdx, FLOAT)=0;
	virtual void	SetNear	(INT nIdx, FLOAT)=0;
	virtual void	SetFar	(INT nIdx, FLOAT)=0;

	virtual FLOAT	GetFov	 (INT nIdx)=0;
	virtual FLOAT	GetScnW	 (INT nIdx)=0;
	virtual FLOAT	GetScnH	 (INT nIdx)=0;
	virtual FLOAT	GetAspect(INT nIdx)=0;
	virtual FLOAT	GetNear	 (INT nIdx)=0;
	virtual FLOAT	GetFar	 (INT nIdx)=0;

	virtual const	D3DXVECTOR3* const GetAxisX(INT nIdx)=0;
	virtual const	D3DXVECTOR3* const GetAxisY(INT nIdx)=0;
	virtual const	D3DXVECTOR3* const GetAxisZ(INT nIdx)=0;

	virtual const	D3DXPLANE*   const GetFrustumPlanes(INT nIdx)=0;
	virtual const	D3DXVECTOR3* const GetFrustumPoints(INT nIdx)=0;
		
	virtual void	SetParamView(INT nIdx, D3DXVECTOR3* Eye, D3DXVECTOR3* Look, D3DXVECTOR3* Up)=0;
	virtual void	SetParamProj(INT nIdx, FLOAT fFov, FLOAT fScnW, FLOAT fScnH, FLOAT fNear, FLOAT fFar)=0;
	
	virtual void	SetTransformViw(INT nIdx)=0;
	virtual void	SetTransformPrj(INT nIdx)=0;

	virtual D3DXVECTOR3	GetDirectionOfMouse(INT nIdx, FLOAT fMouseX, FLOAT fMouseY)=0;

	virtual void	SetMatrixViw(INT nIdx, D3DXMATRIX*)=0;
	virtual void	SetMatrixPrj(INT nIdx, D3DXMATRIX*)=0;

	virtual void	MoveSideward(INT nIdx, FLOAT fSpeed)=0;
	virtual void	MoveForward	(INT nIdx, FLOAT fSpeed, FLOAT fY=1)=0;
	virtual void	RotateYaw	(INT nIdx, FLOAT fDelta, FLOAT fSpeed)=0;
	virtual void	RotatePitch	(INT nIdx, FLOAT fDelta, FLOAT fSpeed)=0;
	virtual void	Zoom		(INT nIdx, FLOAT fDelta)=0;


	// For Third Person Camera
	virtual void	SetBasis(INT nIdx, D3DXVECTOR3*)=0;
	virtual const	D3DXVECTOR3* const GetBasis(INT nIdx) const=0;

	virtual void	SetEpsilonY(INT nIdx, FLOAT)=0;
	virtual FLOAT	GetEpsilonY(INT nIdx)		=0;

	virtual void	SetGap(INT nIdx, FLOAT)	=0;
	virtual FLOAT	GetGap(INT nIdx)		=0;

	// Camera Pointer
	virtual	ILcxCam*	GetCameraPointer(INT nIdx) =0;
};




INT LcxCam_CreateMuti(char* sCmd
				, ILcxCamM** pData
				, void* p1			// LPDIRECT3DDEVICE9
				, void* p2=0		// Camera Number. NULLÀÌ¸é LC_MUTI_CAMERA_MAX¸¸Å­ ¸¸µê
				, void* p3=0		// No Use
				, void* p4=0		// No Use
				, void* p5=0		// No Use
				, void* p6=0		// No Use
				);



#ifdef _DEBUG
	#pragma comment(lib, "LcxCam_.lib")
#else
	#pragma comment(lib, "LcxCam.lib")
#endif

	
#endif



