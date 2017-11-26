// Interface for the CMpWaveFile class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MpWaveFile_H_
#define _MpWaveFile_H_


#include <mmreg.h>


// Typing macros 
#define WAVEFILE_READ   1
#define WAVEFILE_WRITE  2


class CMpWaveFile
{
public:
	WAVEFORMATEX* m_pwfx;        // Pointer to WAVEFORMATEX structure
	HMMIO         m_hmmio;       // MM I/O handle for the WAVE
	MMCKINFO      m_ck;          // Multimedia RIFF chunk
	MMCKINFO      m_ckRiff;      // Use in opening a WAVE file
	DWORD         m_dwSize;      // The size of the wave file
	MMIOINFO      m_mmioinfoOut;
	DWORD         m_dwFlags;
	BOOL          m_bIsReadingFromMemory;
	BYTE*         m_pbData;
	BYTE*         m_pbDataCur;
	ULONG         m_ulDataSize;
	CHAR*         m_pResourceBuffer;
	
protected:
	HRESULT ReadMMIO();
	HRESULT WriteMMIO( WAVEFORMATEX *pwfxDest );
	
public:
	CMpWaveFile();
	virtual ~CMpWaveFile();
	
	HRESULT Open( LPTSTR strFileName, WAVEFORMATEX* pwfx, DWORD dwFlags );
	HRESULT OpenFromMemory( BYTE* pbData, ULONG ulDataSize, WAVEFORMATEX* pwfx, DWORD dwFlags );
	HRESULT Close();
	
	HRESULT Read( BYTE* pBuffer, DWORD dwSizeToRead, DWORD* pdwSizeRead );
	HRESULT Write( UINT nSizeToWrite, BYTE* pbData, UINT* pnSizeWrote );
	
	DWORD   GetSize();
	HRESULT ResetFile();
	WAVEFORMATEX* GetFormat() { return m_pwfx; };
};



#endif