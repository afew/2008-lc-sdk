// Interface for the CLcDevBase class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcDevBase_H_
#define _LcDevBase_H_


class CLcDevBase : public ILcDev
{
protected:

public:
	CLcDevBase();
	virtual ~CLcDevBase();
	
	virtual INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0);

	virtual INT		Init();
	virtual void	Destroy();

	virtual INT		Restore();
	virtual void	Invalidate();

	virtual INT		FrameMove();
	virtual INT		Render();

	virtual	INT		Run();

	virtual INT		Query(char* sCmd, void* pData);


	virtual INT		BeginScene();
	virtual INT		EndScene();
	virtual INT		Clear(DWORD Count,CONST RECT* pRects,DWORD Flags,DWORD Color,float Z,DWORD Stencil);
};


DOUBLE TimeGetTime();															// 하드웨어에서 지원되는 PerformanceCounter를 이용한 FPS구하기

#endif
