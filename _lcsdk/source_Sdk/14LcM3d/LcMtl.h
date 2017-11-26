// Interface for the CLcMtl class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcMtl_H_
#define _LcMtl_H_


class CLcMtl : public ILcMtl
{
protected:
	LPDIRECT3DDEVICE9		m_pDev;
	D3DMATERIAL9			m_D3Mtl;

public:
	CLcMtl();
	virtual ~CLcMtl();

public:
	virtual INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0);
	virtual void	Destroy();

	virtual void	SetMaterial();

	virtual void	SetAmbient(FLOAT* pVal/*D3DXCOLOR*/ );
	virtual void	SetDiffuse(FLOAT* pVal/*D3DXCOLOR*/ );
    virtual void	SetSpecular(FLOAT* pVal/*D3DXCOLOR*/ );
    virtual void	SetEmissive(FLOAT* pVal/*D3DXCOLOR*/ );
    virtual void	SetPower(FLOAT pVal);				// Sharpness if specular highlight

	virtual FLOAT*	GetDiffuse();
    virtual FLOAT*	GetAmbient();
    virtual FLOAT*	GetSpecular();
    virtual FLOAT*	GetEmissive();
    virtual FLOAT	GetPower();
};


#endif

