// Implementation of the Export classes.
//
////////////////////////////////////////////////////////////////////////////////

#pragma comment(lib, "d3dx9.lib")

#include "Max.h"
#include "resource.h"
#include "istdplug.h"

#include "stdmat.h"

#include "iparamb2.h"
#include "iparamm2.h"
#include "modstack.h"

#include "cs\Bipexp.h"
#include "cs\Phyexp.h"
#include "ISkin.h"

#include "LcmExpDX.h"


#define SAFE_DELETE_ARRAY(p){ if(p){ delete [] (p);	(p) = NULL;	} }
DWORD WINAPI fncc(LPVOID arg)	{	return 0;	}	// Dummy for progressbar


INT CLcmExpDX::ExportAcm()
{
	INT	hr;
	INT	i=0, j=0;

	char	LogFile[_MAX_PATH]={0};
	FileRename(LogFile, m_sN, NULL, "log" );

	if(!m_bS)
		hr= DialogBoxParam( ::GetHinstance(), MAKEINTRESOURCE(IDD_EXPORT)
							, m_hWnd, DlgProc, (LPARAM)this			);

	if(FALSE == hr)
	{
		MessageBox(m_hWnd, "Export Cancel", "Message",0);
		return 1;
	}


	// Log File Open
	if(m_bLog)
		m_pFl = fopen(LogFile, "wt");


	//0. 헤더의 정보를 모은다.
	int		 iTick = GetTicksPerFrame();
	Interval range = m_pI->GetAnimRange();
	
	m_Header.nFrmB = range.Start() / iTick;
	m_Header.nFrmE = range.End() / iTick;
	m_Header.nFrmP = GetFrameRate();			// FPS
	m_Header.nFrmT = iTick;


	INode*	pRoot = m_pI->GetRootNode();

	// 1. Root는 제외하고 Node를 모은다
	INT iChild = pRoot->NumberOfChildren();

	for(i=0; i<iChild; ++i)
	{
		INode* pChild = pRoot->GetChildNode(i);

		if(pChild)
			GatherNode(pChild);
	}
	

	// 2. 머티리얼을 모은다.
	GatherMtl();


	////////////////////////////////////////////////////////////////////////////
	// 3. 맥스에서 Node와 Material 정보를 전부 모았다.
	// 3.1 먼저 배열를 만들어 필요한 데이터를 채운다.
	
	m_Header.iNmtl= m_vMaxMtrl.size();
	m_Header.iNgeo= m_vMaxNode.size();
	
	if( 0 == m_Header.iNmtl || 0 == m_Header.iNgeo)
	{
		MsgBox("Material: %d Geometry: %d\nExporting Terminated.", m_Header.iNmtl, m_Header.iNmtl);

		if(m_pFl)
			fclose(m_pFl);

		return TRUE;
	}

	m_pLcMtl	= new LcMtl[m_Header.iNmtl];
	m_pLcGeo	= new LcGeo[m_Header.iNgeo];


	// Geometry의 인덱스와 이름을 연결한다.
	for(i=0; i<m_Header.iNgeo; ++i)
	{
		LcGeo*	pDest	= &m_pLcGeo[i];
		INode*	pNode	= m_vMaxNode[i];
		INode*	pPrn	= pNode->GetParentNode();
		char*	sName	= pNode->GetName();
		char*	sPrnt	= NULL;


		// INode 연결(순전히 맥스 Exporter용)
		pDest->pNode	= pNode;

		// 이름을 복사한다.
		if(sName && strlen(sName))
			strcpy(pDest->sName, sName);

		// 인덱스를 복사한다.
		pDest->nIdx = i;

		// 부모의 노드를 찾아 인덱스를 설정한다.
		if(pPrn)
		{
			for(j=0; j<m_Header.iNgeo; ++j)
			{
				INode*	pCur = m_vMaxNode[j];
				
				// 자기 자신은 제외하고 부모 인덱스를 찾는다.
				if(pCur == pPrn && i != j )
				{
					pDest->nPrn = j;
					break;
				}
			}

			// 부모가 있는 지오 중에서
			if(0 <= pDest->nPrn)
			{
				sPrnt = pPrn->GetName();
				strcpy(pDest->sPrnt, sPrnt);
			}
		}


		// Bone or Not
		pDest->bBone = IsBone(pDest->pNode);
	}


	// 손쉬운 머티리얼부터 해결하자.
	// 3.2 머티리얼 구조체를 채운다.
	// Texture Name Log
	vector<MtlInf>::iterator _F = m_vMaxMtrl.begin();
	vector<MtlInf>::iterator _L = m_vMaxMtrl.end();

	for(i=0; _F != _L; ++_F, ++i)
	{
		LcMtl*	pDest	= &m_pLcMtl[i];
		MtlInf	pMtrl	= (*_F);
		Color	rgb;

		pDest->nIdx		= i;		// Setting Material Index

		rgb = pMtrl.pM->GetAmbient();
		pDest->dMtl.Ambient.r = rgb.r;
		pDest->dMtl.Ambient.g = rgb.g;
		pDest->dMtl.Ambient.b = rgb.b;

		rgb = pMtrl.pM->GetDiffuse();
		pDest->dMtl.Diffuse.r = rgb.r;
		pDest->dMtl.Diffuse.g = rgb.g;
		pDest->dMtl.Diffuse.b = rgb.b;

		rgb = pMtrl.pM->GetSpecular();
		pDest->dMtl.Specular.r = rgb.r;
		pDest->dMtl.Specular.g = rgb.g;
		pDest->dMtl.Specular.b = rgb.b;

		pDest->dMtl.Power = pMtrl.pM->GetShininess();

		INT iSub = pMtrl.pM->NumSubTexmaps();

		for( j=0; j<iSub; ++j)
		{
			Texmap* subTex = pMtrl.pM->GetSubTexmap(j);

			if (subTex)
			{
				DumpTexture(pDest, subTex, j);
			}
		}
	}
	

	////////////////////////////////////////////////////////////////////////////
	// 4. Geometry는 조금 복잡하다. 크게 2가지로 나누어서 작업을 해야하는데
	// 먼저 정점에 대한 처리다. 바이패드도 정점을 가지고 있으나 게임에서는
	// 바이패드의 링크와 행렬만 필요하다. 오브젝트의 경우 정점과 텍스처 좌표,
	// 등을 뽑아야 한다. 그런데 맥스는 DX와 반대로 하나의 텍스처 좌표에 여러 정점이
	// 붙어 있는 경우가 있다. 따라서 DX를 위해서라면 정점을 텍스춰 좌표 만큼 늘려
	// 주어야 한다. 이 부분이 조금 복잡하다.
	// 
	// 다음으로 Geometry에서 처리해야 할 과제는 애니메이션 정보다.
	// 애니메이션을 Rigid, Skinning으로 하는가에 따라서 다르고, 
	// 고정된 프레임에서 데이터를 추출하는 경우와 키에서 데이터를 추출하는 경우를
	// 고려 해야 한다.
	//
	// 이러한 점을 염두(Keeping in mind)해두고 Geometry를 구성한다.


	// 4.1 월드 행렬
	// 맥스는 오른손 좌표계, DX는 왼손 좌표계.. x,y를 교체해준다.
	// 월드 행렬 설정
	
	INT	iTimeBgn	= m_Header.nFrmB * m_Header.nFrmT;

	for(i=0; i<m_Header.iNgeo; ++i)
	{
		LcGeo*	pDest	= &m_pLcGeo[i];
		INode*	pNode	= pDest->pNode;						// Current Node
		Matrix3 mtCur	= pNode->GetNodeTM(iTimeBgn);		// Matrix3 ==>FLOAT m[4][3]; GetNodeTM()은 World Matrix을 구한다.
		MaxMatrixToD3D(&pDest->mtWldT, &mtCur, FALSE);		// 전부 임시 월드 행렬에 넣는다.

		Matrix3	tm		= pNode->GetNodeTM(0);
		tm.Invert();

		MaxMatrixToD3D(&pDest->mtOrn, &tm, TRUE);
	}


	// 4.2 지역 행렬
	// 지역 좌표를 계산해야 한다.
	// 지역좌표의 월드행렬
	// W =  지역좌표 행렬 L * 부모 월드 행렬 W;
	// 이므로
	// 지역좌표 행렬 L = W * 부모 월드 행렬의 역행렬;
	// L = W * PrnW^(-1)

	// 부모가 있는 Geometry만 적용
	for(i=0; i<m_Header.iNgeo; ++i)
	{
		LcGeo*	pDest	= &m_pLcGeo[i];

		if(-1 == pDest->nPrn)
		{
			pDest->mtWld = pDest->mtWldT;
			pDest->mtLcl = pDest->mtWldT;
		}

		// 부모가 있다면 지역행렬은 월드 행렬임
		else
		{
			MATA	mtPrnW = m_pLcGeo[pDest->nPrn].mtWldT;
			MATA	mtPrnI;
			D3DXMatrixInverse(&mtPrnI, NULL, &mtPrnW);
			
			pDest->mtWld = pDest->mtWldT;

			// L = W * PrnW^(-1)
			pDest->mtLcl = pDest->mtWldT * mtPrnI;
		}
	}



	// 4.3. Physique or Skinning을 설정한다.
	for(i=0; i<m_Header.iNgeo; ++i)
	{
		LcGeo*	pDest	= &m_pLcGeo[i];
		GatherBoneWeight(pDest);
	}


	// 4.4 폴리곤을 추출한다.
	for(i=0; i<m_Header.iNgeo; ++i)
	{
		LcGeo*	pDest	= &m_pLcGeo[i];
		GatherMesh(pDest, iTimeBgn);		// Mesh를 모은다.
	}



	// 4.5. Animation Data 정보를 모은다.
	for(i=0; i<m_Header.iNgeo; ++i)
	{
		LcGeo*	pDest	= &m_pLcGeo[i];
		GatherAnimation(pDest);
	}


	// 5. 마지막으로 모델 파일에 출력을 한다.
	if(m_bTxt)
		WriteTxt();

	WriteBin();

	delete [] m_pLcMtl;
	delete [] m_pLcGeo;

	// Export할 때 발생한 로그를 닫는다.
	if(m_pFl)
		fclose(m_pFl);

	return TRUE;
}



