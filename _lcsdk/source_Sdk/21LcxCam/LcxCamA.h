// Interface for the CLcxCamA class.
// First or Third Person Camera
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcxCamA_H_
#define _LcxCamA_H_


class CLcxCamA : public CLcxCam
{
public:
	CLcxCamA();
	virtual ~CLcxCamA();

	virtual INT	Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0);
	virtual INT	FrameMove();

public:
	virtual void	MoveSideward(FLOAT	fSpeed);
	virtual void	MoveForward	(FLOAT	fSpeed, FLOAT fY=1);
	virtual void	RotateYaw	(FLOAT fDelta,FLOAT fSpeed);
	virtual void	RotatePitch	(FLOAT fDelta,FLOAT fSpeed);
	virtual	void	Zoom(FLOAT);

public:	
	
};

#endif

