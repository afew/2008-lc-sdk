// Implementation of the TocAmun class.
//
////////////////////////////////////////////////////////////////////////////////

#pragma warning(disable: 4786)

#include <windows.h>
#include <stdio.h>
#include <memory.h>
#include <d3dx9.h>

#include "ILopAi.h"
#include "ocAmun.h"


TocAmun::TocAmun()
{
	memset(m_AmName, 0, sizeof m_AmName);

	m_AmKaBa	= -1;
	m_AmActv	= ILopAi::STATE_ENABLE;
	m_AmMdl		= NULL;

	m_AmStat	= ILopAi::OCST_NONE;

	m_Thp		= 0;
	m_Tmp		= 0;
	m_Tst		= 0;
	m_TstH		= 0;

	m_Mdlt		= 0.1f;
	memset(m_Mspd, 0, sizeof m_Mspd);
	memset(m_Mpos, 0, sizeof m_Mpos);
	memset(m_Mdir, 0, sizeof m_Mdir);
	memset(m_Mtgt, 0, sizeof m_Mtgt);

	m_pMater	= NULL;
	m_pMatee	= NULL;
}


TocAmun::~TocAmun()
{
	Destroy();
}


INT TocAmun::Create(void* p1,void* p2,void* p3,void* p4)
{
	if(FAILED(_Tentity::Create(p1, p2, p3, p4)))
		return -1;


	ILopAi::_TstateFSM* pFSM = new ILopAi::_TstateFSM;

	pFSM->Create(this);
	m_pFSM = pFSM;

	return 0;
}


void TocAmun::Destroy()
{
	if(m_pFSM)
	{
		ILopAi::_TstateFSM* pFSM = (ILopAi::_TstateFSM*)m_pFSM;
		delete pFSM;
		m_pFSM = NULL;
	}
}


INT TocAmun::UpdateFSM()
{
	ILopAi::_TstateFSM*	pFSM = (ILopAi::_TstateFSM*)m_pFSM;

	if(pFSM)
	{
		if(FAILED(pFSM->Update()))
			return -1;
	}
	
	return 0;
}


