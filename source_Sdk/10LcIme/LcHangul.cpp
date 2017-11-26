// Implementation of the CLcHangul class.
//
////////////////////////////////////////////////////////////////////////////////

#if _MSC_VER >= 1200
#pragma warning(disable : 4996)
#endif

#include <windows.h>


#include "ILcIme.h"
#include "LcHangul.h"


CLcHangul::CLcHangul()
{
	m_bUse	= FALSE;
	
	memset(m_sWrd, 0, sizeof m_sWrd);
	memset(m_sStr, 0, sizeof m_sStr);
	memset(m_sOut, 0, sizeof m_sOut);

	m_dStB	= 0;
}


CLcHangul::~CLcHangul()
{
	CLcHangul::Destroy();
}


void CLcHangul::Destroy()
{
	Reset();
}


INT CLcHangul::Create(void* p1, void* p2, void* p3, void* p4)
{
	if(p1)
		m_dStB = *((DWORD*)p1);

	return 0;
}


void CLcHangul::Set()
{
	m_bUse	= TRUE;
	
	memset(m_sWrd, 0, sizeof(m_sWrd));
	memset(m_sStr, 0, sizeof(m_sStr));
	memset(m_sOut, 0, sizeof(m_sOut));
}


void CLcHangul::Reset()
{
	m_bUse	= FALSE;
	
	memset(m_sWrd, 0, sizeof m_sWrd);
	memset(m_sStr, 0, sizeof m_sStr);
	memset(m_sOut, 0, sizeof m_sOut);
}


BOOL CLcHangul::GetState()			{ return m_bUse;	}
void CLcHangul::SetState(BOOL use)	{ m_bUse = use;		}



void CLcHangul::EraseWord()
{
	if( strlen(m_sWrd) )
		memset(m_sWrd, 0, sizeof(m_sWrd));
	
	else
	{
		int iLength = strlen(m_sStr);
		
		if( iLength > 0)
		{
			if(m_sStr[iLength - 1] < 0)											// �ѱ��� 2����Ʈ �ڵ�...
			{
				m_sStr[iLength - 1] = 0;
				m_sStr[iLength - 2] = 0;
			}
			
			else																// 1����Ʈ �ڵ��...
				m_sStr[iLength - 1] = 0;
		}
	}
}



INT CLcHangul::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if(!m_bUse)
		return FALSE;

	INT		len = 0;
	HIMC	hIMC= 0;
	WPARAM	wLo = LOWORD(wParam);


	if(m_dStB && WM_COMMAND == uMsg && m_dStB == wLo)							// String Table�� ����ϰ� Back space�� ���ǵ� �̺�Ʈ�� �߻��� ���
	{
		EraseWord();
		return FALSE;
	}
	
	
	if(WM_IME_STARTCOMPOSITION == uMsg)
		return TRUE;
		
	else if(WM_IME_COMPOSITION == uMsg)											// �۾� ���� ��
	{
		hIMC = ImmGetContext(hWnd);												// Get IME Handle
		
		if (lParam & GCS_RESULTSTR)
		{
			len = ImmGetCompositionString(hIMC, GCS_RESULTSTR, NULL, 0);

			if( 0 < len)														// �۾��� ������ �Ǿ�����
			{
				ImmGetCompositionString(hIMC, GCS_RESULTSTR, m_sWrd, len);		// ���� IME�� ��Ʈ�� ���̸� ��´�

				if( strlen(m_sStr) <= MAX_CHAT_LEN)
					strcat(m_sStr, m_sWrd);

				memset(m_sWrd,0, sizeof(m_sWrd));
			}
		}
			
		else if (lParam & GCS_COMPSTR)											// �������̸�;
		{
			len = ImmGetCompositionString(hIMC, GCS_COMPSTR, NULL, 0);			// �������� ���̸� ��´�.
			
			if(0==len)															// �����߿� Back space�� �� ���
				EraseWord();

			else
				ImmGetCompositionString(hIMC, GCS_COMPSTR, m_sWrd, len);		// str��  �������� ���ڸ� ��´�.
		}
		
		ImmReleaseContext(hWnd, hIMC);											// IME �ڵ� ��ȯ!!
		
		return TRUE;
	}

	else if(WM_CHAR == uMsg)													// �ѱ��� �������� �ƴ� ��(������, ��Ÿ Ű �̺�Ʈ)
	{
		if( strlen(m_sStr) <= MAX_CHAT_LEN)
		{
			if(wParam>=32 && wParam<127)
				m_sStr[strlen(m_sStr)] = wParam;

			else if(!m_dStB && VK_BACK ==wParam)								// ��Ʈ�� ���̺� ������� �ʰ� �齺���̽��� ���
			{
				INT c;
				EraseWord();
				c=0;
			}
		}	
	}
	
	return FALSE;
}




int CLcHangul::OutStringBeam(char* sOut)
{
	strcpy(m_sOut, m_sStr);
	strcat(m_sOut, m_sWrd);

	if(!m_bUse)
	{
		sOut[0] = 0;
		sOut[1] = 0;
		return 0;
	}

	strcpy(sOut, m_sOut);

	static DWORD start = timeGetTime();
	static DWORD end;
	
	end = timeGetTime();
	
	if(end-start>1200)
		start = end;
	
	if(end-start>600)
		strcat(sOut, " ");
	
	else
		strcat(sOut, "|");

	return strlen(m_sOut);
}



INT CLcHangul::OutStringStar(char* sOut)
{
	char sOut1[512];
	memset(sOut1, 0, sizeof(sOut1));
	
	strcpy(sOut1, m_sStr);
	strcat(sOut1, m_sWrd);
	
	memset(sOut, '*', strlen(sOut1));
	
	static DWORD start = timeGetTime();
	static DWORD end;
	
	end = timeGetTime();
	
	if(end-start>1000)
		start = end;
	
	if(end-start>500)
		strcat(sOut, "|");

	else
		strcat(sOut, " ");

	return strlen(m_sOut);
}


int CLcHangul::OutString(char* sOut)
{
	strcpy(m_sOut, m_sStr);
	strcat(m_sOut, m_sWrd);

	if(!m_bUse)
	{
		sOut[0] = 0;
		sOut[1] = 0;
		return 0;
	}
	
	strcpy(sOut, m_sOut);
	
	return strlen(m_sOut);
}






INT CLcHangul::Query(char* sCmd, void* pData)
{
	return -1;
}

