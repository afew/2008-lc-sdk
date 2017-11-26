// Implementation of the CLcxCamM class.
//
////////////////////////////////////////////////////////////////////////////////

#if _MSC_VER >= 1200
#pragma warning(disable : 4996)
#endif

#include <windows.h>
#include <stdio.h>
#include <d3dx9.h>

#include "ILcxCam.h"
#include "LcxCam.h"
#include "LcxCamA.h"

#include "LcxCamM.h"



CLcxCamM::CLcxCamM()
{
	m_pDev	= NULL;

	for(int i=0; i<LC_MUTI_CAMERA_MAX; ++i)
		m_pCam[i]	= NULL;
}


CLcxCamM::~CLcxCamM()
{
	Destroy();
}



void CLcxCamM::Destroy()
{
	for(int i=0; i<LC_MUTI_CAMERA_MAX; ++i)
	{
		delete m_pCam[i];
		m_pCam[i]	= NULL;
	}
}


INT CLcxCamM::Create(void* p1, void* p2, void* p3, void* p4)
{
	m_pDev	= p1;
	
	INT		i =0;
	char	sName[64];

	for(i=0; i<LC_MUTI_CAMERA_MAX; ++i)
	{
		sprintf(sName, "LcxCamera: %d", i);

		m_pCam[i] = new CLcxCamA;
	
		if(FAILED(m_pCam[i]->Create(sName, NULL, p1)))
			return -1;
	}

	return 0;
}


INT CLcxCamM::FrameMove()
{
	int i=0;

	for(i=0; i<LC_MUTI_CAMERA_MAX; ++i)
		m_pCam[i]->FrameMove();

	return 0;
}



INT  CLcxCamM::Query(char* sCmd, void* pData)
{
	return -1;
}



void CLcxCamM::SetName(INT nIdx, char* sN)
{
}


char* CLcxCamM::GetName(INT nIdx)
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return NULL;

	return m_pCam[nIdx]->GetName();
}



void CLcxCamM::SetType(INT nIdx, INT eType)
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return;
	
	m_pCam[nIdx]->SetType(eType);
}


INT CLcxCamM::GetType(INT nIdx)
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return -1;
	
	return m_pCam[nIdx]->GetType();
}



const D3DXMATRIX*  const CLcxCamM::GetMatrixViw(INT nIdx) const
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return NULL;
	
	return m_pCam[nIdx]->GetMatrixViw();
}


const D3DXMATRIX*  const CLcxCamM::GetMatrixPrj(INT nIdx) const
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return NULL;
	
	return m_pCam[nIdx]->GetMatrixPrj();
}


const D3DXMATRIX*  const CLcxCamM::GetMatrixViwI(INT nIdx) const
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return NULL;
	
	return m_pCam[nIdx]->GetMatrixViwI();
}


const D3DXMATRIX*  const CLcxCamM::GetMatrixBill(INT nIdx) const
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return NULL;
	
	return m_pCam[nIdx]->GetMatrixBill();
}


const D3DXMATRIX*  const CLcxCamM::GetMatrixViwPrj(INT nIdx) const
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return NULL;
	
	return m_pCam[nIdx]->GetMatrixViwPrj();
}


const D3DXMATRIX*  const CLcxCamM::GetMatrixViwPrjI(INT nIdx) const
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return NULL;
	
	return m_pCam[nIdx]->GetMatrixViwPrjI();
}



const D3DXVECTOR3* const CLcxCamM::GetEye(INT nIdx) const
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return NULL;
	
	return m_pCam[nIdx]->GetEye();
}


const D3DXVECTOR3* const CLcxCamM::GetLook(INT nIdx) const
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return NULL;
	
	return m_pCam[nIdx]->GetLook();
}


const D3DXVECTOR3* const CLcxCamM::GetUp(INT nIdx) const
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return NULL;
	
	return m_pCam[nIdx]->GetUp();
}



