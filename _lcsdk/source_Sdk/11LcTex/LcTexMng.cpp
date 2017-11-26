// Implementation of the CLcTexMng class.
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#include <Lc/LcType.h>

#include "ILcTex.h"
#include "LcTexBase.h"
#include "LcTexMng.h"




CLcTexMng::CLcTexMng()
{
	m_nId	= -1;

	memset(m_sName, 0, sizeof m_sName);
}



CLcTexMng::~CLcTexMng()
{
	CLcTexMng::Destroy();
}


INT CLcTexMng::Create(void* p1, void* p2, void* p3, void* p4)
{
	m_pDev = (PDEV)p1;

	return 0;
}

void CLcTexMng::Destroy()
{
	if(!m_lsTex.empty())
	{
		for(itTLcTex	it= m_lsTex.begin(); it !=m_lsTex.end(); )
		{
			ILcTex* pTx = (*it).second;
			pTx->Destroy();
			delete pTx;
			++it;
		}

		m_lsTex.clear();
	}
}


INT CLcTexMng::Query(char* sCmd, void* pData)
{
//	printf("CLcTexMng Query:%s\n", sCmd);
	return 0;
}


INT CLcTexMng::Find(char* sName, ILcTex** pData)
{
	char*		pstr = sName;
	itTLcTex	it;

	it = m_lsTex.find(pstr);

	if(it == m_lsTex.end())
	{
		*pData = NULL;
		return -1;
	}

	if(pData)
	{
		*pData = (*it).second;
	}
	
	return 0;
}

INT CLcTexMng::FindCreate(char* sCmd
						, ILcTex** pData
						, void* pVal1
						, void* pVal2
						, DWORD dColorKey
						, DWORD dFilter
						, DWORD Pool)
{
	INT hr=-1;
	char*	sName=(char*)pVal1;

	if(SUCCEEDED(Find(sName, pData)))
		return 0;

	// 없으면 파일을 생성하고 있으면 기존의 포인터
	

	hr = LcTex_CreateTexture(sCmd
							, pData
							, m_pDev
							, pVal1
							, pVal2
							, dColorKey, dFilter, Pool);

	if(FAILED(hr))
		return -1;

	m_lsTex.insert(	mpTLcTex::value_type( sName, *pData));

	return 0;
}