// Interface for the CXmshInst class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _XmshInst_H_
#define _XmshInst_H_


class CXmshInst : public CLcxDXFile
{
protected:
	void*						m_pAniOrg;				// Original CXmshAniM pointer
	LPD3DXFRAME					m_pFrameOrg;			// Original Frame Root Pointer
	LPD3DXANIMATIONCONTROLLER	m_pAC;

public:
	CXmshInst();
	virtual ~CXmshInst();

	virtual INT		Create(void* =0, void* =0, void* =0, void* =0);
	virtual void    Destroy();
	virtual INT		FrameMove();
	virtual void	Render();
	virtual INT		Query(char* sCmd, void* pData);
};



#endif


