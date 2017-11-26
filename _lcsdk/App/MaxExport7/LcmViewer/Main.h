// Interface for the CMain class.
//
////////////////////////////////////////////////////////////////////////////////


#ifndef _MAIN_H_
#define _MAIN_H_


class CMain : public CD3DApplication
{
public:
	struct VtxD
	{
		VEC3	p;
		DWORD	d;
		
		VtxD()		: p(0,0,0), d(0xFFFFFFFF){}
		VtxD(FLOAT X,FLOAT Y,FLOAT Z,DWORD D=0xFFFFFFFF) : p(X,Y,Z), d(D){}
		enum	{FVF = (D3DFVF_XYZ|D3DFVF_DIFFUSE),};
	};


public:
	ID3DXFont*		m_pD3DXFont	;
	CLcInput*		m_pInput	;
	CLcCam*			m_pCam		;
	CLcGrid*		m_pGrid		;
	CLcField*		m_pField	;
	CLcxPivot*		m_pPivot	;

	LPD3DXEFFECT	m_pEffct	;				// ¿Ã∆Â∆Æ

	Lcm::ILcmMdl*	m_LcmOrg	;
	Lcm::ILcmMdl*	m_LcmMdl	;


	Lcm::ILcmMdl*	m_LcmLnkO	;
	Lcm::ILcmMdl*	m_LcmLnk	;
	INT				m_LcmLnkN	;				// Link Index
	INT				m_LcmLnkT	;				// Link Total
	vector<Lcm::_Tattach> m_LnkLst;				// Link List


	INT				m_LcmObjN	;				// Object Index
	INT				m_LcmObjT	;				// Object Total

	INT				m_LcmMtlN	;				// Material Index
	INT				m_LcmMtlT	;				// Material Total
	vector<Lcm::_Tmtl> m_MtlLst	;				// Material List

	
	PDVB			m_pAttach	;

	INT				m_LcmFrmN	;				// Link Index
	INT				m_LcmFrmT	;				// Total Link Index
	vector<Lcm::_Tframe> m_FrmLst;				// Frame List

	INT				m_nFrmCur	;				// Current Frame

protected:
	BOOL			m_bOptFill	;
	BOOL			m_bOptCull	;
	BOOL			m_bOptBox	;
	BOOL			m_bOptBone	;
	BOOL			m_bOptMdl	;

	BOOL			m_bOptGrid	;
	BOOL			m_bOptField	;
	BOOL			m_bOptAnima	;


	HMODULE			m_hmdRch	;
	HWND			m_hWndDlg	;
	HWND			m_hWndAni	;
	HWND			m_hWndEdt	;

	HWND			m_hWndInf	;

public:
	CMain();

	virtual HRESULT Init();
	virtual HRESULT Destroy();

	virtual HRESULT Restore();
	virtual HRESULT Invalidate();
	
	virtual HRESULT FrameMove();
	virtual HRESULT Render();
	
	HRESULT RenderText();
	
public:
	LRESULT MsgProc( HWND, UINT, WPARAM, LPARAM);
	static	LRESULT FxMainProc(HWND, UINT, WPARAM, LPARAM);
	static	LRESULT FxAniProc(HWND, UINT, WPARAM, LPARAM);
	static	LRESULT	FxInfProc(HWND, UINT, WPARAM, LPARAM);

public:
	INT		ResetModel(char* sFile);
	INT		ResetLink(char* sFile);
	INT		ResetHlsl(char* sCmd, void* pSrc, INT iLen=0);
	void	ResetHlsltoMdl();

	void	ResetFrameIdx();
	void	ResetLinkTM();

	void	ResetObjMtl(INT nObj, INT nMtl);
};

INT		HlslBuild(char* sCmd, void** pData, LPDIRECT3DDEVICE9 pDev, void* pSrc=NULL, INT iLen=0);

extern CMain*	g_pApp;
#define GMAIN	g_pApp


#endif