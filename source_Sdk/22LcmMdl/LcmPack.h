// Interface for the CLcmPack class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcmPack_H_
#define _LcmPack_H_


namespace Lcm
{

class CLcmPack : public ILcmPack
{
public:
	PDEV		m_pDev;
	
	void*		m_pLcl;
	lsLcxMdl		m_vObj;

public:
	CLcmPack();
	virtual ~CLcmPack();
	
	virtual	INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0);
	virtual void	Destroy();

	virtual INT		FrameMove();
	virtual void	Render();

	virtual INT		Query(char* sCmd, void* pData);

	virtual void	SetLcl(void* pLcl);
	virtual void	AddObj(ILcmMdl* pMdB
							, VEC3 vcP		// Position
							, void* p1		// Camera
							, void* p2		// Field
							, void* p3		// Field Local
							, void* p4		// Parent Package
							);

	virtual	void		Push_Back(ILcmMdl* pMdB);
	virtual	INT			Size();
	virtual	ILcmMdl*	GetObj(int n);
	virtual	void		Erase(int n);
};

}// namespace Lcm

#endif

