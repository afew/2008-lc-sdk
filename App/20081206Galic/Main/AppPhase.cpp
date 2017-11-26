// AppPhase.cpp: implementation of the CAppPhase class.
//
//////////////////////////////////////////////////////////////////////


#include "../_StdAfx.h"


CAppPhase::CAppPhase()
{
	m_nPhase	= 0		;
	m_pDevSys	= NULL	;
	m_pLsc		= NULL	;
}

CAppPhase::~CAppPhase()
{
	CAppPhase::Destroy();
}


INT CAppPhase::Create(void* p1, void* p2, void* p3, void* p4)
{
	m_nPhase	= *((INT*)p1);
	m_pDevSys	= (ILcDev*)p2;


	TCHAR	sFile[160]={0};

	memcpy(sFile, &m_nPhase, sizeof m_nPhase);

	if(FAILED(m_pDevSys->Query("Get Script File", sFile)))
		return -1;

	SAFE_NEWCREATE2( m_pLsc, LcLua::CLcLscApp, sFile, m_pDevSys);


	if(FAILED(m_pLsc->Lua_Init()))
		return -1;

	return 0;
}

void CAppPhase::Destroy()
{
	if(m_pLsc)
		m_pLsc->Lua_Destroy();

	SAFE_DELETE(	m_pLsc		);
	
	return;
}

INT CAppPhase::Restore()
{
	return 0;
}

void CAppPhase::Invalidate()
{
	return;
}

INT CAppPhase::FrameMove()
{
	INT hr = 0;
	
	if(m_pLsc)
		hr = m_pLsc->Lua_FrameMove();

	if(FAILED(hr))
		return -1;

	else if(0==hr)
		return 0;

	GMAIN->SetPhase(	hr	);
	
	return 0;
}

void CAppPhase::Render()
{
	if(NULL == m_pLsc)
		return;

	m_pLsc->Lua_Render();
}

INT CAppPhase::Query(char* sCmd, void* pData)
{
	return -1;
}