// Interface for the CApp3D class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _App3D_H_
#define _App3D_H_

class CApp3D
{
public:
	static INT	pFuncInit()		;
	static void pFuncDestroy()	;
	static INT	pFuncRestore()	;
	static void pFuncInvalidate();
	static INT	pFuncFrameMove();
	static void pFuncRender()	;
	static INT	pFuncWndProc(HWND,UINT,WPARAM,LPARAM);

public:
	CApp3D();
	virtual ~CApp3D();

public:
	virtual INT		Init()=0;
	virtual void	Destroy()=0;

	virtual INT		Restore()=0;
	virtual void	Invalidate()=0;

	virtual INT		FrameMove()=0;
	virtual	INT		Render()=0;

public:
	virtual INT		MsgProc(HWND, UINT, WPARAM, LPARAM) =0;
	virtual INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0)=0;
	virtual INT		Run()=0;
};



#endif

