// Implementation of the CLcxMdlSolid class.
//
////////////////////////////////////////////////////////////////////////////////


#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <d3d9.h>
#include <d3dx9.h>

#include <Lc/LcType.h>
#include <Lc/LcUtil.h>

#include "ILcmMdl.h"
#include "ILcmMdlX.h"															// Model Base

#include "LcxMdl.h"																// Model Object

#include "LcmPack.h"															// Model Pack



namespace Lcm
{

CLcmPack::CLcmPack()
{
	m_pDev = NULL;
}


CLcmPack::~CLcmPack()
{
	Destroy();
}


INT CLcmPack::Create(void* p1, void* p2, void* p3, void* p4)
{
	m_pDev = (PDEV)p1;
	m_pLcl = p2;
	
	return 0;
}


void CLcmPack::Destroy()
{
	SAFE_DEL_LST(	m_vObj	);
}


INT CLcmPack::FrameMove()
{
	INT		i;
	INT		iSize;

	iSize = m_vObj.size();

	for(i=0; i<iSize; ++i)
	{
		ILcmMdl*	pMdB = m_vObj[i];
		pMdB->FrameMove();
	}

	return 0;
}


void CLcmPack::Render()
{
	INT		i;
	INT		iSize;
	
	iSize = m_vObj.size();
	
	for(i=0; i<iSize; ++i)
	{
		ILcmMdl*		pMdB  = m_vObj[i];
		CLcxMdl*	pObj  = (CLcxMdl*)pMdB;
		
		pObj->BndBoxRender();
		pObj->Render();
	}
}





INT CLcmPack::Query(char* sCmd, void* pData)
{
	if(0==_stricmp("local", sCmd))
	{
		INT nAdd = ((INT)m_pLcl);

		*((INT*)pData) = nAdd;
		
		return 0;
	}

	return -1;
}





void CLcmPack::AddObj(ILcmMdl* pMdB
							, VEC3 vcP		// Position
							, void* p1		// Camera
							, void* p2		// Field
							, void* p3		// Field Local
							, void* p4		// Parent Package
							)
{
	if(!m_pLcl)
		return;

	CLcxMdl*	pObj = (CLcxMdl*)pMdB;
	INT			nType= pMdB->GetType();

	ILcmMesh*	pSrc = NULL;
	ILcmMdl*	pMdl = NULL;

	pObj->Query("Get Mesh Source", &pSrc);

	if(!pSrc)
		return;

	if(LCX_MDL_BILL == nType)
		MdlX_Create("billboard", &pMdl, m_pDev, pSrc);

	else if(LCX_MDL_SOLID == nType)
		MdlX_Create("solid", &pMdl, m_pDev, pSrc);


	if(!pMdl)
		return;

	pObj  = (CLcxMdl*)pMdl;

	pObj->SetPosition((FLOAT*)&vcP);
	pObj->SetBndInf(pSrc->GetBndInf());

	pObj->Query("set camera", p1);
	pObj->Query("set field", p2);
	pObj->Query("set local", p3);
	pObj->Query("set package", p4);

	m_vObj.push_back(pMdl);
}





void CLcmPack::SetLcl(void*	pLcl)
{
	m_pLcl = pLcl;
}



void CLcmPack::Push_Back(ILcmMdl* pMdB)
{
	m_vObj.push_back(pMdB);
}

INT CLcmPack::Size()
{
	return m_vObj.size();
}

ILcmMdl* CLcmPack::GetObj(int n)
{
	INT iSize = m_vObj.size();

	if(n<0 || n>=iSize)
		return NULL;

	return m_vObj[n];
}


void CLcmPack::Erase(int n)
{
	INT iSize = m_vObj.size();

	if(n<0 || n>=iSize)
		return;

	itLcxMdl it = m_vObj.begin() + n;
	ILcmMdl* p = (*it);
	delete (p);
	m_vObj.erase(it);
}


}// namespace Lcm





