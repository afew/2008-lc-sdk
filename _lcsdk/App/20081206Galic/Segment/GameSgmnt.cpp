// Implementation of the CGameSgmnt class.
//
////////////////////////////////////////////////////////////////////////////////


#define USE_LC_SDK_LIB

#include <windows.h>
#include <stdio.h>
#include <d3d9.h>
#include <d3dx9.h>

#include <Lua/lua.h>
#include <Lua/lualib.h>
#include <Lua/lauxlib.h>

#include <Lc/LcUtil.h>
#include <Lc/LcString.h>
#include <Lc/LcUtilDx.h>
#include <Lc/ILcInput.h>
#include <Lc/ILcDev.h>
#include <Lc/ILcxCam.h>

#include <Lc/ILcM3d.h>

#include <Lc/ILcLsc.h>

#include <Lc/ILcxTbl.h>

#include <Lc/ILcxmFld.h>
#include <Lc/ILcxmT.h>

#include <Lc/ILcLsc.h>
#include <Lc/ILcmMdl.h>

#include "../Lsc/LscMdl.h"

#include "../OcFSM/ILopAi.h"
#include "../OcFSM/ocAmun.h"

#include "IGameData.h"

#include "IGameData.h"
#include "GameData.h"

#include "IGameSgmnt.h"
#include "GameSgmnt.h"


CGameSgmnt::CGameSgmnt()
{
	m_pDev		= NULL;
	m_pDevSys	= NULL;
	m_pInput	= NULL;
	m_pCam		= NULL;

	m_pTbTex	= NULL;
	m_pTbMdl	= NULL;
	m_pFld		= NULL;

	m_pGameData	= NULL;

	m_MousePos	= D3DXVECTOR3(0,0,0);
}



CGameSgmnt::~CGameSgmnt()
{
	Destroy();
}


void CGameSgmnt::Destroy()
{
	int i=0;

	CGameData*	pGameData = (CGameData*)LcDev_GetUserData1(&m_pDevSys);
	pGameData->MateListErase();

	SAFE_DELETE(	m_pTbTex	);
	SAFE_DELETE(	m_pTbMdl	);
	SAFE_DELETE(	m_pFld		);

	m_pDevSys		= NULL;
}



INT CGameSgmnt::Create(void* p1, void* p2, void* p3, void* p4)
{
	INT		hr=-1;

	m_pDevSys	= (ILcDev*)p1;
	m_pDev		= (PDEV)LcDev_GetD3Device((ILcDev**)&m_pDevSys);
	m_pInput	= (ILcInput*)LcDev_GetUserInput(&m_pDevSys);

	m_pGameData	= (CGameData*)LcDev_GetUserData1(&m_pDevSys);


	if(FAILED(LcxTbl_Create("Texture", &m_pTbTex, "Map/Data/TblTex.dat", m_pDev)))
		return -1;

	if(FAILED(LcxTbl_Create("Model", &m_pTbMdl, "Map/Data/TblMdl.dat", m_pDev, m_pTbTex)))
		return -1;


	if(FAILED(LcMap_CreateOutdoor("Out Door", &m_pFld, m_pDev, m_pTbTex, m_pTbMdl	)))
		return -1;

	m_MouseIdx[0]	= LcIdx::Idx16(0, 1, 2);
	m_MouseIdx[1]	= LcIdx::Idx16(0, 2, 3);
	m_MouseIdx[2]	= LcIdx::Idx16(0, 3, 1);
	m_MouseIdx[3]	= LcIdx::Idx16(3, 2, 1);

	m_MouseVtx[0]	= LcVtx::VtxD(  0,  0,  0, D3DXCOLOR(0,0,1,1));
	m_MouseVtx[1]	= LcVtx::VtxD( -2, 5, -2, D3DXCOLOR(1,0,0,1));
	m_MouseVtx[2]	= LcVtx::VtxD(  2, 5, -2, D3DXCOLOR(0,1,0,1));
	m_MouseVtx[3]	= LcVtx::VtxD(  0, 5,  2, D3DXCOLOR(1,0,1,1));

	return 0;
}


INT CGameSgmnt::FrameMove()
{
	SAFE_FRMOV(	m_pFld		);


	INT			hr=-1;
	ILcxmFld*	pHgtFld		= m_pFld;
	CGameData*	pGameData	= (CGameData*)LcDev_GetUserData1(&m_pDevSys);
	BOOL		bCtrlPck	= m_pInput->ButtonDown(LCK_BUTTONL);
	FLOAT*		pMouse		= m_pInput->GetMousePos();

	// Picking
	if(bCtrlPck)
	{
		D3DXVECTOR3* vcRayP	= (D3DXVECTOR3*)m_pCam->GetEye();
		D3DXVECTOR3	 vcRayD	= m_pCam->GetDirectionOfMouse(pMouse[0], pMouse[1]);
		D3DXVECTOR3	 vcPick	;
		
		// Picking position, Ray Begin Position, Ray Direction
		struct _T{ void* P; VEC3* R; VEC3* D; }
				t={ &vcPick, vcRayP, &vcRayD };

		if(SUCCEEDED(pHgtFld->Query("Get Pick Position", &t)))
		{
			m_MousePos = vcPick;

			CGameData::TocMate*			ppMate	= pGameData->GetMate(0);
			TocAmun*					pAmun	= &(ppMate->MateAm[0]);

			ILopAi::_Tmessage pMessage(OCSST_RUN, NULL, 0, ILopAi::CMD_LOOP, NULL, 0);
			pAmun->QueryValue("Set Target", &m_MousePos);
			pAmun->OnMessage("Change State", &pMessage);

			TocAmun*	pMatee	= pAmun->GetMateSub();
			ILopAi::_Tmessage pMessage2(OCSST_FOLLOW, NULL, 0, ILopAi::CMD_LOOP, NULL, 0);
			pMatee->OnMessage("Change State", &pMessage2);
		}
	}


	// Set Direction
	pGameData->MateUpdate();

	return 0;
}


