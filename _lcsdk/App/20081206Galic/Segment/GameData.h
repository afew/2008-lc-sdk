// Interface for the CGameData.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _GameData_H_
#define _GameData_H_


#pragma warning(disable : 4786)
#include <vector>
#include <string>

using namespace std;


class CGameData : public IGameData
{
public:

	// Dual Mate Character
	struct TocMate
	{
		_TLcId		MateId;				// ID
		TocAmun		MateAm[4];			// Total 2 for Character

		TocMate()
		{
			MateAm[2].SetMateMaster(&MateAm[0]);
			MateAm[0].SetMateSub(&MateAm[2]);

			MateAm[3].SetMateMaster(&MateAm[1]);
			MateAm[1].SetMateSub(&MateAm[3]);
		}
	};


protected:
	ILcDev*			m_pDevSys	;		// ILcDev Pointer

	// User Login
	char			m_sUid[32]	;		// User ID
	char			m_sPwd[16]	;		// User Password

	// Server Selection
	vector<string>	m_SvrLst;			// Server List
	INT				m_SvrIdx;			// Selected Server

	// Character Selection
	vector<TocAmun*>	m_CharLstMan;	// Character List
	vector<TocAmun*>	m_CharLstLady;	// Character List
	INT					m_CharIdxMan;	// Selecting Man Character Index
	INT					m_CharIdxLady;	// Selecting Lady Character Index

	// Game Play
	TocMate				m_MateMain;		// Main User
	vector<TocMate*>	m_MateLst;		// User List: Dual Mate List

public:
	CGameData();
	virtual ~CGameData();

	virtual INT		Create(void* p1=NULL, void* p2=NULL, void* p3=NULL, void* p4=NULL);
	virtual void	Destroy();
	virtual INT		Query(char* sCmd, void* pData);

public:
	void		MateListErase();
	void		MateFieldPosition(INT M, INT S, void* pField);
	INT			MateUpdate();
	void		MateRender();

	INT			MateModelSetup(INT M, INT S, void* pV);
	INT			MateSetActivation(INT M, INT S, INT A);
	INT			MateSetRotation(INT M, INT S, void* pV);
	INT			MateSetPosition(INT M, INT S, void* pV);

	void*		GetMateList() const;
	TocMate*	GetMate(INT M) const;
	void*		GetMateModel(INT M, INT S);
};


#endif

