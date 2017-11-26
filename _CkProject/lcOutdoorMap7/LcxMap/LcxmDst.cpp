// Implementation of the CLcxmDst class.
//
////////////////////////////////////////////////////////////////////////////////

#include <algorithm>

#include <windows.h>
#include <d3d9.h>
#include <d3dX9.h>

#include <Lc/LcType.h>
#include <Lc/LcUtil.h>
#include <Lc/LcUtilDx.h>
#include <Lc/LcEuclid.h>
#include <Lc/LcMath.h>

#include <Lc/ILcxCam.h>
#include <Lc/ILcmMdl.h>
#include <Lc/ILcmMdlX.h>

#include <Lc/ILcxTbl.h>

#include "ILcxmFld.h"
#include "ILcxmFldX.h"
#include "LcxmBlc.h"
#include "LcxmDst.h"


CLcxmDst::CLcxmDst()
{
	m_pDev		= NULL;

	m_ppLcl 	= NULL;
	m_pLcT		= NULL;
	
	m_pPkLc 	= NULL;				// Pick Local
	m_bClck 	= FALSE;
	m_pPkObj	= NULL;				// Picking Object

	m_vcPck		= D3DXVECTOR3(0,0,0);


	m_fCull		= 6000;

	m_bCtrlCull	= TRUE;
	m_bCtrlPck	= FALSE;

	m_bCtrlHeight	= FALSE;
	m_bCtrlSpoid	= FALSE;
	m_dFillColor	= 0x0;

	m_bCtrlBrush	= FALSE;
	m_bCtrlModel	= FALSE;
	m_nCtrlModel	= -1;
	m_bCtrlTile		= FALSE;
	m_nCtrlTile		= -1;
	m_bCtrlBndBox	= FALSE;
	m_bCtrlLclLine	= FALSE;

	m_pMdB			= NULL;
	m_pCam			= NULL;

	m_pTbTex		= NULL;
	m_pTbMdl		= NULL;

	m_pEft			= NULL;
	m_pFVF			= NULL;

	m_fRangeMax		= 5.f;
}


CLcxmDst::~CLcxmDst()
{
	Destroy();
}



INT CLcxmDst::Query(char* sCmd, void* pData)
{
	if(0==_stricmp("Get Pick Object", sCmd))
	{
		Lcm::ILcmMdl* pObj = NULL;

		pObj= (Lcm::ILcmMdl*)this->GetPckObj();

		*((Lcm::ILcmMdl**)pData) = pObj;
		return 0;
	}

	else if(0==_stricmp("Get Pick Position",	sCmd))
	{
		// Picking position, Ray Begin Position, Ray Direction
		struct _T{	VEC3* P;	VEC3* R;	VEC3* D;	};
		_T* t = (_T*)pData;

		if(FAILED(PickPosition(NULL, NULL, t->P, t->R, t->D)))
			return -1;

		return 0;
	}

	else if(0==_stricmp("Set Camera", sCmd))
	{
		m_pCam	= (ILcxCam*)pData;
		return 0;
	}

	else if(0==_stricmp("Get Camera", sCmd))
	{
		*((ILcxCam**)pData) = m_pCam;

		return 0;
	}

	else if(0==_stricmp("Set Ray Direction", 	sCmd))
	{
		m_vcRayDir = *((D3DXVECTOR3*)pData);
		return 0;
	}

	else if(0==_stricmp("Set Camera Position", 	sCmd))
	{
		m_vcCamPos = *((D3DXVECTOR3*)pData);
		return 0;
	}

	else if(0==_stricmp("Erase Pick Object", 	sCmd))
	{
		this->EraseObj();
		return 0;
	}

	else if(0==_stricmp("Set Culling Enable", 	sCmd))
	{
		m_bCtrlCull	= *((BOOL*)pData);
		return 0;
	}

	else if(0==_stricmp("Set Culling Range", 	sCmd))
	{
		m_fCull	= *((FLOAT*)pData);
		return 0;
	}

	else if(0==_stricmp("Set Picking Enable", 	sCmd))
	{
		m_bCtrlPck = *((BOOL*)pData);
		return 0;
	}


	else if(0==_stricmp("Set Height Enable", 	sCmd))
	{
		m_bCtrlHeight = *((BOOL*)pData);
		return 0;
	}

	else if(0==_stricmp("Spoid Enable", 		sCmd))
	{
		m_bCtrlSpoid = *((BOOL*)pData);
		return 0;
	}

	else if(0==_stricmp("Brush Enable", 		sCmd))
	{
		m_bCtrlBrush = *((BOOL*)pData);
		return 0;
	}

	else if(0==_stricmp("Model Setup Enable", 	sCmd))
	{
		m_bCtrlModel = *((BOOL*)pData);
		return 0;
	}

	else if(0==_stricmp("Set Model Pointer", 	sCmd))
	{
		Lcm::ILcmMdl* pMdl = (Lcm::ILcmMdl*)pData;
		m_pMdB = pMdl;
		return 0;
	}

	else if(0==_stricmp("Set Model Index", 		sCmd))
	{
		m_nCtrlModel = *((INT*)pData);
		return 0;
	}

	else if(0==_stricmp("Tiling Enable", 		sCmd))
	{
		m_bCtrlTile = *((BOOL*)pData);
		return 0;
	}

	else if(0==_stricmp("Bound Box Enable", 	sCmd))
	{
		m_bCtrlBndBox = *((BOOL*)pData);
		return 0;
	}

	else if(0==_stricmp("Show Local Enable", 	sCmd))
	{
		m_bCtrlLclLine = *((BOOL*)pData);
		return 0;
	}

	else if(0==_stricmp("Set Fill Color", 		sCmd))
	{
		m_dFillColor = *((DWORD*)pData);
		return 0;
	}

	else if(0==_stricmp("Set Tile Index", 		sCmd))
	{
		m_nCtrlTile = *((INT*)pData);
		return 0;
	}

	else if(0==_stricmp("Set Field Info",	 	sCmd))
	{
		memcpy(&m_Hgt, pData, sizeof(Lcxmo::MpHgt));
		return 0;
	}

	else if(0==_stricmp("Clear Layer List", 	sCmd))
	{
		return 0;
	}

	else if(0==_stricmp("Set Layer List", 		sCmd))
	{
		struct _T	{	INT n;	char* s;	};
		_T*	t = (_T*)pData;

		return 0;
	}

	else if(0==_stricmp("Get Fill Color", 		sCmd))
	{
		*((DWORD*)pData) = m_dFillColor;
		return 0;
	}

	else if(0==_stricmp("Get Model Pointer", 	sCmd))
	{
		*((Lcm::ILcmMdl**)pData) = m_pMdB;
		return 0;
	}

	else if(0==_stricmp("File Load", 			sCmd))
	{
		char*	sFile = (char*)pData;
		INT hr = this->FileLoad(sFile);
		return hr;
	}

	else if(0==_stricmp("File Save", 			sCmd))
	{
		char*	sFile = (char*)pData;
		INT hr = this->FileSave(sFile);
		return hr;
	}

	else if(0==_stricmp("Set Info", 			sCmd))
	{
		memcpy(&m_Inf, pData, sizeof(Lcxmo::MpInf));
		return 0;
	}

	else if(0==_stricmp("Get Info", 			sCmd))
	{
		memcpy(pData, &m_Inf, sizeof(Lcxmo::MpInf));
		return 0;
	}

	else if(0==_stricmp("Set Environment", 		sCmd))
	{
		memcpy(&m_Env, pData, sizeof(Lcxmo::MpEnv));
		return 0;
	}

	else if(0==_stricmp("Get Environment", 		sCmd))
	{
		memcpy(pData, &m_Env, sizeof(Lcxmo::MpEnv));
		return 0;
	}
	

	else if(0==_stricmp("Create Field", 		sCmd))
	{
		struct _T{	Lcxmo::MpInf* I; Lcxmo::MpEnv* E; Lcxmo::lsMpTex* L;};
		_T* t = (_T*)pData;
		
		CreateField(t->I, t->E, t->L);
		return 0;
	}

	else if(0==_stricmp("Set Fog", 				sCmd))
	{
		memcpy(&m_Env.m_Fog, pData, sizeof(Lcxmo::MpFog));
		return 0;
	}

	else if(0==_stricmp("Get Fog", 				sCmd))
	{
		memcpy(pData, &m_Env.m_Fog, sizeof(Lcxmo::MpFog));
		return 0;
	}


	else if(0==_stricmp("Set Light", 			sCmd))
	{
		struct _T
		{
			INT		n;							// Number of Lighting
			void*	p;							// Lighting Data
		};
		
		_T*	t =(_T*)pData;
		m_Env.m_iLg = t->n;
		memcpy(m_Env.m_pLg, t->p, sizeof(Lcxmo::MpLgt) * t->n);
		return 0;
	}

	else if(0==_stricmp("Set Material", 		sCmd))
	{
		memcpy( &m_Env.m_Mtl, pData, sizeof(Lcxmo::MpMtl) );
		return 0;
	}

	else if(0==_stricmp("Set Ambient Light", 	sCmd))
	{
		DWORD dAmLgt =*((DWORD*)pData);
		m_Env.m_dAb	= dAmLgt;
		return 0;
	}

	else if(0==_stricmp("Get Field Name", 		sCmd))
	{
		strcpy((char*)pData, m_Inf.sN);
		return 0;
	}

	else if(0==_stricmp("Set HLSL", 		sCmd))
	{
		m_pEft = (LPD3DXEFFECT)pData;
		return 0;
	}

	else if(0==_stricmp("Set View Range",		sCmd))
	{
		m_fRangeMax =*((FLOAT*)pData);
		return 0;
	}

	else if(0==_stricmp("Get View Range",		sCmd))
	{
		*((FLOAT*)pData) = m_fRangeMax;
		return 0;
	}

	return -1;
}


