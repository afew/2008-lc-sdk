// Interface for the ILcmMdl class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcmMdl_H_
#define _LcmMdl_H_


namespace Lcm
{
	
class CLcmMdl : public ILcmMdl
{
public:

public:
	CLcmMdl();
	virtual ~CLcmMdl();

	virtual INT		Create(void* =0, void* =0, void* =0, void* =0);
	virtual void	Destroy();
	virtual INT		FrameMove();
	virtual void	Render();

	virtual INT		Query(char* sCmd, void* pData);
};

}// namespace Lcm

#endif

