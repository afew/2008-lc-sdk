// Interface for the CLcxMdl class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcxMdl_H_
#define _LcxMdl_H_

namespace Lcm
{

class CLcmId
{
public:
	union	{	WORD nI1;	WORD wM;	};
	union	{	WORD nI2;	WORD wS;	};
	union	{	WORD nI3;	WORD wT;	};
	union	{	WORD nI4;	WORD wW;	};

	CLcmId(): nI1(0xFFFF),nI2(0xFFFF),nI3(0xFFFF),nI4(0xFFFF){}
	virtual ~CLcmId(){};

	void SetId(	WORD I1=0xFFFF
			,	WORD I2=0xFFFF
			,	WORD I3=0xFFFF
			,	WORD I4=0xFFFF){nI1=I1;nI2=I2;nI3=I3;nI4=I4;}

	void SetMSTW(WORD I1=0xFFFF
			,	WORD I2=0xFFFF
			,	WORD I3=0xFFFF
			,	WORD I4=0xFFFF){nI1=I1;nI2=I2;nI3=I3;nI4=I4;}

	void	SetM(WORD wV=0xFFFF){	wM = wV;	}
	void	SetS(WORD wV=0xFFFF){	wS = wV;	}
	void	SetT(WORD wV=0xFFFF){	wT = wV;	}
	void	SetW(WORD wV=0xFFFF){	wW = wV;	}

	WORD	GetM()		{	return wM;	}
	WORD	GetS()		{	return wS;	}
	WORD	GetT()		{	return wT;	}
	WORD	GetW()		{	return wW;	}

	WORD	GetId1()	{	return nI1;	}
	WORD	GetId2()	{	return nI2;	}
	WORD	GetId3()	{	return nI3;	}
	WORD	GetId4()	{	return nI4;	}
};


class CLcxMdl : public ILcmMdl, public CLcmId
{
protected:
	PDEV		m_pDev;
	void*		m_pCam;

	void*		m_pMsD;															// Mesh Dest Pointer
	void*		m_pMsS;															// Mesh Source Pointer

	void*		m_pMscl1;														// miscellany	Field
	void*		m_pMscl2;														// miscellany	Local
	void*		m_pMscl3;														// miscellany	Packate

	VEC3		m_vcEps;														// Epsilon
	VEC3		m_vcRot;														// Rotation
	VEC3		m_vcScl;														// Scaling
	VEC3		m_vcTrn;														// Translation

	MATA		m_mtRot;														// Rotation Matrix
	MATA		m_mtScl;														// Scaling Matrix
	MATA		m_mtWld;														// World which is mtW = mtR * mtS, mtW_41 = pos.x, ....
	FLOAT		m_fDstCam;														// Distance from Camera

	FLOAT		m_fStlSrtR;														// Z Direction Value
	TbndAB		m_BndInf;
	
public:
	CLcxMdl();
	virtual ~CLcxMdl();

	virtual	INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0);
	virtual	void	Destroy();
	virtual	INT		FrameMove();
	virtual	void	Render();

	virtual INT		Query(char* sCmd, void* pData);
	
	virtual INT		SetPosition(FLOAT* float3);
	virtual INT		GetPosition(FLOAT* float3);
	virtual INT		SetRotation(FLOAT* float3);
	virtual INT		GetRotation(FLOAT* float3);
	virtual INT		SetScaling(FLOAT* float3);
	virtual INT		GetScaling(FLOAT* float3);

	virtual	FLOAT	GetSortValue();

public:
	void		BndBoxTransform();
	void		BndBoxRender();

	void		SetBndInf(void* pBnd);
	TbndAB*		GetBndInf() const;

protected:
	void		WorldMatrixSetup();
};

}// namespace Lcm

#endif