void CLcmExpDX::GatherNode(INode* pNode)
{
	if(!pNode)
		return;

	m_vMaxNode.push_back(pNode);

	INT iChild = pNode->NumberOfChildren();

	for(int i=0; i<iChild; ++i)
	{
		INode* pChild = pNode->GetChildNode(i);

		if(pChild)
			GatherNode(pChild);
	}
}



void CLcmExpDX::GatherMtl()
{
	INT nNode = m_vMaxNode.size();

	for(int i=0; i<nNode; ++i)
	{
		INode*	pNode = m_vMaxNode[i];
		Mtl*	pMtrl = pNode->GetMtl();

		if(!pMtrl)
			continue;

		int	iSub = pMtrl->NumSubMtls();

		// sub Material이 없는 경우
		if(0==iSub)
		{
			BOOL	bFind=FALSE;

			int iSize = m_vMaxMtrl.size();

			for(int k=0; k<iSize; ++k)
			{
				if(pMtrl == m_vMaxMtrl[k].pM)
				{
					bFind = TRUE;
					break;
				}
			}

			if(FALSE == bFind)
				m_vMaxMtrl.push_back(MtlInf(pMtrl, i, 0));
		}
		else
		{
			for(int j=0; j<iSub; ++j)
			{
				Mtl* pSub = pMtrl->GetSubMtl(j);

				BOOL	bFind=FALSE;

				int iSize = m_vMaxMtrl.size();

				for(int k=0; k<iSize; ++k)
				{
					if(pSub == m_vMaxMtrl[k].pM)
					{
						bFind = TRUE;
						break;
					}
				}

				if(FALSE == bFind)
					m_vMaxMtrl.push_back(MtlInf(pSub, i, j));
			}
		}
	}
}



