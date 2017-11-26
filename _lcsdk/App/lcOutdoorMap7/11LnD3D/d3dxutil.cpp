// Shortcut macros and functions for using DX objects.
//
////////////////////////////////////////////////////////////////////////////////


#ifndef STRICT
	#define STRICT
#endif

#include <windows.h>
#include <mmsystem.h>
#include <tchar.h>
#include <stdio.h> 
#include <stdarg.h>

#include <D3D9.h>
#include <D3DX9.h>

#include "D3DxUtil.h"


// Initializes a D3DMATERIAL9 structure, setting the diffuse and ambient
// colors. It does not set emissive or specular colors.

VOID D3DUtil_InitMaterial(D3DMATERIAL9& mtrl, FLOAT r, FLOAT g, FLOAT b, FLOAT a)
{
	ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
	mtrl.Diffuse.r = mtrl.Ambient.r = r;
	mtrl.Diffuse.g = mtrl.Ambient.g = g;
	mtrl.Diffuse.b = mtrl.Ambient.b = b;
	mtrl.Diffuse.a = mtrl.Ambient.a = a;
}



// Initializes a D3DLIGHT structure, setting the light position. The
// diffuse color is set to white; specular and ambient are left as black.

VOID D3DUtil_InitLight( D3DLIGHT9& light, D3DLIGHTTYPE ltType, FLOAT x, FLOAT y, FLOAT z )
{
	D3DXVECTOR3 vecLightDirUnnormalized(x, y, z);
	ZeroMemory( &light, sizeof(D3DLIGHT9) );
	light.Type        = ltType;
	light.Diffuse.r   = 1.0f;
	light.Diffuse.g   = 1.0f;
	light.Diffuse.b   = 1.0f;
	D3DXVec3Normalize( (D3DXVECTOR3*)&light.Direction, &vecLightDirUnnormalized );
	light.Position.x   = x;
	light.Position.y   = y;
	light.Position.z   = z;
	light.Range        = 1000.0f;
}



// Helper function to create a texture. It checks the root path first,
// then tries the DXSDK media path (as specified in the system registry).

HRESULT D3DUtil_CreateTexture( LPDIRECT3DDEVICE9 pd3dDevice, TCHAR* strTexture,
							  LPDIRECT3DTEXTURE9* ppTexture, D3DFORMAT d3dFormat )
{
	HRESULT hr;
	TCHAR strPath[MAX_PATH];
	
	// Get the path to the texture
	if( FAILED( hr = DXUtil_FindMediaFileCb( strPath, sizeof(strPath), strTexture ) ) )
		return hr;
	
	// Create the texture using D3DX
	return D3DXCreateTextureFromFileEx( pd3dDevice, strPath, 
		D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, d3dFormat, 
		D3DPOOL_MANAGED, D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, 
		D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, 0, NULL, NULL, ppTexture );
}




// Returns a view matrix for rendering to a face of a cubemap.

D3DXMATRIX D3DUtil_GetCubeMapViewMatrix( DWORD dwFace )
{
	D3DXVECTOR3 vEyePt   = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 vLookDir;
	D3DXVECTOR3 vUpDir;
	
	switch( dwFace )
	{
	case D3DCUBEMAP_FACE_POSITIVE_X:
		vLookDir = D3DXVECTOR3( 1.0f, 0.0f, 0.0f );
		vUpDir   = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
		break;
	case D3DCUBEMAP_FACE_NEGATIVE_X:
		vLookDir = D3DXVECTOR3(-1.0f, 0.0f, 0.0f );
		vUpDir   = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
		break;
	case D3DCUBEMAP_FACE_POSITIVE_Y:
		vLookDir = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
		vUpDir   = D3DXVECTOR3( 0.0f, 0.0f,-1.0f );
		break;
	case D3DCUBEMAP_FACE_NEGATIVE_Y:
		vLookDir = D3DXVECTOR3( 0.0f,-1.0f, 0.0f );
		vUpDir   = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
		break;
	case D3DCUBEMAP_FACE_POSITIVE_Z:
		vLookDir = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
		vUpDir   = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
		break;
	case D3DCUBEMAP_FACE_NEGATIVE_Z:
		vLookDir = D3DXVECTOR3( 0.0f, 0.0f,-1.0f );
		vUpDir   = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
		break;
	}
	
	// Set the view transform for this cubemap surface
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookDir, &vUpDir );
	return matView;
}



// Returns a quaternion for the rotation implied by the window's cursor position.

D3DXQUATERNION D3DUtil_GetRotationFromCursor( HWND hWnd, FLOAT fTrackBallRadius )
{
	POINT pt;
	RECT  rc;
	GetCursorPos( &pt );
	GetClientRect( hWnd, &rc );
	ScreenToClient( hWnd, &pt );
	FLOAT sx = ( ( ( 2.0f * pt.x ) / (rc.right-rc.left) ) - 1 );
	FLOAT sy = ( ( ( 2.0f * pt.y ) / (rc.bottom-rc.top) ) - 1 );
	FLOAT sz;
	
	if( sx == 0.0f && sy == 0.0f )
		return D3DXQUATERNION( 0.0f, 0.0f, 0.0f, 1.0f );
	
	FLOAT d2 = sqrtf( sx*sx + sy*sy );
	
	if( d2 < fTrackBallRadius * 0.70710678118654752440 ) // Inside sphere
		sz = sqrtf( fTrackBallRadius*fTrackBallRadius - d2*d2 );
	else                                                 // On hyperbola
		sz = (fTrackBallRadius*fTrackBallRadius) / (2.0f*d2);
	
	// Get two points on trackball's sphere
	D3DXVECTOR3 p1( sx, sy, sz );
	D3DXVECTOR3 p2( 0.0f, 0.0f, fTrackBallRadius );
	
	// Get axis of rotation, which is cross product of p1 and p2
	D3DXVECTOR3 vAxis;
	D3DXVec3Cross( &vAxis, &p1, &p2);
	
	// Calculate angle for the rotation about that axis
	D3DXVECTOR3 vecDiff = p2-p1;
	FLOAT t = D3DXVec3Length( &vecDiff ) / ( 2.0f*fTrackBallRadius );
	if( t > +1.0f) t = +1.0f;
	if( t < -1.0f) t = -1.0f;
	FLOAT fAngle = 2.0f * asinf( t );
	
	// Convert axis to quaternion
	D3DXQUATERNION quat;
	D3DXQuaternionRotationAxis( &quat, &vAxis, fAngle );
	return quat;
}




// Gives the D3D device a cursor with image and hotspot from hCursor.