INT CLcxmDst::Create(void* p1, void* p2, void* p3, void* p4)
{
	INT	hr	= 0;

	m_pDev			= (PDEV)p1;
	m_pTbTex		= (ILcxTbl*)p2;
	m_pTbMdl		= (ILcxTbl*)p3;
	char*	sFile	= (char*)p4;


	DWORD				dFVF = Lcxmo::VtxSpl::FVF;
	D3DVERTEXELEMENT9	pDcl[MAX_FVF_DECL_SIZE]={0};

	D3DXDeclaratorFromFVF(dFVF, pDcl);

	if(FAILED(m_pDev->CreateVertexDeclaration( pDcl, &m_pFVF)))
		return -1;

	if(sFile)
		hr = FileLoad(sFile);

	return hr;
}


void CLcxmDst::Destroy()
{
	SAFE_DELETE(	m_pLcT		);

	if(m_ppLcl)
	{
		for(int z=0; z<m_Inf.iNfZ; ++z)
			SAFE_DELETE_ARRAY(	m_ppLcl[z]	);

		SAFE_DELETE_ARRAY(	m_ppLcl );
	}

	SAFE_RELEASE(	m_pFVF	);
}


INT CLcxmDst::FrameMove()
{
	INT		z, x;
	VEC3	vcEye;

	m_vObj2D.clear();
	m_vObj3D.clear();

	vcEye = *m_pCam->GetEye();
	
	if(m_ppLcl)
	{
		// 1. 카메라의 위치에 따른 렌더링 블록을 선택, 렌더링 결정	
		for(z=0; z<m_Inf.iNfZ; ++z)
		{
			for(x=0; x<m_Inf.iNfX; ++x)
			{
				INT	iNx	= m_ppLcl[z][x].GetTileN();			// Number of tile for Width
				INT	iWx	= m_ppLcl[z][x].GetTileW();			// Width of tile for x;

				FLOAT fBlockWidth = iNx * iWx;
				FLOAT fRangeView;							// View Range
				
				fRangeView	= m_fRangeMax* fBlockWidth;
				fRangeView	*= fRangeView;

				VEC3	vcCenter = m_ppLcl[z][x].m_vcP + VEC3(1,0,1) * fBlockWidth/2.F;
				VEC3	vcT = vcCenter- vcEye;
				FLOAT fLenSq = D3DXVec3LengthSq(&vcT);

				if(fLenSq>= fRangeView)
					m_ppLcl[z][x].m_bRend = FALSE;

				else
					m_ppLcl[z][x].m_bRend = TRUE;
			}
		}

		// 2. Field와 Local의 Picking처리
		PickField();
		PickLocal();

		
		// 3. Data 갱신
		for(z=0; z<m_Inf.iNfZ; ++z)
		{
			for(x=0; x<m_Inf.iNfX; ++x)
			{
				if(FALSE == m_ppLcl[z][x].m_bRend)
					continue;

				m_ppLcl[z][x].FrameMove();
			}
		}
	}
	
	SAFE_FRMOV( m_pLcT	);

	SAFE_UPDATE(this);

	m_bCtrlPck = FALSE;

	return 0;
}