void CLcmExpDX::GatherMesh(LcGeo* pDest, TimeValue t)
{
	INode*	pNode	= pDest->pNode;				// Current Node

	BOOL	needDel;
	TriObject* tri = GetTriObjectFromNode(pNode, t, needDel);

	if (!tri)
		return;

	INT	 i=0, j=0;
	
	Mesh*	pMesh	= &tri->GetMesh();

	if(NULL ==pMesh)
		return;

	INT	iNvtx	= pMesh->getNumVerts();
	INT	iNfce	= pMesh->getNumFaces();
	INT	iNtvt	= pMesh->getNumTVerts();		// Tface와 Face숫자는 같음


	if(0 == iNfce || 0 == iNvtx)				// 정점이나 페이스가 없는 경우
		return;


	Matrix3		tm	 = pNode->GetObjTMAfterWSM(t);
	//	Matrix3 matNodeTM = pNode->GetNodeTM(0);
	BOOL bParity = tm.Parity();

	VtxNDUV1*	pVtN = NULL;					// DirectX에서 사용되는 최종 정점 데이터
	VtxD*		pVtD = NULL;					// DirectX에서 사용되는 최종 정점 데이터

	VEC3*		pPos = NULL;					// 정점의 위치를 추출용(임시)
	VEC3*		pNor = NULL;					// 정점의 법선용(임시)
	VtxIdx*		pFce = NULL;					// 정점의 Face용(임시)
	VEC2*		pTvt = NULL;					// 정점의 UV용(임시)

	VtxIdx*		pTct = NULL;					// Geometry의 Face로 사용한다.(최종)
	LcmBoneW*	pBdst= NULL;

	if(0 == iNtvt)								// 본과 바이패드의 경우 정점과 Face만 추출
	{
		pVtD = new VtxD[iNvtx];
		pFce = new VtxIdx[iNfce];

		for (i=0; i<iNvtx; ++i)
		{
			Point3 v = tm * pMesh->verts[i];
			pVtD[i] = VtxD(v.x, v.z, v.y);
		}

		for (i=0; i<iNfce; ++i)
		{

			if(!bParity)
			{
				pFce[i] = VtxIdx( pMesh->faces[i].v[0]
								, pMesh->faces[i].v[2]
								, pMesh->faces[i].v[1]);
			}
			else
			{
				pFce[i] = VtxIdx( pMesh->faces[i].v[0]
								, pMesh->faces[i].v[1]
								, pMesh->faces[i].v[2]);
			}

			
		}

		pDest->dFVF = VtxD::FVF;
		pDest->iNvtx= iNvtx;
		pDest->iNfce= iNfce;
		pDest->pVxO	= pVtD;
		pDest->pFce	= pFce;


		// Material과 Geometry를 연결

		Mtl* pMtl = pNode->GetMtl();
		vector<MtlInf>::iterator _F = m_vMaxMtrl.begin();
		vector<MtlInf>::iterator _L = m_vMaxMtrl.end();

		for(i=0; _F != _L; ++_F, ++i)
		{
			if( pMtl == (*_F).pM)
			{
				pDest->nMtl = i;
				break;
			}
		}

		return;
	}


	////////////////////////////////// 텍스처가 있는 경우
	// 노말 벡터를 만들고
	pMesh->buildNormals();

	pPos = new VEC3[iNvtx];
	pNor = new VEC3[iNvtx];

	pFce = new VtxIdx[iNfce];
	pTct = new VtxIdx[iNfce];

	pTvt = new VEC2[iNtvt];
	

	// Vertex and Normal
	for (i=0; i<iNvtx; ++i)
	{
		Point3 v = tm * pMesh->verts[i];
		pPos[i] = VEC3(v.x, v.z, v.y);
	
		Point3 n = Normalize(pMesh->getNormal(i));
		pNor[i] = VEC3(n.x, n.z, n.y);
	}


	//Tvertex
	for (i=0; i<iNtvt; i++)
	{
			UVVert t = pMesh->tVerts[i];
			pTvt[i].x = t.x;
			pTvt[i].y = 1.f - t.y;
	}


	// Face and TFace
	mpTfce		mpTfaceId;							// Tface와 Face의 인덱스를 연결하기위해 인덱스를 저장.
	for (i=0; i<iNfce; ++i)
	{

		if(!bParity)
		{
			pFce[i] = VtxIdx( pMesh->faces[i].v[0]
							, pMesh->faces[i].v[2]
							, pMesh->faces[i].v[1]);

			pTct[i] = VtxIdx( pMesh->tvFace[i].t[0]
							, pMesh->tvFace[i].t[2]
							, pMesh->tvFace[i].t[1]);
		}
		else
		{
			pFce[i] = VtxIdx( pMesh->faces[i].v[0]
							, pMesh->faces[i].v[1]
							, pMesh->faces[i].v[2]);

			pTct[i] = VtxIdx( pMesh->tvFace[i].t[0]
							, pMesh->tvFace[i].t[1]
							, pMesh->tvFace[i].t[2]);
		}

		// Face와 TFace의 인덱스를 결합
		mpTfaceId.insert(	mpTfce::value_type( TfceIdx(pTct[i].a),  pFce[i].a)		);
		mpTfaceId.insert(	mpTfce::value_type( TfceIdx(pTct[i].b),  pFce[i].b)		);
		mpTfaceId.insert(	mpTfce::value_type( TfceIdx(pTct[i].c),  pFce[i].c)		);
	}


	// Tvtx의 시작 인덱스는 0이 아닌경우가 많고, 작업자가 UV를 지우면 그 인덱스가 남아 
	// 있어 Tverts들을 패킹을 해야 한다.
	// Tface또한 조정해야 한다.

//	WriteLog("T-Face and Face Index\n");

	itTfce	itF		= mpTfaceId.begin();
	itTfce	itL		= mpTfaceId.end();

	vector<_Tpck >	lsFace;

	for(; itF != itL; ++itF)
	{
		int nT = (*itF).first.n;
		int nV = (*itF).second;

		// U, V정보까지 추가한다.
		FLOAT U = pTvt[nT].x;
		FLOAT V = pTvt[nT].y;

//		WriteLog("[%4d %4d	%6.4f	%6.4f]\n", nT, nV, u, v);

		lsFace.push_back( _Tpck(nT, nV, U, V));
	}

	mpTfaceId.clear();


	// 게임에 사용될 최종 Face를 조정한다.
	for(int nIdx=0; nIdx<lsFace.size(); ++nIdx)
	{
		int nT = lsFace[nIdx].nT;

		for(j=0; j<iNfce; ++j)
		{
			if(nT == pTct[j].a)
				pTct[j].a = nIdx;

			if(nT == pTct[j].b)
				pTct[j].b = nIdx;

			if(nT == pTct[j].c)
				pTct[j].c = nIdx;
		}
	}

	// Face 인덱스 리스트가 게임에서 사용되는 정점 인덱스다.
	// 따라서 정점의 갯수는 Face 인덱스와 동일하다.

	INT	iVtxSize =  lsFace.size();
	pVtN = new VtxNDUV1[iVtxSize];
	pBdst= new LcmBoneW[iVtxSize];

	// Map의 인덱스를 가지고 정점을 찾는다.
	for(i=0; i<iVtxSize; ++i)
	{
		int nV = lsFace[i].nV;		// 정점의 인덱스
		FLOAT u = lsFace[i].u;
		FLOAT v = lsFace[i].v;

		pVtN[i].p = pPos[nV];
		pVtN[i].n = pNor[nV];
		pVtN[i].u = u;
		pVtN[i].v = v;

		// Bone Index를 연결한다.
		if(nV>=pDest->nBsrc)
		{
			WriteLog("Over flow Vertex Index\n");
			continue;
		}


		LcmBoneW* pBsrc = &pDest->pBsrc[nV];
		int		 iBone = pBsrc->vB.size();

		map<INT, FLOAT >::iterator	_F = pBsrc->vB.begin();
		map<INT, FLOAT >::iterator	_L = pBsrc->vB.end();

		for(; _F != _L; ++_F)
		{
			pBdst[i].vB.insert( *_F );
		}
	}

	lsFace.clear();


	// 임시 변수를 삭제
	SAFE_DELETE_ARRAY(	pPos	);
	SAFE_DELETE_ARRAY(	pNor	);
	SAFE_DELETE_ARRAY(	pFce	);
	SAFE_DELETE_ARRAY(	pTvt	);


	// 마지막으로 Geometry에 연결
	pDest->dFVF  = VtxNDUV1::FVF;
	pDest->iNvtx = iVtxSize;
	pDest->iNfce = iNfce;
	pDest->pVxO	 = pVtN;
	pDest->pFce	 = pTct;
	pDest->pBdst = pBdst;


	// Material과 Geometry를 연결
	Mtl*	pMtl	= pNode->GetMtl();
	int		iSub	= pMtl->NumSubMtls();

	// sub Material이 있 경우
	if(0<iSub)
		pMtl = pMtl->GetSubMtl(0);

	vector<MtlInf>::iterator _F = m_vMaxMtrl.begin();
	vector<MtlInf>::iterator _L = m_vMaxMtrl.end();

	for(i=0; _F != _L; ++_F, ++i)
	{
		if( pMtl == (*_F).pM)
		{
			pDest->nMtl = i;
			break;
		}
	}

	WriteLog("MaterialID: %2d %2d\n", pDest->nMtl, iSub);


	//(E) 2008-05-24 Editor: AFEW
	//Face 시작	Face 수	Material Index
	int nStart = pDest->nMtl;
	int	nOld	=-1000000;
	int	nNew	=-1000000;
	int	nFce	=0;


	for(i=0; i<iNfce; ++i)
	{
		nNew = pMesh->faces[i].getMatID();

		if(nNew != nOld)
		{
			nFce = pDest->vFceRnd.size();
			pDest->vFceRnd.push_back(LcFceR(i, 1, pDest->nMtl + nNew));
			nOld= nNew;
		}
		else
		{
			pDest->vFceRnd[nFce].nNum += 1;
		}
	}

	int iSize = pDest->vFceRnd.size();

	for (i=0; i<iSize; ++i)
	{
		WriteLog("Face_Mtrl: %4d %4d %2d\n"
					, pDest->vFceRnd[i].nBgn
					, pDest->vFceRnd[i].nNum
					, pDest->vFceRnd[i].nMtl);
	}
}