HRESULT D3DUtil_SetDeviceCursor( LPDIRECT3DDEVICE9 pd3dDevice, HCURSOR hCursor, BOOL bAddWatermark )
{
	HRESULT hr = E_FAIL;
	ICONINFO iconinfo;
	BOOL bBWCursor;
	LPDIRECT3DSURFACE9 pCursorSurface = NULL;
	HDC hdcColor = NULL;
	HDC hdcMask = NULL;
	HDC hdcScreen = NULL;
	BITMAP bm;
	DWORD dwWidth;
	DWORD dwHeightSrc;
	DWORD dwHeightDest;
	COLORREF crColor;
	COLORREF crMask;
	UINT x;
	UINT y;
	BITMAPINFO bmi;
	COLORREF* pcrArrayColor = NULL;
	COLORREF* pcrArrayMask = NULL;
	DWORD* pBitmap;
	HGDIOBJ hgdiobjOld;
	
	ZeroMemory( &iconinfo, sizeof(iconinfo) );
	if( !GetIconInfo( hCursor, &iconinfo ) )
		goto End;
	
	if (0 == GetObject((HGDIOBJ)iconinfo.hbmMask, sizeof(BITMAP), (LPVOID)&bm))
		goto End;
	dwWidth = bm.bmWidth;
	dwHeightSrc = bm.bmHeight;
	
	if( iconinfo.hbmColor == NULL )
	{
		bBWCursor = TRUE;
		dwHeightDest = dwHeightSrc / 2;
	}
	else 
	{
		bBWCursor = FALSE;
		dwHeightDest = dwHeightSrc;
	}
	
	// Create a surface for the fullscreen cursor
	if( FAILED( hr = pd3dDevice->CreateOffscreenPlainSurface( dwWidth, dwHeightDest, 
		D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, &pCursorSurface, NULL ) ) )
	{
		goto End;
	}
	
	pcrArrayMask = new DWORD[dwWidth * dwHeightSrc];
	
	ZeroMemory(&bmi, sizeof(bmi));
	bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
	bmi.bmiHeader.biWidth = dwWidth;
	bmi.bmiHeader.biHeight = dwHeightSrc;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	
	hdcScreen = GetDC( NULL );
	hdcMask = CreateCompatibleDC( hdcScreen );
	if( hdcMask == NULL )
	{
		hr = E_FAIL;
		goto End;
	}
	hgdiobjOld = SelectObject(hdcMask, iconinfo.hbmMask);
	GetDIBits(hdcMask, iconinfo.hbmMask, 0, dwHeightSrc, 
		pcrArrayMask, &bmi, DIB_RGB_COLORS);
	SelectObject(hdcMask, hgdiobjOld);
	
	if (!bBWCursor)
	{
		pcrArrayColor = new DWORD[dwWidth * dwHeightDest];
		hdcColor = CreateCompatibleDC( hdcScreen );
		if( hdcColor == NULL )
		{
			hr = E_FAIL;
			goto End;
		}
		SelectObject(hdcColor, iconinfo.hbmColor);
		GetDIBits(hdcColor, iconinfo.hbmColor, 0, dwHeightDest, 
			pcrArrayColor, &bmi, DIB_RGB_COLORS);
	}
	
	// Transfer cursor image into the surface
	D3DLOCKED_RECT lr;
	pCursorSurface->LockRect( &lr, NULL, 0 );
	pBitmap = (DWORD*)lr.pBits;
	for( y = 0; y < dwHeightDest; y++ )
	{
		for( x = 0; x < dwWidth; x++ )
		{
			if (bBWCursor)
			{
				crColor = pcrArrayMask[dwWidth*(dwHeightDest-1-y) + x];
				crMask = pcrArrayMask[dwWidth*(dwHeightSrc-1-y) + x];
			}
			else
			{
				crColor = pcrArrayColor[dwWidth*(dwHeightDest-1-y) + x];
				crMask = pcrArrayMask[dwWidth*(dwHeightDest-1-y) + x];
			}

			if (crMask == 0)
				pBitmap[dwWidth*y + x] = 0xff000000 | crColor;
			else
				pBitmap[dwWidth*y + x] = 0x00000000;
		}
	}

	pCursorSurface->UnlockRect();
	
	// Set the device cursor
	if( FAILED( hr = pd3dDevice->SetCursorProperties( iconinfo.xHotspot, 
		iconinfo.yHotspot, pCursorSurface ) ) )
	{
		goto End;
	}
	
	hr = S_OK;
	
End:
	if( iconinfo.hbmMask != NULL )
		DeleteObject( iconinfo.hbmMask );
	if( iconinfo.hbmColor != NULL )
		DeleteObject( iconinfo.hbmColor );
	if( hdcScreen != NULL )
		ReleaseDC( NULL, hdcScreen );
	if( hdcColor != NULL )
		DeleteDC( hdcColor );
	if( hdcMask != NULL )
		DeleteDC( hdcMask );
	SAFE_DELETE_ARRAY( pcrArrayColor );
	SAFE_DELETE_ARRAY( pcrArrayMask );
	SAFE_RELEASE( pCursorSurface );
	return hr;
}



// Returns the string for the given D3DFORMAT.

