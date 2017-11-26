// Interface for the CLcIdx class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcIdx_H_
#define _LcIdx_H_


class CLcIdx : public ILcIdx
{
protected:
	LPDIRECT3DDEVICE9	m_pDev;

	DWORD				m_dFMT;
	void*				m_pIB;				// 시스템 메모리 사용. malloc 으로 생성

public:
	CLcIdx();
	virtual ~CLcIdx();

public:
	virtual INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0);
	virtual void	Destroy();

	virtual void*	GetIndex();
	virtual DWORD	GetFMT();

protected:
	INT		SetupDevice(void* p);
};



class CLcIdx16 : public CLcIdx
{
public:
	
	struct Idx16																// Index structure
	{
		union	{	struct	{	WORD a;	WORD b;	WORD c;	};	WORD m[3];	};

		Idx16()						: a(0),b(1),c(2){}
		Idx16(WORD A,WORD B,WORD C)	: a(A),b(B),c(C){}
		Idx16(WORD* R)					: a(R[0]),b(R[1]),c(R[2]){}

		operator WORD* ()				{	return (WORD *) &a;			}
		operator const WORD* () const	{	return (const WORD *) &a;	}
	};


};



class CLcIdx32 : public CLcIdx
{
public:
	
	struct Idx32																// Index structure
	{
		union	{	struct	{	DWORD a;	DWORD b;	DWORD c;	};	DWORD m[3];	};

		Idx32()						: a(0),b(1),c(2){}
		Idx32(DWORD A,DWORD B,DWORD C)	: a(A),b(B),c(C){}
		Idx32(DWORD* R)					: a(R[0]),b(R[1]),c(R[2]){}

		operator DWORD* ()				{	return (DWORD *) &a;			}
		operator const DWORD* () const	{	return (const DWORD *) &a;	}
	};


};

#endif

