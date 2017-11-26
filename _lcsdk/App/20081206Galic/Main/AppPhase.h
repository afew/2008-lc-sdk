// Interface for the CAppPhase class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _AppPhase_H_
#define _AppPhase_H_

class CAppPhase  : public IGmPhase
{
protected:
	INT			m_nPhase	;
	ILcDev*		m_pDevSys	;
	ILcLsc*		m_pLsc		;
	
public:
	CAppPhase();
	virtual ~CAppPhase();

	virtual INT		Create(void* p1=NULL, void* p2=NULL, void* p3=NULL, void* p4=NULL);
	virtual void	Destroy();

	virtual INT		Restore();
	virtual void	Invalidate();

	virtual INT		FrameMove();
	virtual void	Render();

	virtual INT		Query(char* sCmd, void* pData);
};

#endif

