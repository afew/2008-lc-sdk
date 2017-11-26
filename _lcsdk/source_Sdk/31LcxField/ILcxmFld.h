// Interface for the ILcxmFld class.
// Out Door Map
////////////////////////////////////////////////////////////////////////////////

#ifndef _ILcxmFld_H_
#define _ILcxmFld_H_


#pragma warning(disable : 4786)
#include <vector>


#ifndef interface
#define interface struct
#endif


#ifndef LC_CLASS_DESTROYER
#define LC_CLASS_DESTROYER( CLASS_NAME )	\
virtual ~CLASS_NAME(){}
#endif


interface ILcxmFld						// Outdoor Map
{
	LC_CLASS_DESTROYER(	ILcxmFld	);

	virtual	INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0)=0;
	virtual	void	Destroy()=0;

	virtual	INT		FrameMove()=0;
	virtual	void	Render()=0;

	virtual INT		Query(char* sCmd, void* pData)=0;
	virtual	INT		GetHeight(float* vcOut, const float* vcIn)=0;
};



typedef std::vector<void* >		lsLcxHandle;
typedef lsLcxHandle::iterator	itLcxHandle;


INT LcMap_CreateOutdoor(char* sCmd
				, ILcxmFld** pData
				, void* p1			// Device
				, void* p2			// Texture Table
				, void* p3			// Model Table
				, void* p4=0		// Map File Name
				, void* p5=0		// No Use
				, void* p6=0		// No Use
				, void* p7=0		// No Use
				);



#ifdef USE_LC_SDK_LIB

	#ifdef _DEBUG
		#pragma comment(lib, "LcxmFld_.lib")
	#else
		#pragma comment(lib, "LcxmFld.lib")
	#endif
#endif


#endif