TCHAR* D3DUtil_D3DFormatToString( D3DFORMAT format, BOOL bWithPrefix )
{
	TCHAR* pstr = NULL;
	switch( format )
	{
	case D3DFMT_UNKNOWN:         pstr = TEXT("D3DFMT_UNKNOWN"); break;
	case D3DFMT_R8G8B8:          pstr = TEXT("D3DFMT_R8G8B8"); break;
	case D3DFMT_A8R8G8B8:        pstr = TEXT("D3DFMT_A8R8G8B8"); break;
	case D3DFMT_X8R8G8B8:        pstr = TEXT("D3DFMT_X8R8G8B8"); break;
	case D3DFMT_R5G6B5:          pstr = TEXT("D3DFMT_R5G6B5"); break;
	case D3DFMT_X1R5G5B5:        pstr = TEXT("D3DFMT_X1R5G5B5"); break;
	case D3DFMT_A1R5G5B5:        pstr = TEXT("D3DFMT_A1R5G5B5"); break;
	case D3DFMT_A4R4G4B4:        pstr = TEXT("D3DFMT_A4R4G4B4"); break;
	case D3DFMT_R3G3B2:          pstr = TEXT("D3DFMT_R3G3B2"); break;
	case D3DFMT_A8:              pstr = TEXT("D3DFMT_A8"); break;
	case D3DFMT_A8R3G3B2:        pstr = TEXT("D3DFMT_A8R3G3B2"); break;
	case D3DFMT_X4R4G4B4:        pstr = TEXT("D3DFMT_X4R4G4B4"); break;
	case D3DFMT_A2B10G10R10:     pstr = TEXT("D3DFMT_A2B10G10R10"); break;
	case D3DFMT_A8B8G8R8:        pstr = TEXT("D3DFMT_A8B8G8R8"); break;
	case D3DFMT_X8B8G8R8:        pstr = TEXT("D3DFMT_X8B8G8R8"); break;
	case D3DFMT_G16R16:          pstr = TEXT("D3DFMT_G16R16"); break;
	case D3DFMT_A2R10G10B10:     pstr = TEXT("D3DFMT_A2R10G10B10"); break;
	case D3DFMT_A16B16G16R16:    pstr = TEXT("D3DFMT_A16B16G16R16"); break;
	case D3DFMT_A8P8:            pstr = TEXT("D3DFMT_A8P8"); break;
	case D3DFMT_P8:              pstr = TEXT("D3DFMT_P8"); break;
	case D3DFMT_L8:              pstr = TEXT("D3DFMT_L8"); break;
	case D3DFMT_A8L8:            pstr = TEXT("D3DFMT_A8L8"); break;
	case D3DFMT_A4L4:            pstr = TEXT("D3DFMT_A4L4"); break;
	case D3DFMT_V8U8:            pstr = TEXT("D3DFMT_V8U8"); break;
	case D3DFMT_L6V5U5:          pstr = TEXT("D3DFMT_L6V5U5"); break;
	case D3DFMT_X8L8V8U8:        pstr = TEXT("D3DFMT_X8L8V8U8"); break;
	case D3DFMT_Q8W8V8U8:        pstr = TEXT("D3DFMT_Q8W8V8U8"); break;
	case D3DFMT_V16U16:          pstr = TEXT("D3DFMT_V16U16"); break;
	case D3DFMT_A2W10V10U10:     pstr = TEXT("D3DFMT_A2W10V10U10"); break;
	case D3DFMT_UYVY:            pstr = TEXT("D3DFMT_UYVY"); break;
	case D3DFMT_YUY2:            pstr = TEXT("D3DFMT_YUY2"); break;
	case D3DFMT_DXT1:            pstr = TEXT("D3DFMT_DXT1"); break;
	case D3DFMT_DXT2:            pstr = TEXT("D3DFMT_DXT2"); break;
	case D3DFMT_DXT3:            pstr = TEXT("D3DFMT_DXT3"); break;
	case D3DFMT_DXT4:            pstr = TEXT("D3DFMT_DXT4"); break;
	case D3DFMT_DXT5:            pstr = TEXT("D3DFMT_DXT5"); break;
	case D3DFMT_D16_LOCKABLE:    pstr = TEXT("D3DFMT_D16_LOCKABLE"); break;
	case D3DFMT_D32:             pstr = TEXT("D3DFMT_D32"); break;
	case D3DFMT_D15S1:           pstr = TEXT("D3DFMT_D15S1"); break;
	case D3DFMT_D24S8:           pstr = TEXT("D3DFMT_D24S8"); break;
	case D3DFMT_D24X8:           pstr = TEXT("D3DFMT_D24X8"); break;
	case D3DFMT_D24X4S4:         pstr = TEXT("D3DFMT_D24X4S4"); break;
	case D3DFMT_D16:             pstr = TEXT("D3DFMT_D16"); break;
	case D3DFMT_L16:             pstr = TEXT("D3DFMT_L16"); break;
	case D3DFMT_VERTEXDATA:      pstr = TEXT("D3DFMT_VERTEXDATA"); break;
	case D3DFMT_INDEX16:         pstr = TEXT("D3DFMT_INDEX16"); break;
	case D3DFMT_INDEX32:         pstr = TEXT("D3DFMT_INDEX32"); break;
	case D3DFMT_Q16W16V16U16:    pstr = TEXT("D3DFMT_Q16W16V16U16"); break;
	case D3DFMT_MULTI2_ARGB8:    pstr = TEXT("D3DFMT_MULTI2_ARGB8"); break;
	case D3DFMT_R16F:            pstr = TEXT("D3DFMT_R16F"); break;
	case D3DFMT_G16R16F:         pstr = TEXT("D3DFMT_G16R16F"); break;
	case D3DFMT_A16B16G16R16F:   pstr = TEXT("D3DFMT_A16B16G16R16F"); break;
	case D3DFMT_R32F:            pstr = TEXT("D3DFMT_R32F"); break;
	case D3DFMT_G32R32F:         pstr = TEXT("D3DFMT_G32R32F"); break;
	case D3DFMT_A32B32G32R32F:   pstr = TEXT("D3DFMT_A32B32G32R32F"); break;
	case D3DFMT_CxV8U8:          pstr = TEXT("D3DFMT_CxV8U8"); break;
	default:                     pstr = TEXT("Unknown format"); break;
	}
	if( bWithPrefix || _tcsstr( pstr, TEXT("D3DFMT_") )== NULL )
		return pstr;
	else
		return pstr + lstrlen( TEXT("D3DFMT_") );
}


// Axis to axis quaternion double angle (no normalization)
// Takes two points on unit sphere an angle THETA apart, returns
// quaternion that represents a rotation around cross product by 2*THETA.

inline D3DXQUATERNION* WINAPI D3DXQuaternionUnitAxisToUnitAxis2
( D3DXQUATERNION *pOut, const D3DXVECTOR3 *pvFrom, const D3DXVECTOR3 *pvTo)
{
	D3DXVECTOR3 vAxis;
	D3DXVec3Cross(&vAxis, pvFrom, pvTo);    // proportional to sin(theta)
	pOut->x = vAxis.x;
	pOut->y = vAxis.y;
	pOut->z = vAxis.z;
	pOut->w = D3DXVec3Dot( pvFrom, pvTo );
	return pOut;
}



// Axis to axis quaternion 
// Takes two points on unit sphere an angle THETA apart, returns
// quaternion that represents a rotation around cross product by theta.

inline D3DXQUATERNION* WINAPI D3DXQuaternionAxisToAxis
( D3DXQUATERNION *pOut, const D3DXVECTOR3 *pvFrom, const D3DXVECTOR3 *pvTo)
{
	D3DXVECTOR3 vA, vB;
	D3DXVec3Normalize(&vA, pvFrom);
	D3DXVec3Normalize(&vB, pvTo);
	D3DXVECTOR3 vHalf(vA + vB);
	D3DXVec3Normalize(&vHalf, &vHalf);
	return D3DXQuaternionUnitAxisToUnitAxis2(pOut, &vA, &vHalf);
}