void CLcmExpDX::GatherAnimation(LcGeo* pDest)
{
	if(!pDest->bBone)
		return;

	GatherBoneAni(pDest);
}


void CLcmExpDX::GatherBoneAni(LcGeo* pDest)
{
	INode*	pNode = pDest->pNode;			// Current Node

	DWORD	dTick	= m_Header.nFrmT;
	DWORD	dTimeB	= m_Header.nFrmB * dTick;
	DWORD	dTimeE	= m_Header.nFrmE * dTick;
	
	Matrix3 mtCur;
	Matrix3 mtPrn;

	for(DWORD dTime = dTimeB ; dTime<=dTimeE ; dTime += dTick)
	{
		mtCur = pNode->GetNodeTM(dTime);
		mtPrn = pNode->GetParentTM(dTime);

		mtCur.NoScale();
		
		Quat qt(mtCur);
		qt.Normalize();
		
		QUAT vRot;
		vRot.x = qt.x;
		vRot.y = qt.z;
		vRot.z = qt.y;
		vRot.w = qt.w;
		pDest->vAniRot.push_back(vRot);

		VEC3 vTrn;
		Point3 pos = mtCur.GetTrans();
		vTrn.x = pos.x;
		vTrn.y = pos.z;
		vTrn.z = pos.y;

		pDest->vAniTrn.push_back(vTrn);
	}
}



