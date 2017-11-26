// Implementation of the ILcFont class.
//
////////////////////////////////////////////////////////////////////////////////

#if _MSC_VER >= 1200
#pragma warning(disable : 4996)
#endif

#pragma warning(disable : 4786)

#include <string>
#include <vector>

#include <stdio.h>
#include <windows.h>
#include <tchar.h>


#include <Lc/LcType.h>

#include "ILcFont.h"
#include "LcFont2D.h"


TLcFnt::TLcFnt()
: iHeight	(12)
, iWeight	(FW_NORMAL)
, iItalic	(0)
, iThckX	(0)
, iThckY	(0)
, dString	(0xFFFFFFFF)
, dGirth	(0xFF000099)
{
	_tcscpy(sName, "Arial");
}

TLcFnt::TLcFnt(	TCHAR*	_sN						// Font Name
			,	INT		_iHeight				// Height
			,	INT		_iWeight				// Weight, Normal, Bold, thin...
			,	INT		_iItalic				// Is Italic
			,	INT		_iThckX					// Girth of thick X
			,	INT		_iThckY					// Girth of thick Y
			,	DWORD	_dString				// String color
			,	DWORD	_dGirth					// Girth of Font color
			)
			: iHeight	(_iHeight)
			, iWeight	(_iWeight)
			, iItalic	(_iItalic)
			, iThckX	(_iThckX)
			, iThckY	(_iThckY)
			, dString	(_dString)
			, dGirth	(_dGirth)
{
	_tcscpy(sName, _sN);
}

const TLcFnt& TLcFnt::operator=(const TLcFnt& r)	// r: right hand side(rhs)
{	
	iHeight	= r.iHeight;
	iWeight	= r.iWeight;
	iItalic	= r.iItalic;
	iThckX	= r.iThckX;
	iThckY	= r.iThckY;
	dString	= r.dString;
	dGirth	= r.dGirth;
	_tcscpy(sName, r.sName);
	return *this;
}

const TLcFnt& TLcFnt::operator=(const TLcFnt* r)
{	
	iHeight	= r->iHeight;
	iWeight	= r->iWeight;
	iItalic	= r->iItalic;
	iThckX	= r->iThckX;
	iThckY	= r->iThckY;
	dString	= r->dString;
	dGirth	= r->dGirth;
	_tcscpy(sName, r->sName);
	return *this;
}




INT LcFont_Create(char* sCmd
				 , ILcFont** pData
				 , void* p1		// LPDIRECT3DDEVICE9
				 , void* p2		// TLcFnt
				 , void* p3		// No Use
				 , void* p4		// No Use
				 )
{
	(*pData) = NULL;

	if(0==_stricmp("PC Font", sCmd))
	{
		CLcFont2D* pObj = NULL;

		pObj = new CLcFont2D;

		if(FAILED(pObj->Create(p1, p2, p3, p4)))
		{
			// Return Error Notification
			delete pObj;
			return -1;
		}

		(*pData) = pObj;
		
		return 0;
	}

	return -1;
}
