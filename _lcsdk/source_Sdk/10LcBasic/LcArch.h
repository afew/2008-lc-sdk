// Interface for the Architecture classes.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcARCH_H_
#define _LcARCH_H_


typedef int						INT;
typedef int						BOOL;
typedef signed char				INT8;
typedef	short					INT16;
typedef int						INT32;
typedef __int64					INT64;

typedef	unsigned char			BYTE;			// 8 bit
typedef unsigned short			WORD;			// 16 bit
typedef unsigned long			DWORD;			// 32 bit
//typedef unsigned char			TWORD[6];		// 48 bit
typedef unsigned __int64		QWORD;			// 64 bit
//typedef unsigned char			TBYTE[10];		// 80 bit

typedef float					FLOAT;
typedef double					DOUBLE;



// Base Class.
struct _ILc	{	public:	virtual ~_ILc(){};	};



struct ILc : public _ILc
{
	union
	{
		struct
		{
			union	{	WORD _a;	WORD _I1;	WORD _wM;	};
			union	{	WORD _b;	WORD _I2;	WORD _wS;	};
			union	{	WORD _c;	WORD _I3;	WORD _wT;	};
			union	{	WORD _d;	WORD _I4;	WORD _wW;	};
		};

		ULONGLONG _Id;
	};

	ILc(): _Id(0xFFFFFFFFFFFFFFFF)	{}
	ILc(WORD I1,WORD I2=0xFFFF,WORD I3=0xFFFF,WORD I4=0xFFFF): _I1(I1),_I2(I2),_I3(I3),_I4(I4)	{}
	ILc(ULONGLONG I) : _Id(I){}

	void		SetId(WORD I1=0xFFFF,WORD I2=0xFFFF,WORD I3=0xFFFF,WORD I4=0xFFFF){_I1=I1;_I2=I2;_I3=I3;_I4=I4;}
	void		SetId(ULONGLONG Id=0xFFFFFFFFFFFFFFFF){	_Id = Id;	}
	
	void		SetId1(WORD I1=0xFFFF)	{	_I1 = I1;	}
	void		SetId2(WORD I2=0xFFFF)	{	_I2 = I2;	}
	void		SetId3(WORD I3=0xFFFF)	{	_I3 = I3;	}
	void		SetId4(WORD I4=0xFFFF)	{	_I4 = I4;	}

	void		SetM(WORD wM=0xFFFF)	{	_wM  = wM;	}
	void		SetS(WORD wS=0xFFFF)	{	_wS  = wS;	}
	void		SetT(WORD wT=0xFFFF)	{	_wT  = wT;	}
	void		SetW(WORD wW=0xFFFF)	{	_wW  = wW;	}

	ULONGLONG	GetId()		{	return _Id;	}

	WORD		GetId1()	{	return _I1;	}
	WORD		GetId2()	{	return _I2;	}
	WORD		GetId3()	{	return _I3;	}
	WORD		GetId4()	{	return _I4;	}

	WORD		GetM()		{	return _wM;	}
	WORD		GetS()		{	return _wS;	}
	WORD		GetT()		{	return _wT;	}
	WORD		GetW()		{	return _wW;	}
};


//	typedef std::vector<ILc>	lsLc;
//	typedef lsLc::iterator		itLc;



// Rendering Object Base Class.
struct ILcObj : public ILc
{
	BOOL	m_bFm;																// FrameMove
	BOOL	m_bTr;																// FrameMove �ȿ��� Move?
	BOOL	m_bUp;																// FrameMove �ȿ��� Animation?

	BOOL	m_bRn;																// ������ �� ���ΰ�?
	
	ILcObj() : m_bFm(1), m_bTr(1), m_bUp(1), m_bRn(1){}
	
	void	SetFrmMov(BOOL _bFm=1)	{	m_bFm	= _bFm;					}
	BOOL	IsFrmMov()				{	return m_bFm;					}

	void	SetTrans(BOOL _bTr=1)	{	m_bTr	= _bTr;	m_bFm=1;		}
	BOOL	IsFTrans()				{	return m_bTr;					}

	void	SetUpdate(BOOL _bUp=1)	{	m_bUp= _bUp; m_bFm=1; m_bTr=1;	}
	BOOL	GetUpate()				{	return m_bUp;					}

	void	SetRender(BOOL  _bRn=1)	{	m_bRn	= _bRn;					}
	BOOL	IsRender()				{	return m_bRn;					}

	virtual INT	 Create(void* =0,void* =0,void* =0,void* =0)=0;
	virtual void Destroy()=0;

	virtual INT	 Restore()=0;
	virtual void Invalidate()=0;

	virtual INT	 FrameMove()=0;
	virtual void Render()=0;
};


#endif


