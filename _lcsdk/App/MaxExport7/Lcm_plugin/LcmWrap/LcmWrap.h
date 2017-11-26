//
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcmWrap_H_
#define _LcmWrap_H_

#include "Max.h"
#include "resource.h"
#include "istdplug.h"
#include "iparamb2.h"
#include "iparamm2.h"

#define LcmWrap_CLASS_ID	Class_ID(0x75b65f29, 0x1f3b19b5)


extern TCHAR*GetString(int id);
extern ClassDesc2* GetLcmWrapDesc();
extern void MsgBox(char *format,...);


class LcmWrap : public SceneExport
{
public:
	SceneExport*	m_pChild;
	ClassDesc*		m_pChildDesc;
	
public:
	LcmWrap();
	virtual ~LcmWrap();	
	
	int				ExtCount();					// Number of extensions supported
	const TCHAR*	Ext(int n);					// Extension #n (i.e. "3DS")
	const TCHAR*	LongDesc();					// Long ASCII description (i.e. "Autodesk 3D Studio File")
	const TCHAR*	ShortDesc();				// Short ASCII description (i.e. "3D Studio")
	const TCHAR*	AuthorName();				// ASCII Author name
	const TCHAR*	CopyrightMessage();			// ASCII Copyright message
	const TCHAR*	OtherMessage1();			// Other message #1
	const TCHAR*	OtherMessage2();			// Other message #2
	unsigned int	Version();					// Version number * 100 (i.e. v3.01 = 301)
	void			ShowAbout(HWND hWnd);		// Show DLL's "About..." box
	
	BOOL	SupportsOptions(int ext, DWORD options);
	int		DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts=FALSE, DWORD options=0);
};


class LcmWrapClassDesc:public ClassDesc2
{
public:
	int 			IsPublic() { return TRUE; }
	void *			Create(BOOL loading = FALSE) { return new LcmWrap(); }
	
	SClass_ID		SuperClassID()	{ return SCENE_EXPORT_CLASS_ID; }
	Class_ID		ClassID()		{ return LcmWrap_CLASS_ID; }
	const TCHAR*	InternalName()	{ return _T("LcmWrap"); }	// returns fixed parsable name (scripter-visible name)
	const TCHAR*	ClassName();
	const TCHAR* 	Category();
};


#endif

