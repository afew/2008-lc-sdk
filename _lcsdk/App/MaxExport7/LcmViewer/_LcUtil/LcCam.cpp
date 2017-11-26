// Implementation of the CLcCam class.
//
////////////////////////////////////////////////////////////////////////////////


#include "../_StdAfx.h"


CLcCam::CLcCam()
{
	m_pDev	= NULL;
}

CLcCam::~CLcCam()
{

}


INT CLcCam::Create(LPDIRECT3DDEVICE9 pDev)
{
	m_pDev		= pDev;

	LPDIRECT3DSURFACE9		pSfc;	// BackBuffer
	D3DSURFACE_DESC			dsc	;	// Surface desc of the backbuffer

	if(FAILED(m_pDev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pSfc)))
			return-1;
	
	pSfc->GetDesc( &dsc );
	pSfc->Release();
		
	m_fYaw		= 0.f;
	m_fPitch	= 0.f;
	m_vcEye		= VEC3(150, 10, 10);
	m_vcLook	= VEC3(  0, 10, 10);

	m_vcEye		= VEC3( 0, 10, -50);
	m_vcLook	= VEC3( 0, 10,   0);
	
	m_vcUp		= VEC3(0,1,0);

	m_fFv		= D3DX_PI/4.f;
	m_fAs		= FLOAT(dsc.Width)/FLOAT(dsc.Height);
	m_fNr		= 1.f;
	m_fFr		= 5000.f;

	D3DXMatrixPerspectiveFovLH(&m_mtPrj,m_fFv, m_fAs, m_fNr, m_fFr);
	D3DXMatrixLookAtLH(&m_mtViw, &m_vcEye, &m_vcLook, &m_vcUp);

	return 1;
}


INT CLcCam::FrameMove()
{
	D3DXMatrixInverse(&m_mtViwI, 0, &m_mtViw);
	
	m_mtBill = m_mtViwI;
	m_mtBill._41 = m_mtBill._42 = m_mtBill._43 = 0;								// Bill board Matrix
	
	m_vcEye		= VEC3( m_mtViwI._41, m_mtViwI._42, m_mtViwI._43);				// Camera Position ReSetting
	m_mtVwPj	= m_mtViw * m_mtPrj;

	VEC3 vcLf = -VEC3(m_mtVwPj._14 + m_mtVwPj._11, m_mtVwPj._24 + m_mtVwPj._21, m_mtVwPj._34 + m_mtVwPj._31);		// Left
	VEC3 vcRg = -VEC3(m_mtVwPj._14 - m_mtVwPj._11, m_mtVwPj._24 - m_mtVwPj._21, m_mtVwPj._34 - m_mtVwPj._31);		// Right
	VEC3 vcTp = -VEC3(m_mtVwPj._14 - m_mtVwPj._12, m_mtVwPj._24 - m_mtVwPj._22, m_mtVwPj._34 - m_mtVwPj._32);		// Top
	VEC3 vcBt = -VEC3(m_mtVwPj._14 + m_mtVwPj._12, m_mtVwPj._24 + m_mtVwPj._22, m_mtVwPj._34 + m_mtVwPj._32);		// Bottom
	VEC3 vcNr = -VEC3(m_mtViwI._31, m_mtViwI._32, m_mtViwI._33);				// Near
	VEC3 vcFr = -vcNr;															// Far
	VEC3 vcZn = m_vcEye + vcNr * m_fNr;
	VEC3 vcZf = m_vcEye + vcNr * m_fFr;

	D3DXPlaneFromPointNormal((D3DXPLANE*)&m_Frsm[0], &vcZn, &vcNr);				// Near Plane
	D3DXPlaneFromPointNormal((D3DXPLANE*)&m_Frsm[1], &vcZf, &vcFr);				// Far Plane
	D3DXPlaneFromPointNormal((D3DXPLANE*)&m_Frsm[2], &m_vcEye, &vcLf);			// Left Plane
	D3DXPlaneFromPointNormal((D3DXPLANE*)&m_Frsm[3], &m_vcEye, &vcRg);			// Right Plane
	D3DXPlaneFromPointNormal((D3DXPLANE*)&m_Frsm[4], &m_vcEye, &vcTp);			// Top Plane
	D3DXPlaneFromPointNormal((D3DXPLANE*)&m_Frsm[5], &m_vcEye, &vcBt);			// Bottom Plane
	

	m_pDev->SetTransform(D3DTS_VIEW, &m_mtViw);
	m_pDev->SetTransform(D3DTS_PROJECTION, &m_mtPrj);	

	return 1;
}

void CLcCam::MoveSideward(FLOAT fSpeed)
{
	VEC3 tmp(m_mtViw._11, 0, m_mtViw._31);
	D3DXVec3Normalize(&tmp,&tmp);

	m_vcEye  += tmp * fSpeed;
	m_vcLook += tmp * fSpeed;
	D3DXMatrixLookAtLH(&m_mtViw, &m_vcEye, &m_vcLook, &m_vcUp);
}


void CLcCam::MoveForward(FLOAT fSpeed, FLOAT fY)
{
	VEC3 tmp(m_mtViw._13, m_mtViw._23*fY, m_mtViw._33);
	D3DXVec3Normalize(&tmp,&tmp);

	m_vcEye  += tmp * fSpeed;
	m_vcLook += tmp * fSpeed;
	D3DXMatrixLookAtLH(&m_mtViw, &m_vcEye, &m_vcLook, &m_vcUp);
}



void CLcCam::Rotation(FLOAT fYaw, FLOAT fPitch, FLOAT fSpeed)
{
	m_fYaw   = D3DXToRadian(fYaw  * fSpeed);
	m_fPitch = D3DXToRadian(fPitch* fSpeed);
	
	D3DXMATRIX rot;
	VEC3 vcZ = m_vcLook-m_vcEye;
	VEC3 vcX;
	D3DXMatrixRotationY(&rot, m_fYaw);
	D3DXVec3TransformCoord(&vcZ, &vcZ, &rot);
	D3DXVec3TransformCoord(&m_vcUp, &m_vcUp, &rot);
	
	m_vcLook = vcZ + m_vcEye;
	D3DXMatrixLookAtLH(&m_mtViw, &m_vcEye, &m_vcLook, &m_vcUp);

	
	vcZ = m_vcLook - m_vcEye;
	vcX =VEC3(m_mtViw._11, m_mtViw._21, m_mtViw._31);
	
	D3DXMatrixRotationAxis(&rot, & vcX, m_fPitch);
	D3DXVec3TransformCoord(&vcZ, &vcZ, &rot);
	D3DXVec3TransformCoord(&m_vcUp, &m_vcUp, &rot);

	m_vcLook = vcZ + m_vcEye;
	D3DXMatrixLookAtLH(&m_mtViw, &m_vcEye, &m_vcLook, &m_vcUp);
}