#ifdef UNICODE
typedef HINSTANCE (WINAPI* LPShellExecute)(HWND hwnd, LPCWSTR lpOperation, LPCWSTR lpFile, LPCWSTR lpParameters, LPCWSTR lpDirectory, INT nShowCmd);
#else
typedef HINSTANCE (WINAPI* LPShellExecute)(HWND hwnd, LPCSTR lpOperation, LPCSTR lpFile, LPCSTR lpParameters, LPCSTR lpDirectory, INT nShowCmd);
#endif


#ifndef UNDER_CE

// Returns the DirectX SDK media path
// cchDest is the size in TCHARs of strDest.  Be careful not to 
// pass in sizeof(strDest) on UNICODE builds.

HRESULT DXUtil_GetDXSDKMediaPathCch( TCHAR* strDest, int cchDest )
{
	if( strDest == NULL || cchDest < 1 )
		return E_INVALIDARG;
	
	lstrcpy( strDest, TEXT("") );
	
	// Open the appropriate registry key
	HKEY  hKey;
	LONG lResult = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
		_T("Software\\Microsoft\\DirectX SDK"),
		0, KEY_READ, &hKey );
	if( ERROR_SUCCESS != lResult )
		return E_FAIL;
	
	DWORD dwType;
	DWORD dwSize = cchDest * sizeof(TCHAR);
	lResult = RegQueryValueEx( hKey, _T("DX9SDK Samples Path"), NULL,
		&dwType, (BYTE*)strDest, &dwSize );
	strDest[cchDest-1] = 0; // RegQueryValueEx doesn't NULL term if buffer too small
	RegCloseKey( hKey );
	
	if( ERROR_SUCCESS != lResult )
		return E_FAIL;
	
	const TCHAR* strMedia = _T("\\Media\\");
	if( lstrlen(strDest) + lstrlen(strMedia) < cchDest )
		_tcscat( strDest, strMedia );
	else
		return E_INVALIDARG;
	
	return S_OK;
}
#endif // !UNDER_CE


#ifndef UNDER_CE

// Returns a valid path to a DXSDK media file
// cchDest is the size in TCHARs of strDestPath.  Be careful not to 
// pass in sizeof(strDest) on UNICODE builds.

HRESULT DXUtil_FindMediaFileCch( TCHAR* strDestPath, int cchDest, TCHAR* strFilename )
{
	HRESULT hr;
	HANDLE file;
	TCHAR* strShortNameTmp = NULL;
	TCHAR strShortName[MAX_PATH];
	int cchPath;
	
	if( NULL==strFilename || NULL==strDestPath || cchDest < 1 )
		return E_INVALIDARG;
	
	lstrcpy( strDestPath, TEXT("") );
	lstrcpy( strShortName, TEXT("") );
	
	// Build full path name from strFileName (strShortName will be just the leaf filename)
	cchPath = GetFullPathName(strFilename, cchDest, strDestPath, &strShortNameTmp);
	if ((cchPath == 0) || (cchDest <= cchPath))
		return E_FAIL;
	if( strShortNameTmp )
		lstrcpyn( strShortName, strShortNameTmp, MAX_PATH );
	
	// first try to find the filename given a full path
	file = CreateFile( strDestPath, GENERIC_READ, FILE_SHARE_READ, NULL, 
		OPEN_EXISTING, 0, NULL );
	if( INVALID_HANDLE_VALUE != file )
	{
		CloseHandle( file );
		return S_OK;
	}
	
	// next try to find the filename in the current working directory (path stripped)
	file = CreateFile( strShortName, GENERIC_READ, FILE_SHARE_READ, NULL, 
		OPEN_EXISTING, 0, NULL );
	if( INVALID_HANDLE_VALUE != file )
	{
		_tcsncpy( strDestPath, strShortName, cchDest );
		strDestPath[cchDest-1] = 0; // _tcsncpy doesn't NULL term if it runs out of space
		CloseHandle( file );
		return S_OK;
	}
	
	// last, check if the file exists in the media directory
	if( FAILED( hr = DXUtil_GetDXSDKMediaPathCch( strDestPath, cchDest ) ) )
		return hr;
	
	if( lstrlen(strDestPath) + lstrlen(strShortName) < cchDest )
		lstrcat( strDestPath, strShortName );
	else
		return E_INVALIDARG;
	
	file = CreateFile( strDestPath, GENERIC_READ, FILE_SHARE_READ, NULL, 
		OPEN_EXISTING, 0, NULL );
	if( INVALID_HANDLE_VALUE != file )
	{
		CloseHandle( file );
		return S_OK;
	}
	
	// On failure, just return the file as the path
	_tcsncpy( strDestPath, strFilename, cchDest );
	strDestPath[cchDest-1] = 0; // _tcsncpy doesn't NULL term if it runs out of space
	return HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND );
}
#endif // !UNDER_CE



// Helper function to read a registry key string
// cchDest is the size in TCHARs of strDest.  Be careful not to 
// pass in sizeof(strDest) on UNICODE builds.

HRESULT DXUtil_ReadStringRegKeyCch( HKEY hKey, TCHAR* strRegName, TCHAR* strDest, 
								   DWORD cchDest, TCHAR* strDefault )
{
	DWORD dwType;
	DWORD cbDest = cchDest * sizeof(TCHAR);
	
	if( ERROR_SUCCESS != RegQueryValueEx( hKey, strRegName, 0, &dwType, 
		(BYTE*)strDest, &cbDest ) )
	{
		_tcsncpy( strDest, strDefault, cchDest );
		strDest[cchDest-1] = 0;
		
		if( dwType != REG_SZ )
			return E_FAIL;
		
		return S_OK;
	}
	
	return E_FAIL;
}




// Helper function to write a registry key string

HRESULT DXUtil_WriteStringRegKey( HKEY hKey, TCHAR* strRegName,
								 TCHAR* strValue )
{
	if( NULL == strValue )
		return E_INVALIDARG;
	
	DWORD cbValue = ((DWORD)_tcslen(strValue)+1) * sizeof(TCHAR);
	
	if( ERROR_SUCCESS != RegSetValueEx( hKey, strRegName, 0, REG_SZ, 
		(BYTE*)strValue, cbValue ) )
		return E_FAIL;
	
	return S_OK;
}




// Helper function to read a registry key int

