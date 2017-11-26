// Interface for the CLcmSolid class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcmSolid_H_
#define _LcmSolid_H_


namespace Lcm
{

class CLcmBill : public CLcxMdl
{
public:
	DCLR		m_dC;															// Color

public:
	CLcmBill();
	virtual ~CLcmBill();

	virtual	INT		Create(void* =0, void* =0, void* =0, void* =0);
	virtual	void	Destroy();

	virtual	INT		FrameMove();
	virtual	void	Render();

	virtual	INT		GetType();

public:
	void			SetColor(DWORD dc)		{	m_dC= dc;			}
};



class CLcmSolid : public CLcxMdl
{
public:
	CLcmSolid();
	virtual ~CLcmSolid();

	virtual	INT		Create(void* =0, void* =0, void* =0, void* =0);
	virtual	void	Destroy();

	virtual	INT		FrameMove();
	virtual	void	Render();

	virtual	INT		GetType();
};


}// namespace Lcm

#endif

