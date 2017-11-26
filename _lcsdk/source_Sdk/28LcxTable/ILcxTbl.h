// Interface for the ILcxTbl class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _ILcxTbl_H_
#define _ILcxTbl_H_


#pragma warning(disable : 4786)
#include <vector>


#ifndef interface
#define interface struct
#endif


#ifndef LC_CLASS_DESTROYER
#define LC_CLASS_DESTROYER( CLASS_NAME )	\
virtual ~CLASS_NAME(){}
#endif


interface ILcxTbl
{
	LC_CLASS_DESTROYER(	ILcxTbl	);

	virtual INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0)=0;
	virtual void	Destroy()=0;

	virtual	INT		Select(void** pOut, INT nM/*In*/, INT nS/*In*/)=0;
	virtual	INT		Select(void** pOut, INT* nM/*Out*/, INT* nS/*Out*/, char* sFile)=0;
	virtual	INT		SelectInfo(void** pOut, INT nM/*In*/, INT nS/*In*/)=0;
};


typedef	std::vector<ILcxTbl* >	lsILcxTbl;
typedef lsILcxTbl::iterator		itILcxTbl;


INT LcxTbl_Create(char* sCmd
				 , ILcxTbl** pData
				 , void* p1			// File Name
				 , void* p2			// LPDIRECT3DDEVICE9
				 , void* p3=NULL	// Etc
				 , void* p4=NULL	// No Use
				 );

#ifdef _DEBUG
	#pragma comment(lib, "LcxTbl_.lib")
#else
	#pragma comment(lib, "LcxTbl.lib")
#endif



#endif