INT CLcmExpDX::FindBoneId(INode* pNode)
{
	for(int i=0; i<m_Header.iNgeo; ++i)
	{
		LcGeo*	pDest	= &m_pLcGeo[i];

		if(pDest->pNode == pNode)
			return i;
	}

	return -1;
}




void CLcmExpDX::WriteBin()
{
	INT		i=0;
	INT		j=0;
	INT		k=0;


	// 5. 파일에 기록한다.
	FILE*	fp = fopen(m_sN, "wb");

	// Export Header Data
	fwrite(&m_Header, sizeof(LcmHeader), 1, fp);
	fseek(fp, LCM_HEADER_SIZE, SEEK_SET);			// chunck 만큼 이동
		
	
	// Writing Material
	for(i=0; i<m_Header.iNmtl; ++i)
	{
		LcMtl*	pDest	= &m_pLcMtl[i];

		fwrite(&pDest->nIdx,  sizeof(INT ), 1, fp);
		fwrite(&pDest->dMtl,  sizeof(DMTL), 1, fp);

		fwrite(pDest->sDif, sizeof(char), LCM_TX_NAME, fp);
		fwrite(pDest->sNor, sizeof(char), LCM_TX_NAME, fp);
		fwrite(pDest->sSpc, sizeof(char), LCM_TX_NAME, fp);
		fwrite(pDest->sEms, sizeof(char), LCM_TX_NAME, fp);
	}


	// Writing Geometry
	for(i=0; i<m_Header.iNgeo; ++i)
	{
		LcGeo*	pDest	= &m_pLcGeo[i];

		fwrite(pDest->sName,  sizeof(char), 32, fp);
		fwrite(pDest->sPrnt,  sizeof(char), 32, fp);

		fwrite(&pDest->nIdx,  sizeof(INT ), 1, fp);
		fwrite(&pDest->nPrn,  sizeof(INT ), 1, fp);
		fwrite(&pDest->nMtl,  sizeof(INT ), 1, fp);

		fwrite(&pDest->dFVF, sizeof(DWORD), 1, fp);
		fwrite(&pDest->iNfce, sizeof(INT ), 1, fp);
		fwrite(&pDest->iNvtx, sizeof(INT ), 1, fp);

		if(pDest->iNfce)
			fwrite(pDest->pFce, sizeof(VtxIdx), pDest->iNfce, fp);


		if(pDest->iNvtx)
		{
			int		iSize= (VtxD::FVF == pDest->dFVF)? sizeof(VtxD) : sizeof(VtxNDUV1);
			fwrite(pDest->pVxO, iSize, pDest->iNvtx, fp);
		}


		int iSize = pDest->vFceRnd.size();
		fwrite(&iSize, sizeof(INT ), 1, fp);

		for(int k=0; k<iSize; ++k)
		{
			LcFceR*	pFceRnd	= &pDest->vFceRnd[k];

			fwrite(pFceRnd, sizeof(LcFceR), 1, fp);
		}

	}

	// Writing Animation
	for(i=0; i<m_Header.iNgeo; ++i)
	{
		LcGeo*	pDest	= &m_pLcGeo[i];

		fwrite(&pDest->mtLcl, sizeof(MATA), 1, fp);
		fwrite(&pDest->mtWld, sizeof(MATA), 1, fp);
		fwrite(&pDest->mtOrn, sizeof(MATA), 1, fp);

		int iSize = pDest->vAniRot.size();
		fwrite(&iSize, sizeof(INT ), 1, fp);

		for(int k=0; k<iSize; ++k)
		{
			QUAT*	vcR	= &pDest->vAniRot[k];
			VEC3*	vcT	= &pDest->vAniTrn[k];

			fwrite(vcR, sizeof(QUAT), 1, fp);
			fwrite(vcT, sizeof(VEC3), 1, fp);
		}
	}



	// Export Vertex Weight Data
	for(i=0; i<m_Header.iNgeo; ++i)
	{
		LcGeo*	pDest	= &m_pLcGeo[i];

		fwrite(&pDest->nBsrc, sizeof(INT ), 1, fp);

		if(0>=pDest->nBsrc)
			continue;

		for(j=0; j<pDest->iNvtx; ++j)
		{
			LcmBoneW*	pBdst = &pDest->pBdst[j];
			INT			iBone = pBdst->vB.size();

			fwrite(&iBone, sizeof(INT ), 1, fp);

			map<INT, FLOAT >::iterator	__F = pBdst->vB.begin();
			map<INT, FLOAT >::iterator	__L = pBdst->vB.end();

			for(; __F != __L; ++__F)
			{
				INT		nB = (*__F).first;
				FLOAT	fW = (*__F).second;
				fwrite(&nB, sizeof(INT  ), 1, fp);
				fwrite(&fW, sizeof(FLOAT), 1, fp);
			}

		}
	}

	fclose(fp);
}



