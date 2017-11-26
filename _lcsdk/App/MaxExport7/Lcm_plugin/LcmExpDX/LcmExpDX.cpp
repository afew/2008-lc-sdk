// Implementation of the Export classes.
//
////////////////////////////////////////////////////////////////////////////////


#include "Max.h"
#include "resource.h"
#include "stdmat.h"
#include "iparamb2.h"

#include "cs\Bipexp.h"
#include "cs\Phyexp.h"
#include "ISkin.h"


#include "LcmExpDX.h"


#define SAFE_DELETE_ARRAY(p){ if(p){ delete [] (p);	(p) = NULL;	} }


vector<string >	g_lsLogMsg;


////////////////////////////////////////////////////////////////////////////////
//
// Implementation of the LcmExpDXClassDesc class.
//
////////////////////////////////////////////////////////////////////////////////


int CLcmExpDXClassDesc::IsPublic()
{
	return TRUE;
}

void* CLcmExpDXClassDesc::Create(BOOL loading)
{
	return new CLcmExpDX();
}

const TCHAR* CLcmExpDXClassDesc::ClassName()
{
	return GetString(IDS_CLASS_NAME);
}

SClass_ID CLcmExpDXClassDesc::SuperClassID()
{
	return SCENE_EXPORT_CLASS_ID;
}

Class_ID CLcmExpDXClassDesc::ClassID()
{
	return LcmExpDX_CLASS_ID;
}

const TCHAR* CLcmExpDXClassDesc::Category()
{
	return GetString(IDS_CATEGORY);
}

const TCHAR* CLcmExpDXClassDesc::InternalName()
{
	return _T("CLcmExpDX");
}

HINSTANCE CLcmExpDXClassDesc::HInstance()
{
	return GetHinstance();
}



////////////////////////////////////////////////////////////////////////////////
//
// Implementation of the CLcmExpDX class.
//
////////////////////////////////////////////////////////////////////////////////


int			 CLcmExpDX::ExtCount()		{	return 1;					}
const TCHAR* CLcmExpDX::Ext(int)		{	return _T("Acm");			}
const TCHAR* CLcmExpDX::LongDesc()		{	return _T("Lcm Exporter");	}
const TCHAR* CLcmExpDX::ShortDesc()		{	return _T("Lcm Exporter");	}
const TCHAR* CLcmExpDX::AuthorName()	{	return _T("Heesung Oh");	}
const TCHAR* CLcmExpDX::CopyrightMessage(){	return _T("CopyLeft");		}

const TCHAR* CLcmExpDX::OtherMessage1()	{	return _T("");				}
const TCHAR* CLcmExpDX::OtherMessage2()	{	return _T("");				}
unsigned int CLcmExpDX::Version()		{	return 100;					}
void CLcmExpDX::ShowAbout(HWND hWnd)	{								}
BOOL CLcmExpDX::SupportsOptions(int,DWORD){	return TRUE;				}


CLcmExpDX::LcmHeader::LcmHeader()
{
	memset(sVer, 0, sizeof sVer);
	strcpy(sVer, "00.01");
	iNmtl	= 0;			// Merterial Num
	iNgeo	= 0;			// Merterial Num
	
	nFrmB	= 0;
	nFrmE	= 0;
	nFrmP	= 0;
	nFrmT	= 0;
}



CLcmExpDX::LcMtl::LcMtl()
{
	nIdx	= -1;
	memset(&dMtl, 0, sizeof dMtl);

	dMtl.Ambient.a = 1.f;
	dMtl.Diffuse.a = 1.f;
	dMtl.Specular.a= 1.f;
	dMtl.Emissive.a= 1.f;
	dMtl.Power	   =10.f;

	memset(sDif, 0, sizeof sDif);
	memset(sNor, 0, sizeof sNor);
	memset(sSpc, 0, sizeof sSpc);
	memset(sEms, 0, sizeof sEms);
}



CLcmExpDX::LcGeo::LcGeo()
{
	memset(sName, 0, sizeof sName);
	memset(sPrnt, 0, sizeof sPrnt);

	strcpy(sName, "<NULL>");
	strcpy(sPrnt, "<NULL>");

	nIdx	= -1;					// Index;
	nPrn	= -1;					// Parent Index
	nMtl	= -1;					// Material Index

	D3DXMatrixIdentity(&mtLcl);		// 항등 행렬로 만든다.
	D3DXMatrixIdentity(&mtWld);		// 단위행렬은 항등행렬
	D3DXMatrixIdentity(&mtWldT);	// 단위행렬은 항등행렬
	D3DXMatrixIdentity(&mtOrn);		// 단위행렬은 항등행렬

	dFVF	= 0;
	iNvtx	= 0;
	iNfce	= 0;

	pFce	= NULL;
	pVxO	= NULL;

	pNode	= NULL;

	bBone	= FALSE;
	nBsrc	= 0;					// Bone에 영향받는 버텍스 수
	pBsrc	= NULL;
	pBdst	= NULL;
}