INT CLcxmDst::Update()
{
	INT i;
	INT iSize;

	m_vObj.clear();

	iSize = m_vObj2D.size();

	for(i=0; i<iSize; ++i)
	{
		Lcm::ILcmMdl*	pObj = m_vObj2D[i];

		if(this->m_bCtrlCull)
		{
			Lcm::TbndAB*		pBnd = NULL;
			ILcxCam*	pCam = NULL;
			
			pObj->Query("Get Camera", &pCam);
			pObj->Query("get bound", &pBnd);
			

			VEC3	vcEye = *pCam->GetEye();
			VEC3	vcBnd = *pBnd->GetCenter();
			VEC3	vcT = vcBnd - vcEye;
			FLOAT	fL = D3DXVec3Length(&vcT);

			FLOAT	fR = pBnd->GetRadius();

			if(fL>(m_fCull+fR))
				continue;

			const DPLN* pFrsm  = pCam->GetFrustumPlanes();

			INT j =0;
			for(j=0; j<6; ++j)
			{
				fL = pFrsm[j].a * vcBnd.x + pFrsm[j].b * vcBnd.y + pFrsm[j].c * vcBnd.z + pFrsm[j].d - fR;

				if(fL>0)
					break;
			}

			if(j<6)
				continue;
		}

		m_vObj.push_back(pObj);
	}


	iSize = m_vObj3D.size();

	for(i=0; i<iSize; ++i)
	{
		Lcm::ILcmMdl*	pObj = m_vObj3D[i];

		if(this->m_bCtrlCull)
		{
			Lcm::TbndAB*		pBnd = NULL;
			ILcxCam*	pCam = NULL;
			
			pObj->Query("Get Camera", &pCam);
			pObj->Query("get bound", &pBnd);

			
			VEC3	vcEye = *pCam->GetEye();
			VEC3	vcBnd = *pBnd->GetCenter();
			VEC3	vcT = vcBnd - vcEye;

			FLOAT	fL = D3DXVec3Length(&vcT);
			FLOAT	fR = pBnd->GetRadius();

			if(fL>(m_fCull+fR))
				continue;

			const DPLN* pFrsm = pCam->GetFrustumPlanes();

			INT j=0;

			for(j=0; j<6; ++j)
			{
				fL = pFrsm[j].a * vcBnd.x + pFrsm[j].b * vcBnd.y + pFrsm[j].c * vcBnd.z + pFrsm[j].d - fR;

				if(fL>0)
					break;
			}

			if(j<6)
				continue;
		}

		m_vObj.push_back(pObj);
	}


	std::sort(m_vObj.begin(), m_vObj.end(), TsrtGpf<Lcm::ILcmMdl* >());


	// Picking Object
	PickObj();

	return 0;
}



void CLcxmDst::AddObj2D(Lcm::ILcmMdl* pMdB)
{
	m_vObj2D.push_back(pMdB);
}


void CLcxmDst::AddObj3D(Lcm::ILcmMdl* pMdB)
{
	m_vObj3D.push_back(pMdB);
}






INT CLcxmDst::PickField()
{
	if(!m_ppLcl)
		return -1;

	if(!m_bCtrlPck)
		return -1;

	// Picking Local Area
	m_pPkLc = NULL;
	m_vPkLc.clear();

	if(FAILED(PickPosition(&m_pPkLc, &m_vPkLc, &m_vcPck, &m_vcCamPos, &m_vcRayDir)))
		return -1;

	m_bClck = TRUE;
	
	return 0;
}


INT CLcxmDst::PickPosition(	CLcxmBlc**			pBlock
						,	Lcxmo::lsTlPkT*	pPickInf	// Picking Information
						,	VEC3*				pvcOut		// Picking position
						,	const VEC3*			pvcRayPos	// Ray Begin Position
						,	const VEC3*			pvcRayDir	// Ray Direction
							)
{
	INT		z;
	INT		x;
	INT		i;

	D3DXVECTOR3		vcPick	;
	Lcxmo::lsTlPkT	vPkLc	;			// Picking Triangle from Local

	for(z=0; z<m_Inf.iNfZ; ++z)
	{
		for(x=0; x<m_Inf.iNfX; ++x)
		{
			CLcxmBlc* pLcl = &m_ppLcl[z][x];

			for(i=0; i<pLcl->m_iNi; ++i)
			{
				WORD	a, b, c;

				a = pLcl->m_pFce[i].a;
				b = pLcl->m_pFce[i].b;
				c = pLcl->m_pFce[i].c;

				VEC3	vcPk;
				VEC3	p0 = pLcl->m_pVtx[a].p;
				VEC3	p1 = pLcl->m_pVtx[b].p;
				VEC3	p2 = pLcl->m_pVtx[c].p;
				FLOAT	fD;

				if(SUCCEEDED(LcMath_LineToTriangle(&vcPk, &fD, pvcRayPos, pvcRayDir, &p0, &p1, &p2)))
					vPkLc.push_back(Lcxmo::MpPckT(vcPk, p0, p1, p2, fD));
			}
		}
	}

	if(vPkLc.empty())
		return -1;

	
	std::sort(vPkLc.begin(), vPkLc.end(), TsrtL<Lcxmo::MpPckT >());
	vcPick = vPkLc[0].vcPk;
	
	x = INT(vcPick.x/m_Inf.iExt);
	z = INT(vcPick.z/m_Inf.iExt);

	if(pBlock)
		*pBlock = &m_ppLcl[z][x];

	if(pPickInf)
		pPickInf->push_back( vPkLc[0] );

	if(pvcOut)
		*pvcOut = vcPick;

	return 0;
}


INT CLcxmDst::PickLocal()
{
	if(!m_pPkLc)
		return -1;

	if(m_bClck && m_bCtrlHeight)
	{
		SetupHeight();
	}

	if(m_bClck && m_bCtrlSpoid)
	{
		SetupSpoid();
	}

	if(m_bClck && m_bCtrlBrush)
	{
		SetupBrush();
	}

	if(m_bClck && m_bCtrlModel && m_pMdB)
	{
		SetupModel();
	}

	if(m_bClck && m_bCtrlTile)
	{
		SetupTile();
	}

	return 0;
}


void CLcxmDst::SetupHeight()
{
	INT x, z, k;

	INT 	iFlat= m_Hgt.iFlat;
	INT 	iRng= m_Hgt.iRng;
	INT 	bAc = m_Hgt.bAc;
	FLOAT	fH	= m_Hgt.fH;
	FLOAT	fDH = m_Hgt.fDH;

	std::vector<Lcxmo::MpVec3T >	vVtxT;

	for(z=0; z<m_Inf.iNfZ; ++z)
	{
		for(x=0; x<m_Inf.iNfX; ++x)
		{
			for(k=0; k<m_ppLcl[z][x].m_iNv; ++k)
			{
				VEC3	pVc = m_vcPck - m_ppLcl[z][x].m_pVtx[k].p;
				INT 	iRT = INT(sqrtf(pVc.x * pVc.x + pVc.z * pVc.z)/m_ppLcl[z][x].m_fWx);

				if( iRT<iRng)
				{
					vVtxT.push_back(Lcxmo::MpVec3T(z, x, k, m_ppLcl[z][x].m_pVtx[k].p));
				}
			}

		}
	}

	int iSize = vVtxT.size();

	for(k=0; k<iSize; ++k)
	{
		FLOAT	xSq;
		VEC3	pVc = m_vcPck - vVtxT[k].p;

		xSq = pVc.x * pVc.x + pVc.z * pVc.z;

		xSq /=(m_pPkLc->m_fWx * m_pPkLc->m_fWx * iRng*iRng*0.25);
		fH = fDH * expf( -xSq);

		INT Z = vVtxT[k].iZ;
		INT X = vVtxT[k].iX;
		INT K = vVtxT[k].iK;

		m_ppLcl[Z][X].m_pVtx[K].p.y += fH;

		if(bAc)
		{
			int idx = Z * m_ppLcl[Z][X].m_iNx + X;
			m_ppLcl[Z][X].SetDiffuse(K);
		}
	}


	for(z=0; z<m_Inf.iNfZ; ++z)
	{
		for(x=0; x<m_Inf.iNfX; ++x)
		{
			INT Z = vVtxT[k].iZ;
			INT X = vVtxT[k].iX;
			INT K = vVtxT[k].iK;

			m_ppLcl[z][x].SetNormal();

			if(m_ppLcl[z][x].m_pMdb2D)
			{
				CLcxmBlc*	pLcl = NULL;
				Lcm::ILcmPack*	pPack = m_ppLcl[z][x].m_pMdb2D;

				if(SUCCEEDED(pPack->Query("local", &pLcl)))
				{
					INT		i;
					INT		iSize;
					
					iSize = pPack->Size();
					
					for(i=0; i<iSize; ++i)
					{
						Lcm::ILcmMdl*	pMdB = pPack->GetObj(i);
						VEC3	vcPos;
						
						pMdB->GetPosition((FLOAT*)&vcPos);
						FLOAT	fY   = pLcl->GetHeight(vcPos);
						
						vcPos.y = fY;
						pMdB->SetPosition((FLOAT*)&vcPos);
					}

				}
			}

			if(m_ppLcl[z][x].m_pMdb3D)
			{
				CLcxmBlc*	pLcl = NULL;
				Lcm::ILcmPack*	pPack = m_ppLcl[z][x].m_pMdb3D;

				if(SUCCEEDED(pPack->Query("local", &pLcl)))
				{
					INT		i;
					INT		iSize;
					
					iSize = pPack->Size();
					
					for(i=0; i<iSize; ++i)
					{
						Lcm::ILcmMdl* pMdB = pPack->GetObj(i);
						VEC3	 vcPos;
						
						pMdB->GetPosition((FLOAT*)&vcPos);
						FLOAT	 fY   = pLcl->GetHeight(vcPos);
						
						vcPos.y = fY;
						pMdB->SetPosition((FLOAT*)&vcPos);
					}

				}
			}
		}
	}
}