void CLcmExpDX::WriteTxt()
{
	INT		i=0;
	INT		j=0;
	INT		k=0;

	char TxtFile[_MAX_PATH]={0};
	FileRename(TxtFile, m_sN, NULL, "act" );

	FILE*	fp = fopen(TxtFile, "wt");

	// Export Header Data
	fprintf(fp, "Version: %s\n", m_Header.sVer);
	fprintf(fp, "Material: %3d  NodeNum: %3d\n", m_Header.iNmtl, m_Header.iNgeo);
	fprintf(fp, "FrameBeginEndFpsTick: %3d %3d %3d %3d\n\n", m_Header.nFrmB, m_Header.nFrmE, m_Header.nFrmP, m_Header.nFrmT);


	// Export Vertex and Index Data
	for(i=0; i<m_Header.iNmtl; ++i)
	{
		LcMtl*	pDest =&m_pLcMtl[i];

		fprintf(fp, "\nMaterial: %3d\n", pDest->nIdx);
		fprintf(fp, "	TextureDif: \"%s\"\n", pDest->sDif);
		fprintf(fp, "	TextureNor: \"%s\"\n", pDest->sNor);
		fprintf(fp, "	TextureSpc: \"%s\"\n", pDest->sSpc);
		fprintf(fp, "	TextureEms: \"%s\"\n", pDest->sEms);
	}

	fprintf(fp, "\n\n");



	// Export Vertex and Index Data
	for(i=0; i<m_Header.iNgeo; ++i)
	{
		LcGeo*	pDest	= &m_pLcGeo[i];

		char*	sName = pDest->sName;
		char*	sPrnt = pDest->sPrnt;

		fprintf(fp, "\nGeoIdxPrnMtl: %2d %2d %2d ", pDest->nIdx, pDest->nPrn, pDest->nMtl);
		fprintf(fp, "\"%s\"	\"%s\"\n", sName, sPrnt);


		fprintf(fp, "FaceVtxFVF: %3d  %3d 0x%X\n", pDest->iNfce, pDest->iNvtx, pDest->dFVF);

		if(pDest->iNfce <=0)
			continue;

		fprintf(fp, "Faces:\n");

		for(j=0; j<pDest->iNfce; ++j)
		{
			fprintf(fp, "	%4d %4d %4d\n", pDest->pFce[j].a, pDest->pFce[j].b, pDest->pFce[j].c);
		}

		fprintf(fp, "Vertices:\n");
		for(j=0; j<pDest->iNvtx; ++j)
		{
			if(VtxD::FVF == pDest->dFVF)
			{
				VtxD*	pVtD = (VtxD*)pDest->pVxO;
				fprintf(fp, "	%10.5f %10.5f %10.5f   0x%8X\n"
								, pVtD[j].p.x, pVtD[j].p.y, pVtD[j].p.z, pVtD[j].d);
			}
			else
			{
				VtxNDUV1*	pVtN = (VtxNDUV1*)pDest->pVxO;
				fprintf(fp, "	%10.5f %10.5f %10.5f   %10.5f %10.5f %10.5f   0x%8X  %10.5f %10.5f\n"
								, pVtN[j].p.x, pVtN[j].p.y, pVtN[j].p.z
								, pVtN[j].n.x, pVtN[j].n.y, pVtN[j].n.z
								, pVtN[j].d  , pVtN[j].u, pVtN[j].v);
			}
		}

		fprintf(fp, "Face Begin, Face Number, Material Index:\n");
		int iSize = pDest->vFceRnd.size();

		for(j=0; j<iSize; ++j)
		{
			LcFceR*	pFceRnd	= &pDest->vFceRnd[j];
			fprintf(fp, "	%4d %4d %4d\n", pFceRnd->nBgn, pFceRnd->nNum, pFceRnd->nMtl);
		}

		fprintf(fp, "\n");
	}


	fprintf(fp, "\n\n");


	// Export Animation Data
	for(i=0; i<m_Header.iNgeo; ++i)
	{
		LcGeo*	pDest	= &m_pLcGeo[i];
		char*	sName = pDest->sName;

		fprintf(fp, "AniGeoIdx: %2d ", pDest->nIdx);
		if(strlen(sName))	fprintf(fp, "\"%s\"\n", sName);
		else				fprintf(fp, "\"<NULL>\"\n");

		fprintf(fp, "World:\n");

		for(j=0; j<4; ++j)
		{
			for(k=0; k<4; ++k)
			{
				fprintf(fp, "%12.6f", pDest->mtWld.m[j][k]);
			}

			fprintf(fp, "\n");
		}


		fprintf(fp, "Local:\n");
		for(j=0; j<4; ++j)
		{
			for(k=0; k<4; ++k)
			{
				fprintf(fp, "%12.6f", pDest->mtLcl.m[j][k]);
			}

			fprintf(fp, "\n");
		}

		fprintf(fp, "\n");
		

		// Animation Matrix Data
		INT		nAni = pDest->vAniRot.size();

		fprintf(fp, "AnimationMatrix: %3d\n", nAni);

		for(j=0; j<nAni; ++j)
		{
			VEC3*	pT = &pDest->vAniTrn[j];
			fprintf(fp, "	%4d	%12.5f %12.5f %12.5f\n", j, pT->x, pT->y, pT->z);
		}

		fprintf(fp, "\n");

		for(j=0; j<nAni; ++j)
		{
			QUAT*	pT = &pDest->vAniRot[j];
			fprintf(fp, "	%4d	%12.5f %12.5f %12.5f %12.5f\n", j, pT->x, pT->y, pT->z, pT->w);
		}
		
		// Animation Rotation Data
		fprintf(fp, "\n");
	}

	fprintf(fp, "\n\n");

	// Export Vertex Weight Data
	for(i=0; i<m_Header.iNgeo; ++i)
	{
		LcGeo*	pDest	= &m_pLcGeo[i];

		if(!pDest->pBdst)
			continue;

		fprintf(fp, "VtxWeightGeo: %2d  \"%s\"\n", pDest->nIdx, pDest->sName);

		for(j=0; j<pDest->iNvtx; ++j)
		{
			LcmBoneW*	pBdst = &pDest->pBdst[j];
			INT			iBone = pBdst->vB.size();

			fprintf(fp, "		%4d %2d : ", j, iBone);


			map<INT, FLOAT >::iterator	__F = pBdst->vB.begin();
			map<INT, FLOAT >::iterator	__L = pBdst->vB.end();

			for(; __F != __L; ++__F)
			{
				INT		nB = (*__F).first;
				FLOAT	fW = (*__F).second;
				fprintf(fp, "[%4d %7.6f]", nB, fW);
			}

			fprintf(fp, "\n");
		}

		fprintf(fp, "\n");
	}


	fclose(fp);
}