HRESULT DXUtil_ReadIntRegKey( HKEY hKey, TCHAR* strRegName, DWORD* pdwDest, 
							 DWORD dwDefault )
{
	DWORD dwType;
	DWORD dwLength = sizeof(DWORD);
	
	if( ERROR_SUCCESS != RegQueryValueEx( hKey, strRegName, 0, &dwType, 
		(BYTE*)pdwDest, &dwLength ) )
	{
		*pdwDest = dwDefault;
		if( dwType != REG_DWORD )
			return E_FAIL;
		
		return S_OK;
	}
	
	return E_FAIL;
}



// Helper function to write a registry key int

HRESULT DXUtil_WriteIntRegKey( HKEY hKey, TCHAR* strRegName, DWORD dwValue )
{
	if( ERROR_SUCCESS != RegSetValueEx( hKey, strRegName, 0, REG_DWORD, 
		(BYTE*)&dwValue, sizeof(DWORD) ) )
		return E_FAIL;
	
	return S_OK;
}




// Helper function to read a registry key BOOL

HRESULT DXUtil_ReadBoolRegKey( HKEY hKey, TCHAR* strRegName, BOOL* pbDest, BOOL bDefault )
{
	DWORD dwType;
	DWORD dwLength = sizeof(BOOL);
	
	if( ERROR_SUCCESS != RegQueryValueEx( hKey, strRegName, 0, &dwType, 
		(BYTE*)pbDest, &dwLength ) )
	{
		*pbDest = bDefault;
		if( dwType != REG_DWORD )
			return E_FAIL;
		
		return S_OK;
	}
	
	return E_FAIL;
}



// Helper function to write a registry key BOOL

HRESULT DXUtil_WriteBoolRegKey( HKEY hKey, TCHAR* strRegName, BOOL bValue )
{
	if( ERROR_SUCCESS != RegSetValueEx( hKey, strRegName, 0, REG_DWORD, 
		(BYTE*)&bValue, sizeof(BOOL) ) )
		return E_FAIL;
	
	return S_OK;
}




// Helper function to read a registry key guid

HRESULT DXUtil_ReadGuidRegKey( HKEY hKey, TCHAR* strRegName, GUID* pGuidDest, 
							  GUID& guidDefault )
{
	DWORD dwType;
	DWORD dwLength = sizeof(GUID);
	
	if( ERROR_SUCCESS != RegQueryValueEx( hKey, strRegName, 0, &dwType, 
		(LPBYTE) pGuidDest, &dwLength ) )
	{
		*pGuidDest = guidDefault;
		if( dwType != REG_BINARY )
			return E_FAIL;
		
		return S_OK;
	}
	
	return E_FAIL;
}



// Helper function to write a registry key guid

HRESULT DXUtil_WriteGuidRegKey( HKEY hKey, TCHAR* strRegName, GUID guidValue )
{
	if( ERROR_SUCCESS != RegSetValueEx( hKey, strRegName, 0, REG_BINARY, 
		(BYTE*)&guidValue, sizeof(GUID) ) )
		return E_FAIL;
	
	return S_OK;
}




// Performs timer opertations. Use the following commands:
//    TIMER_RESET           - to reset the timer
//    TIMER_START           - to start the timer
//    TIMER_STOP            - to stop (or pause) the timer
//    TIMER_ADVANCE         - to advance the timer by 0.1 seconds
//    TIMER_GETABSOLUTETIME - to get the absolute system time
//    TIMER_GETAPPTIME      - to get the current time
//    TIMER_GETELAPSEDTIME  - to get the time that elapsed between 
//                            TIMER_GETELAPSEDTIME calls

FLOAT __stdcall DXUtil_Timer( TIMER_COMMAND command )
{
	static BOOL     m_bTimerInitialized = FALSE;
	static BOOL     m_bUsingQPF         = FALSE;
	static BOOL     m_bTimerStopped     = TRUE;
	static LONGLONG m_llQPFTicksPerSec  = 0;
	
	// Initialize the timer
	if( FALSE == m_bTimerInitialized )
	{
		m_bTimerInitialized = TRUE;
		
		// Use QueryPerformanceFrequency() to get frequency of timer.  If QPF is
		// not supported, we will timeGetTime() which returns milliseconds.
		LARGE_INTEGER qwTicksPerSec;
		m_bUsingQPF = QueryPerformanceFrequency( &qwTicksPerSec );
		if( m_bUsingQPF )
			m_llQPFTicksPerSec = qwTicksPerSec.QuadPart;
	}
	
	if( m_bUsingQPF )
	{
		static LONGLONG m_llStopTime        = 0;
		static LONGLONG m_llLastElapsedTime = 0;
		static LONGLONG m_llBaseTime        = 0;
		double fTime;
		double fElapsedTime;
		LARGE_INTEGER qwTime;
		
		// Get either the current time or the stop time, depending
		// on whether we're stopped and what command was sent
		if( m_llStopTime != 0 && command != TIMER_START && command != TIMER_GETABSOLUTETIME)
			qwTime.QuadPart = m_llStopTime;
		else
			QueryPerformanceCounter( &qwTime );
		
		// Return the elapsed time
		if( command == TIMER_GETELAPSEDTIME )
		{
			fElapsedTime = (double) ( qwTime.QuadPart - m_llLastElapsedTime ) / (double) m_llQPFTicksPerSec;
			m_llLastElapsedTime = qwTime.QuadPart;
			return (FLOAT) fElapsedTime;
		}
		
		// Return the current time
		if( command == TIMER_GETAPPTIME )
		{
			double fAppTime = (double) ( qwTime.QuadPart - m_llBaseTime ) / (double) m_llQPFTicksPerSec;
			return (FLOAT) fAppTime;
		}
		
		// Reset the timer
		if( command == TIMER_RESET )
		{
			m_llBaseTime        = qwTime.QuadPart;
			m_llLastElapsedTime = qwTime.QuadPart;
			m_llStopTime        = 0;
			m_bTimerStopped     = FALSE;
			return 0.0f;
		}
		
		// Start the timer
		if( command == TIMER_START )
		{
			if( m_bTimerStopped )
				m_llBaseTime += qwTime.QuadPart - m_llStopTime;
			m_llStopTime = 0;
			m_llLastElapsedTime = qwTime.QuadPart;
			m_bTimerStopped = FALSE;
			return 0.0f;
		}
		
		// Stop the timer
		if( command == TIMER_STOP )
		{
			if( !m_bTimerStopped )
			{
				m_llStopTime = qwTime.QuadPart;
				m_llLastElapsedTime = qwTime.QuadPart;
				m_bTimerStopped = TRUE;
			}
			return 0.0f;
		}
		
		// Advance the timer by 1/10th second
		if( command == TIMER_ADVANCE )
		{
			m_llStopTime += m_llQPFTicksPerSec/10;
			return 0.0f;
		}
		
		if( command == TIMER_GETABSOLUTETIME )
		{
			fTime = qwTime.QuadPart / (double) m_llQPFTicksPerSec;
			return (FLOAT) fTime;
		}
		
		return -1.0f; // Invalid command specified
	}
	else
	{
		// Get the time using timeGetTime()
		static double m_fLastElapsedTime  = 0.0;
		static double m_fBaseTime         = 0.0;
		static double m_fStopTime         = 0.0;
		double fTime;
		double fElapsedTime;
		
		// Get either the current time or the stop time, depending
		// on whether we're stopped and what command was sent
		if( m_fStopTime != 0.0 && command != TIMER_START && command != TIMER_GETABSOLUTETIME)
			fTime = m_fStopTime;
		else
			fTime = GETTIMESTAMP() * 0.001;
		
		// Return the elapsed time
		if( command == TIMER_GETELAPSEDTIME )
		{   
			fElapsedTime = (double) (fTime - m_fLastElapsedTime);
			m_fLastElapsedTime = fTime;
			return (FLOAT) fElapsedTime;
		}
		
		// Return the current time
		if( command == TIMER_GETAPPTIME )
		{
			return (FLOAT) (fTime - m_fBaseTime);
		}
		
		// Reset the timer
		if( command == TIMER_RESET )
		{
			m_fBaseTime         = fTime;
			m_fLastElapsedTime  = fTime;
			m_fStopTime         = 0;
			m_bTimerStopped     = FALSE;
			return 0.0f;
		}
		
		// Start the timer
		if( command == TIMER_START )
		{
			if( m_bTimerStopped )
				m_fBaseTime += fTime - m_fStopTime;
			m_fStopTime = 0.0f;
			m_fLastElapsedTime  = fTime;
			m_bTimerStopped = FALSE;
			return 0.0f;
		}
		
		// Stop the timer
		if( command == TIMER_STOP )
		{
			if( !m_bTimerStopped )
			{
				m_fStopTime = fTime;
				m_fLastElapsedTime  = fTime;
				m_bTimerStopped = TRUE;
			}
			return 0.0f;
		}
		
		// Advance the timer by 1/10th second
		if( command == TIMER_ADVANCE )
		{
			m_fStopTime += 0.1f;
			return 0.0f;
		}
		
		if( command == TIMER_GETABSOLUTETIME )
		{
			return (FLOAT) fTime;
		}
		
		return -1.0f; // Invalid command specified
	}
}



