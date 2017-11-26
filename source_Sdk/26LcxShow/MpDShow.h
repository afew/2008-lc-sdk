// Interface for the CMpDShow class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MpDShow_H_
#define _MpDShow_H_


class CMpDShow : public ILcSmd
{
public:
	class IMpDShowClip
	{
	public:
		IMpDShowClip(){};
		virtual ~IMpDShowClip(){};

		virtual INT		Create(void* pDev, INT nEventID, char* sFile)=0;
		virtual void	Destroy()=0;
		virtual INT		ProcessEvent()=0;

	public:
		virtual INT		Run()=0;
		virtual INT		Stop()=0;
		virtual INT		Reset()=0;

		virtual void*	GetTexture()=0;
		virtual INT		GetVideoW()=0;
		virtual INT		GetVideoH()=0;
		virtual	INT		GetEventID()=0;

		virtual void	SetRepeat(DWORD dRepeat= 0xFFFFFFFF/*INFINITE*/)=0;


		virtual void	SetScnPosX(INT nVal)=0;
		virtual void	SetScnPosY(INT nVal)=0;
		virtual void	SetScnWidth(INT nVal)=0;
		virtual	void	SetScnHeight(INT nVal)=0;
		virtual	void	SetDiffuse(DWORD nVal)=0;
		virtual INT		Draw()=0;
	};



protected:
	void*				m_pDev;
	IMpDShowClip*		m_pVid;

public:
	CMpDShow();
	virtual ~CMpDShow();

	virtual INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0);
	virtual void	Destroy();
	virtual INT		Query(char* sCmd, void* pData);

	virtual DWORD	GetType();
	virtual void	Play();
	virtual void	Stop();
	virtual void	Reset();
	virtual void	Pause();
	virtual void	SetVolume(LONG dVol);
	virtual LONG	GetVolume();
	virtual void	SetRepeat(DWORD dRepeat= 0xFFFFFFFF/*INFINITE*/);
	virtual DWORD	GetStatus();

	virtual void*	GetTexture();
	virtual INT		GetVideoW();
	virtual INT		GetVideoH();

protected:
	INT				ProcessEvent();
	INT				GetEventID();
};


INT		LnDms_CreateDShowFromFile(ILcSmd** ppDShow
								  , void* pDev			// LPDIRECT3DDEVICE9 pointer
								  , INT nEvntId
								  ,	char* sFile);



#ifdef _DEBUG
#pragma comment(lib, "DShowStrmbase_.lib")
#else
#pragma comment(lib, "DShowStrmbase.lib")
#endif


#ifdef _DEBUG
	#pragma comment(lib, "MpDShow_.lib")
#else
	#pragma comment(lib, "MpDShow.lib")
#endif



#endif



