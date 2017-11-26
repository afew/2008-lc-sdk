// Interface for the CLcCam class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _LcCam_H_
#define _LcCam_H_

class CLcCam
{
protected:
	LPDIRECT3DDEVICE9	m_pDev;
	FLOAT		m_fFv;															// Field of View
    FLOAT		m_fAs;															// Aspect Ratio
    FLOAT		m_fNr;															// Near
    FLOAT		m_fFr;															// Far

	VEC3		m_vcEye;														// Camera position
	VEC3		m_vcLook;														// Look vector
	VEC3		m_vcUp;															// up vector

	FLOAT		m_fYaw;
	FLOAT		m_fPitch;

	MATA		m_mtViw;														// View Matrix
	MATA		m_mtPrj;														// Projection Matrix

	MATA		m_mtViwI;														// View Matrix Inverse
	MATA		m_mtBill;														// BillBoard Matrix
	MATA		m_mtVwPj;														// m_mtViw * m_mtPrj;
	
	D3DXPLANE	m_Frsm[6];														// Near, Far, Left, Right, Up, Down
	
public:
	CLcCam();
	~CLcCam();

	INT		Create(LPDIRECT3DDEVICE9 pDev);
	INT		FrameMove();

	MATA	GetMatrixViw()		{	return m_mtViw;		}
	MATA	GetMatrixViwI()		{	return m_mtViwI;	}
	MATA	GetMatrixBll()		{	return m_mtBill;	}
	MATA	GetMatrixPrj()		{	return m_mtPrj;		}
	MATA	GetMatrixViwPrj()	{	return m_mtVwPj;	}

	VEC3	GetEye()			{	return m_vcEye;		}
	VEC3	GetLook()			{	return m_vcLook;	}
	VEC3	GetUp()				{	return m_vcUp;		}

public:
	void	MoveSideward(FLOAT	fSpeed);
	void	MoveForward	(FLOAT	fSpeed, FLOAT fY=0);
	void	Rotation(FLOAT fYaw, FLOAT fPitch, FLOAT fSpeed);
};

#endif