void CGameSgmnt::Render()
{
	SAFE_RENDER(	m_pFld		);

	m_pGameData->MateRender();

	D3DXMATRIX mtMouse;
	D3DXMatrixIdentity(&mtMouse);
	mtMouse._41 = m_MousePos.x;
	mtMouse._42 = m_MousePos.y;
	mtMouse._43 = m_MousePos.z;

	m_pDev->SetTransform(D3DTS_WORLD, &mtMouse);
	m_pDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pDev->SetTexture(0, NULL);
	m_pDev->SetFVF(LcVtx::VtxD::FVF);
	m_pDev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 4, m_MouseIdx, D3DFMT_INDEX16, m_MouseVtx, sizeof( LcVtx::VtxD) );
	LcD3D_SetWorldIdentity(m_pDev, D3DTS_WORLD);
}


INT CGameSgmnt::Restore()
{
	return 0;
}


void CGameSgmnt::Invalidate()
{
}





INT CGameSgmnt::Query(char* sCmd, void* pData)
{
	INT			hr = -1;
	CGameData*	pGameData = (CGameData*)LcDev_GetUserData1(&m_pDevSys);

	if(0==_stricmp("Set Field Camera", sCmd))
	{
		hr = m_pFld->Query("Set Camera", pData);

		m_pFld->Query("Get Camera", &m_pCam);

		return hr;
	}

	else if(0==_stricmp("Set HLSL", sCmd))
	{
		char*	sEft= (char*)pData;
		PDEV	m_pDev= (PDEV)LcDev_GetD3Device((ILcDev**)&m_pDevSys);
		void*	pEft= LcD3D_EffectFileBuild((PDEV)m_pDev, sEft);
	
		if(NULL == pEft)
			return -1;

		hr = m_pFld->Query("Set HLSL", pEft);

		return hr;
	}

	else if(0==_stricmp("Load Field", sCmd))
	{
		char*	sFile = (char*)pData;
		hr  = m_pFld->Query("File Load", sFile);

		return hr;
	}

	else if(0==_stricmp("Get Field", sCmd))
	{
		*((ILcxmFld**)pData) = m_pFld;

		return 0;
	}
	
	else if(0==_stricmp("Load Character", sCmd))
	{
		struct _T{	INT	_n1; INT _n2; char* mdl; char* tex;	char* shd;	};
		_T*	t = (_T*)pData;

		INT		M	= t->_n1;
		INT		S	= t->_n2;

		char*	sMdl = t->mdl;
		char*	sTex = t->tex;
		char*	sEft = t->shd;

		ILcLsc::LscH*	pOrg	= NULL;


		//1. Lcm을 Origin에서 찾는다.
		pOrg	= LcLua::LscMdl::m_pContAuto->FindOrg(sMdl);

		//2. Lcm이 없으면 생성하고 Origin에 등록을 한다.
		if(!pOrg)
		{
			if(FAILED(LcLua::LscMdl::m_pContAuto->AllocOrg(sMdl, sTex, sEft)))
			{
				MessageBox(GetActiveWindow(), "Model Create Error", "Err", MB_OK | MB_ICONERROR);
				return -1;
			}

			//3. Lcm을 다시 Origin에서 찾아서 인스턴스를 생성한다.
			pOrg	= LcLua::LscMdl::m_pContAuto->FindOrg(sMdl);
		}

		ILcmMdl*	pMdl= NULL;
		PDEV		m_pDev= (PDEV)LcDev_GetD3Device((ILcDev**)&m_pDevSys);
		INT			hr	= LcMdl_Create("Acm", &pMdl, pOrg->pIns, NULL, (PDEV)m_pDev);

		if(FAILED(hr))
			return -1;
	
		pGameData->MateModelSetup(M, S, pMdl);
		pGameData->MateFieldPosition(M, S, m_pFld);

		return 0;
	}


	else if(0==_stricmp("Mate Activation", sCmd))
	{
		struct _T{	INT	_n1; INT _n2; INT _v;	};
		_T*	t = (_T*)pData;

		INT		M	= t->_n1;
		INT		S	= t->_n2;
		INT		A	= t->_v;

		hr = pGameData->MateSetActivation(M, S, A);
		return hr;
	}

	else if(0==_stricmp("Set Rotation", sCmd))
	{
		struct _T{	INT	_n1; INT _n2; VEC4* pV;	};
		_T*	t = (_T*)pData;

		INT		M	= t->_n1;
		INT		S	= t->_n2;
		VEC4*	pV	= t->pV;

		hr = pGameData->MateSetRotation(M, S, pV);
		return hr;
	}

	else if(0==_stricmp("Set Position", sCmd))
	{
		struct _T{	INT	_n1; INT _n2; VEC3* pV;	};
		_T*	t = (_T*)pData;

		INT		M	= t->_n1;
		INT		S	= t->_n2;
		VEC3*	pV	= t->pV;

		hr = pGameData->MateSetPosition(M, S, pV);
		return hr;
	}


	else if(0==_stricmp("Get Main Character", sCmd))
	{
		ILcmMdl*	pMdl = (ILcmMdl*)pGameData->GetMateModel(0, 0);
		*((ILcmMdl**)pData) = pMdl;

		return 0;
	}


	return -1;
}




ILcxmFld* CGameSgmnt::GetOutdoor()
{
	return m_pFld;
}
