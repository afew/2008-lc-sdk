// Interface for the ILcDev class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _ILcDev_H_
#define _ILcDev_H_


#ifndef interface
#define interface struct
#endif


#ifndef LC_CLASS_DESTROYER
#define LC_CLASS_DESTROYER( CLASS_NAME )	\
virtual ~CLASS_NAME(){}
#endif


interface IGmPhase
{
	LC_CLASS_DESTROYER(	IGmPhase	);
	
	virtual INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0)=0;
	virtual void	Destroy()=0;

	virtual INT		Restore()=0;
	virtual void	Invalidate()=0;

	virtual INT		FrameMove()=0;
	virtual void	Render()=0;

	virtual INT		Query(char* sCmd, void* pData)=0;
};



struct LcDev
{
	INT			PosX;		// Position X
	INT			PosY;		// Position Y
	INT			ScnX;		// Client Rect Width
	INT			ScnY;		// Client Rect Height

	HINSTANCE	hInst;
	HWND		hWnd;		// Window Handle. In, out value

	TCHAR		sCls[256];	// Class Name
	

	LcDev();
	LcDev(INT _PosX, INT _PosY, INT _Wx, INT _Wy, const TCHAR* _sCls, HINSTANCE _hInst=NULL, HWND _hWnd= NULL);
};



interface ILcDev
{
	LC_CLASS_DESTROYER(	ILcDev	);
	
	virtual INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0)=0;

	virtual INT		Init()=0;
	virtual void	Destroy()=0;

	virtual INT		Restore()=0;
	virtual void	Invalidate()=0;

	virtual INT		FrameMove()=0;
	virtual INT		Render()=0;

	virtual	INT		Run()=0;

	virtual INT		Query(char* sCmd, void* pData)=0;


	virtual INT		BeginScene()=0;
	virtual INT		EndScene()=0;
	virtual INT		Clear(DWORD Count,CONST RECT* pRects,DWORD Flags,DWORD Color,float Z,DWORD Stencil)=0;
};


INT		LcDev_CreateWindowInstance(char* sCmd
						, ILcDev** pData
						, void* p1=0		// No Use
						, void* p2=0		// No Use
						, void* p3=0		// No Use
						, void* p4=0		// No Use
						);


INT		LcDev_CreateDevice(char* sCmd
						, ILcDev** pData
						, void* p1=0		// LcDev pointer
						, void* p2=0		// No Use
						, void* p3=0		// No Use
						, void* p4=0		// No Use
						);


void*	LcDev_GetHinst(ILcDev** pDev);
void*	LcDev_GetHwnd(ILcDev** pDev);
void*	LcDev_GetD3Device(ILcDev** pDev);
void*	LcDev_GetBackBufferSurface(ILcDev** pDev);

//Query 대신 편리하게 사용하기 위해서...
void*	LcDev_GetUserSprite	(ILcDev** ppDev);
void*	LcDev_GetUserInput	(ILcDev** ppDev);
void*	LcDev_GetUserCamera	(ILcDev** ppDev);
void*	LcDev_GetUserNetwork(ILcDev** ppDev);
void*	LcDev_GetUserIme	(ILcDev** ppDev);
void*	LcDev_GetUserCommand(ILcDev** ppDev);

void*	LcDev_GetUserData1	(ILcDev** ppDev);
void*	LcDev_GetUserData2	(ILcDev** ppDev);
void*	LcDev_GetUserData3	(ILcDev** ppDev);
void*	LcDev_GetUserData4	(ILcDev** ppDev);


INT		LcDev_SetUserSprite	(ILcDev** ppDev, void* p);
INT		LcDev_SetUserInput	(ILcDev** ppDev, void* p);
INT		LcDev_SetUserCamera	(ILcDev** ppDev, void* p);
INT		LcDev_SetUserNetwork(ILcDev** ppDev, void* p);
INT		LcDev_SetUserIme	(ILcDev** ppDev, void* p);
INT		LcDev_SetUserCommand(ILcDev** ppDev, void* p);

INT		LcDev_SetUserData1	(ILcDev** ppDev, void* p);
INT		LcDev_SetUserData2	(ILcDev** ppDev, void* p);
INT		LcDev_SetUserData3	(ILcDev** ppDev, void* p);
INT		LcDev_SetUserData4	(ILcDev** ppDev, void* p);




interface ILcDevSprite
{
	LC_CLASS_DESTROYER(	ILcDevSprite	);
	
	virtual INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0)=0;
	virtual void	Destroy()=0;

	virtual	INT		Begin(DWORD=0)	=0;
	virtual	INT		End(DWORD=0)	=0;

	virtual	INT		Draw( void* pTx0				// 0 Stage Texture(<==ILcTex)
						, void* pTx1				// 1 Stage Texture(<==ILcTex)
						, const RECT* pRC0			// Draw Region 0 State Texture
						, const RECT* pRC1			// Draw Region 1 State Texture
						, void* pScl=NULL			// Scaling
						, void* pPos=NULL			// Position
						, DWORD dColor =0xFFFFFFFF	// color
						, DWORD dFilter=0x0			// Filter(<==D3DMTEXF_POINT)
						)=0;
};


INT	LcDev_CreateSprite(char* sCmd
					, ILcDevSprite** pData
					, void* p1							// Need D3D Device
					, void* p2=0						// No Use
					, void* p3=0						// No Use
					, void* p4=0						// No Use
					);


//#pragma comment(lib, "d3dm.lib")
//#pragma comment(lib, "d3dmx.lib")
//#pragma comment(lib, "d3dmguid.lib")

#ifdef _DEBUG
	#pragma comment(lib, "LcDev_.lib")
#else
	#pragma comment(lib, "LcDev.lib")
#endif


#endif

