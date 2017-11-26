// Interface for the IGameSgmnt.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _IGameSgmnt_H_
#define _IGameSgmnt_H_


#ifndef interface
#define interface struct
#endif


#ifndef LC_CLASS_DESTROYER
#define LC_CLASS_DESTROYER( CLASS_NAME )	\
virtual ~CLASS_NAME(){}
#endif


// Game Play Segment
interface IGameSgmnt
{
	LC_CLASS_DESTROYER(	IGameSgmnt	);

	virtual INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0)=0;
	virtual void	Destroy()=0;

	virtual INT		FrameMove()=0;
	virtual void	Render()=0;

	virtual INT		Restore()=0;
	virtual void	Invalidate()=0;

	virtual INT		Query(char* sCmd, void* pData)=0;
};


INT LcxApp_CreateGameSegment(char* sCmd
				 , IGameSgmnt** pData
				 , void* p1			// CMain Instance Pointer
				 , void* p2=0		// No Use
				 , void* p3=0		// No Use
				 , void* p4=0		// No Use
				 );


#endif

