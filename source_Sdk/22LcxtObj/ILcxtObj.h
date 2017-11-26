// Interface for the 3D utilities classes.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _ILcxtObj_H_
#define _ILcxtObj_H_


#ifndef interface
#define interface struct
#endif


#ifndef LC_CLASS_DESTROYER
#define LC_CLASS_DESTROYER( CLASS_NAME )	\
virtual ~CLASS_NAME(){}
#endif



// Interface
interface ILcxtObj
{
	LC_CLASS_DESTROYER(	ILcxtObj	);

	virtual INT		Create(void* =0, void* =0, void* =0, void* =0)=0;
	virtual void	Destroy()=0;
	virtual INT		FrameMove()=0;
	virtual void	Render()=0;

	virtual INT		Query(char* sCmd, void* pData)=0;
};


//
INT	LcxtObj_Create(char* sCmd				// Command
					, ILcxtObj** pData		//
					, void* p1				// LPDIRECT3DDEVICE9
					, void* p2=0			// Number or Etc...
					, void* p3=0			//
					, void* p4=0			//
					);


#ifdef _DEBUG
	#pragma comment(lib, "LcxtObj_.lib")
#else
	#pragma comment(lib, "LcxtObj.lib")
#endif


#endif

