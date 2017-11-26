// Interface for the CLcHangul class.

//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcHangul_H_
#define _LcHangul_H_


class CLcHangul : public ILcIme
{
public:
	enum EImeState
	{
		IHS_ENG = 0,
		IHS_KOR,
	};

protected:
	TCHAR	m_sWrd[4];															// �������� ����!!
	TCHAR	m_sStr[MAX_CHAT_LEN*2+8];											// Output buffer..
	TCHAR	m_sOut[MAX_CHAT_LEN*2+8];											// Output buffer..
	BOOL	m_bUse;																// ���� IME�� ����ϰ� �ִ��� üũ
	BOOL	m_bCmp;																// ���� ������������ ��Ÿ���� �÷���

	DWORD	m_dStB;																// String Table�� ����ؼ� Back Space Key��
																				// �������� ��� WM_COMMAND�� ����ؾ� �Ѵ�.

public:
	CLcHangul();
	virtual ~CLcHangul();

	virtual INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0);
	virtual void	Destroy();
	virtual INT		MsgProc(HWND,UINT,WPARAM,LPARAM);

	virtual INT		Query(char* sCmd, void* pData);

	virtual	void	Set();
	virtual	void	Reset();

	virtual void	SetState(BOOL);
	virtual INT		GetState();

	virtual	INT		OutString(char*);
	virtual	INT		OutStringBeam(char*);
	virtual	INT		OutStringStar(char*);


protected:
	void	EraseWord();
};


#endif