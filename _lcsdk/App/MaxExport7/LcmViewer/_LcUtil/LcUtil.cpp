//
//
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include <d3d9.h>
#include <d3dx9.h>

#include "LcUtil.h"


void LcUtil_ErrMsgBox(HWND hWnd, char *format,...)
{
	va_list ap;
	char s[2048];
	
	if (format == NULL) return;
	
	va_start(ap, format);
	vsprintf((char *)s, format, ap);
	va_end(ap);
	
	if (s == NULL)	return;
	
	MessageBox(hWnd, s, "Err", MB_OK | MB_ICONERROR);
}


void LcUtil_SetWindowTitle(HWND hWnd, char *format,...)
{
	va_list ap;
	char s[2048];
	
	if (format == NULL) return;
	
	va_start(ap, format);
	vsprintf((char *)s, format, ap);
	va_end(ap);
	
	if (s == NULL)	return;
	
	SetWindowText(hWnd, s);
}



// Texture Load
INT	 LcUtil_TextureLoad(LPDIRECT3DDEVICE9 pDev
						, char* sFile
						, LPDIRECT3DTEXTURE9& pTx
						, DWORD color
						, D3DXIMAGE_INFO *pSrcInfo
						, DWORD Filter
						, DWORD MipFilter
						, D3DFORMAT d3dFormat)
{
	if ( FAILED(D3DXCreateTextureFromFileEx(
		pDev
		, sFile
		, D3DX_DEFAULT
		, D3DX_DEFAULT
		, D3DX_DEFAULT
		, 0
		, d3dFormat
		, D3DPOOL_MANAGED
		, Filter
		, MipFilter
		, color
		, pSrcInfo
		, NULL
		, &pTx
		)) )
	{
		pTx = NULL;
		return -1;
	}
	
	return 0;
}

