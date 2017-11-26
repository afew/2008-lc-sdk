// Implementation of the Export classes.
//
////////////////////////////////////////////////////////////////////////////////

#include "Max.h"
#include "resource.h"
#include "istdplug.h"

#include "iparamb2.h"
#include "iparamm2.h"
#include "modstack.h"

#include "LcmExpGL.h"




////////////////////////////////////////////////////////////////////////////////
//
// Implementation of the LcmExpDXClassDesc class.
//
////////////////////////////////////////////////////////////////////////////////


int LcmExpGLClassDesc::IsPublic()
{
	return TRUE;
}

void* LcmExpGLClassDesc::Create(BOOL loading)
{
	return new LcmExpGL();
}

const TCHAR* LcmExpGLClassDesc::ClassName()
{
	return GetString(IDS_CLASS_NAME);
}

SClass_ID LcmExpGLClassDesc::SuperClassID()
{
	return SCENE_EXPORT_CLASS_ID;
}

Class_ID LcmExpGLClassDesc::ClassID()
{
	return LcmExpGL_CLASS_ID;
}

const TCHAR* LcmExpGLClassDesc::Category()
{
	return GetString(IDS_CATEGORY);
}

const TCHAR* LcmExpGLClassDesc::InternalName()
{
	return _T("LcmExpGL");
}

HINSTANCE LcmExpGLClassDesc::HInstance()
{
	return GetHinstance();
}



////////////////////////////////////////////////////////////////////////////////
//
// Implementation of the LcmExpGL class.
//
////////////////////////////////////////////////////////////////////////////////


int			 LcmExpGL::ExtCount()		{	return 1;					}
const TCHAR* LcmExpGL::Ext(int n)		{	return _T("Arc");			}
const TCHAR* LcmExpGL::LongDesc()		{	return _T("Lcm Exporter");	}
const TCHAR* LcmExpGL::ShortDesc()		{	return _T("Lcm Exporter");	}
const TCHAR* LcmExpGL::AuthorName()		{	return _T("Heesung Oh");	}
const TCHAR* LcmExpGL::CopyrightMessage(){	return _T("CopyLeft");		}

const TCHAR* LcmExpGL::OtherMessage1()	{	return _T("");				}
const TCHAR* LcmExpGL::OtherMessage2()	{	return _T("");				}
unsigned int LcmExpGL::Version()		{	return 100;					}
void LcmExpGL::ShowAbout(HWND hWnd)	{								}
BOOL LcmExpGL::SupportsOptions(int,DWORD){	return TRUE;				}

	
LcmExpGL::LcmExpGL()
{
	m_pI	= NULL;
	m_pE	= NULL;

	sampleDelta=100;
}

LcmExpGL::~LcmExpGL() 
{

}


BOOL CALLBACK LcmExpGL::DlgProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	static LcmExpGL *exp = NULL;

	switch(message) {
		case WM_INITDIALOG:
			exp = (LcmExpGL *)lParam;
			CenterWindow(hWnd,GetParent(hWnd));
			return FALSE;

		case WM_CLOSE:
			delete exp;
			EndDialog(hWnd, 0);
			break;

		case WM_COMMAND:
			switch(LOWORD(wParam)) 
			{
				case IDC_BUTTON_OK:
					/// set export sample rate
					{
						BOOL ok;
						UINT val = GetDlgItemInt(hWnd, IDC_EDIT, &ok, false);
						if (ok)
						{
							exp->setSampleDelta(val);
						}
					}

					EndDialog(hWnd, 1);
					break;
					
				case IDC_BUTTON_CANCEL:
					EndDialog(hWnd, 0);
					break;
			}
		
		default:		//message not catched
			return FALSE;
	}
	return TRUE;		//message catched
}

