// Implementation of the CLcxCam class.
//
////////////////////////////////////////////////////////////////////////////////

#if _MSC_VER >= 1200
#pragma warning(disable : 4996)
#endif

#include <windows.h>
#include <d3dx9.h>

#include "ILcxCam.h"
#include "LcxCam.h"


CLcxCam::CLcxCam()
{
	m_pDev	= NULL;
	
	strcpy(m_sN, "LC_DEFAULT_CAMERA");
	m_eType	= LC_CAM_NONE;
	
	D3DXMatrixIdentity(&m_mtViw);
	D3DXMatrixIdentity(&m_mtPrj);

	m_fYaw		= 0.f;
	m_fPitch	= 0.f;
	m_vcEye		= D3DXVECTOR3(10,1000,-2000);
	m_vcLook	= D3DXVECTOR3(0,0,0);
	m_vcUp		= D3DXVECTOR3(0,1,0);
	
	m_fFv		= D3DX_PI/4.f;
	m_fScnW		= 1024;
	m_fScnH		= 768;
	m_fAs		= m_fScnW/m_fScnH;
	m_fNr		= 1.f;
	m_fFr		= 8000.f;
	
	m_fSpeed	= 10.f;


	m_vcBasis	= D3DXVECTOR3( 0, 0, 0);
	m_fEpslnY	=  20.f;
	m_fGap		= 100.f;
}

CLcxCam::~CLcxCam()
{
	Destroy();
}



void CLcxCam::Destroy()
{
}


INT CLcxCam::Create(void* p1, void* p2, void* p3, void* p4)
{
	return -1;
}


INT CLcxCam::FrameMove()
{
	return -1;
}

INT CLcxCam::Query(char*, void*)
{
	return -1;
}

void CLcxCam::Update()
{
	INT		i=0;
	D3DXVECTOR3	vcH[8];


	D3DXMatrixPerspectiveFovLH(&m_mtPrj, m_fFv, m_fAs, m_fNr, m_fFr);
	D3DXMatrixLookAtLH(&m_mtViw, &m_vcEye, &m_vcLook, &m_vcUp);
	D3DXMatrixInverse(&m_mtViwI, 0, &m_mtViw);

	m_mtVP	= m_mtViw * m_mtPrj;
	D3DXMatrixInverse(&m_mtVPI, 0, &m_mtVP);
	
	m_mtBill = m_mtViwI;
	m_mtBill._41 = m_mtBill._42 = m_mtBill._43 = 0;								// Bill board Matrix
	
	m_vcX.x = m_mtViw._11;
	m_vcX.y = m_mtViw._21;
	m_vcX.z = m_mtViw._31;
	
	m_vcY.x = m_mtViw._12;
	m_vcY.y = m_mtViw._22;
	m_vcY.z = m_mtViw._32;
	
	m_vcZ.x = m_mtViw._13;
	m_vcZ.y = m_mtViw._23;
	m_vcZ.z = m_mtViw._33;


//											Far
//							7------------6
//						   /|           /|
//						  / |  T(top)  / |
//						 /  |         /  |
//				L(left)	3---+--------2   |	R(right)
//						|   |        |   |
//						|   4--------+---5
//						|  /     .   |  /
//					 .	| / B(bottom)| /
//						|/   .       |/
//					.	0------------1		Near
//					   .  .    .
//				  .	 ..    .
//				   .  .
//	 Camera Pos ��


	
	vcH[0] = D3DXVECTOR3(-1,-1, 0);
	vcH[1] = D3DXVECTOR3( 1,-1, 0);
	vcH[2] = D3DXVECTOR3( 1, 1, 0);
	vcH[3] = D3DXVECTOR3(-1, 1, 0);

	vcH[4] = D3DXVECTOR3(-1,-1, 1);
	vcH[5] = D3DXVECTOR3( 1,-1, 1);
	vcH[6] = D3DXVECTOR3( 1, 1, 1);
	vcH[7] = D3DXVECTOR3(-1, 1, 1);

	for(i=0; i<8; ++i)
		D3DXVec3TransformCoord(&m_vcFrsm[i], &vcH[i], &m_mtVPI);


	// Frustum Process
	D3DXVECTOR3 vcN = -m_vcZ;			// Near
	D3DXVECTOR3 vcF = m_vcZ;			// Far
	
	D3DXVECTOR3 vcP_n= m_vcEye + vcF * m_fNr;	// Camera Position + Near Value * Z direction
	D3DXVECTOR3 vcP_f= m_vcEye + vcF * m_fFr;	// Camera Position + Far Value * Z direction


	D3DXPlaneFromPointNormal((D3DXPLANE*)&m_plFrsm[0], &vcP_n, &vcN);			// Near Plane
	D3DXPlaneFromPointNormal((D3DXPLANE*)&m_plFrsm[1], &vcP_f, &vcF);			// Far Plane

	D3DXPlaneFromPoints((D3DXPLANE*)&m_plFrsm[2], &m_vcEye, &m_vcFrsm[4], &m_vcFrsm[7]);	// Left
	D3DXPlaneFromPoints((D3DXPLANE*)&m_plFrsm[3], &m_vcEye, &m_vcFrsm[6], &m_vcFrsm[5]);	// Right
	D3DXPlaneFromPoints((D3DXPLANE*)&m_plFrsm[4], &m_vcEye, &m_vcFrsm[7], &m_vcFrsm[6]);	// Top
	D3DXPlaneFromPoints((D3DXPLANE*)&m_plFrsm[5], &m_vcEye, &m_vcFrsm[5], &m_vcFrsm[4]);	// Bottom

}




