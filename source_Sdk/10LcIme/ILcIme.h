// Interface for the ILcIme class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _ILcIme_H_
#define _ILcIme_H_

#if _MSC_VER > 1000
#pragma once
#endif


#ifndef interface
#define interface struct
#endif


#ifndef LC_CLASS_DESTROYER
#define LC_CLASS_DESTROYER( CLASS_NAME )	\
virtual ~CLASS_NAME(){}
#endif


#define MAX_CHAT_LEN	240														// 최대 채팅 길이

interface ILcIme
{
	LC_CLASS_DESTROYER(	ILcIme	);

	enum EImeState
	{
		IHS_ENG = 0,
		IHS_KOR,			// Korean
		IHS_JPN,			// Japanese.	It's not Implemental
		IHS_CHR,			// Chinese.		It's not Implemental
	};

	
	virtual INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0)=0;
	virtual void	Destroy()=0;
	virtual INT		MsgProc(HWND,UINT,WPARAM,LPARAM)=0;

	virtual INT		Query(char* sCmd, void* pData)=0;

	virtual	void	Set()=0;
	virtual	void	Reset()=0;

	virtual void	SetState(BOOL)=0;
	virtual INT		GetState()=0;

	virtual	INT		OutString(char*)=0;
	virtual	INT		OutStringBeam(char*)=0;
	virtual	INT		OutStringStar(char*)=0;
};


INT LcIme_Create(char* sCmd
				 , ILcIme** pData
				 , void* p1=0		// String Table Back space Id
				 , void* p2=0		// No Use
				 , void* p3=0		// No Use
				 , void* p4=0		// No Use
				 );



#pragma comment(lib, "imm32")

#ifdef _DEBUG
	#pragma comment(lib, "LcIme_.lib")
#else
	#pragma comment(lib, "LcIme.lib")
#endif


#endif

