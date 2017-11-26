// Interface for the ILopAi class.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _ILopAi_H_
#define _ILopAi_H_


#pragma warning(disable: 4786)
#include <vector>
#include <list>
#include <map>
#include <string>

using namespace std;


namespace ILopAi
{

enum EocState
{
	OCST_NONE	= 0x00000000,
	OCST_IDLE	= 0x00000001,

	OCST_MOVE	= 0x01000000,
	OCST_WALK	= 0x01000002,
	OCST_RUN	= 0x01000003,
	OCST_DASH	= 0x01000004,
	OCST_FOLLOW	= 0x01000005,

	OCST_ATTACK	= 0x02000006,
	OCST_DAMAGE	= 0x02000007,
};


#define OCSST_NONE		"OCST_None"
#define OCSST_IDLE		"OCST_Idle"

#define OCSST_WALK		"OCST_Walk"
#define OCSST_RUN		"OCST_Run"
#define OCSST_DASH		"OCST_Dash"
#define OCSST_FOLLOW	"OCST_Follow"

#define OCSST_ATTACK	"OCST_Attack"
#define OCSST_DAMAGE	"OCST_Damage"


//struct _TocState
//{
//	const char *sN;
//	EocState	eS;
//};


class _Tentity
{
protected:
	INT		m_nID;
	void*	m_pFSM;

public:
	_Tentity();
	virtual ~_Tentity();

	virtual	INT		Create(void* =0,void* =0,void* =0,void* =0);
	virtual	void	Destroy();
	virtual	INT		UpdateFSM();
	virtual	INT		QueryState(char* sCmd, void* pData);
	virtual	INT		QueryValue(char* sCmd, void* pData);

	INT		OnMessage(char* sMsg, void* pMessage);
	INT		GetID();
	void	SetID(INT nID);

	void*	GetFSM();
};


enum EocValue
{
	MAX_STATE_NAME	= 32,
	MAX_MSG_VALUE	= 64,
	CMD_ONCE		= 0,
	CMD_LOOP		= 1,
	MAX_LIFE		= 0x00FFFFFF,	//0x00FFFFFF,	// 16 * 1000 초 ~ 267분~ 4.4 시간

	MAX_ENTITY_NAME	= 64,
	STATE_DISABLE	= 0,
	STATE_ENABLE	= 1,
};


class _Tmessage
{
public:
	char	Phase[ILopAi::MAX_STATE_NAME];

	INT		MsgSize;						// Massage Size
	char	MsgText[ILopAi::MAX_MSG_VALUE];	// Massage Content
	INT		MsgType;						// 0: 1번만 실행, 1: Loop Command
	void*	MsgSend;						// Message 전송자

	DWORD	TimeCur;						// Sending Time or Current Time
	INT		TimeDly;						// Delay Time: 0 is No delay
	INT		TimeDur;						// Duration Time
	
public:
	_Tmessage();
	_Tmessage(const _Tmessage& r);
	_Tmessage(const _Tmessage* r);

	_Tmessage(char*	_Phase
			, INT	_MsgSize	= 0
			, void*	_MsgText	= NULL
			, INT	_MsgType	= ILopAi::CMD_ONCE
			, void*	_MsgSender	= NULL
			, INT	_Delay		= 0
			, INT	_Duration	= ILopAi::MAX_LIFE
			);

	const _Tmessage& operator=(const _Tmessage& r);	// r: right hand side(rhs)
	const _Tmessage& operator=(const _Tmessage* r);	// r: right hand side(rhs)

	void	SetText(INT  nSize, void* _MsgText);
	void	GetText(INT* nSize, void* _MsgText);
	void	SetType(INT _MsgType);
	INT		GetType();
	void	SetSender(void* _MsgSender);
	void*	GetSender();

	void	SetTimeCur(DWORD);
	void	SetTimeDly(INT);
	void	SetTimeDur(INT);

	DWORD	GetTimeCur();
	INT		GetTimeDly();
	INT		GetTimeDur();
};



class _Tstate
{
public:
	INT			Active;
	_Tmessage	Message;

public:
	_Tstate();
	_Tstate(const _Tmessage&);
	_Tstate(const _Tmessage*);
	
	INT		Enter(_Tentity* pEntity);				// Enter State
	INT		Exit(_Tentity* pEntity);				// Exit State
	INT		Exec(_Tentity* pEntity);				// Excute State

	const _Tmessage* const GetPhase() const;
	const char*		 const GetPhaseName() const;

	void	SetPhase(const _Tmessage* );
	void	SetPhaseName(const char*);
	void	SetActive(INT bActive);
	INT		GetActive();
};


typedef	std::map<std::string, _Tstate*	>	mpTstate;
typedef	mpTstate::iterator					itTstate;

typedef	std::list<_Tmessage >	lsTmsg;
typedef	lsTmsg::iterator		isTmsg;

typedef	std::vector<_Tmessage >	lvTmsg;
typedef	lvTmsg::iterator		ivTmsg;

class _TstateFSM
{
protected:
	_Tentity*	m_pEntity;

	_Tstate*	m_pStCur;	// Current State
	_Tstate*	m_pStOld;	// Old State
	_Tstate*	m_pStInt;	// Interrupt State

	lsTmsg		m_LsEvt;	// Event State List
	lvTmsg		m_LvAgt;	// Agent List

public:
	_TstateFSM();
	virtual ~_TstateFSM();

	INT			Create(_Tentity* pEntity);
	void		Destroy();
	INT			Update();

	INT			OnMessage(char* sMsg, _Tmessage* pMessage);

	_Tstate*	GetStateCur();
	_Tstate*	GetStateOld();
	_Tstate*	GetStateInt();
};

}//name space ILopAi


#endif

