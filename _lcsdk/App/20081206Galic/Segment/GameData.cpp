// Implementation of the CGameData class.
//
////////////////////////////////////////////////////////////////////////////////


#include <windows.h>

#include <d3d9.h>
#include <d3dx9.h>

#include <Lc/ILcDev.h>
#include <Lc/ILcInput.h>
#include <Lc/ILcxCam.h>
#include <Lc/ILcmMdl.h>
#include <Lc/ILcxmFld.h>
#include <Lc/LcUtil.h>

#include <Lc/LcType.h>

#include <Lc/ILcM3d.h>
#include <Lc/ILcxTbl.h>

#include <Lc/ILcxmFld.h>
#include <Lc/ILcxmT.h>

#include "../OcFSM/ILopAi.h"
#include "../OcFSM/ocAmun.h"

#include "IGameData.h"
#include "GameData.h"

#include "IGameSgmnt.h"
#include "GameSgmnt.h"


CGameData::CGameData()
{
	m_pDevSys		= NULL;

	memset(m_sUid, 0, sizeof m_sUid);
	memset(m_sPwd, 0, sizeof m_sPwd);


	m_SvrIdx	= -1;					// Play Server

	m_CharIdxMan	= -1;					// Selecting Man Character Index
	m_CharIdxLady	= -1;					// Selecting Lady Character Index
}



CGameData::~CGameData()
{
	Destroy();
}


INT CGameData::Create(void* p1, void* p2, void* p3, void* p4)
{
	INT		hr=-1;
	INT		i=0;

	m_pDevSys	= (ILcDev*)p1;

	m_MateLst.push_back(&m_MateMain);

	INT	nId = 20;

	CGameData::TocMate*	pMate = &m_MateMain;

	
	for(i=0; i<4; ++i)
	{
		TocAmun*	pAmun = &pMate->MateAm[i];

		pAmun->Create((void*)nId);


		INT Thp	= 10;
		INT Tmp	= 20;
		INT Tst	= 10;
		INT stThreshold	= 20;

		D3DXVECTOR3 vcS	= D3DXVECTOR3(1, 1.5f, 2.f);
		D3DXVECTOR3 vcB	= D3DXVECTOR3(3, 1, 2);
		D3DXVECTOR3 vcE	= D3DXVECTOR3(300, 0, 10);
		D3DXVECTOR3 vcD	= vcE - vcB;
		D3DXVec3Normalize(&vcD, &vcD);

		vcS	*= 0.1f;
		stThreshold	*=0.6f;
		vcD	= vcE - vcB;
		D3DXVec3Normalize(&vcD, &vcD);

		pAmun->QueryValue("Set Speed", &vcS);
		pAmun->QueryValue("Set Position", &vcB);
		pAmun->QueryValue("Set Direction", &vcD);
		pAmun->QueryValue("Set Target", &vcE);

		pAmun->QueryValue("Set Hp", &Thp);
		pAmun->QueryValue("Set Mp", &Tmp);
		pAmun->QueryValue("Set Stamina", &Tst);
		pAmun->QueryValue("Set St Threshold", &stThreshold);

//		{
//			ILopAi::_Tmessage pMessage(OCSST_ATTACK, NULL, 0, ILopAi::CMD_ONCE, NULL, 100);
//			pAmun->OnMessage("Change State", &pMessage);
//		}
//		{
//			ILopAi::_Tmessage pMessage(OCSST_DAMAGE, NULL, 0, ILopAi::CMD_ONCE, NULL, 200);
//			pAmun->OnMessage("Change State", &pMessage);
//		}
//		{
//			ILopAi::_Tmessage pMessage(OCSST_RUN   , NULL, 0, ILopAi::CMD_LOOP, NULL, 300);
//			pAmun->OnMessage("Change State", &pMessage);
//		}
	}


	ILopAi::_Tmessage pMessage(OCSST_FOLLOW, NULL, 0, ILopAi::CMD_ONCE, NULL, 100);
	m_MateMain.MateAm[2].OnMessage("Change State", &pMessage);
	m_MateMain.MateAm[3].OnMessage("Change State", &pMessage);

	return 0;
}


