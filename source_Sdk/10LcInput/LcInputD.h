// Interface for the CLcInputD class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcInputD_H_
#define _LcInputD_H_


class CLcInputD : public CLcInput
{
protected:
	LPDIRECT3DDEVICE9		m_pDev	;

	LPDIRECTINPUT8			m_pDiI	;
	LPDIRECTINPUTDEVICE8	m_pDiK	;	// Keyboard
	LPDIRECTINPUTDEVICE8	m_pDiM	;	// Mouse	

public:
	CLcInputD();
	virtual ~CLcInputD();

	virtual INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0);
	virtual void	Destroy();
	virtual INT		FrameMove();
	virtual INT		Query(char* sCmd, void* pData);

protected:
	void	SetMousePos(FLOAT* vcPos);
	void	OnReset();
};

#endif

