// Interface for the ILcXmsh class.
//
////////////////////////////////////////////////////////////////////////////////


#ifndef _ILcXmsh_H_
#define _ILcXmsh_H_


#pragma warning(disable : 4786)
#include <vector>


#ifndef interface
#define interface struct
#endif


#ifndef LC_CLASS_DESTROYER
#define LC_CLASS_DESTROYER( CLASS_NAME )	\
virtual ~CLASS_NAME(){}
#endif


interface ILcXmsh
{
	LC_CLASS_DESTROYER(	ILcXmsh	);

	virtual	INT		Create(void* p1=0 /*Device*/, void* p2=0 /*X File Name*/, void* p3=0, void* p4=0)=0;
	virtual	void	Destroy()=0;

	virtual	INT		FrameMove()=0;
	virtual	void	Render()=0;

	virtual INT		Query(char* sCmd, void* pData)=0;

	virtual INT		Lock(UINT nGeo, UINT OffsetToLock, UINT SizeToLock, void** ppbData)=0;
	virtual INT		Unlock(UINT nGeo)=0;

	// Vertex SetUp( if Lock ppbdata is Null.
	virtual	void	SetPos		(INT nGeo, INT nVtx, FLOAT X, FLOAT Y, FLOAT Z, FLOAT W=1.F)=0;
	virtual	void	SetNormal	(INT nGeo, INT nVtx, FLOAT X, FLOAT Y, FLOAT Z)=0;

	// rgba is 0~ 1.f
	virtual	void	SetDiffuse	(INT nGeo, INT nVtx, FLOAT R, FLOAT G, FLOAT B, FLOAT A)=0;
	virtual	void	SetUV		(INT nGeo, INT nVtx, INT nUV, FLOAT U, FLOAT V)=0;

	virtual	void	GetPos		(INT nGeo, INT nVtx, FLOAT* X, FLOAT* Y, FLOAT* Z, FLOAT* W)=0;
	virtual	void	GetNormal	(INT nGeo, INT nVtx, FLOAT* X, FLOAT* Y, FLOAT* Z)=0;
	virtual	void	GetDiffuse	(INT nGeo, INT nVtx, FLOAT* R, FLOAT* G, FLOAT* B, FLOAT* A)=0;
	virtual	void	GetUV		(INT nGeo, INT nVtx, INT  nUV, FLOAT* U, FLOAT* V)=0;
};


INT LcXmsh_Create(char* sCmd
				 , ILcXmsh** pData
				 , void* p1			// LPDIRECT3DDEVICE9
				 , void* p2			// X-file Name
				 , void* p3=0		// No Use
				 , void* p4=0		// No Use
				 );



typedef std::vector<ILcXmsh* >		lsLcXmsh;
typedef lsLcXmsh::iterator			itLcXmsh;


#endif