// This is a UNICODE conversion utility to convert a CHAR string into a
// WCHAR string. 
// cchDestChar is the size in TCHARs of wstrDestination.  Be careful not to 
// pass in sizeof(strDest) 

HRESULT DXUtil_ConvertAnsiStringToWideCch( WCHAR* wstrDestination, const CHAR* strSource, 
										  int cchDestChar )
{
	if( wstrDestination==NULL || strSource==NULL || cchDestChar < 1 )
		return E_INVALIDARG;
	
	int nResult = MultiByteToWideChar( CP_ACP, 0, strSource, -1, 
		wstrDestination, cchDestChar );
	wstrDestination[cchDestChar-1] = 0;
	
	if( nResult == 0 )
		return E_FAIL;
	return S_OK;
}



// This is a UNICODE conversion utility to convert a WCHAR string into a
// CHAR string. 
// cchDestChar is the size in TCHARs of strDestination

HRESULT DXUtil_ConvertWideStringToAnsiCch( CHAR* strDestination, const WCHAR* wstrSource, 
										  int cchDestChar )
{
	if( strDestination==NULL || wstrSource==NULL || cchDestChar < 1 )
		return E_INVALIDARG;
	
	int nResult = WideCharToMultiByte( CP_ACP, 0, wstrSource, -1, strDestination, 
		cchDestChar*sizeof(CHAR), NULL, NULL );
	strDestination[cchDestChar-1] = 0;
	
	if( nResult == 0 )
		return E_FAIL;
	return S_OK;
}



// This is a UNICODE conversion utility to convert a TCHAR string into a
// CHAR string. 
// cchDestChar is the size in TCHARs of strDestination

HRESULT DXUtil_ConvertGenericStringToAnsiCch( CHAR* strDestination, const TCHAR* tstrSource, 
											 int cchDestChar )
{
	if( strDestination==NULL || tstrSource==NULL || cchDestChar < 1 )
		return E_INVALIDARG;
	
#ifdef _UNICODE
	return DXUtil_ConvertWideStringToAnsiCch( strDestination, tstrSource, cchDestChar );
#else
	strncpy( strDestination, tstrSource, cchDestChar );
	strDestination[cchDestChar-1] = '\0';
	return S_OK;
#endif   
}




// This is a UNICODE conversion utility to convert a TCHAR string into a
// WCHAR string. 
// cchDestChar is the size in TCHARs of wstrDestination.  Be careful not to 
// pass in sizeof(strDest) 

HRESULT DXUtil_ConvertGenericStringToWideCch( WCHAR* wstrDestination, const TCHAR* tstrSource, 
											 int cchDestChar )
{
	if( wstrDestination==NULL || tstrSource==NULL || cchDestChar < 1 )
		return E_INVALIDARG;
	
#ifdef _UNICODE
	wcsncpy( wstrDestination, tstrSource, cchDestChar );
	wstrDestination[cchDestChar-1] = L'\0';
	return S_OK;
#else
	return DXUtil_ConvertAnsiStringToWideCch( wstrDestination, tstrSource, cchDestChar );
#endif    
}




// This is a UNICODE conversion utility to convert a CHAR string into a
// TCHAR string. 
// cchDestChar is the size in TCHARs of tstrDestination.  Be careful not to 
// pass in sizeof(strDest) on UNICODE builds

HRESULT DXUtil_ConvertAnsiStringToGenericCch( TCHAR* tstrDestination, const CHAR* strSource, 
											 int cchDestChar )
{
	if( tstrDestination==NULL || strSource==NULL || cchDestChar < 1 )
		return E_INVALIDARG;
	
#ifdef _UNICODE
	return DXUtil_ConvertAnsiStringToWideCch( tstrDestination, strSource, cchDestChar );
#else
	strncpy( tstrDestination, strSource, cchDestChar );
	tstrDestination[cchDestChar-1] = '\0';
	return S_OK;
#endif    
}




// This is a UNICODE conversion utility to convert a WCHAR string into a
// TCHAR string. 
// cchDestChar is the size in TCHARs of tstrDestination.  Be careful not to 
// pass in sizeof(strDest) on UNICODE builds