D3DXVECTOR3 CLcxCam::GetDirectionOfMouse(FLOAT fMouseX, FLOAT fMouseY)
{
	// Get the pick ray from the mouse position
	FLOAT w = m_mtPrj._11;
	FLOAT h = m_mtPrj._22;
	
	D3DXVECTOR3	vcScnP;
	D3DXVECTOR3	vcRayDir;
	
	vcScnP.x =  ( 2.f * fMouseX/ m_fScnW - 1.f ) / w;
	vcScnP.y = -( 2.f * fMouseY/ m_fScnH - 1.f ) / h;
	vcScnP.z =  1.f;
	
	// Transform the screen space pick ray into 3D space
	// Ray Direction
	vcRayDir.x  = D3DXVec3Dot(&vcScnP, &D3DXVECTOR3(m_mtViwI._11, m_mtViwI._21, m_mtViwI._31));
	vcRayDir.y  = D3DXVec3Dot(&vcScnP, &D3DXVECTOR3(m_mtViwI._12, m_mtViwI._22, m_mtViwI._32));
	vcRayDir.z  = D3DXVec3Dot(&vcScnP, &D3DXVECTOR3(m_mtViwI._13, m_mtViwI._23, m_mtViwI._33));
	
	return vcRayDir;
}


void CLcxCam::SetTransformViw()
{
	LPDIRECT3DDEVICE9 p = (LPDIRECT3DDEVICE9)m_pDev;
	p->SetTransform(D3DTS_VIEW, &m_mtViw);
}

void CLcxCam::SetTransformPrj()
{
	LPDIRECT3DDEVICE9 p = (LPDIRECT3DDEVICE9)m_pDev;
	p->SetTransform(D3DTS_PROJECTION, &m_mtPrj);	
}


void CLcxCam::SetParamView(D3DXVECTOR3* vcEye, D3DXVECTOR3* vcLook, D3DXVECTOR3* vcUp)
{
	m_vcEye		= *vcEye;
	m_vcLook	= *vcLook;
	m_vcUp		= *vcUp;
}

void CLcxCam::SetParamProj(FLOAT fFov, FLOAT fScnW, FLOAT fScnH, FLOAT fNear, FLOAT fFar)
{
	m_fFv	= fFov;
	m_fScnW	= fScnW;
	m_fScnH	= fScnH;
	m_fAs	= m_fScnW/m_fScnH;
	m_fNr	= fNear;
	m_fFr	= fFar;
}



void CLcxCam::SetName(char* sN)
{
	strcpy(m_sN, sN);
}
char* CLcxCam::GetName()
{
	return m_sN;
}

void	CLcxCam::SetType(INT eType)
{
	m_eType = ELcxCam(eType);
}

CLcxCam::ELcxCam	CLcxCam::GetType()
{
	return m_eType;
}

const D3DXMATRIX* const CLcxCam::GetMatrixViw() const
{
	return &m_mtViw;
}

const D3DXMATRIX* const CLcxCam::GetMatrixPrj() const
{
	return &m_mtPrj;
}

const D3DXMATRIX* const CLcxCam::GetMatrixViwI() const
{
	return &m_mtViwI;
}

const D3DXMATRIX* const CLcxCam::GetMatrixBill() const
{
	return &m_mtBill;
}