void CGameData::Destroy()
{
	int i=0;

	m_pDevSys		= NULL;

	memset(m_sUid, 0, sizeof m_sUid);
	memset(m_sPwd, 0, sizeof m_sPwd);


	m_SvrIdx		= -1;
	m_CharIdxMan	= -1;
	m_CharIdxLady	= -1;

	m_SvrLst.clear();

	for(i=0; i<m_CharLstMan.size(); ++i)
		delete m_CharLstMan[i];

	for(i=0; i<m_CharLstLady.size(); ++i)
		delete m_CharLstLady[i];

	m_CharLstMan.clear();
	m_CharLstLady.clear();
}


INT CGameData::Query(char* sCmd, void* pData)
{
	if(0==_stricmp("Setup Login UID", sCmd))
	{
		memset(m_sUid, 0, sizeof m_sUid);
		strcpy(m_sUid, (char*)pData);
	
		return 0;
	}

	else if(0==_stricmp("Get Login UID", sCmd))
	{
		strcpy((char*)pData, m_sUid);
		return 0;
	}

	else if(0==_stricmp("Setup Login PWD", sCmd))
	{
		memset(m_sPwd, 0, sizeof m_sUid);
		strcpy(m_sPwd, (char*)pData);
	
		return 0;
	}

	else if(0==_stricmp("Get Login PWD", sCmd))
	{
		strcpy((char*)pData, m_sPwd);
		return 0;
	}

	else if(0 == _stricmp("Add Server List", sCmd))
	{
		std::string s = (char*)pData;
		m_SvrLst.push_back(s);

		return 0;
	}

	else if(0 == _stricmp("Select Server", sCmd))
	{
		int nIdx = atoi( (char*)pData) -1;

		if(nIdx<0 || nIdx>=m_SvrLst.size())
			return -1;

		m_SvrIdx	= nIdx;
		strcpy((char*)pData, m_SvrLst[nIdx].c_str() );

		return 0;
	}

	else if(0 == _stricmp("Selected Server", sCmd))
	{
		if(m_SvrIdx<0)
			strcpy((char*)pData, "<<No Selected Server>>" );
		
		else
			strcpy((char*)pData, m_SvrLst[m_SvrIdx].c_str() );

		return 0;
	}


	else if(0 == _stricmp("Add Man Character", sCmd))
	{
		TocAmun* p = new TocAmun;

		p->SetName((char*)pData);
		m_CharLstMan.push_back(p);
	
		return 0;
	}

	else if(0 == _stricmp("Add Lady Character", sCmd))
	{
		TocAmun* p = new TocAmun;

		p->SetName((char*)pData);
		m_CharLstLady.push_back(p);
	
		return 0;
	}

	else if(0 == _stricmp("Setup Man Character Index", sCmd))
	{
		int nIdx = atoi( (char*)pData) -1;

		if(nIdx<0 || nIdx>=m_CharLstMan.size())
			return -1;

		m_CharIdxMan	= nIdx;

		return 0;
	}

	else if(0 == _stricmp("Setup Lady Character Index", sCmd))
	{
		int nIdx = atoi( (char*)pData) -1;

		if(nIdx<0 || nIdx>=m_CharLstMan.size())
			return -1;

		m_CharIdxLady	= nIdx;

		return 0;
	}


	else if(0 == _stricmp("Setup Man Character Name", sCmd))
	{
		TocAmun*	p = &m_MateMain.MateAm[0];
		p->SetName( (char*)pData);
		return 0;
	}

	else if(0 == _stricmp("Setup Lady Character Name", sCmd))
	{
		TocAmun*	p = &m_MateMain.MateAm[1];
		p->SetName( (char*)pData);
		return 0;
	}


	else if(0 == _stricmp("Selected Man Character", sCmd))
	{
		if(m_CharIdxMan<0)
			strcpy((char*)pData, "<<No Selected Character>>" );
		
		else
		{
			TocAmun*	p = &m_MateMain.MateAm[0];
			strcpy((char*)pData, p->GetName() );
		}

		return 0;
	}

	else if(0 == _stricmp("Selected Lady Character", sCmd))
	{
		if(m_CharIdxLady<0)
			strcpy((char*)pData, "<<No Selected Character>>" );
		
		else
		{
			TocAmun*	p = &m_MateMain.MateAm[1];
			strcpy((char*)pData, p->GetName());
		}

		return 0;
	}


	return -1;
}