CLcmExpDX::LcGeo::~LcGeo()
{
	SAFE_DELETE_ARRAY(	pFce	);

	if(pVxO)
	{
		if(VtxD::FVF == dFVF)
		{
			VtxD*	p	= (VtxD*)pVxO;
			delete [] p;
		}

		else
		{
			VtxNDUV1* p	= (VtxNDUV1*)pVxO;
			delete [] p;
		}

		pVxO = NULL;
	}


	SAFE_DELETE_ARRAY(	pBsrc	);
	SAFE_DELETE_ARRAY(	pBdst	);
}







CLcmExpDX::CLcmExpDX()
{
	m_bLog	= TRUE;				// IDC_EXP_LOG
	m_bAni	= TRUE;				// IDC_EXP_ANI
	m_bTxt	= TRUE;				// IDC_EXP_TXT

	m_hWnd	= NULL;
	m_pE	= NULL;
	m_pI	= NULL;
	m_bS	= 0;
	m_dO	= 0;

	m_pLcMtl= NULL;
	m_pLcGeo= NULL;

	m_pFl	= NULL;
}


CLcmExpDX::~CLcmExpDX() 
{
}



BOOL CALLBACK CLcmExpDX::DlgProc(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static CLcmExpDX *pLcm = NULL;

	WPARAM wmId    = LOWORD(wParam); 
	WPARAM wmEvent = HIWORD(wParam); 
	
	switch(uMsg)
	{
		case WM_INITDIALOG:
		{
			pLcm = (CLcmExpDX *)lParam;
			CenterWindow(hWnd,GetParent(hWnd));

			HWND hW;
			hW = GetDlgItem(hWnd, IDC_EXP_LOG);	SendMessage(hW, BM_SETCHECK, pLcm->m_bLog, 0);
			hW = GetDlgItem(hWnd, IDC_EXP_ANI);	SendMessage(hW, BM_SETCHECK, pLcm->m_bAni, 0);
			hW = GetDlgItem(hWnd, IDC_EXP_TXT);	SendMessage(hW, BM_SETCHECK, pLcm->m_bTxt, 0);

			return TRUE;
		}
			
		case WM_CLOSE:
		{
			EndDialog(hWnd, 0);
			return TRUE;
		}


		case WM_COMMAND:
		{
			if(IDC_EXP_LOG == wmId)
			{
				HWND hW;
				hW = GetDlgItem(hWnd, IDC_EXP_LOG);

				if(BST_UNCHECKED == SendMessage(hW, BM_GETCHECK, 0, 0))
					pLcm->m_bLog = FALSE;
				else
					pLcm->m_bLog = TRUE;
			}
			else if(IDC_EXP_ANI == wmId)
			{
				HWND hW;
				hW = GetDlgItem(hWnd, IDC_EXP_ANI);

				if(BST_UNCHECKED == SendMessage(hW, BM_GETCHECK, 0, 0))
					pLcm->m_bAni = FALSE;
				else
					pLcm->m_bAni = TRUE;
			}
			else if(IDC_EXP_TXT == wmId)
			{
				HWND hW;
				hW = GetDlgItem(hWnd, IDC_EXP_TXT);

				if(BST_UNCHECKED == SendMessage(hW, BM_GETCHECK, 0, 0))
					pLcm->m_bTxt = FALSE;
				else
					pLcm->m_bTxt = TRUE;
			}



			else if(IDC_DOEXPORT == wmId)
			{
				EndDialog(hWnd, TRUE);
			}
			
			else if(IDC_CANCEL == wmId)
			{
				EndDialog(hWnd, FALSE);
			}

			return TRUE;
		}
	}// switch

	return FALSE;
}




