// Implementation of the _TstateFSM class.
//
////////////////////////////////////////////////////////////////////////////////


#include <windows.h>
#include <stdio.h>
#include <memory.h>
#include <mmsystem.h>

#include <d3dx9.h>

#include "ILopAi.h"
#include "ocAmun.h"


#ifndef SAFE_DELETE
#define SAFE_DELETE(p)			{ if(p){ delete    p; p = NULL; } }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p)	{ if(p){ delete [] p; p = NULL;	} }
#endif

////////////////////////////////////////////////////////////////////////////////
namespace ILopAi
{

_Tentity::_Tentity()
{
	m_nID	= 0xFFFFFFFF;
	m_pFSM	= NULL;
}


_Tentity::~_Tentity()
{
	Destroy();
}


INT _Tentity::Create(void* p1, void*, void*, void*)
{
	INT nId = (INT)p1;
	SetID(nId);

	return 0;
}

void _Tentity::Destroy()
{
	if(m_pFSM)
	{
		ILopAi::_TstateFSM* pFSM	= (ILopAi::_TstateFSM*)m_pFSM;
		delete pFSM;
		m_pFSM = NULL;
	}
}


INT _Tentity::UpdateFSM()
{
	return 0;
}


INT _Tentity::QueryState(char* sCmd, void* pData)
{
	return -1;
}


INT _Tentity::QueryValue(char* sCmd, void* pData)
{
	if(0 ==_stricmp(sCmd, "Get ID"))
	{
		*((INT*)pData)	= m_nID;
		return 0;
	}

	else if(0 ==_stricmp(sCmd, "Set ID"))
	{
		m_nID = *((INT*)pData);
		return 0;
	}

	else if(0 ==_stricmp(sCmd, "Get FSM"))
	{
		*((void**)pData)	= m_pFSM;
		return 0;
	}

	else if(0 ==_stricmp(sCmd, "Set FSM"))
	{
		m_pFSM = *((void**)pData);
		return 0;
	}

	return -1;
}

INT _Tentity::OnMessage(char* sMsg, void* pMessage)
{
	_TstateFSM*	pFSM = (_TstateFSM*)m_pFSM;
	return pFSM->OnMessage(sMsg, (_Tmessage*)pMessage);
}


void _Tentity::SetID(INT nID)
{
	m_nID	= nID;
}


INT _Tentity::GetID()
{
	return m_nID;
}


void* _Tentity::GetFSM()
{
	return m_pFSM;
}


////////////////////////////////////////////////////////////////////////////////

_Tstate::_Tstate()
{
	Active	= ILopAi::STATE_ENABLE;
}

_Tstate::_Tstate(const _Tmessage& r)
{
	Active	= ILopAi::STATE_ENABLE;
	memcpy(&Message, &r, sizeof(ILopAi::_Tmessage));
}

_Tstate::_Tstate(const _Tmessage* r)
{
	Active	= ILopAi::STATE_ENABLE;
	memcpy(&Message, r, sizeof(ILopAi::_Tmessage));
}


INT	_Tstate::Enter(_Tentity* pEntity)
{
	char	sCmd[64]={0};
	sprintf(sCmd, "Enter %s", Message.Phase);
	return pEntity->QueryState(sCmd, NULL);
}

INT	_Tstate::Exit(_Tentity* pEntity)
{
	char	sCmd[64]={0};

	if(0==strlen(Message.Phase))
		return -1;

	sprintf(sCmd, "Exit %s", Message.Phase);
	return pEntity->QueryState(sCmd, NULL);
}

INT	_Tstate::Exec(_Tentity* pEntity)
{
	char	sCmd[64]={0};

	if(0==strlen(Message.Phase))
		return -1;

	sprintf(sCmd, "Exec %s", Message.Phase);
	return pEntity->QueryState(sCmd, NULL);
}


const ILopAi::_Tmessage* const _Tstate::GetPhase() const
{
	return &Message;
}

const char*	 const _Tstate::GetPhaseName() const
{
	return Message.Phase;
}


void _Tstate::SetPhase(const _Tmessage* r)
{
	memcpy(&Message, r, sizeof(ILopAi::_Tmessage));
}


void _Tstate::SetPhaseName(const char* _sName)
{
	strcpy(Message.Phase,  _sName);
}


void _Tstate::SetActive(INT bActive)
{
	Active = bActive;
}

INT _Tstate::GetActive()
{
	return Active;
}



////////////////////////////////////////////////////////////////////////////////
_Tmessage::_Tmessage()
{
	MsgSize		= 0;
	MsgType		= 0;
	MsgSend		= NULL;

	TimeCur		= ::timeGetTime();
	TimeDly		= 0;
	TimeDur		= ILopAi::MAX_LIFE;

	memset(Phase,   0, ILopAi::MAX_STATE_NAME);
	memset(MsgText, 0, ILopAi::MAX_MSG_VALUE);
}


_Tmessage::_Tmessage(const _Tmessage& r)
{
	MsgSize		= r.MsgSize;
	MsgType		= r.MsgType;
	MsgSend		= r.MsgSend;

	TimeCur		= r.TimeCur;
	TimeDly		= r.TimeDly;
	TimeDur		= r.TimeDur;

	memset(Phase,   0, ILopAi::MAX_STATE_NAME);
	memset(MsgText, 0, ILopAi::MAX_MSG_VALUE);

	strcpy(Phase, r.Phase);
	memcpy(MsgText, r.MsgText, ILopAi::MAX_MSG_VALUE);
}


_Tmessage::_Tmessage(const _Tmessage* r)
{
	MsgSize		= r->MsgSize;
	MsgType		= r->MsgType;
	MsgSend		= r->MsgSend;

	TimeCur		= r->TimeCur;
	TimeDly		= r->TimeDly;
	TimeDur		= r->TimeDur;

	memset(Phase,   0, ILopAi::MAX_STATE_NAME);
	memset(MsgText, 0, ILopAi::MAX_MSG_VALUE);

	strcpy(Phase, r->Phase);
	memcpy(MsgText, r->MsgText, ILopAi::MAX_MSG_VALUE);
}


_Tmessage::_Tmessage(char* _Phase, INT _MsgSize, void* _MsgText, INT _MsgType, void* _MsgSend, INT _Delay, INT _Duration)
{
	MsgSize		= _MsgSize;
	MsgType		= _MsgType;
	MsgSend		= _MsgSend;

	TimeCur		= ::timeGetTime();
	TimeDly		= _Delay;
	TimeDur		= _Duration;

	memset(Phase, 0, ILopAi::MAX_STATE_NAME);
	if(_Phase)
		strcpy(Phase, _Phase);

	memset(MsgText, 0, ILopAi::MAX_MSG_VALUE);
	if(MsgSize && _MsgText)
		memcpy(MsgText, _MsgText, MsgSize);
}



const _Tmessage& _Tmessage::operator=(const _Tmessage& r)	// r: right hand side(rhs)
{	
	MsgSize		= r.MsgSize;
	MsgType		= r.MsgType;
	MsgSend		= r.MsgSend;

	TimeCur		= r.TimeCur;
	TimeDly		= r.TimeDly;
	TimeDur		= r.TimeDur;

	memset(Phase,   0, ILopAi::MAX_STATE_NAME);
	memset(MsgText, 0, ILopAi::MAX_MSG_VALUE);

	strcpy(Phase, r.Phase);
	memcpy(MsgText, r.MsgText, ILopAi::MAX_MSG_VALUE);

	return *this;
}

const _Tmessage& _Tmessage::operator=(const _Tmessage* r)	// r: right hand side(rhs)
{	
	MsgSize		= r->MsgSize;
	MsgType		= r->MsgType;
	MsgSend		= r->MsgSend;

	TimeCur		= r->TimeCur;
	TimeDly		= r->TimeDly;
	TimeDur		= r->TimeDur;

	memset(Phase,   0, ILopAi::MAX_STATE_NAME);
	memset(MsgText, 0, ILopAi::MAX_MSG_VALUE);

	strcpy(Phase, r->Phase);
	memcpy(MsgText, r->MsgText, ILopAi::MAX_MSG_VALUE);

	return *this;
}




void _Tmessage::SetText(INT _MsgSize, void* _MsgText)
{
	MsgSize		= _MsgSize;

	memset(MsgText, 0, ILopAi::MAX_MSG_VALUE);
	if(MsgSize && _MsgText)
		memcpy(MsgText, _MsgText, MsgSize);
}

void _Tmessage::GetText(INT* pMsgSize, void* pMsgText)
{
	if(pMsgSize)
		*pMsgSize = MsgSize;

	if(MsgSize && MsgText)
		memcpy(pMsgText, MsgText, MsgSize);
}


void _Tmessage::SetType(INT _MsgType)
{
	MsgType		= _MsgType;
}

INT _Tmessage::GetType()
{
	return MsgType;
}

void _Tmessage::SetSender(void* _MsgSend)
{
	MsgSend		= _MsgSend;
}


void* _Tmessage::GetSender()
{
	return MsgSend;
}


void _Tmessage::SetTimeCur(DWORD v)
{
	TimeCur = v;
}

void _Tmessage::SetTimeDly(INT v)
{
	TimeDly = v;
}

void _Tmessage::SetTimeDur(INT v)
{
	TimeDur = v;
}

DWORD _Tmessage::GetTimeCur()
{
	return TimeCur;
}

INT _Tmessage::GetTimeDly()
{
	return TimeDly;
}

INT _Tmessage::GetTimeDur()
{
	return TimeDur;
}



////////////////////////////////////////////////////////////////////////////////
_TstateFSM::_TstateFSM() : m_LsEvt(0), m_LvAgt(0)
{
	m_pStCur	= NULL;
	m_pStOld	= NULL;
	m_pStInt	= NULL;
}

_TstateFSM::~_TstateFSM()
{
	Destroy();
}


void _TstateFSM::Destroy()
{
	SAFE_DELETE(	m_pStCur	);
	SAFE_DELETE(	m_pStOld	);
	SAFE_DELETE(	m_pStInt	);

	m_LsEvt.clear();
	m_LvAgt.clear();
}


INT _TstateFSM::Create(_Tentity* pEntity)
{
	m_pEntity	= pEntity;

	m_pStCur	= new _Tstate;
	m_pStOld	= new _Tstate;
	m_pStInt	= new _Tstate;

	m_pStInt->SetActive( ILopAi::STATE_DISABLE );

	_Tmessage pMessage(OCSST_IDLE, NULL);
	this->OnMessage("Change State", &pMessage);

//	{
//		ILopAi::_Tmessage t(OCSST_WALK, NULL);
//		m_LvAgt.push_back(t);
//	}
	{
		ILopAi::_Tmessage t(OCSST_IDLE, NULL);
		m_LvAgt.push_back(t);
	}



	return 0;
}


INT _TstateFSM::Update()
{
	// 1. 시간을 갱신한다.
	DWORD	dGetTime = ::timeGetTime();

	// 2. 이벤트 리스트의 시간을 갱신한다.
	if(!m_LsEvt.empty())
	{
		isTmsg	_F	= m_LsEvt.begin();
		isTmsg	_L	= m_LsEvt.end();

		for(; _F != _L; ++_F)
		{
			INT	TimeDly = (*_F).TimeDly;
			(*_F).TimeDly = TimeDly - (dGetTime - (*_F).TimeCur);
			(*_F).TimeCur = dGetTime;
		}

		_F	= m_LsEvt.begin();

		if( (*_F).TimeDly <= 0.f)		// 지체시간이 0 보다 작으면
		{
			(*_F).TimeDly = 0;						// 1. Delay Time 을 0으로 설정
			(*_F).TimeCur = dGetTime;				// 2. 시작 시간을 다시 재조정
			_Tmessage t = (*_F);					// 3. 복사 한 다음

			this->OnMessage("Change State", &t);	// 4. 이벤트 설정
			m_LsEvt.erase(_F);						// 5. 리스트에서 제거
		}
	}

	// 3. 인터럽트 상태를 실행한다.
	if( ILopAi::STATE_ENABLE == m_pStInt->GetActive())
	{
		INT	TimeCur = m_pStInt->Message.GetTimeCur();
		INT	TimeDur = m_pStInt->Message.GetTimeDur();
		INT	TimeInt = dGetTime - TimeCur;			// 1. 현재 시간에서 상태의 현재 시간을 뺀다.

		if(TimeDur<0)								// 2. 지속 시간이 음수이면 상태를 다시 설정한다.
		{
			INT	iSize = this->m_LvAgt.size();
			int nIdx = rand() % iSize;				// 3. Agent list에서 임의의 상태를 가져온다.

			_Tmessage t( m_LvAgt[nIdx] );

			t.SetTimeCur(::timeGetTime());
			t.SetTimeDly(0);
			t.SetTimeDur(1300 + 200*(rand()%10));
			
			if( 0 == strcmp(t.Phase, OCSST_WALK))
			{
				D3DXVECTOR3	vcT(0,0,0);

				TocAmun*	pAmun = (TocAmun*)m_pEntity;

				vcT = pAmun->m_Mtgt;
				vcT[0] += (rand()%401 - 200);
				vcT[1] = 0;
				vcT[2] += (rand()%401 - 200);

				t.SetText( sizeof(D3DXVECTOR3), &vcT);
			}

			this->OnMessage("Change State", &t);	// 3. 상태를 설정한다.
			
			m_pStInt->SetActive( ILopAi::STATE_DISABLE );	// 4. 인터럽트 상태를 비활성화 한다.
		}

		else
		{
			TimeDur	-= TimeInt;						// 5. 지속 시간을 줄인다.
			m_pStInt->Message.TimeDur = TimeDur;	// 6. 지속 시간을 설정한다.
			m_pStInt->Message.TimeCur = dGetTime;	// 7. 상태의 현재 시간을 설정한다.
			m_pStInt->Exec(m_pEntity);				// 8. 상태를 실행한다.
			return 0;
		}
	}

	// 4. 현재 상태를 실행한다.
	if( ILopAi::STATE_ENABLE == m_pStCur->GetActive())
	{
		INT	TimeCur = m_pStCur->Message.TimeCur;
		INT	TimeDur = m_pStCur->Message.TimeDur;
		INT	TimeInt = dGetTime - TimeCur;			// 1. 현재 시간에서 상태의 현재 시간을 뺀다.

		if(TimeDur<0)								// 2. 지속 시간이 음수이면 상태를 다시 설정한다.
		{
			INT	iSize = this->m_LvAgt.size();
			int nIdx = rand() % iSize;				// 3. Agent list에서 임의의 상태를 가져온다.

			ivTmsg	it = m_LvAgt.begin() + nIdx;

			_Tmessage t(it);

			t.SetTimeCur(::timeGetTime());
			t.SetTimeDly(0);
			t.SetTimeDur(1300 + 200*(rand()%10));
			
			if( 0 == strcmp(t.Phase, OCSST_WALK))
			{
				D3DXVECTOR3	vcT(0,0,0);

				TocAmun*	pAmun = (TocAmun*)m_pEntity;

				vcT = pAmun->m_Mtgt;
				vcT[0] += (rand()%401 - 200);
				vcT[1] = 0;
				vcT[2] += (rand()%401 - 200);

				t.SetText( sizeof(D3DXVECTOR3), &vcT);
			}

			this->OnMessage("Change State", &t);	// 3. 상태 설정
		}
		else
		{
			TimeDur	-= TimeInt;						// 4. 지속 시간을 줄인다.
			m_pStCur->Message.TimeDur = TimeDur;	// 5. 지속 시간을 설정한다.
			m_pStCur->Message.TimeCur = dGetTime;	// 6. 상태의 현재 시간을 설정한다.
		}
	
		m_pStCur->Exec(m_pEntity);					// 7. 상태를 실행한다.
	}

	return 0;
}


INT _TstateFSM::OnMessage(char* sMsg, _Tmessage* pMessage)
{
	if(0==_stricmp("Change State", sMsg))
	{
		// Delay가 없는 경우 바로 실행
		if(0 >= pMessage->TimeDly)
		{
			int hr;
			char	sCmd[64]={0};
			sprintf(sCmd, "Enter %s", pMessage->Phase);

			// 새로운 상태가 가능한지 확인한다.
			hr = m_pEntity->QueryState(sCmd, pMessage);

			if(SUCCEEDED(hr))
			{
				// 과거 상태에서 빠져 나간다.
				m_pStCur->Exit(m_pEntity);

				// 과거 상태의 값을 보존한다.
				m_pStOld->SetPhase(m_pStCur->GetPhase() );
				m_pStCur->SetPhase(pMessage);
			}
		}

		// 우선 순위 큐에 넣는다.
		else
		{
			_Tmessage	t	= *pMessage;
			isTmsg	_F	= m_LsEvt.begin();
			isTmsg	_L	= m_LsEvt.end();

			for(; _F != _L; ++_F)
			{
				if(t.TimeDly< (*_F).TimeDly)
					break;
			}

			m_LsEvt.insert(_F, t);
		}

		return 0;
	}


	return -1;
}

_Tstate* _TstateFSM::GetStateCur()	{	return m_pStCur;	}
_Tstate* _TstateFSM::GetStateOld()	{	return m_pStOld;	}
_Tstate* _TstateFSM::GetStateInt()	{	return m_pStInt;	}



}// namespace ILopAi