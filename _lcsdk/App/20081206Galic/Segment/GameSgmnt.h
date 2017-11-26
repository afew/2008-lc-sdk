// Interface for the CGameSgmnt.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _GameSgmnt_H_
#define _GameSgmnt_H_


#pragma warning(disable : 4786)
#include <vector>
#include <string>

using namespace std;
 

class CGameData;

class CGameSgmnt : public IGameSgmnt
{
protected:
	PDEV			m_pDev		;
	ILcDev*			m_pDevSys	;
	ILcInput*		m_pInput	;
	ILcxCam*		m_pCam		;

	ILcxTbl*		m_pTbTex	;
	ILcxTbl*		m_pTbMdl	;
	ILcxmFld*		m_pFld		;

	CGameData*		m_pGameData	;

	D3DXVECTOR3		m_MousePos	;
	LcIdx::Idx16	m_MouseIdx[4];
	LcVtx::VtxD		m_MouseVtx[4];

public:
	CGameSgmnt();
	virtual ~CGameSgmnt();

	virtual INT		Create(void* p1=NULL, void* p2=NULL, void* p3=NULL, void* p4=NULL);
	virtual void	Destroy();

	virtual INT		FrameMove();
	virtual void	Render();

	virtual INT		Restore();
	virtual void	Invalidate();

	virtual INT		Query(char* sCmd, void* pData);
public:
	ILcxmFld*		GetOutdoor();
};


#endif

