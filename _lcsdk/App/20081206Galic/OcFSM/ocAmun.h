// Interface for the CocAmun class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _TocAmun_H_
#define _TocAmun_H_


class TocAmun : public ILopAi::_Tentity
{
protected:
	char			m_AmName[ILopAi::MAX_STATE_NAME];		// Character Name
	INT				m_AmActv;			// Activation
	INT				m_AmStat;			// State

	void*			m_AmMdl;			// Model 3D Data

	INT				m_AmKaBa;			// Man[0: Human, arc: 1], Woman[2: Human, Arc: 3]

	INT				m_Thp;				// Hp
	INT				m_Tmp;				// Mana Point
	INT				m_Tst;				// Stamina
	INT				m_TstH;				// Stamina threshold

public:
	FLOAT			m_Mdlt;				// Time Interval(¥Ät)
	D3DXVECTOR3		m_Mspd;				// Move Speed: 0 Walk, 1: Run 2: Dash
	D3DXVECTOR3		m_Mpos;				// Move Position
	D3DXVECTOR3		m_Mdir;				// Move Direction
	D3DXVECTOR3		m_Mtgt;				// Move Target

protected:
	TocAmun*		m_pMater;			// Master for Follow
	TocAmun*		m_pMatee;			// Sub	for Follow

public:
	TocAmun();
	virtual ~TocAmun();

	virtual	INT		Create(void* =0,void* =0,void* =0,void* =0);
	virtual	void	Destroy();
	virtual	INT		UpdateFSM();
	virtual	INT		QueryState(char* sCmd, void* pData);
	virtual	INT		QueryValue(char* sCmd, void* pData);

	void		SetName(char*);
	const char*	GetName() const;

	void		SeActive(INT);
	const INT	GetActive()	const;

	void		SetState(INT);
	const INT	GetState()	const;

	void		SetModel(void*);
	void*		GetModel();

	void		SetMateMaster(TocAmun*);
	TocAmun*	GetMateMaster();

	void		SetMateSub(TocAmun*);
	TocAmun*	GetMateSub();
};

#endif