void CLcxmDst::SetupSpoid()
{
	INT 	nIdxZ = INT((-m_pPkLc->m_vcP.z + m_vcPck.z+0.5F*m_pPkLc->m_fWx)/ m_pPkLc->m_fWx);
	INT 	nIdxX = INT((-m_pPkLc->m_vcP.x + m_vcPck.x+0.5F*m_pPkLc->m_fWx)/ m_pPkLc->m_fWx);

	m_dFillColor  = m_pPkLc->m_pVtx[nIdxZ * m_pPkLc->m_iNx + nIdxX].d;
}



void CLcxmDst::SetupBrush()
{
	// 피킹 포지션: m_vcPck
	INT		z1=0;
	INT		x1=0;
	INT		z2=0;
	INT		x2=0;

	INT 	iRng= m_Hgt.iRng;
	DWORD	dC	= m_dFillColor;

	INT x, z, k;

	std::vector<Lcxmo::MpVec3T >	vVtxT;

	for(z=0; z<m_Inf.iNfZ; ++z)
	{
		for(x=0; x<m_Inf.iNfX; ++x)
		{
			for(k=0; k<m_ppLcl[z][x].m_iNv; ++k)
			{
				VEC3	pVc = m_vcPck - m_ppLcl[z][x].m_pVtx[k].p;
				INT 	iRT = 100 * INT(sqrtf(pVc.x * pVc.x + pVc.z * pVc.z)/m_ppLcl[z][x].m_fWx);

				INT		E	= iRT- 100 * iRng;

				if( E<0)
				{
					vVtxT.push_back(Lcxmo::MpVec3T(z, x, k, m_ppLcl[z][x].m_pVtx[k].p, E));
				}
			}

		}
	}

	int iSize = vVtxT.size();

	for(k=0; k<iSize; ++k)
	{
		INT Z = vVtxT[k].iZ;
		INT X = vVtxT[k].iX;
		INT K = vVtxT[k].iK;
		INT E = vVtxT[k].iE;

		if( E<-100)
		{
			D3DXCOLOR Dnew = dC;
			Dnew	*=1.f;
			Dnew.a = 1.f;
			m_ppLcl[Z][X].m_pVtx[K].d = Dnew;
		}
		else
		{
			FLOAT	fW = -float(E)/100.f;

			fW	= exp( - fW * fW * 1.f);
			D3DXCOLOR Dnew = dC;
			D3DXCOLOR Dold = m_ppLcl[Z][X].m_pVtx[K].d;
			Dnew	*= fW;
			Dold	*= (1.f - fW);
			Dnew	+= Dold;
			Dnew.a = 1.f;
			m_ppLcl[Z][X].m_pVtx[K].d = Dnew;
		}
		
	}


}





