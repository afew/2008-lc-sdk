// Interface for the ILcLsc class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _ILcLsc_H_
#define _ILcLsc_H_


#ifndef interface
#define interface struct
#endif


#ifndef LC_CLASS_DESTROYER
#define LC_CLASS_DESTROYER( CLASS_NAME )	\
virtual ~CLASS_NAME(){}
#endif



interface ILcLsc
{
	LC_CLASS_DESTROYER(	ILcLsc	);

	struct LscH							// Lua Handle
	{
		DWORD	nId;
		void*	pIns;					// Instance
		void*	pMsc;					// Miscellany data
		char	sName[160];				// Name

		LscH();
	};
	
	// General Member Functions.
	virtual INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0)=0;
	virtual void	Destroy()=0;
	virtual INT		Query(char* sCmd, void* pData)=0;

	// Lua Glue Functions.
	virtual INT		Lua_Init()=0;
	virtual INT		Lua_Destroy()=0;
	virtual INT		Lua_FrameMove()=0;
	virtual INT		Lua_Render()=0;
};



#ifdef _DEBUG
	#pragma comment(lib, "LcLsc_.lib")
#else
	#pragma comment(lib, "LcLsc.lib")
#endif


#endif


