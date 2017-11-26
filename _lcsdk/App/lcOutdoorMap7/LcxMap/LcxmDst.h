// Interface for the CLcxmDst class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcxmDst_H_
#define _LcxmDst_H_

#include <vector>
using namespace std;

class CLcxmDst : public ILcxmFld		// Lcx Map District Field Mananger
{
protected:
	PDEV			m_pDev	;			// LPDIRECT3DDEVICE9

	ILcxmT::MpInf	m_Inf	;
	ILcxmT::MpEnv	m_Env	;
	CLcxmBlc**		m_ppLcl	;

	CLcxmBlc*		m_pLcT	;			// Local Temp
	CLcxmBlc*		m_pPkLc	;			// Picking Local
	ILcxmT::lsTlPkT	m_vPkLc	;			// Picking Triangle from Local
	BOOL			m_bClck	;
	VEC3			m_vcPck	;			// Final Picking Position from Local

public:
	Lcm::lsPkBndA	m_vPkObj;			// Picking Object Bound Box
	Lcm::ILcmMdl*	m_pPkObj;			// Picking Object
	VEC3			m_vcPkObj;			// Final Picking Object Position

	Lcm::lsLcxMdl	m_vObj2D;			// 2D Object List
	Lcm::lsLcxMdl	m_vObj3D;			// 3D Object List
	Lcm::lsLcxMdl	m_vObj	;			// Object List	

protected:
	vector<CLcxmBlc*> m_vLcl	;
	FLOAT			m_fRangeMax	;		// Max View Range

	PDVD			m_pFVF	;
	LPD3DXEFFECT	m_pEft	;

public:
	CLcxmDst();
	virtual ~CLcxmDst();

	virtual	INT		Create(void* p1=0, void* p2=0, void* p3=0, void* p4=0);
	virtual	void	Destroy();

	virtual	INT		FrameMove();
	virtual	void	Render();

	virtual INT		Query(char* sCmd, void* pData);
	virtual	INT		GetHeight(float* vcOut, const float* vcIn);

public:
	INT				FileLoad(char* sFile);
	INT				FileSave(char* sFile);
	void			CreateField(ILcxmT::MpInf*, ILcxmT::MpEnv*, ILcxmT::lsMpTex*);			// Create Field
	
	INT				Update();
	void			AddObj2D(Lcm::ILcmMdl* pMdB);
	void			AddObj3D(Lcm::ILcmMdl* pMdB);
	void			RenderPck();
	
	ILcxmT::MpInf*	GetFldInf();
	char*			GetFldName();

	void			EraseObj();
	void*			GetPckObj();
	
	PDVD			GetVertexDeclarator();
	LPD3DXEFFECT	GetD3DXEffect();
	ILcxCam*		GetCam();
	
protected:
	INT			PickField();
	INT			PickPosition(CLcxmBlc**			pBlock
							, ILcxmT::lsTlPkT*	pPickInf	// Picking Triangle
							, VEC3*				pvcOut		// Picking position
							, const VEC3*		pvcRayPos	// Ray Begin Position
							, const VEC3*		pvcRayDir	// Ray Direction
						   );

	INT			PickLocal();
	INT			PickObj();

public:
	FLOAT		m_fCull			;
	BOOL		m_bCtrlCull		;
	BOOL		m_bCtrlPck		;
	BOOL		m_bCtrlHeight	;
	BOOL		m_bCtrlSpoid	;
	DWORD		m_dFillColor	;
	BOOL		m_bCtrlBrush	;
	BOOL		m_bCtrlModel	;
	INT			m_nCtrlModel	;
	BOOL		m_bCtrlTile		;
	INT			m_nCtrlTile		;
	BOOL		m_bCtrlBndBox	;
	BOOL		m_bCtrlLclLine	;

	ILcxmT::MpHgt	m_Hgt		;
	VEC3			m_vcRayDir	;
	VEC3			m_vcCamPos	;
	Lcm::ILcmMdl*	m_pMdB		;
	ILcxCam*		m_pCam		;
	ILcxTbl*		m_pTbTex	;
	ILcxTbl*		m_pTbMdl	;

protected:
	void	SetupHeight();
	void	SetupSpoid();
	void	SetupBrush();
	void	SetupModel();
	void	SetupTile();
};

#endif