void CLcxmDst::SetupModel()
{
	m_bClck = FALSE;

	if(m_bCtrlPck)
	{
		WORD	nIds[4]={0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
		WORD	nI1 = 0xFFFF;
		WORD	nI2 = 0xFFFF;
		WORD	nI3 = 0xFFFF;

		INT 	nLst	= m_nCtrlModel;
		VEC3	pos		= m_vcPck;

		CLcxmDst*	pField	= this;
		CLcxmBlc*	pLocal	= m_pPkLc;
		Lcm::ILcmPack* pPack= NULL;

		
		m_pMdB->Query("get ids", nIds);
		nI1 = nIds[0];
		nI2 = nIds[1];
		nI3 = nIds[2];


		if(0 == nI1)															// 풀
		{
			pPack	= m_pPkLc->m_pMdb2D;

			if(pPack)
				pPack->AddObj(m_pMdB, pos, m_pCam, pField, pLocal, pPack);

			if(pPack)
			{
				CLcxmBlc*		pLcl = NULL;

				if(SUCCEEDED(pPack->Query("local", &pLcl)))
				{
					INT		i;
					INT		iSize;
					
					iSize = pPack->Size();
					
					for(i=0; i<iSize; ++i)
					{
						Lcm::ILcmMdl*	m_pMdB = pPack->GetObj(i);
						VEC3	vcPos;
						m_pMdB->GetPosition((FLOAT*)&vcPos);

						FLOAT	fY   = pLcl->GetHeight(vcPos);

						vcPos.y = fY;
						m_pMdB->SetPosition((FLOAT*)&vcPos);
					}

				}
			}
		}

		else if(1 == nI1)														// 3D Tree
		{
			pPack	= m_pPkLc->m_pMdb3D;

			if(pPack)
				pPack->AddObj(m_pMdB, pos, m_pCam, pField, pLocal, pPack);

			if(pPack)
			{
				CLcxmBlc*		pLcl = NULL;

				if(SUCCEEDED(pPack->Query("local", &pLcl)))
				{
					INT		i;
					INT		iSize;
					
					iSize = pPack->Size();
					
					for(i=0; i<iSize; ++i)
					{
						Lcm::ILcmMdl* m_pMdB = pPack->GetObj(i);
						VEC3	 vcPos;
						
						m_pMdB->GetPosition((FLOAT*)&vcPos);
						FLOAT	 fY   = pLcl->GetHeight(vcPos);
						
						vcPos.y = fY;
						m_pMdB->SetPosition((FLOAT*)&vcPos);
					}

				}
			}

		}
	}// if(GINPUT->ButtonDown(0))


	m_pMdB = NULL;
}



void CLcxmDst::SetupTile()
{
	// 피킹 포지션: m_vcPck
	INT		z1=0;
	INT		x1=0;
	INT		z2=0;
	INT		x2=0;

	INT 	nTile = m_nCtrlTile;
	INT 	iRng  = m_Hgt.iRng;

	FLOAT	fWidth = iRng * m_pPkLc->m_fWx;

	VEC3	vcLclMin(m_vcPck.x - fWidth, 0, m_vcPck.z - fWidth);
	VEC3	vcLclMax(m_vcPck.x + fWidth, 0, m_vcPck.z + fWidth);

	INT		nLclBgnX = INT(vcLclMin.x/m_Inf.iExt);
	INT		nLclBgnZ = INT(vcLclMin.z/m_Inf.iExt);

	INT		nLclEndX = INT(vcLclMax.x/m_Inf.iExt);
	INT		nLclEndZ = INT(vcLclMax.z/m_Inf.iExt);


	if(nLclBgnZ<0)
		nLclBgnZ = 0;

	if(nLclBgnX<0)
		nLclBgnX = 0;

	if(nLclBgnZ>=m_Inf.iNfZ)
		nLclBgnZ = m_Inf.iNfZ-1;

	if(nLclBgnX>=m_Inf.iNfX)
		nLclBgnX = m_Inf.iNfX-1;


	if(nLclEndZ<0)
		nLclEndZ = 0;

	if(nLclEndX<0)
		nLclEndX = 0;

	if(nLclEndZ>=m_Inf.iNfZ)
		nLclEndZ = m_Inf.iNfZ-1;

	if(nLclEndX>=m_Inf.iNfX)
		nLclEndX = m_Inf.iNfX-1;


	for(z1=nLclBgnZ; z1<=nLclEndZ; ++z1)
	{
		for(x1=nLclBgnX; x1<=nLclEndX; ++x1)
		{
			CLcxmBlc*		pLcl = &m_ppLcl[z1][x1];
			Lcxmo::VtxSpl*	pVtx = pLcl->m_pVtx;
			INT*			pVtt = pLcl->m_pVtt;

			if(NULL == pVtx)
				continue;

			INT 	nIdxZ = INT((-pLcl->m_vcP.z + m_vcPck.z+0.5F*pLcl->m_fWx)/ pLcl->m_fWx);
			INT 	nIdxX = INT((-pLcl->m_vcP.x + m_vcPck.x+0.5F*pLcl->m_fWx)/ pLcl->m_fWx);

			int 	nIdxBgnZ = nIdxZ-iRng;
			int 	nIdxBgnX = nIdxX-iRng;

			int 	nIdxEndZ = nIdxZ+iRng;
			int 	nIdxEndX = nIdxX+iRng;

			if(nIdxBgnZ<0)
				nIdxBgnZ = 0;

			if(nIdxBgnX<0)
				nIdxBgnX = 0;

			if(nIdxEndZ>=pLcl->m_iNx)
				nIdxEndZ = pLcl->m_iNx;

			if(nIdxEndX>=pLcl->m_iNx)
				nIdxEndX = pLcl->m_iNx;


			for(z2= nIdxBgnZ; z2<nIdxEndZ; ++z2)
			{
				for(x2= nIdxBgnX; x2<nIdxEndX; ++x2)
				{
					int idx = z2 * pLcl->m_iNx + x2;

					int r = (x2-nIdxX) * (x2-nIdxX) + (z2-nIdxZ) * (z2-nIdxZ) - (iRng * iRng);

					if(r>0)
						continue;

					pVtt[idx] = nTile;

//					pVtx[idx].ts[nTile] = 1.f;
				}// for(x2= nIdxBgnX; x2<nIdxEndX; ++x2)
			}// for(z2= nIdxBgnZ; z2<nIdxEndZ; ++z2)


			pLcl->CalculateMap();

		}// for(x1=nLclBgnX; x1<=nLclEndX; ++x1)
	}// for(z1=nLclBgnZ; z1<=nLclEndZ; ++z1)
}


INT CLcxmDst::PickObj()
{
	if(m_bCtrlModel)
		return -1;

	if(!m_bCtrlBndBox)
		return -1;

	int iObjSize = m_vObj.size();

	if(iObjSize<1)
		return -1;


	INT	i;


	if(m_bCtrlPck)
	{
		VEC3	vcPk;
		FLOAT	fDist;
		Lcm::TbndAB*	pBndBox=NULL;


		if(m_pPkObj)
		{
			Lcm::TbndAB*	pBnd = NULL;
			m_pPkObj->Query("get bound", &pBnd);

			pBnd->SetColor(0xFF666666);
		}

		// Picking Object
		m_pPkObj= NULL;
		m_vPkObj.clear();

		int iObjSize = m_vObj.size();

		for(i=0; i<iObjSize; ++i)
		{
			Lcm::ILcmMdl*	pMdl = m_vObj[i];
			Lcm::TbndAB* 	pBnd = NULL;
			
			if(SUCCEEDED(pMdl->Query("get bound", &pBnd)))
				pBndBox = pBnd;

			if(SUCCEEDED(Lcm::LcxUtil_PickAABB(&fDist, &vcPk, &m_vcCamPos, &m_vcRayDir, pBndBox)))
			{
				Lcm::TbndAABB Box(vcPk, pBndBox, fDist);
				m_vPkObj.push_back(Box);
			}
		}// for Object


		if(!m_vPkObj.empty())
		{
			m_bClck = TRUE;
			
			std::sort(m_vPkObj.begin(), m_vPkObj.end(), TsrtL<Lcm::TbndAABB >());
			m_vcPkObj = m_vPkObj[0].vPck;

			m_pPkObj = (Lcm::ILcmMdl*)m_vPkObj[0].pBnd->GetOwner();					// Picking Object
		}

		if(m_pPkObj)
		{
			Lcm::ILcmMdl*	pMdl = m_pPkObj;
			Lcm::TbndAB*	pBnd = NULL;
			
			pMdl->Query("get bound", &pBnd);
			pBnd->SetColor(0xFFFF0000);

			return 0;
		}
	}

	return -1;
}


void CLcxmDst::Render()
{
	INT iSize;
	INT i;
	INT x, z;

	if(!m_ppLcl && !m_pLcT)
		return;

	LcD3D_SetWorldIdentity(m_pDev);

	m_pDev->SetRenderState( D3DRS_CULLMODE,	D3DCULL_NONE);


	for(i=0; i<7; ++i)
		m_pDev->LightEnable( i, FALSE);

	for(i=0; i<m_Env.m_iLg; ++i)
	{
		const D3DLIGHT9*	pLight = m_Env.m_pLg[i];
		m_pDev->SetLight( i, pLight);
		m_pDev->LightEnable( i, TRUE );
	}

	m_pDev->SetRenderState( D3DRS_AMBIENT, m_Env.m_dAb);					// Set Ambient
	m_pDev->SetMaterial( &m_Env.m_Mtl.m );									// Set Material

	

	m_Env.m_Fog.SetFog(m_pDev);

	
	if(m_bCtrlLclLine && m_ppLcl)
	{
		for(z=0; z<m_Inf.iNfZ; ++z)
		{
			for(x=0; x<m_Inf.iNfX; ++x)
			{
				m_ppLcl[z][x].RenderTline();								// Render Block Line
			}
		}
	}




	if(m_ppLcl)
	{
		// 3. Data 갱신
		for(int z=0; z<m_Inf.iNfZ; ++z)
		{
			for(int x=0; x<m_Inf.iNfX; ++x)
			{
				m_ppLcl[z][x].RenderBlc();
			}
		}
	}


	if(m_pLcT)
		m_pLcT->RenderBlc();




	iSize = m_vObj.size();

	for(i=0; i<iSize; ++i)
	{
		Lcm::ILcmMdl*	pMdl = m_vObj[i];
		
		if(m_bCtrlBndBox)
			pMdl->Query("bound box render", NULL);
		
		pMdl->Render();
	}
}



void CLcxmDst::RenderPck()
{
	DWORD dMnLgt;
	m_pDev->GetRenderState( D3DRS_LIGHTING, &dMnLgt);

	m_pDev->SetRenderState( D3DRS_LIGHTING,	FALSE);
	m_pDev->SetRenderState(D3DRS_FOGENABLE,	FALSE);
	m_pDev->SetRenderState( D3DRS_FILLMODE,	D3DFILL_SOLID);
	m_pDev->SetRenderState( D3DRS_CULLMODE,	D3DCULL_NONE);

	if(!m_vPkLc.empty())
	{
		Lcxmo::VtxD	pVtx[3];

		pVtx[0].p = m_vPkLc[0].p0;
		pVtx[1].p = m_vPkLc[0].p1;
		pVtx[2].p = m_vPkLc[0].p2;

		pVtx[0].d = 0x9988AAFF;
		pVtx[1].d = 0x9988AAFF;
		pVtx[2].d = 0x9988AAFF;

		m_pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
		m_pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTCOLOR);

		m_pDev->SetRenderState(D3DRS_FOGENABLE,	FALSE);
		m_pDev->SetRenderState( D3DRS_FILLMODE,	D3DFILL_SOLID);
		m_pDev->SetRenderState( D3DRS_CULLMODE,	D3DCULL_NONE);

		m_pDev->SetTexture(0, 0);
		m_pDev->SetFVF(Lcxmo::VtxD::FVF);
		m_pDev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, pVtx, sizeof(Lcxmo::VtxD));

		m_pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	}

	m_pDev->SetRenderState( D3DRS_LIGHTING, dMnLgt);
}