HRESULT DXUtil_ConvertWideStringToGenericCch( TCHAR* tstrDestination, const WCHAR* wstrSource, 
											 int cchDestChar )
{
	if( tstrDestination==NULL || wstrSource==NULL || cchDestChar < 1 )
		return E_INVALIDARG;
	
#ifdef _UNICODE
	wcsncpy( tstrDestination, wstrSource, cchDestChar );
	tstrDestination[cchDestChar-1] = L'\0';    
	return S_OK;
#else
	return DXUtil_ConvertWideStringToAnsiCch( tstrDestination, wstrSource, cchDestChar );
#endif
}



// Finds and opens the readme.txt for this sample

VOID DXUtil_LaunchReadme( HWND hWnd, TCHAR* strLoc )
{
	
#ifdef UNDER_CE
	// This is not available on PocketPC
	MessageBox( hWnd, TEXT("For operating instructions, please open the ")
		TEXT("readme.txt file included with the project."),
		TEXT("DirectX SDK Sample"), MB_ICONWARNING | MB_OK );
	
	return;
#else 
	
	BOOL bSuccess = FALSE;
	BOOL bFound = FALSE;
	TCHAR strReadmePath[1024];
	TCHAR strExeName[MAX_PATH];
	TCHAR strExePath[MAX_PATH];
	TCHAR strSamplePath[MAX_PATH];
	TCHAR* strLastSlash = NULL;
	
	lstrcpy( strReadmePath, TEXT("") );
	lstrcpy( strExePath, TEXT("") );
	lstrcpy( strExeName, TEXT("") );
	lstrcpy( strSamplePath, TEXT("") );
	
	// If the user provided a location for the readme, check there first.
	if( strLoc )
	{
		HKEY  hKey;
		LONG lResult = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
			_T("Software\\Microsoft\\DirectX SDK"),
			0, KEY_READ, &hKey );
		if( ERROR_SUCCESS == lResult )
		{
			DWORD dwType;
			DWORD dwSize = MAX_PATH * sizeof(TCHAR);
			lResult = RegQueryValueEx( hKey, _T("DX9SDK Samples Path"), NULL,
				&dwType, (BYTE*)strSamplePath, &dwSize );
			strSamplePath[MAX_PATH-1] = 0; // RegQueryValueEx doesn't NULL term if buffer too small
			
			if( ERROR_SUCCESS == lResult )
			{
				_sntprintf( strReadmePath, 1023, TEXT("%s\\C++\\%s\\readme.txt"), 
					strSamplePath, strLoc );
				strReadmePath[1023] = 0;
				
				if( GetFileAttributes( strReadmePath ) != 0xFFFFFFFF )
					bFound = TRUE;
			}
		}
		
		RegCloseKey( hKey );
	}
	
	// Get the exe name, and exe path
	GetModuleFileName( NULL, strExePath, MAX_PATH );
	strExePath[MAX_PATH-1]=0;
	
	strLastSlash = _tcsrchr( strExePath, TEXT('\\') );
	if( strLastSlash )
	{
		_tcsncpy( strExeName, &strLastSlash[1], MAX_PATH );
		strExeName[MAX_PATH-1]=0;
		
		// Chop the exe name from the exe path
		*strLastSlash = 0;
		
		// Chop the .exe from the exe name
		strLastSlash = _tcsrchr( strExeName, TEXT('.') );
		if( strLastSlash )
			*strLastSlash = 0;
	}
	
	if( !bFound )
	{
		// Search in "%EXE_DIR%\..\%EXE_NAME%".  This matchs the DirectX SDK layout
		_tcscpy( strReadmePath, strExePath );
		
		strLastSlash = _tcsrchr( strReadmePath, TEXT('\\') );
		if( strLastSlash )
			*strLastSlash = 0;
		lstrcat( strReadmePath, TEXT("\\") );
		lstrcat( strReadmePath, strExeName );
		lstrcat( strReadmePath, TEXT("\\readme.txt") );
		if( GetFileAttributes( strReadmePath ) != 0xFFFFFFFF )
			bFound = TRUE;
	}
	
	if( !bFound )
	{
		// Search in "%EXE_DIR%\"
		_tcscpy( strReadmePath, strExePath );
		lstrcat( strReadmePath, TEXT("\\readme.txt") );
		if( GetFileAttributes( strReadmePath ) != 0xFFFFFFFF )
			bFound = TRUE;
	}
	
	if( !bFound )
	{
		// Search in "%EXE_DIR%\.."
		_tcscpy( strReadmePath, strExePath );
		strLastSlash = _tcsrchr( strReadmePath, TEXT('\\') );
		if( strLastSlash )
			*strLastSlash = 0;
		lstrcat( strReadmePath, TEXT("\\readme.txt") );
		if( GetFileAttributes( strReadmePath ) != 0xFFFFFFFF )
			bFound = TRUE;
	}
	
	if( !bFound )
	{
		// Search in "%EXE_DIR%\..\.."
		_tcscpy( strReadmePath, strExePath );
		strLastSlash = _tcsrchr( strReadmePath, TEXT('\\') );
		if( strLastSlash )
			*strLastSlash = 0;
		strLastSlash = _tcsrchr( strReadmePath, TEXT('\\') );
		if( strLastSlash )
			*strLastSlash = 0;
		lstrcat( strReadmePath, TEXT("\\readme.txt") );
		if( GetFileAttributes( strReadmePath ) != 0xFFFFFFFF )
			bFound = TRUE;
	}
	
	if( bFound )
	{
		// GetProcAddress for ShellExecute, so we don't have to include shell32.lib 
		// in every project that uses dxutil.cpp
		LPShellExecute pShellExecute = NULL;
		HINSTANCE hInstShell32 = LoadLibrary(TEXT("shell32.dll"));
		if (hInstShell32 != NULL)
		{
#ifdef UNICODE
			pShellExecute = (LPShellExecute)GetProcAddress(hInstShell32, _TWINCE("ShellExecuteW"));
#else
			pShellExecute = (LPShellExecute)GetProcAddress(hInstShell32, _TWINCE("ShellExecuteA"));
#endif
			if( pShellExecute != NULL )
			{
				if( pShellExecute( hWnd, TEXT("open"), strReadmePath, NULL, NULL, SW_SHOW ) > (HINSTANCE) 32 )
					bSuccess = TRUE;
			}
			
			FreeLibrary(hInstShell32);
		}
	}
	
	if( !bSuccess )
	{
		// Tell the user that the readme couldn't be opened
		MessageBox( hWnd, TEXT("Could not find readme.txt"), 
			TEXT("DirectX SDK Sample"), MB_ICONWARNING | MB_OK );
	}
	