INT TocAmun::QueryState(char* sCmd, void* pData)
{
	INT nId	= this->GetID();
	ILopAi::_TstateFSM*	pFSM = (ILopAi::_TstateFSM*)m_pFSM;



	if(0 ==_stricmp(sCmd, "Enter " OCSST_ATTACK))
	{
		m_AmStat = ILopAi::OCST_ATTACK;
//		printf("(%4d): Enter " OCSST_ATTACK "\n", nId );
		return 0;
	}
	else if(0 ==_stricmp(sCmd, "Exit " OCSST_ATTACK))
	{
//		printf("(%4d): Exit " OCSST_ATTACK "\n", nId );
		return 0;
	}
	else if(0 ==_stricmp(sCmd, "Exec " OCSST_ATTACK))
	{
//		printf("(%4d): Exec " OCSST_ATTACK "\n", nId );
		return 0;
	}


	if(0 ==_stricmp(sCmd, "Enter " OCSST_DAMAGE))
	{
		m_AmStat = ILopAi::OCST_DAMAGE;
//		printf("(%4d): Enter " OCSST_DAMAGE "\n", nId );
		return 0;
	}
	else if(0 ==_stricmp(sCmd, "Exit " OCSST_DAMAGE))
	{
//		printf("(%4d): Exit " OCSST_DAMAGE "\n", nId );
		return 0;
	}
	else if(0 ==_stricmp(sCmd, "Exec " OCSST_DAMAGE))
	{
//		printf("(%4d): Exec " OCSST_DAMAGE "\n", nId );
		return 0;
	}



	// Idle
	else if(0 ==_stricmp(sCmd, "Enter " OCSST_IDLE))
	{
		m_AmStat = ILopAi::OCST_IDLE;
//		printf("(%4d): £¢Enter " OCSST_IDLE "£¢\n", nId );
		return 0;
	}

	else if(0 ==_stricmp(sCmd, "Exit " OCSST_IDLE))
	{
//		printf("(%4d): £¢Exit " OCSST_IDLE "£¢\n", nId );
		return 0;
	}

	else if(0 ==_stricmp(sCmd, "Exec " OCSST_IDLE))
	{
//		printf("(%4d): £¢Exec " OCSST_IDLE "£¢\n", nId );
		return 0;
	}


	// Walk
	else if(0 ==_stricmp(sCmd, "Enter " OCSST_WALK))
	{
		m_AmStat = ILopAi::OCST_WALK;

		if(pData)
		{
			ILopAi::_Tmessage* pMessage	= (ILopAi::_Tmessage*)pData;

			INT	nSize = 0;
			D3DXVECTOR3	vcT;
			pMessage->GetText(&nSize, &vcT);

			if(nSize)
				m_Mtgt = vcT;
		}
//		printf("(%4d): £¢Enter " OCSST_WALK £¢\n", nId );
		return 0;
	}


	else if(0 ==_stricmp(sCmd, "Exit " OCSST_WALK))
	{
//		printf("(%4d): £¢Exit " OCSST_WALK "£¢\n", nId );
		return 0;
	}

	else if(0 ==_stricmp(sCmd, "Exec " OCSST_WALK))
	{
		D3DXVECTOR3	vcT = m_Mtgt - m_Mpos;
		float		fL	= D3DXVec3LengthSq(&vcT);

		float		fEps = m_Mspd[0] * m_Mdlt;

		if(fL< fEps * fEps )
		{
			ILopAi::_Tmessage pMessage(OCSST_IDLE, NULL);
			this->OnMessage("Change State", &pMessage);
			return 0;
		}

		D3DXVec3Normalize(&m_Mdir, &vcT);

		m_Mpos[0] += m_Mspd[0] * m_Mdir[0] * m_Mdlt;
		m_Mpos[1] += m_Mspd[0] * m_Mdir[1] * m_Mdlt;
		m_Mpos[2] += m_Mspd[0] * m_Mdir[2] * m_Mdlt;

//		printf("(%4d): £¢Exec  " OCSST_WALK "(%f %f %f)£¢\n", nId,  m_Mpos[0], m_Mpos[1], m_Mpos[2]);
		return 0;
	}



	// Follow
	else if(0 ==_stricmp(sCmd, "Enter " OCSST_FOLLOW))
	{
		m_AmStat = ILopAi::OCST_FOLLOW;

//		printf("(%4d): £¢Enter " OCSST_FOLLOW £¢\n", nId );
		return 0;
	}


	else if(0 ==_stricmp(sCmd, "Exit " OCSST_FOLLOW))
	{
//		printf("(%4d): £¢Exit " OCSST_FOLLOW "£¢\n", nId );
		return 0;
	}

	else if(0 ==_stricmp(sCmd, "Exec " OCSST_FOLLOW))
	{
		if(NULL == m_pMater)
			return 0;

		D3DXVECTOR3	vcRight;
		D3DXVECTOR3	vcDir;

		D3DXVec3Cross(&vcRight, &D3DXVECTOR3(0, 1, 0), &m_pMater->m_Mdir);

		D3DXVec3Normalize(&vcRight, &vcRight);
		D3DXVec3Normalize(&vcDir, &m_pMater->m_Mdir);

		vcRight -= (vcDir *0.5f);
		D3DXVec3Normalize(&vcRight, &vcRight);
		

		m_Mtgt = m_pMater->m_Mpos + vcRight * 20.0f;
		D3DXVECTOR3	vcT = m_Mtgt - m_Mpos;

		vcT.y = 0;
		float		fL	= D3DXVec3LengthSq(&vcT);

		float		fEps = m_Mspd[0] * m_Mdlt* 4.F;

		if(fL< fEps * fEps )
		{
			ILopAi::_Tmessage pMessage(OCSST_IDLE, NULL);
			this->OnMessage("Change State", &pMessage);
			return 0;
		}

		D3DXVec3Normalize(&m_Mdir, &vcT);

		m_Mpos[0] += m_Mspd[0] * m_Mdir[0] * m_Mdlt;
		m_Mpos[1] += m_Mspd[0] * m_Mdir[1] * m_Mdlt;
		m_Mpos[2] += m_Mspd[0] * m_Mdir[2] * m_Mdlt;

//		printf("(%4d): £¢Exec  " OCSST_FOLLOW "(%f %f %f)£¢\n", nId,  m_Mpos[0], m_Mpos[1], m_Mpos[2]);
		return 0;
	}



	// Run
	else if(0 ==_stricmp(sCmd, "Enter " OCSST_RUN))
	{
		m_AmStat = ILopAi::OCST_RUN;
//		printf("(%4d): £¢Enter " OCSST_RUN "£¢\n", nId );
		return 0;
	}

	else if(0 ==_stricmp(sCmd, "Exit " OCSST_RUN))
	{
//		printf("(%4d): £¢Exit " OCSST_RUN "£¢\n", nId );
		return 0;
	}

	else if(0 ==_stricmp(sCmd, "Exec " OCSST_RUN))
	{
		D3DXVECTOR3	vcT = m_Mtgt - m_Mpos;
		float		fL	= D3DXVec3LengthSq(&vcT);
		float		fEps = m_Mspd[1] * m_Mdlt;

		if(fL< fEps * fEps * 1.4f)
		{
			ILopAi::_Tmessage pMessage(OCSST_WALK, NULL);
			this->OnMessage("Change State", &pMessage);
			return 0;
		}

		D3DXVec3Normalize(&m_Mdir, &vcT);

		m_Mpos[0] += m_Mspd[1] * m_Mdir[0] * m_Mdlt;
		m_Mpos[1] += m_Mspd[1] * m_Mdir[1] * m_Mdlt;
		m_Mpos[2] += m_Mspd[1] * m_Mdir[2] * m_Mdlt;

//		printf("(%4d): £¢Exec Run(%f %f %f)£¢\n", nId,  m_Mpos[0], m_Mpos[1], m_Mpos[2]);
		return 0;
	}


	// Dash
	else if(0 ==_stricmp(sCmd, "Enter " OCSST_DASH))
	{
		m_AmStat = ILopAi::OCST_DASH;
//		printf("(%4d): £¢Enter " OCSST_RUN "£¢\n", nId );
		return 0;
	}

	else if(0 ==_stricmp(sCmd, "Exit " OCSST_DASH))
	{
//		printf("(%4d): £¢Exit " OCSST_RUN "£¢\n", nId );
		return 0;
	}

	else if(0 ==_stricmp(sCmd, "Exec " OCSST_DASH))
	{
		m_Mpos[0] += m_Mspd[2] * m_Mdir[0] * m_Mdlt;
		m_Mpos[1] += m_Mspd[2] * m_Mdir[1] * m_Mdlt;
		m_Mpos[2] += m_Mspd[2] * m_Mdir[2] * m_Mdlt;

//		printf("(%4d): £¢Exec " OCSST_DASH "£¢\n", nId );
		return 0;
	}

	else
	{
		if(FAILED(_Tentity::QueryState(sCmd, pData)))
			return -1;
	}

	return 0;
}