void CLcxmDst::CreateField(Lcxmo::MpInf* Inf, Lcxmo::MpEnv* Env, Lcxmo::lsMpTex* pTxLst)
{
	if(m_ppLcl)
	{
		for(int z=0; z<m_Inf.iNfZ; ++z)
			SAFE_DELETE_ARRAY(	m_ppLcl[z]	);

		SAFE_DELETE_ARRAY(	m_ppLcl );
	}

	memset(&m_Inf, 0, sizeof(Lcxmo::MpInf));
	memset(&m_Env, 0, sizeof(Lcxmo::MpEnv));
		
	memcpy(&m_Inf, Inf, sizeof(Lcxmo::MpInf));
	memcpy(&m_Env, Env, sizeof(Lcxmo::MpEnv));


	m_ppLcl 	= new CLcxmBlc*[m_Inf.iNfZ];

	INT	iWx = m_Inf.iExt/m_Inf.iNx;

	for(int z=0; z<m_Inf.iNfZ; ++z)
	{
		m_ppLcl[z] = new CLcxmBlc[m_Inf.iNfX];

		for(int x=0; x<m_Inf.iNfX; ++x)
		{
			VEC3	pos = VEC3( x * m_Inf.iExt, 0, z * m_Inf.iExt);

			m_ppLcl[z][x].Create(m_pDev, this, m_pFVF, m_pEft);
			m_ppLcl[z][x].SetPos(pos);
			m_ppLcl[z][x].CreateMesh(m_Inf.iNx+1, iWx, m_Inf.fUV);
			m_ppLcl[z][x].LayerSetup(pTxLst);
		}
	}

}


