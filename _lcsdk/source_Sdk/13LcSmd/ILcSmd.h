// Interface for the ILcSmd class.
// SMD(Music, Sound, DShow)
////////////////////////////////////////////////////////////////////////////////

#ifndef _ILcSmd_H_
#define _ILcSmd_H_

#ifndef interface
#define interface struct
#endif


#ifndef LC_CLASS_DESTROYER
#define LC_CLASS_DESTROYER( CLASS_NAME )	\
virtual ~CLASS_NAME(){}
#endif


interface ILcSmd
{
	LC_CLASS_DESTROYER(	ILcSmd	);

	enum EMediaType
	{
		LC_SMD_UNKNOWN	= 0x0000,	// 0000 0000 0000	: Loader	Player
		
		LC_SMD_WV_D		= 0x0008,	// 0000 0000 1000	: Wave		Default
		LC_SMD_WV_M		= 0x0009,	// 0000 0000 1001	: Wave		Mono and Stero
		LC_SMD_WV_S		= 0x000C,	// 0000 0000 1100	: 3d		Sound

		LC_SMD_MW_D		= 0x0080,	// 0000 1000 0000	: Music		Default
		LC_SMD_MW_M		= 0x0081,	// 0000 1000 0001	: Music		Wave
		LC_SMD_MW_S		= 0x0084,	// 0000 1000 0100	: Music		Wave 3D

		LC_SMD_MC_M		= 0x0090,	// 0000 1001 0000	: Music		Mono and Stero
		LC_SMD_MC_S		= 0x00C0,	// 0000 1100 0000	: Music		3D
		
		LC_SMD_SH_D		= 0x0800,	// 1000 0000 0000	: DShow		Default
		LC_SMD_SH_M		= 0x0900,	// 1001 0000 0000	: DShow		Mp3
		LC_SMD_SH_A		= 0x0A00,	// 1010 0000 0000	: DShow		AVI
	};


	virtual INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0)=0;
	virtual void	Destroy()=0;

	virtual INT		Query(char* sCmd, void* pData)=0;

	virtual DWORD	GetType()=0;
	virtual void	Play()=0;
	virtual void	Stop()=0;
	virtual void	Reset()=0;
	virtual void	Pause()=0;
	virtual void	SetVolume(LONG dVol)=0;
	virtual LONG	GetVolume()=0;
	virtual void	SetRepeat(DWORD dRepeat= 0xFFFFFFFF/*INFINITE*/)=0;
	virtual DWORD	GetStatus()=0;


	virtual void*	GetTexture()=0;
	virtual INT		GetVideoW()=0;
	virtual INT		GetVideoH()=0;
};


INT		LcSmd_DeviceInit(HWND hWnd);
void	LcSmd_DeviceClose();


INT		LcSmd_Create(char* sCmd
					, ILcSmd** pData
					, void* p1			// Wave File Name
					, void* p2=0		// Sound Format(EMediaType)
					, void* p3=0		// Flag for 3D sound
					, void* p4=0		// Event ID
					, void* p5=0		// LPDIRECT3DDEVICE9 pointer for Direct Show
					, void* p6=0		// No Use
					, void* p7=0		// No Use
					, void* p8=0		// No Use
					);


void*	LcSmd_GetDirectSound();			// Get IDirectSound8 pointer
void*	LcSmd_GetDirectMusicLoader();	// Get Dierect Music Loader
HWND	LcSmd_GetHwnd()		;			// Get Window Handle about to use Sound and Music.




//#pragma comment(lib, "winmm.lib") ==> #pragma comment(lib, "mmtimer.lib")


//#pragma comment(lib, "dsound.lib"	)	// Sound
//#pragma comment(lib, "dxguid.lib"	)	//
//#pragma comment(lib, "dxerr9.lib"	)	//
//#pragma comment(lib, "winmm.lib"	)	//



// Mp3 and Direct Show needs DShowStrmBase.lib.
// This Library file is located D3DSDK Samples.

#ifdef _DEBUG
#pragma comment(lib, "DShowStrmbase_.lib")
#else
#pragma comment(lib, "DShowStrmbase.lib")
#endif


#ifdef _DEBUG
	#pragma comment(lib, "LcSmd_.lib")
#else
	#pragma comment(lib, "LcSmd.lib")
#endif

	
#endif