void CLcxCamM::SetEye(INT nIdx, D3DXVECTOR3* pVal)
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return;
	
	m_pCam[nIdx]->SetEye(pVal);
}


void CLcxCamM::SetLook(INT nIdx, D3DXVECTOR3* pVal)
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return;
	
	m_pCam[nIdx]->SetLook(pVal);
}


void CLcxCamM::SetUp(INT nIdx, D3DXVECTOR3* pVal)
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return;
	
	m_pCam[nIdx]->SetUp(pVal);
}



FLOAT CLcxCamM::GetYaw(INT nIdx)
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return FLT_MAX;
	
	return m_pCam[nIdx]->GetYaw();
}


FLOAT CLcxCamM::GetPitch(INT nIdx)
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return FLT_MAX;
	
	return m_pCam[nIdx]->GetPitch();
}



void CLcxCamM::SetFov(INT nIdx, FLOAT pVal)
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return;
	
	m_pCam[nIdx]->SetFov(pVal);
}


void CLcxCamM::SetScnW(INT nIdx, FLOAT pVal)
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return;
	
	m_pCam[nIdx]->SetScnW(pVal);
}


void CLcxCamM::SetScnH(INT nIdx, FLOAT pVal)
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return;
	
	m_pCam[nIdx]->SetScnH(pVal);
}


void CLcxCamM::SetNear(INT nIdx, FLOAT pVal)
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return;
	
	m_pCam[nIdx]->SetNear(pVal);
}


void CLcxCamM::SetFar(INT nIdx, FLOAT pVal)
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return;
	
	m_pCam[nIdx]->SetFar(pVal);
}



FLOAT CLcxCamM::GetFov(INT nIdx)
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return FLT_MAX;
	
	return m_pCam[nIdx]->GetFov();
}


FLOAT CLcxCamM::GetScnW(INT nIdx)
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return FLT_MAX;
	
	return m_pCam[nIdx]->GetScnW();
}


FLOAT CLcxCamM::GetScnH(INT nIdx)
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return FLT_MAX;
	
	return m_pCam[nIdx]->GetScnH();
}


FLOAT CLcxCamM::GetAspect(INT nIdx)
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return FLT_MAX;
	
	return m_pCam[nIdx]->GetAspect();
}


FLOAT CLcxCamM::GetNear(INT nIdx)
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return FLT_MAX;
	
	return m_pCam[nIdx]->GetNear();
}


FLOAT CLcxCamM::GetFar(INT nIdx)
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return FLT_MAX;
	
	return m_pCam[nIdx]->GetFar();
}



const D3DXVECTOR3* const CLcxCamM::GetAxisX(INT nIdx)
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return NULL;
	
	return m_pCam[nIdx]->GetAxisX();
}


const D3DXVECTOR3* const CLcxCamM::GetAxisY(INT nIdx)
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return NULL;
	
	return m_pCam[nIdx]->GetAxisY();
}


const D3DXVECTOR3* const CLcxCamM::GetAxisZ(INT nIdx)
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return NULL;
	
	return m_pCam[nIdx]->GetAxisZ();
}



const D3DXPLANE*   const CLcxCamM::GetFrustumPlanes(INT nIdx)
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return NULL;
	
	return m_pCam[nIdx]->GetFrustumPlanes();
}


const D3DXVECTOR3* const CLcxCamM::GetFrustumPoints(INT nIdx)
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return NULL;
	
	return m_pCam[nIdx]->GetFrustumPoints();
}



void CLcxCamM::SetParamView(INT nIdx, D3DXVECTOR3* Eye, D3DXVECTOR3* Look, D3DXVECTOR3* Up)
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return;
	
	m_pCam[nIdx]->SetParamView(Eye, Look, Up);
}


void CLcxCamM::SetParamProj(INT nIdx, FLOAT fFov, FLOAT fScnW, FLOAT fScnH, FLOAT fNear, FLOAT fFar)
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return;
	
	m_pCam[nIdx]->SetParamProj(fFov, fScnW, fScnH, fNear, fFar);
}



