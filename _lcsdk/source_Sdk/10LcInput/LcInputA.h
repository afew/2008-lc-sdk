// Interface for the CLcInputA class.
// Input WinAPI Base.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcInputA_H_
#define _LcInputA_H_


class CLcInputA : public CLcInput
{
protected:
	FLOAT		m_fWheel;		// Wheel Position

public:
	CLcInputA();
	virtual ~CLcInputA();

	virtual INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0);
	virtual void	Destroy();
	virtual INT		FrameMove();
	virtual INT		Query(char* sCmd, void* pData);
	virtual INT		MsgProc(HWND,UINT,WPARAM,LPARAM);

protected:
	void	SetMousePos(FLOAT* vcPos);
	void	AddWheelPos(INT d);
	void	OnReset();
};


#endif