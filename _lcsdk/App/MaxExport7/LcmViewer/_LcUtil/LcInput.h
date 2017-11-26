// Interface for the CLcInput class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcInput_H_
#define _LcInput_H_


#define MAX_INPUT_KEY	256
#define MAX_INPUT_BTN	8


class CLcInput  
{
public:
	enum EInputState
	{
		EINPUT_NONE  = 0,
		EINPUT_DOWN  = 1,
		EINPUT_UP	 = 2,
		EINPUT_PRESS = 3,
		EINPUT_DBCLC = 4,
	};

protected:
	HWND		m_hWnd;
	
	BYTE		m_KeyCur[MAX_INPUT_KEY];	// 키보드 현재 상태
	BYTE		m_KeyOld[MAX_INPUT_KEY];	// 키보드 이전 상태
	BYTE		m_KeyMap[MAX_INPUT_KEY];	// 키보드 맵

	BYTE		m_BtnCur[MAX_INPUT_BTN];	// 마우스 현재 상태
	BYTE		m_BtnOld[MAX_INPUT_BTN];	// 마우스 이전 상태
	BYTE		m_BtnMap[MAX_INPUT_BTN];	// 마우스 맵

	D3DXVECTOR3	m_vcCur;					// 휠 마우스 Z
	D3DXVECTOR3 m_vcOld;
	D3DXVECTOR3 m_vcEps;

	DWORD		m_dTimeDC;					// Double Click Time Interval
	DWORD		m_dBtnBgn[MAX_INPUT_BTN];	// Double Click Start
	INT			m_dBtnCnt[MAX_INPUT_BTN];	// Double Click Count


public:
	CLcInput();
	virtual ~CLcInput();

	virtual INT		Create(HWND);
	virtual INT		FrameMove();
	virtual LRESULT	MsgProc(HWND, UINT, WPARAM, LPARAM);

	void	AddZ(INT d);

	BOOL	KeyDown	(INT	nKey);
	BOOL	KeyUp	(INT	nKey);
	BOOL	KeyPress(INT	nKey);
	INT		KeyState(INT	nKey);

	BOOL	BtnDown	 (INT nBtn);
	BOOL	BtnUp	 (INT nBtn);
	BOOL	BtnPress (INT nBtn);
	INT		BtnState (INT nBtn);

	D3DXVECTOR3 GetMousePos();
	D3DXVECTOR3 GetMouseEps();
};

#endif