void CGameData::MateListErase()
{
	int iSize = m_MateLst.size();
	
	for(int M=0; M<iSize; ++M)
	{
		for(int S=0; S<4; ++S)
		{
			CGameData::TocMate*	pMate = m_MateLst[M];
			TocAmun*	pAmun= &pMate->MateAm[S];
			ILcmMdl*	pMdl = (ILcmMdl*)pAmun->GetModel();
			
			if(pMdl)
			{
				delete pMdl;
				pAmun->SetModel(NULL);
			}

			pAmun->Destroy();
		}
	}
}


void CGameData::MateFieldPosition(INT M, INT S, void* pField)
{
	ILcxmFld*	pHgtFld = (ILcxmFld*)pField;

	CGameData::TocMate*	pMate = m_MateLst[M];
	TocAmun*	pAmun = &pMate->MateAm[S];
	ILcmMdl*	pMdl = (ILcmMdl*)pAmun->GetModel();

	if(ILopAi::STATE_ENABLE != pAmun->GetActive() )
		return;

	D3DXVECTOR3	vcIn;

	if(pMdl && SUCCEEDED(pMdl->GetPosition((FLOAT*)&vcIn)))
	{
		D3DXVECTOR3	vcPos;
		if(SUCCEEDED(pHgtFld->GetHeight( (FLOAT*)&vcPos, (FLOAT*)&vcIn)))
		{
			pMdl->SetPosition((FLOAT*)&vcPos);
		}
	}
}



INT CGameData::MateUpdate()
{
	DOUBLE		dAvg=0;
	CGameSgmnt*	pSgmnt	= (CGameSgmnt*)LcDev_GetUserData2(&m_pDevSys);
	ILcxmFld*	pHgtFld	= NULL;

	if(pSgmnt)
		pHgtFld = pSgmnt->GetOutdoor();

	m_pDevSys->Query("Get Elapsed Average Time", &dAvg);
	FLOAT fTime = dAvg;

	int iSize = m_MateLst.size();
	for(int M=0; M<iSize; ++M)
	{
		for(int S=0; S<4; ++S)
		{
			CGameData::TocMate*	pMate = m_MateLst[M];
			TocAmun*	pAmun = &pMate->MateAm[S];
			ILcmMdl*	pMdl = (ILcmMdl*)pAmun->GetModel();

			if(ILopAi::STATE_ENABLE != pAmun->GetActive() )
				continue;

			if(NULL == pMdl)
				continue;

			INT nState = pAmun->GetState();



			// 1. FSM에 필요한 데이터를 만든다.

			// 이동
			if( ILopAi::OCST_MOVE & nState)
			{
				D3DXVECTOR3	vcPos(0,0,0);
				pAmun->QueryValue("Get Position", &vcPos);
	
				if(SUCCEEDED(pHgtFld->GetHeight( (FLOAT*)&vcPos, (FLOAT*)&vcPos)))
				{
					pAmun->QueryValue("Set Position", &vcPos);
					pMdl->SetPosition((FLOAT*)&vcPos);
				}
			}


			// 2. 시간 변수를 전달한다.
			pAmun->QueryValue("Set Time Delta", &fTime);

			// 3. FSM을 갱신한다.
			pAmun->UpdateFSM();


			// 4. FSM을 통해서 갱신된 데이터를 이용해서 렌더링에 필요한 데이터를 갱신한다.
			// 방향 설정
			if( ILopAi::OCST_MOVE & nState)
			{
				D3DXVECTOR3	v(0, 0, 0);
				D3DXVECTOR4	t(0, 1, 0, 0);
				pAmun->QueryValue("Get Direction", &v);

				v.y = 0.f;
				D3DXVec3Normalize(&v, &v);
//				printf("%f %f %f", v.x, v.y, v.z);

				if(v.x>0.9999f)
					t.w = 0;

				else if(v.x<=-0.9999f)
					t.w = D3DX_PI;

				else
					t.w = -acosf(v.x);

				if(v.z<0)
					t.w *= -1;

				t.w -= (D3DX_PI/2.F);

//				printf("    %f\n", t.w);

				pMdl->SetRotation((FLOAT*)&t);
			}


			// 5. Animation 설정
			INT nAniCur = nState & 0x00FFFFFF;
			pMdl->Query("Set Frame Index", &nAniCur);


			pMdl->FrameMove();
		}
	}


	// 카메라 이동
	CGameData::TocMate*	pMate = m_MateLst[0];
	TocAmun*	pAmun	= &pMate->MateAm[0];
	INT			nState	= pAmun->GetState();

	if( ILopAi::OCST_MOVE & nState)
	{

		D3DXVECTOR3	vcPos(0,0,0);
		pAmun->QueryValue("Get Position", &vcPos);

		ILcxCam*	pCam	= NULL;
		pHgtFld->Query("Get Camera", &pCam);

		pCam->SetBasis(&vcPos);
		pCam->FrameMove();
	}

	return 0;
}