INT CLcxmDst::FileLoad(char* sFile)
{
	FILE*	fp;
	INT 	x, z, i=0, j=0;
	
	fp = fopen(sFile, "rb");

	if(!fp)
		return -1;


	if(m_ppLcl)
	{
		for(int z=0; z<m_Inf.iNfZ; ++z)
			SAFE_DELETE_ARRAY(	m_ppLcl[z]	);

		SAFE_DELETE_ARRAY(	m_ppLcl );
	}

	memset(&m_Inf, 0, sizeof(m_Inf));
	memset(&m_Env, 0, sizeof(m_Env));

	fread(&m_Inf, 1, sizeof(Lcxmo::MpInf), fp);
	fread(&m_Env, 1, sizeof(Lcxmo::MpEnv), fp);


	m_ppLcl 	= new CLcxmBlc*[m_Inf.iNfZ];

	for(z=0; z<m_Inf.iNfZ; ++z)
	{
		m_ppLcl[z] = new CLcxmBlc[m_Inf.iNfX];
		
		for(x=0; x<m_Inf.iNfX; ++x)
			m_ppLcl[z][x].Create(m_pDev, this, m_pFVF, m_pEft);
	}


	for(z=0; z<m_Inf.iNfZ; ++z)
	{
		for(x=0; x<m_Inf.iNfX; ++x)
		{
			CLcxmBlc* pLc = &m_ppLcl[z][x];

			fread(&pLc->m_sN,	1, sizeof(pLc->m_sN),	fp);					// Map Name
			fread(&pLc->m_mtW,	1, sizeof(pLc->m_mtW),	fp);					// World Matrix
			fread(&pLc->m_vcP,	1, sizeof(pLc->m_vcP),	fp);					// Position

			
			// Mesh
			fread(&pLc->m_iNx,	1, sizeof(pLc->m_iNx), fp); 					// Number of tile for Width
			fread(&pLc->m_fWx,	1, sizeof(pLc->m_fWx), fp); 					// Width of tile for x;
			fread(&pLc->m_iVs,	1, sizeof(pLc->m_iVs), fp); 					// Vertex Size
			fread(&pLc->m_FVF,	1, sizeof(pLc->m_FVF), fp);
			fread(&pLc->m_fUV,	1, sizeof(pLc->m_fUV), fp); 					// UV Width
			fread(&pLc->m_iNi,	1, sizeof(pLc->m_iNi), fp); 					// Index Number
			fread(&pLc->m_iNv,	1, sizeof(pLc->m_iNv), fp); 					// Vertex Number

			pLc->m_pFce	= new Lcxmo::VtxIdx[pLc->m_iNi];
			pLc->m_pVtx	= new Lcxmo::VtxSpl[pLc->m_iNv];
			pLc->m_pVtt = new Lcxmo::SplIdx[pLc->m_iNv];
			fread(pLc->m_pFce, pLc->m_iNi, sizeof(Lcxmo::VtxIdx), fp);
			fread(pLc->m_pVtx, pLc->m_iNv, sizeof(Lcxmo::VtxSpl), fp);
			fread(pLc->m_pVtt, pLc->m_iNv, sizeof(Lcxmo::SplIdx), fp);


			// Tiling
			INT iSize = 0;
			fread(&iSize, 1, sizeof(INT), fp);

			for(int n=0; n<iSize; ++n)
			{
				Lcxmo::MpTex	pLayer;
				fread(pLayer.TexB.sName, 1, 128, fp);
				pLc->m_vTex.push_back(pLayer);
			}


			WORD	nIds[4]={0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
			VEC3	vcR;
			VEC3	vcS;
			VEC3	vcT;

			char	sMdN[128];


			// Object List 2D
			fread(&iSize, 1, sizeof(INT), fp);

			for(i=0; i<iSize; ++i)
			{
				fread(&nIds[0],	 1, sizeof(WORD), fp);
				fread(&nIds[1],	 1, sizeof(WORD), fp);
				fread(&nIds[2],	 1, sizeof(WORD), fp);
				fread(&nIds[3],	 1, sizeof(WORD), fp);
				
				fread(&vcR,		 1, sizeof(VEC3), fp);
				fread(&vcS,		 1, sizeof(VEC3), fp);
				fread(&vcT,		 1, sizeof(VEC3), fp);

				fread(sMdN,		 1, sizeof(sMdN), fp);
				
				INT		nM=-1, nS=-1;
				void*	pSrc;

				if(SUCCEEDED(m_pTbMdl->Select(&pSrc, &nM, &nS, sMdN)))
				{
					Lcm::ILcmMdl*	pMdB = NULL;
					
					if(FAILED(Lcm::MdlX_Create("billboard", &pMdB, m_pDev, pSrc)))
						return -1;

					pMdB->SetRotation((FLOAT*)&vcR);
					pMdB->SetScaling(vcS);
					pMdB->SetPosition((FLOAT*)&vcT);

					pMdB->Query("Set Camera", &m_pCam);
					pMdB->Query("set ids", nIds);
					pMdB->Query("set field", this);
					pMdB->Query("set local", pLc);
					pMdB->Query("set package", pLc->m_pMdb2D);

					pLc->m_pMdb2D->Push_Back(pMdB);
				}
			}


			// Object List 3D
			fread(&iSize, 1, sizeof(INT), fp);

			for(i=0; i<iSize; ++i)
			{
				fread(&nIds[0],	 1, sizeof(WORD), fp);
				fread(&nIds[1],	 1, sizeof(WORD), fp);
				fread(&nIds[2],	 1, sizeof(WORD), fp);
				fread(&nIds[3],	 1, sizeof(WORD), fp);
				
				fread(&vcR,		 1, sizeof(VEC3), fp);
				fread(&vcS,		 1, sizeof(VEC3), fp);
				fread(&vcT,		 1, sizeof(VEC3), fp);

				fread(sMdN,		 1, sizeof(sMdN), fp);

				INT		nM=-1, nS=-1;
				void*	pSrc = NULL;

				if(SUCCEEDED(m_pTbMdl->Select(&pSrc, &nM, &nS, sMdN)))
				{					
					Lcm::ILcmMesh*	pMsh = (Lcm::ILcmMesh*)pSrc;
					Lcm::ILcmMdl*	pMdB = NULL;
					void*			pBnd = pMsh->GetBndInf();
					
					if(FAILED(Lcm::MdlX_Create("solid", &pMdB, m_pDev, pSrc)))
						return -1;

					pMdB->SetRotation((FLOAT*)&vcR);
					pMdB->SetScaling(vcS);
					pMdB->SetPosition((FLOAT*)&vcT);
					
					pMdB->Query("Set Camera", &m_pCam);
					pMdB->Query("set bound", pBnd);

					pMdB->Query("set ids", nIds);
					pMdB->Query("set field", this);
					pMdB->Query("set local", pLc);
					pMdB->Query("set package", pLc->m_pMdb3D);

					pLc->m_pMdb3D->Push_Back(pMdB);
				}
			}
		}
	}

	fclose(fp);


	for(z=0; z<m_Inf.iNfZ; ++z)
	{
		for(x=0; x<m_Inf.iNfX; ++x)
		{
			CLcxmBlc* pLc = &m_ppLcl[z][x];

			// Tiling
			INT iSize = pLc->m_vTex.size();

			for(int n=0; n<iSize; ++n)
			{
				Lcxmo::MpTex*	pLayer = &pLc->m_vTex[n];

				INT	nM = -1;
				INT nS = -1;
				char* sTexB = pLayer->TexB.sName;

				m_pTbTex->Select((void**)&pLayer->TexB.pTex, &nM, &nS, sTexB);
				m_pTbTex->SelectInfo((void**)&pLayer->TexB.Image, nM, nS);
			}

		}
	}


	return 0;
}


INT CLcxmDst::FileSave(char* sFile)
{
	FILE*	fp;
	INT 	x, z, i=0, j=0;

	fp = fopen(sFile, "wb");

	fwrite(&m_Inf, 1, sizeof(Lcxmo::MpInf), fp);
	fwrite(&m_Env, 1, sizeof(Lcxmo::MpEnv), fp);

	for(z=0; z<m_Inf.iNfZ; ++z)
	{
		for(x=0; x<m_Inf.iNfX; ++x)
		{
			CLcxmBlc* pLc = &m_ppLcl[z][x];

			fwrite(&pLc->m_sN,	1, sizeof(pLc->m_sN),	fp);					// Map Name
			fwrite(&pLc->m_mtW, 1, sizeof(pLc->m_mtW),	fp);					// World Matrix
			fwrite(&pLc->m_vcP, 1, sizeof(pLc->m_vcP),	fp);					// Position

			// Mesh
			fwrite(&pLc->m_iNx, 1, sizeof(pLc->m_iNx), fp); 					// Number of tile for Width
			fwrite(&pLc->m_fWx, 1, sizeof(pLc->m_fWx), fp); 					// Width of tile for x;
			fwrite(&pLc->m_iVs, 1, sizeof(pLc->m_iVs), fp); 					// Vertex Size
			fwrite(&pLc->m_FVF, 1, sizeof(pLc->m_FVF), fp);
			fwrite(&pLc->m_fUV, 1, sizeof(pLc->m_fUV), fp); 					// UV Width
			fwrite(&pLc->m_iNi, 1, sizeof(pLc->m_iNi), fp); 					// Index Number
			fwrite(&pLc->m_iNv, 1, sizeof(pLc->m_iNv), fp); 					// Vertex Number

			fwrite(pLc->m_pFce, pLc->m_iNi, sizeof(Lcxmo::VtxIdx), fp);
			fwrite(pLc->m_pVtx, pLc->m_iNv, sizeof(Lcxmo::VtxSpl), fp);
			fwrite(pLc->m_pVtt, pLc->m_iNv, sizeof(Lcxmo::SplIdx), fp);

			// Tiling
			INT iSize = pLc->m_vTex.size();
			fwrite(&iSize, 1, sizeof(INT), fp);

			for(i=0; i<iSize; ++i)
			{
				Lcxmo::MpTex*	pLayer = &pLc->m_vTex[i];
				fwrite(pLayer->TexB.sName, 1, 128, fp);
			}


			WORD	nIds[4]={0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
			VEC3	vcR;
			VEC3	vcS;
			VEC3	vcT;

			char	sMdN[128];

			
			// Object List 2D
			iSize = pLc->m_pMdb2D->Size();
			
			fwrite(&iSize, 1, sizeof(INT), fp);

			for(i=0; i<iSize; ++i)
			{
				Lcm::ILcmMdl*	pMdB = pLc->m_pMdb2D->GetObj(i);
				Lcm::ILcmMesh*	pSrc = NULL;
				
				pMdB->Query("Get Mesh Source", &pSrc);

				memset(sMdN, 0, sizeof(sMdN));

				Lcm::TmshInf*	pGeo = (Lcm::TmshInf*)pSrc->GetGeometry();

				sprintf(sMdN, "%s", pGeo->GetMdlName());

				pMdB->Query("get ids", nIds);

				pMdB->GetRotation((FLOAT*)&vcR);
				pMdB->GetScaling((FLOAT*)&vcS);
				pMdB->GetPosition((FLOAT*)&vcT);

				fwrite(&nIds[0], 1, sizeof(WORD), fp);
				fwrite(&nIds[1], 1, sizeof(WORD), fp);
				fwrite(&nIds[2], 1, sizeof(WORD), fp);
				fwrite(&nIds[3], 1, sizeof(WORD), fp);

				fwrite(&vcR,	 1, sizeof(VEC3), fp);
				fwrite(&vcS,	 1, sizeof(VEC3), fp);
				fwrite(&vcT,	 1, sizeof(VEC3), fp);

				fwrite(sMdN,	 1, sizeof(sMdN), fp);
			}


			// Object List 3D
			iSize = pLc->m_pMdb3D->Size();

			fwrite(&iSize, 1, sizeof(INT), fp);

			for(i=0; i<iSize; ++i)
			{
				Lcm::ILcmMdl*		pMdB = pLc->m_pMdb3D->GetObj(i);
				Lcm::ILcmMesh*		pSrc = NULL;
				
				pMdB->Query("Get Mesh Source", &pSrc);

				Lcm::TmshInf*	pGeo = (Lcm::TmshInf*)pSrc->GetGeometry();

				memset(sMdN, 0, sizeof(sMdN));
				sprintf(sMdN, "%s", pGeo->GetMdlName());

				pMdB->Query("get ids", nIds);

				pMdB->GetRotation((FLOAT*)&vcR);
				pMdB->GetScaling((FLOAT*)&vcS);
				pMdB->GetPosition((FLOAT*)&vcT);

				fwrite(&nIds[0], 1, sizeof(WORD), fp);
				fwrite(&nIds[1], 1, sizeof(WORD), fp);
				fwrite(&nIds[2], 1, sizeof(WORD), fp);
				fwrite(&nIds[3], 1, sizeof(WORD), fp);

				fwrite(&vcR,	 1, sizeof(VEC3), fp);
				fwrite(&vcS,	 1, sizeof(VEC3), fp);
				fwrite(&vcT,	 1, sizeof(VEC3), fp);

				fwrite(sMdN,	 1, sizeof(sMdN), fp);
			}
		}
	}


	fclose(fp);

	return 0;
}




Lcxmo::MpInf* CLcxmDst::GetFldInf()
{
	return &m_Inf;
}

char* CLcxmDst::GetFldName()
{
	return m_Inf.sN;
}


void CLcxmDst::EraseObj()
{
	if(!m_ppLcl || !m_pPkObj)
		return;


	CLcxmBlc*		pLcl	= NULL;
	Lcm::ILcmMdl*	pObj1	= m_pPkObj;
	Lcm::ILcmMdl*	pObj2	= NULL;
	BOOL			bFind	= FALSE;
	Lcm::ILcmPack*	pPack	= NULL;


	if(FAILED(pObj1->Query("get package", &pPack)))
		return;


	int iSize=0;

	iSize = pPack->Size();

	for(int k = 0; k<iSize; ++k)
	{
		pObj2 = pPack->GetObj(k);

		if(pObj2 == pObj1)
		{
			bFind = TRUE;

			pPack->Erase(k);
			m_pPkObj = NULL;
			
			break;
		}
	}


	if(TRUE == bFind)
		return;

	return;
	
//	for(int z=0; z<m_Inf.iNfZ; ++z)
//	{
//		for(int x=0; x<m_Inf.iNfX; ++x)
//		{
//			int k=0;
//			int iSize=0;
//
//			iSize = m_ppLcl[z][x].m_pMdb2D->Size();
//
//			for(k = 0; k<iSize; ++k)
//			{
//				pObj2 = m_ppLcl[z][x].m_pMdb2D->GetObj(k);
//
//				if(pObj2 == pObj1)
//				{
//					bFind = TRUE;
//
//					m_ppLcl[z][x].m_pMdb2D->Erase(k);
//					m_pPkObj = NULL;
//					
//					break;
//				}
//			}
//
//
//			if(TRUE == bFind)
//				break;
//
//
//			iSize = m_ppLcl[z][x].m_pMdb3D->Size();
//
//			for(k = 0; k<iSize; ++k)
//			{
//				pObj2 = m_ppLcl[z][x].m_pMdb3D->GetObj(k);
//
//				if(pObj2 == pObj1)
//				{
//					bFind = TRUE;
//
//					m_ppLcl[z][x].m_pMdb3D->Erase(k);
//					m_pPkObj = NULL;
//
//					break;
//				}
//			}
//		}
//
//		if(TRUE == bFind)
//			break;
//	}
}



void* CLcxmDst::GetPckObj()
{
	return m_pPkObj;
}


PDVD CLcxmDst::GetVertexDeclarator()
{
	return m_pFVF;
}


LPD3DXEFFECT CLcxmDst::GetD3DXEffect()
{
	return m_pEft;
}


ILcxCam* CLcxmDst::GetCam()
{
	return m_pCam;
}


INT CLcxmDst::GetHeight(float* vcOut, const float* vcIn)
{
	D3DXVECTOR3 vcP	= *((D3DXVECTOR3*)vcIn);
	INT	nX = INT(vcP.x/m_Inf.iExt);
	INT	nZ = INT(vcP.z/m_Inf.iExt);

	CLcxmBlc*	pLcl	= NULL;

	if(nX>=0 && nZ>=0)
		pLcl = &m_ppLcl[nZ][nX];
	else
		return -1;


	FLOAT	fY   = pLcl->GetHeight(vcP);
	*((D3DXVECTOR3*)vcOut) = vcP;
	((D3DXVECTOR3*)vcOut)->y =fY;
	return 0;
}