int	CLcmExpDX::DoExport(const TCHAR* name,ExpInterface* ei,Interface* pi, BOOL Prompt, DWORD Option)
{
	INT		hr;

	// Setup File Name to Lower
	{
		memset(m_sN, 0, sizeof m_sN);
		_tcscpy(m_sN, name);
		
		char *s = (char*)name;
		char *e = (char*)name;
		char *d = (char*)m_sN;

		e+= strlen( name );

		for( ; s<e; ++s, ++d)
			*d = tolower(*s);
	}

	m_hWnd	= pi->GetMAXHWnd();
	m_pE	= ei;
	m_pI	= pi;
	m_bS	= Prompt;
	m_dO	= Option;

	FileRename(m_sP, m_sN, NULL, NULL);

	hr = ExportAcm();

	if(!g_lsLogMsg.empty())
	{
		char	sMsg[8096]={0};
		char*	p = sMsg;

		for(int i=0; i<g_lsLogMsg.size(); ++i)
		{
			p += sprintf(p, "%s\n", g_lsLogMsg[i].c_str());
		}

		MsgBox(sMsg);
	}

	return hr;
}




void CLcmExpDX::WriteLog(char *format,...)
{
	if(NULL == m_pFl)
		return;

	va_list ap;
	char s[2048];
	
	if (format == NULL)
		return;
	
	va_start(ap, format);
	vsprintf((char *)s, format, ap);
	va_end(ap);
	
	if (s == NULL)
		return;
	

	fprintf(m_pFl, s);
}


//test if is a bone or a biped controller
BOOL CLcmExpDX::IsBone(INode *pNode)
{
	if(pNode == NULL)
		return FALSE;

	ObjectState os = pNode->EvalWorldState(0);

	if (!os.obj)
		return FALSE;

	if( 0 == _strnicmp(pNode->GetName(), "Bone", 4))
		return TRUE;

	if(os.obj->ClassID() == Class_ID(BONE_CLASS_ID, 0))
		return TRUE;

	if ( os.obj->ClassID() == BONE_OBJ_CLASSID )
		return TRUE;

	if ( os.obj->ClassID() == Class_ID(DUMMY_CLASS_ID, 0) )
		return TRUE;

	Control *cont = pNode->GetTMController();
	if(cont->ClassID() == BIPSLAVE_CONTROL_CLASS_ID ||		//others biped parts
		cont->ClassID() == BIPBODY_CONTROL_CLASS_ID			//biped root "Bip01"
		
	) return TRUE;

	return FALSE;
}



// iSubNo: Texture Map Index.
// ID_DI : DIFFUSE, ID_BU : Bump, ID_SP: Specular, ID_SI: self-illumination
void CLcmExpDX::DumpTexture(LcMtl* pDest, Texmap* txm, INT iSubNo)
{
	if(!txm)
		return;
	
	// Is this a bitmap texture? We know some extra bits 'n pieces about the bitmap texture
	if (txm->ClassID() == Class_ID(BMTEX_CLASS_ID, 0x00))
	{
		TSTR bitmapFile = ((BitmapTex *)txm)->GetMapName();

		if( bitmapFile.data() && strlen(bitmapFile)>3)
		{
			TSTR className;
			txm->GetClassName(className);

			// Split the name up
			TSTR fileName;
			
			SplitPathFile(bitmapFile, NULL, &fileName);

//			MsgBox("DumpTexture: %d", iSubNo);

			if(ID_AM == iSubNo)	// ambient
			{
			}
			else if(ID_DI == iSubNo)	// diffuse
				_tcscpy(pDest->sDif, fileName);

			else if(ID_SP == iSubNo || ID_SH == iSubNo)	// specular
				_tcscpy(pDest->sSpc, fileName);

			else if(ID_SI == iSubNo)	// self-illumination
				_tcscpy(pDest->sEms, fileName);

			else if(ID_BU == iSubNo)	// bump
				_tcscpy(pDest->sNor, fileName);



			if(fileName.Length()>4)
			{
				char	sDest[MAX_PATH]={0};

				_stprintf(sDest, _T("%s%s"), m_sP, fileName);
				INT hr = CopyFile(bitmapFile, sDest, TRUE);

				if(0==hr)
				{
					char	sMsg[256]={0};
					sprintf(sMsg, "Texture File(%s)을 복사할 수 없습니다. 수동으로 복사해야 합니다.", fileName);
					g_lsLogMsg.push_back(sMsg);
				}

			}

		}
	}
	
	INT iSub = txm->NumSubTexmaps();
	
	for(int i=0; i<iSub; ++i)
	{
		DumpTexture(pDest, txm->GetSubTexmap(i), iSubNo);		// iSubNo ==> 이것은 계속 유지되어야 한다.
	}
}




