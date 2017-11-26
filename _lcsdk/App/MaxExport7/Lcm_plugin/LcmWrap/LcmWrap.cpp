//
//
////////////////////////////////////////////////////////////////////////////////


#include "LcmWrap.h"

static LcmWrapClassDesc LcmWrapDesc;

char		g_sDll[] = "c:/LcmExpDX.dle";						// Reference
HINSTANCE	g_hDll = NULL;										// Reference
int			g_nRef = 0;											// Reference Count


ClassDesc2* GetLcmWrapDesc()
{
	return &LcmWrapDesc;
}


void DllLoad()
{
	g_nRef++;
	
	if (!g_hDll)
	{
		g_hDll = LoadLibrary( g_sDll );
		
		if (!g_hDll)
			printf("Failed to load %s\n", g_sDll);
		else
			printf("Loaded ", g_sDll);
	}
	
	printf("%s: g_nRef %d\n", g_sDll, g_nRef);
}

void DllFree()
{
	g_nRef--;
	
	if (g_nRef==0 && g_hDll)
	{
		printf("Released ");
		FreeLibrary(g_hDll);
		g_hDll = NULL;
	}
	
	printf("%s: g_nRef %d\n", g_sDll, g_nRef);
}



typedef ClassDesc* ( __stdcall  *tLibClassDesc)( int i);

ClassDesc* getChildDesc()
{
	ClassDesc* child=NULL;
	
	DllLoad();
	
	if (g_hDll)
	{
		tLibClassDesc pLibClassDesc = (tLibClassDesc) GetProcAddress(g_hDll, (LPCSTR)3);
		
		if (pLibClassDesc)
		{
			child = pLibClassDesc(0);
		}
	}
	
	return child;
}

void freeChildDesc(ClassDesc* child)
{
	DllFree();
}



////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////



const TCHAR* LcmWrapClassDesc::ClassName()
{ 
	static TCHAR s[1024];
	ClassDesc* child = getChildDesc();

	memset(s, 0, sizeof s);
	_tcscpy(s, _T("Failed to load LcmWrap"));

	if (child)
	{
		TCHAR*	sCategory = (TCHAR*)child->Category();

		if(sCategory)
			_tcscpy(s, sCategory);
	}
	
	freeChildDesc(child);
	
	return s;
}

const TCHAR* LcmWrapClassDesc::Category()
{
	static TCHAR s[1024];
	ClassDesc* child = getChildDesc();

	memset(s, 0, sizeof s);
	_tcscpy(s, _T("Failed to load LcmWrap"));
	
	if (child)
	{
		TCHAR*	sCategory = (TCHAR*)child->Category();

		if(sCategory)
			_tcscpy(s, sCategory);
	}
	
	freeChildDesc(child);
	
	return s;
}


////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////


LcmWrap::LcmWrap()
{
	m_pChildDesc = getChildDesc();
	
	if (m_pChildDesc)
		m_pChild = (SceneExport*)m_pChildDesc->Create();
	else
		m_pChild = NULL;
}

LcmWrap::~LcmWrap() 
{
	if(m_pChild)
		delete m_pChild;
	
	freeChildDesc(m_pChildDesc);	
}

int LcmWrap::ExtCount()
{
	if(m_pChild)
		return m_pChild->ExtCount();
	
	return 0;
}

const TCHAR* LcmWrap::Ext(int n)
{		
	if(m_pChild)
		return m_pChild->Ext(n);

	return _T("Failed to load Wrap");	
}

const TCHAR* LcmWrap::LongDesc()
{
	static TCHAR s[1024];
	
	if(m_pChild)
		_stprintf(s, "Wrap(%s)", m_pChild->LongDesc());
	
	else
		_stprintf(s, _T("Failed to load Wrap"));
	
	return s;
}

const TCHAR* LcmWrap::ShortDesc() 
{			
	static TCHAR s[1024];
	
	if(m_pChild)
		_stprintf(s, "Wrap(%s)", m_pChild->ShortDesc());
	
	else
		_stprintf(s, _T("Failed to load Wrap"));
	
	return s;
}

const TCHAR* LcmWrap::AuthorName()
{			
	if(m_pChild)
		return m_pChild->AuthorName();

	return _T("Failed to load Wrap");	
}

const TCHAR* LcmWrap::CopyrightMessage() 
{	
	if(m_pChild)
		return m_pChild->CopyrightMessage();

	return _T("Failed to load Wrap");	
}

const TCHAR* LcmWrap::OtherMessage1() 
{		
	if(m_pChild)
		return m_pChild->OtherMessage1();

	return _T("Failed to load Wrap");	
}

const TCHAR* LcmWrap::OtherMessage2() 
{		
	if(m_pChild)
		return m_pChild->OtherMessage2();
	
	return _T("Failed to load Wrap");	
}

unsigned int LcmWrap::Version()
{				
	if(m_pChild)
		return m_pChild->Version();
	
	return 0;	
}

void LcmWrap::ShowAbout(HWND hWnd)
{			
	if(m_pChild)
		m_pChild->ShowAbout(hWnd);
}

BOOL LcmWrap::SupportsOptions(int ext, DWORD options)
{
	if(m_pChild)
		return m_pChild->SupportsOptions(ext, options);
	
	return FALSE;
}


int	LcmWrap::DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options)
{
	if(m_pChild)
		return m_pChild->DoExport(name, ei, i, suppressPrompts, options);

	MessageBox(NULL, "Failed to load Wrap", "Error", MB_ICONWARNING);
	return FALSE;
}