INT TocAmun::QueryValue(char* sCmd, void* pData)
{
	if(0 ==_stricmp(sCmd, "Set KaBa"))		{	m_AmKaBa = *((INT*)pData);	return 0;	}
	else if(0 ==_stricmp(sCmd, "Get KaBa"))	{	*((INT*)pData)	= m_AmKaBa;	return 0;	}

	else if(0 ==_stricmp(sCmd, "Set Hp"))	{	m_Thp = *((INT*)pData);		return 0;	}
	else if(0 ==_stricmp(sCmd, "Get Hp"))	{	*((INT*)pData)	= m_Thp;	return 0;	}

	else if(0 ==_stricmp(sCmd, "Set Mp"))	{	m_Tmp = *((INT*)pData);		return 0;	}
	else if(0 ==_stricmp(sCmd, "Get Mp"))	{	*((INT*)pData)	= m_Tmp;	return 0;	}
	
	else if(0 ==_stricmp(sCmd, "Set Stamina")){	m_Tst = *((INT*)pData);		return 0;	}
	else if(0 ==_stricmp(sCmd, "Get Stamina")){	*((INT*)pData)	= m_Tst;	return 0;	}

	else if(0 ==_stricmp(sCmd, "Set St Threshold"))	{	m_TstH = *((INT*)pData);	return 0;	}
	else if(0 ==_stricmp(sCmd, "Get St Threshold"))	{	*((INT*)pData) 	= m_TstH;	return 0;	}
	
	else if(0 ==_stricmp(sCmd, "Set Time Delta"))	{	m_Mdlt = *((FLOAT*)pData);	return 0;	}
	else if(0 ==_stricmp(sCmd, "Set Time Delta"))	{	*((FLOAT*)pData) = m_Mdlt;	return 0;	}

	else if(0 ==_stricmp(sCmd, "Set Speed"))	{	m_Mspd = *((D3DXVECTOR3*)pData);	return 0;	}
	else if(0 ==_stricmp(sCmd, "Get Speed"))	{	*((D3DXVECTOR3*)pData) = m_Mspd;	return 0;	}

	else if(0 ==_stricmp(sCmd, "Set Position"))	{	m_Mpos = *((D3DXVECTOR3*)pData);	return 0;	}
	else if(0 ==_stricmp(sCmd, "Get Position"))	{	*((D3DXVECTOR3*)pData) = m_Mpos;	return 0;	}

	else if(0 ==_stricmp(sCmd, "Set Direction")){	m_Mdir = *((D3DXVECTOR3*)pData);	return 0;	}
	else if(0 ==_stricmp(sCmd, "Get Direction")){	*((D3DXVECTOR3*)pData) = m_Mdir;	return 0;	}

	else if(0 ==_stricmp(sCmd, "Set Target"))	{	m_Mtgt = *((D3DXVECTOR3*)pData);	return 0;	}
	else if(0 ==_stricmp(sCmd, "Get Target"))	{	*((D3DXVECTOR3*)pData) = m_Mtgt;	return 0;	}

	else
	{
		if(FAILED(_Tentity::QueryValue(sCmd, pData)))
			return -1;
	}

	return 0;
}


void		TocAmun::SetName(char* v){	memset(m_AmName, 0, ILopAi::MAX_STATE_NAME); strcpy(m_AmName, v);	}
const char*	TocAmun::GetName() const{	return m_AmName;	}

void		TocAmun::SeActive(INT v)	{	m_AmActv= v;	}
void		TocAmun::SetState(INT v)	{	m_AmStat = v;	}
const INT	TocAmun::GetActive() const	{	return m_AmActv;}
const INT	TocAmun::GetState() const	{	return m_AmStat;}

void		TocAmun::SetModel(void* v)	{	m_AmMdl = v;	}
void*		TocAmun::GetModel()			{	return m_AmMdl;	}

void		TocAmun::SetMateMaster(TocAmun* v){	m_pMater	= v;	}
TocAmun*	TocAmun::GetMateMaster()			{	return m_pMater;	}

void		TocAmun::SetMateSub(TocAmun* v)	{	m_pMatee	= v;	}
TocAmun*	TocAmun::GetMateSub()			{	return m_pMatee;	}