void CGameData::MateRender()
{
	int iSize = m_MateLst.size();
	for(int M=0; M<iSize; ++M)
	{
		for(int S=0; S<4; ++S)
		{
			CGameData::TocMate*	pMate = m_MateLst[M];
			TocAmun*	pAmun = &pMate->MateAm[S];
			ILcmMdl*	pMdl = (ILcmMdl*)pAmun->GetModel();

			if(ILopAi::STATE_ENABLE != pAmun->GetActive() )
				continue;

			pMdl->Render();
		}
	}
}



INT CGameData::MateModelSetup(INT M, INT S, void* pMdl)
{
	CGameData::TocMate*	pMate = m_MateLst[M];
	TocAmun*	pAmun	= &pMate->MateAm[S];

	pAmun->SetModel(pMdl);

	return 0;
}

INT CGameData::MateSetActivation(INT M, INT S, INT A)
{
	INT hr=0;

	TocMate*	pMate = m_MateLst[M];
	TocAmun*	pAmun = &pMate->MateAm[S];

	pAmun->SeActive(A);
	return 0;
}


INT CGameData::MateSetRotation(INT M, INT S, void* pV)
{
	INT hr=0;

	TocMate*	pMate = m_MateLst[M];
	TocAmun*	pAmun = &pMate->MateAm[S];
	ILcmMdl*	pMdl = (ILcmMdl*)pAmun->GetModel();

	hr = pMdl->SetRotation( (FLOAT*) pV);
	return hr;
}


INT CGameData::MateSetPosition(INT M, INT S, void* pV)
{
	INT hr=0;
	
	CGameData::TocMate*	pMate = m_MateLst[M];
	TocAmun*	pAmun = &pMate->MateAm[S];
	ILcmMdl*	pMdl = (ILcmMdl*)pAmun->GetModel();

	pAmun->QueryValue("Set Position", (D3DXVECTOR3*)pV);
	pMdl->SetPosition((float*) pV);

	if(0 == M && 0 == S)
	{
		CGameSgmnt*	pSgmnt	= (CGameSgmnt*)LcDev_GetUserData2(&m_pDevSys);
		ILcxmFld*	pHgtFld	= NULL;

		if(pSgmnt)
		{
			pHgtFld = pSgmnt->GetOutdoor();

			ILcxCam*	pCam	= NULL;
			pHgtFld->Query("Get Camera", &pCam);

			pCam->SetBasis((D3DXVECTOR3*)pV);
			pCam->FrameMove();
		}
	}

	return hr;
}


void* CGameData::GetMateList() const
{
	return (void*)&m_MateLst;
}


CGameData::TocMate* CGameData::GetMate(INT M) const
{
	if(M<0 || M>= m_MateLst.size())
		return NULL;

	return m_MateLst[M];
}

void* CGameData::GetMateModel(INT M, INT S)
{
	CGameData::TocMate*	pMate = m_MateLst[M];
	TocAmun*	pAmun = &pMate->MateAm[S];
	void*		pMdl = pAmun->GetModel();

	return pMdl;
}