TriObject* CLcmExpDX::GetTriObjectFromNode(INode *node, TimeValue t, INT &deleteIt)
{
	deleteIt = FALSE;
	Object *obj = node->EvalWorldState(t).obj;

	if (obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
	{ 
		TriObject *tri = (TriObject *) obj->ConvertToType(t, Class_ID(TRIOBJ_CLASS_ID, 0));

		if (obj != tri)
			deleteIt = TRUE;

		return tri;
	}

	return NULL;
}





void CLcmExpDX::GatherBoneWeight(LcGeo* pDest)
{
	INode*		pNode = pDest->pNode;			// Current Node
	Modifier*	pMod = NULL;


	pMod = GetModifier(pNode, PHYSQ_CLASSID);	// Physique 선택

	if(pMod)
	{
		GetPhysiqueWeights(pDest, pMod);
		return;
	}

	
	pMod = GetModifier(pNode, SKIN_CLASSID);	// Skin 선택

	if(pMod)
		GetSkinWeights(pDest, pMod);
}



void CLcmExpDX::GetPhysiqueWeights(LcGeo* pDest, Modifier*	pMod)
{
	INode*		pNode = pDest->pNode;			// Current Node

	// create a Physique Export Interface for the given Physique Modifier
	IPhysiqueExport *pPhy = (IPhysiqueExport*)pMod->GetInterface(I_PHYINTERFACE);

	if(!pPhy)
		return;


	// create a ModContext Export Interface for the specific node of the Physique Modifier
	IPhyContextExport *pPhyContext = (IPhyContextExport*)pPhy->GetContextInterface(pNode);

	if(!pPhyContext)
	{
		pMod->ReleaseInterface(I_PHYINTERFACE, pPhy);
		return;
	}

	pPhyContext->ConvertToRigid(TRUE);	// needed by vertex interface (only Rigid one supported for now)
	pPhyContext->AllowBlending(TRUE);	// more than a single bone per vertex


	int nVtx = pPhyContext->GetNumberVertices();

	if(nVtx <=0)
		return;

	pDest->nBsrc = nVtx;					// Bone에 영향받는 버텍스 수
	pDest->pBsrc = new LcmBoneW[nVtx];

	
	for(int j=0; j<nVtx; ++j)
	{
		//get i-th vertex
		IPhyVertexExport *pVtxInfc = (IPhyVertexExport*)pPhyContext->GetVertexInterface(j);

		if(!pVtxInfc)
			continue;

		int vtxType = pVtxInfc->GetVertexType();

		// RIGID_TYPE는 Weight=1
		if(vtxType == RIGID_TYPE)
		{
			//본을 찾는다.
			INode* boneNode = ((IPhyRigidVertex*)pVtxInfc)->GetNode();

			//Weight값을 설정.
			INT		nBone	= FindBoneId(boneNode);
			FLOAT	fWeight	= 1.f;
		
			//pDest->pBsrc[j].vB.push_back(TboneW(nBone, fWeight));
			pDest->pBsrc[j].vB.insert(pair<INT, FLOAT>(nBone, fWeight));
			continue;
		}
		
		// RIGID_BLENDED_TYPE는 관계된 본을 찾아야 한다.
//		else if(vtxType == RIGID_BLENDED_TYPE)
		{
			IPhyBlendedRigidVertex *vtxBlendedInt = (IPhyBlendedRigidVertex*)pVtxInfc;

			for(int k = 0; k< vtxBlendedInt->GetNumberNodes(); ++k)
			{
				// k번째의 본을 찾는다.
				INode*	pBone	= vtxBlendedInt->GetNode(k);
				INT		nBone	= FindBoneId(pBone);
				FLOAT	fWeight	= vtxBlendedInt->GetWeight(k);
				
				if(fWeight>0.00001f)
				{
					//pDest->pBsrc[j].vB.push_back(TboneW(nBone, fWeight));
					pDest->pBsrc[j].vB.insert(pair<INT, FLOAT>(nBone, fWeight));
				}
			}
		}
	}
	
	pPhy->ReleaseContextInterface(pPhyContext);
	pMod->ReleaseInterface(I_PHYINTERFACE, pPhy);
}





void CLcmExpDX::GetSkinWeights(LcGeo* pDest, Modifier*	pMod)
{
	// Current Node
	INode*	pNode = pDest->pNode;
	
	ISkin*	pSkin = (ISkin*)pMod->GetInterface(I_SKIN);
	
	if(!pSkin)
		return;

	ISkinContextData *pSkinContext = pSkin->GetContextInterface(pNode);

	if(!pSkinContext)
		return;

	int nVtx = pSkinContext->GetNumPoints();

	if(nVtx <=0)
		return;

	pDest->nBsrc = nVtx;							// Bone에 영향받는 버텍스 수
	pDest->pBsrc = new LcmBoneW[nVtx];



	for(int j=0; j<nVtx;  ++j)
	{
		int numBones = pSkinContext->GetNumAssignedBones(j);
	
		// DirectX는 많아야 4개까지 설정 가능
		for(int k=0; k<numBones; ++k)
		{
			int assignedBone = pSkinContext->GetAssignedBone(j, k);

			if(assignedBone < 0)
				continue;

			INode*	pBone	= pSkin->GetBone(assignedBone);
			INT		nBone	= FindBoneId(pBone);
			FLOAT	fWeight = pSkinContext->GetBoneWeight(j, k);

			// Weight가 작은 값은 지나친다.
			if(fWeight<0.00005f)
				continue;

			//pDest->pBsrc[j].vB.push_back(TboneW(nBone, fWeight));
			pDest->pBsrc[j].vB.insert(pair<INT, FLOAT>(nBone, fWeight));
		}
	}

//	pSkin->ReleaseContextInterface(pSkinContext);
	pMod->ReleaseInterface(I_SKIN, pSkin);
}



Modifier* CLcmExpDX::GetModifier(INode *pNode, Class_ID nType)
{
	Object *pObj = pNode->GetObjectRef();

	if(!pObj)
		return NULL;

	// Is derived object ?
	while(pObj && pObj->SuperClassID() == GEN_DERIVOB_CLASS_ID)
	{
		// Yes -> Cast.
		IDerivedObject *pDerivedObj = static_cast<IDerivedObject*>(pObj);

		// Iterate over all entries of the modifier stack.
		int ModStackIndex = 0;

		while(ModStackIndex < pDerivedObj->NumModifiers())
		{
			// Get current modifier.
			Modifier* pMod = pDerivedObj->GetModifier(ModStackIndex);

			// Is this Physique or Skin?
			if(nType == pMod->ClassID())
				return pMod;

			
			// Next modifier stack entry.
			ModStackIndex++;
		}

		pObj = pDerivedObj->GetObjRef();
	}

	// Not found.
	return NULL;
}





void CLcmExpDX::FileRename(char* pOut
				, char* pIn				// Original FullPath File Name
				, char* sFileName		// New File Name. if it is NULL, then The File Name will be Conserved.
				, char* sExt			// New Extension. if it is NULL, then The Extension will be Conserved.
				)
{
	char tDrive[_MAX_DRIVE]={0};
	char tDir[_MAX_DIR]={0};
	char tFile[_MAX_FNAME]={0};
	char tExt[_MAX_EXT]={0};
	
	_splitpath( pIn, tDrive, tDir, tFile, tExt );

	if(sFileName && sExt)
	{
		_makepath( pOut, tDrive, tDir, sFileName, sExt );
	}
	else if(sFileName && NULL == sExt)
	{
		_makepath( pOut, tDrive, tDir, sFileName, tExt );
	}

	else if(NULL == sFileName && sExt)
	{
		_makepath( pOut, tDrive, tDir, tFile, sExt );
	}

	else
	{
		_makepath( pOut, tDrive, tDir, sFileName, sExt );
	}
}




void CLcmExpDX::MaxMatrixToD3D(D3DXMATRIX* pDst, Matrix3* pSrc, BOOL bIdentity)
{
	Point3	v3;
	v3	= pSrc->GetRow(0);	pDst->_11 = v3.x;	pDst->_12 = v3.z;	pDst->_13 = v3.y;
	v3	= pSrc->GetRow(2);	pDst->_21 = v3.x;	pDst->_22 = v3.z;	pDst->_23 = v3.y;
	v3	= pSrc->GetRow(1);	pDst->_31 = v3.x;	pDst->_32 = v3.z;	pDst->_33 = v3.y;
	v3	= pSrc->GetRow(3);	pDst->_41 = v3.x;	pDst->_42 = v3.z;	pDst->_43 = v3.y;

	if(bIdentity)
	{
		pDst->_14 = 0.f;
		pDst->_24 = 0.f;
		pDst->_34 = 0.f;
		pDst->_44 = 1.f;
	}
}