#endif // UNDER_CE
}




// Outputs to the debug stream a formatted string with a variable-
// argument list.

VOID DXUtil_Trace( TCHAR* strMsg, ... )
{
#if defined(DEBUG) | defined(_DEBUG)
	TCHAR strBuffer[512];
	
	va_list args;
	va_start(args, strMsg);
	_vsntprintf( strBuffer, 512, strMsg, args );
	va_end(args);
	
	OutputDebugString( strBuffer );
#else
	UNREFERENCED_PARAMETER(strMsg);
#endif
}




// Converts a string to a GUID

HRESULT DXUtil_ConvertStringToGUID( const TCHAR* strSrc, GUID* pGuidDest )
{
	UINT aiTmp[10];
	
	if( _stscanf( strSrc, TEXT("{%8X-%4X-%4X-%2X%2X-%2X%2X%2X%2X%2X%2X}"),
		&pGuidDest->Data1, 
		&aiTmp[0], &aiTmp[1], 
		&aiTmp[2], &aiTmp[3],
		&aiTmp[4], &aiTmp[5],
		&aiTmp[6], &aiTmp[7],
		&aiTmp[8], &aiTmp[9] ) != 11 )
	{
		ZeroMemory( pGuidDest, sizeof(GUID) );
		return E_FAIL;
	}
	else
	{
		pGuidDest->Data2       = (USHORT) aiTmp[0];
		pGuidDest->Data3       = (USHORT) aiTmp[1];
		pGuidDest->Data4[0]    = (BYTE) aiTmp[2];
		pGuidDest->Data4[1]    = (BYTE) aiTmp[3];
		pGuidDest->Data4[2]    = (BYTE) aiTmp[4];
		pGuidDest->Data4[3]    = (BYTE) aiTmp[5];
		pGuidDest->Data4[4]    = (BYTE) aiTmp[6];
		pGuidDest->Data4[5]    = (BYTE) aiTmp[7];
		pGuidDest->Data4[6]    = (BYTE) aiTmp[8];
		pGuidDest->Data4[7]    = (BYTE) aiTmp[9];
		return S_OK;
	}
}




// Converts a GUID to a string 
// cchDestChar is the size in TCHARs of strDest.  Be careful not to 
// pass in sizeof(strDest) on UNICODE builds

HRESULT DXUtil_ConvertGUIDToStringCch( const GUID* pGuidSrc, TCHAR* strDest, int cchDestChar )
{
	int nResult = _sntprintf( strDest, cchDestChar, TEXT("{%0.8X-%0.4X-%0.4X-%0.2X%0.2X-%0.2X%0.2X%0.2X%0.2X%0.2X%0.2X}"),
		pGuidSrc->Data1, pGuidSrc->Data2, pGuidSrc->Data3,
		pGuidSrc->Data4[0], pGuidSrc->Data4[1],
		pGuidSrc->Data4[2], pGuidSrc->Data4[3],
		pGuidSrc->Data4[4], pGuidSrc->Data4[5],
		pGuidSrc->Data4[6], pGuidSrc->Data4[7] );
	
	if( nResult < 0 )
		return E_FAIL;
	return S_OK;
}


// cchDestChar is the size in BYTEs of strDest.  Be careful not to 
// pass use sizeof() if the strDest is a string pointer.  
// eg.
// TCHAR* sz = new TCHAR[100]; // sizeof(sz)  == 4
// TCHAR sz2[100];             // sizeof(sz2) == 200

HRESULT DXUtil_ConvertAnsiStringToWideCb( WCHAR* wstrDestination, const CHAR* strSource, int cbDestChar )
{
	return DXUtil_ConvertAnsiStringToWideCch( wstrDestination, strSource, cbDestChar / sizeof(WCHAR) );
}

HRESULT DXUtil_ConvertWideStringToAnsiCb( CHAR* strDestination, const WCHAR* wstrSource, int cbDestChar )
{
	return DXUtil_ConvertWideStringToAnsiCch( strDestination, wstrSource, cbDestChar / sizeof(CHAR) );
}

HRESULT DXUtil_ConvertGenericStringToAnsiCb( CHAR* strDestination, const TCHAR* tstrSource, int cbDestChar )
{
	return DXUtil_ConvertGenericStringToAnsiCch( strDestination, tstrSource, cbDestChar / sizeof(CHAR) );
}

HRESULT DXUtil_ConvertGenericStringToWideCb( WCHAR* wstrDestination, const TCHAR* tstrSource, int cbDestChar )
{
	return DXUtil_ConvertGenericStringToWideCch( wstrDestination, tstrSource, cbDestChar / sizeof(WCHAR) );
}

HRESULT DXUtil_ConvertAnsiStringToGenericCb( TCHAR* tstrDestination, const CHAR* strSource, int cbDestChar )
{
	return DXUtil_ConvertAnsiStringToGenericCch( tstrDestination, strSource, cbDestChar / sizeof(TCHAR) );
}

HRESULT DXUtil_ConvertWideStringToGenericCb( TCHAR* tstrDestination, const WCHAR* wstrSource, int cbDestChar )
{
	return DXUtil_ConvertWideStringToGenericCch( tstrDestination, wstrSource, cbDestChar / sizeof(TCHAR) );
}

HRESULT DXUtil_ReadStringRegKeyCb( HKEY hKey, TCHAR* strRegName, TCHAR* strDest, DWORD cbDest, TCHAR* strDefault )
{
	return DXUtil_ReadStringRegKeyCch( hKey, strRegName, strDest, cbDest / sizeof(TCHAR), strDefault );
}

HRESULT DXUtil_ConvertGUIDToStringCb( const GUID* pGuidSrc, TCHAR* strDest, int cbDestChar )
{
	return DXUtil_ConvertGUIDToStringCch( pGuidSrc, strDest, cbDestChar / sizeof(TCHAR) );
}

#ifndef UNDER_CE
HRESULT DXUtil_GetDXSDKMediaPathCb( TCHAR* szDest, int cbDest )
{
	return DXUtil_GetDXSDKMediaPathCch( szDest, cbDest / sizeof(TCHAR) );
}

HRESULT DXUtil_FindMediaFileCb( TCHAR* szDestPath, int cbDest, TCHAR* strFilename )
{
	return DXUtil_FindMediaFileCch( szDestPath, cbDest / sizeof(TCHAR), strFilename );
}
#endif // !UNDER_CE
