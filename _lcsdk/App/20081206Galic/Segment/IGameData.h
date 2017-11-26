// Interface for the IGameData.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _IGameData_H_
#define _IGameData_H_


#ifndef interface
#define interface struct
#endif


#ifndef LC_CLASS_DESTROYER
#define LC_CLASS_DESTROYER( CLASS_NAME )	\
virtual ~CLASS_NAME(){}
#endif


interface IGameData
{
	LC_CLASS_DESTROYER(	IGameData	);


	struct _TLcId
	{
		union
		{
			struct
			{
				union	{	WORD _a;	WORD _I1;	};
				union	{	WORD _b;	WORD _I2;	};
				union	{	WORD _c;	WORD _I3;	};
				union	{	WORD _d;	WORD _I4;	};
			};
			ULONGLONG _Id;
		};

		_TLcId(): _Id(0xFFFFFFFFFFFFFFFF)	{}
		_TLcId(WORD I1,WORD I2=0xFFFF,WORD I3=0xFFFF,WORD I4=0xFFFF): _I1(I1),_I2(I2),_I3(I3),_I4(I4)	{}
		_TLcId(ULONGLONG I) : _Id(I){}

		void		SetId(WORD I1=0xFFFF,WORD I2=0xFFFF,WORD I3=0xFFFF,WORD I4=0xFFFF){_I1=I1;_I2=I2;_I3=I3;_I4=I4;}
		void		SetId(ULONGLONG Id=0xFFFFFFFFFFFFFFFF){	_Id = Id;	}
		void		SetId1(WORD I1=0xFFFF)	{	_I1 = I1;	}
		void		SetId2(WORD I2=0xFFFF)	{	_I2 = I2;	}
		void		SetId3(WORD I3=0xFFFF)	{	_I3 = I3;	}
		void		SetId4(WORD I4=0xFFFF)	{	_I4 = I4;	}

		ULONGLONG	GetId()		{	return _Id;	}
		WORD		GetId1()	{	return _I1;	}
		WORD		GetId2()	{	return _I2;	}
		WORD		GetId3()	{	return _I3;	}
		WORD		GetId4()	{	return _I4;	}
	};

	virtual INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0)=0;
	virtual void	Destroy()=0;
	
	virtual INT		Query(char* sCmd, void* pData)=0;
};


INT LcxApp_CreateGameData(char* sCmd
				 , IGameData** pData
				 , void* p1			// CMain Instance Pointer
				 , void* p2=0		// No Use
				 , void* p3=0		// No Use
				 , void* p4=0		// No Use
				 );


#endif