void CLcxCamM::SetTransformViw(INT nIdx)
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return;
	
	m_pCam[nIdx]->SetTransformViw();
}


void CLcxCamM::SetTransformPrj(INT nIdx)
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return;
	
	m_pCam[nIdx]->SetTransformPrj();
}



D3DXVECTOR3 CLcxCamM::GetDirectionOfMouse(INT nIdx, FLOAT fMouseX, FLOAT fMouseY)
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
	
	return m_pCam[nIdx]->GetDirectionOfMouse(fMouseX, fMouseY);
}



void CLcxCamM::SetMatrixViw(INT nIdx, D3DXMATRIX* pVal)
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return;
	
	m_pCam[nIdx]->SetMatrixViw(pVal);
}


void CLcxCamM::SetMatrixPrj(INT nIdx, D3DXMATRIX* pVal)
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return;
	
	m_pCam[nIdx]->SetMatrixPrj(pVal);
}



void CLcxCamM::MoveSideward(INT nIdx, FLOAT fSpeed)
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return;
	
	m_pCam[nIdx]->MoveSideward(fSpeed);
}


void CLcxCamM::MoveForward(INT nIdx, FLOAT fSpeed, FLOAT fY)
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return;
	
	m_pCam[nIdx]->MoveForward(fSpeed, fY);
}


void CLcxCamM::RotateYaw(INT nIdx, FLOAT fDelta, FLOAT fSpeed)
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return;
	
	m_pCam[nIdx]->RotateYaw(fDelta, fSpeed);
}


void CLcxCamM::RotatePitch(INT nIdx, FLOAT fDelta, FLOAT fSpeed)
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return;
	
	m_pCam[nIdx]->RotatePitch(fDelta, fSpeed);
}


void CLcxCamM::Zoom(INT nIdx, FLOAT fDelta)
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return;
	
	m_pCam[nIdx]->Zoom(fDelta);
}



// For Third Person Camera
void CLcxCamM::SetBasis(INT nIdx, D3DXVECTOR3* vcP)
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return;
	
	m_pCam[nIdx]->SetBasis(vcP);
}

const D3DXVECTOR3* const CLcxCamM::GetBasis(INT nIdx) const
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return NULL;
	
	return m_pCam[nIdx]->GetBasis();
}

void CLcxCamM::SetEpsilonY(INT nIdx, FLOAT fY)
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return;
	
	m_pCam[nIdx]->SetEpsilonY(fY);
}

FLOAT CLcxCamM::GetEpsilonY(INT nIdx)
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return FLT_MAX;
	
	return m_pCam[nIdx]->GetEpsilonY();
}

void CLcxCamM::SetGap(INT nIdx, FLOAT fG)
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return;
	
	m_pCam[nIdx]->SetGap(fG);
}

FLOAT CLcxCamM::GetGap(INT nIdx)
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return FLT_MAX;
	
	return m_pCam[nIdx]->GetGap();
}



ILcxCam* CLcxCamM::GetCameraPointer(INT nIdx)
{
	if(nIdx>=LC_MUTI_CAMERA_MAX)
		return NULL;
	
	return m_pCam[nIdx];
}



INT LcxCam_CreateMuti(char* sCmd
				, ILcxCamM** pData
				, void* p1			// LPDIRECT3DDEVICE9
				, void* p2			// Camera Number. NULL이면 LC_MUTI_CAMERA_MAX만큼 만듦
				, void* p3			// No Use
				, void* p4			// No Use
				, void* p5			// No Use
				, void* p6			// No Use
				)
{
	(*pData) = NULL;

	CLcxCamM* pObj = new CLcxCamM;

	if(FAILED(pObj->Create(p1, p2, p3, p4)))
	{
		delete pObj;
		return -1;
	}

	(*pData) = pObj;
	return 0;
}