const D3DXMATRIX* const CLcxCam::GetMatrixViwPrj() const
{
	return &m_mtVP;
}

const D3DXMATRIX* const CLcxCam::GetMatrixViwPrjI() const
{
	return &m_mtVPI;
}


const D3DXVECTOR3* const CLcxCam::GetEye() const
{
	return &m_vcEye;
}

const D3DXVECTOR3* const CLcxCam::GetLook() const
{
	return &m_vcLook;
}

const D3DXVECTOR3* const CLcxCam::GetUp() const
{
	return &m_vcUp;
}

void CLcxCam::SetEye(D3DXVECTOR3* v)
{
	m_vcEye = *v;
}

void CLcxCam::SetLook(D3DXVECTOR3* v)
{
	m_vcLook = *v;
}

void CLcxCam::SetUp(D3DXVECTOR3* v)
{
	m_vcUp = *v;
}

FLOAT CLcxCam::GetYaw()
{
	return m_fYaw;
}

FLOAT CLcxCam::GetPitch()
{
	return m_fPitch;
}

void CLcxCam::SetFov(FLOAT fFov)
{
	m_fFv	= fFov;
}

void CLcxCam::SetScnW(FLOAT fScnW)
{
	m_fScnW	= fScnW;		m_fAs	= m_fScnW/m_fScnH;
}

void CLcxCam::SetScnH(FLOAT fScnH)
{
	m_fScnH	= fScnH;		m_fAs	= m_fScnW/m_fScnH;
}

void CLcxCam::SetNear(FLOAT fNear)
{
	m_fNr	= fNear;
}

void CLcxCam::SetFar(FLOAT fFar)
{
	m_fFr	= fFar;
}

FLOAT CLcxCam::GetFov()
{
	return m_fFv;
}

FLOAT CLcxCam::GetScnW()
{
	return m_fScnW;
}

FLOAT CLcxCam::GetScnH()
{
	return m_fScnH;
}

FLOAT CLcxCam::GetAspect()
{
	return m_fAs;
}

FLOAT CLcxCam::GetNear()
{
	return m_fNr;
}

FLOAT CLcxCam::GetFar()
{
	return m_fFr;
}

const D3DXVECTOR3* const CLcxCam::GetAxisX()
{
	return &m_vcX;
}

const D3DXVECTOR3* const CLcxCam::GetAxisY()
{
	return &m_vcY;
}

const D3DXVECTOR3* const CLcxCam::GetAxisZ()
{
	return &m_vcZ;
}


const D3DXPLANE* const CLcxCam::GetFrustumPlanes()
{
	return &m_plFrsm[0];
}


const D3DXVECTOR3* const CLcxCam::GetFrustumPoints()
{
	return &m_vcFrsm[0];
}


// Individual Camera
void CLcxCam::SetMatrixViw(D3DXMATRIX* mtViw)
{
	m_mtViw = *mtViw;
}
void CLcxCam::SetMatrixPrj(D3DXMATRIX* mtPrj)
{
	m_mtPrj = *mtPrj;
}


void CLcxCam::MoveSideward(FLOAT)
{
}

void CLcxCam::MoveForward(FLOAT, FLOAT)
{
}

void CLcxCam::RotateYaw(FLOAT, FLOAT)
{
}

void CLcxCam::RotatePitch(FLOAT, FLOAT)
{
}

void CLcxCam::Zoom(FLOAT)
{
}



void CLcxCam::SetBasis(D3DXVECTOR3* vcP)
{
	if(LC_CAM_THIRD != m_eType)
		return;

	D3DXVECTOR3	vcZ = m_vcEye - m_vcLook;

	D3DXVec3Normalize(&vcZ, &vcZ);

	vcZ	*= m_fGap;

	m_vcBasis	= *vcP;
	m_vcLook	= m_vcBasis + D3DXVECTOR3(0, m_fEpslnY, 0);
	m_vcEye		= m_vcLook + vcZ;
}

const D3DXVECTOR3* const CLcxCam::GetBasis() const
{
	return &m_vcBasis;
}

void CLcxCam::SetEpsilonY(FLOAT fY)
{
	m_fEpslnY =fY;
}

FLOAT CLcxCam::GetEpsilonY()
{
	return m_fEpslnY;
}

void CLcxCam::SetGap(FLOAT fG)
{
	m_fGap =fG;
}

FLOAT CLcxCam::GetGap()
{
	return m_fGap;
}