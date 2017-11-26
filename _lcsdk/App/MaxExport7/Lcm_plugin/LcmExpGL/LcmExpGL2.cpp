// Implementation of the Export classes.
//
////////////////////////////////////////////////////////////////////////////////

#include <d3dx9.h>

#include "Max.h"
#include "resource.h"

#include "iparamb2.h"

#include "cs\Bipexp.h"
#include "cs\Phyexp.h"
#include "ISkin.h"

#include "LcmExpGL.h"


static DWORD WINAPI fnc(LPVOID arg) { return(0); }

int LcmExpGL::DoExport(const TCHAR* name,ExpInterface* ei,Interface* pi, BOOL Prompt, DWORD Option)
{
	INT		hr;
	INT		i=0;
	INT		j=0;
	int		k=0;

	char	LogFile[_MAX_PATH]={0};

	// Setup File Name to Lower
	memset(m_sN, 0, sizeof m_sN);
	_tcscpy(m_sN, name);
	
	char *s = (char*)name;
	char *e = (char*)name;
	char *d = (char*)m_sN;

	e+= strlen( name );

	for( ; s<e; ++s, ++d)
		*d = tolower(*s);


	m_hWnd	= pi->GetMAXHWnd();
	m_pE	= ei;
	m_pI	= pi;
	m_bS	= Prompt;
	m_dO	= Option;


	hr = DialogBoxParam(::GetHinstance(), MAKEINTRESOURCE(IDD_PANEL), GetActiveWindow(), DlgProc, (LPARAM)this);

	if(!hr)
		return FALSE;


	m_pI->ProgressStart( "Exporting to Arc File: ...", TRUE, fnc, NULL);
	
	FILE* fp = fopen(name, "wb");

	if(!fp)
	{
		m_pI->ProgressEnd();
		MessageBox(NULL, "Couldn't open file for writing.", NULL, MB_OK);
		return 0;
	}
	
	Write(fp);

	if(fp)
		fclose(fp);

	m_pI->ProgressEnd();

	return TRUE;
}


BOOL LcmExpGL::Write(FILE* fp)
{
	rewind(fp);
	//write format version info
	fwrite(&mrc_version, sizeof(mrc_version), 1, fp);

	BOOL ok=FALSE;
	LONG startPos = ftell(fp);
	fseek(fp, sizeof(MrcObj), SEEK_CUR);

	MrcObj oHdr;
	memset(&oHdr, 0, sizeof(MrcObj));

	if (!m_pI->GetSelNodeCount())
	{
		MessageBox(NULL,"You must select a mesh to export",NULL,MB_OK);
		return FALSE;
	}

	INode* pNode = m_pI->GetSelNode(0);
	INode* const pRoot = m_pI->GetRootNode();	

	m_pI->ProgressUpdate(0,FALSE,_T("..writing MESH data..."));

	// writing Mesh data to file
	ok = ExportMesh(pNode, fp);
	
	oHdr.boneCnt = CountBones(pRoot);
	// Not only a mesh
	if (oHdr.boneCnt) 
	{	
		//Bone data
		m_pI->ProgressUpdate(15,FALSE,_T("..Processing BONE data..."));
////
		oHdr.boneOfs = ftell(fp);		
////
		MrcLink* boneData = new MrcLink [ oHdr.boneCnt ];
		
		ok = ProcessBoneWeights (pNode,pRoot, boneData);
			
		m_pI->ProgressUpdate(20,FALSE,_T(".....Skel struct..."));
		
		ProcessBoneStruct (pRoot, pRoot, -1, boneData);
	
		m_pI->ProgressUpdate(30,FALSE,_T(".....ANIM data..."));
		
			Interval range = m_pI->GetAnimRange();
			oHdr.nFrmE	= TicksToMilliSec(range.Duration());
			oHdr.nFrmP	= GetFrameRate();

			oHdr.keyCnt = ProcessBoneAnim (pRoot, range, sampleDelta, boneData);
		
		m_pI->ProgressUpdate(50,FALSE,_T("..writing BONE data..."));

		// writing Bones to file
			ok = ExportBoneData(boneData, oHdr.boneCnt, oHdr.keyCnt, fp);

		// bone data deallocation
		if (boneData) delete [] boneData;
	}

	LONG endPos = ftell(fp);
	fseek(fp, startPos, SEEK_SET);
	fwrite(&oHdr, sizeof(MrcObj), 1, fp);
	fseek(fp, endPos, SEEK_SET);
	
	return ok;
}





// methods used to change coordinate system (then in OpenGL you'll get MAX's Front View)
void LcmExpGL::MAXtoGL(Matrix3 &tm)
{
	// swap 2nd and 3rd rows
	Point3 row;
	row = tm.GetRow(1);
	tm.SetRow(1, tm.GetRow(2));
	tm.SetRow(2, -row);

	// swap 2nd and 3rd columns
	Point4 column = tm.GetColumn(1);
	tm.SetColumn(1, tm.GetColumn(2));
	tm.SetColumn(2, -column);
}


void LcmExpGL::MAXtoGL(Point3 &pnt)
{
	float tmp = pnt.y;
	pnt.y = pnt.z;
	pnt.z = -tmp;
}


void LcmExpGL::MAXtoGL(const Matrix3 &tm, float glTM[16])
{
	Matrix3 maxTM = tm;
	MAXtoGL(maxTM);

	Point3 row;
	row = maxTM.GetRow(0);	glTM[0] = row.x;	glTM[1] = row.y;	glTM[2] = row.z;	glTM[3] = 0.f;
	row = maxTM.GetRow(1);	glTM[4] = row.x;	glTM[5] = row.y;	glTM[6] = row.z;	glTM[7] = 0.f;
	row = maxTM.GetRow(2);	glTM[8] = row.x;	glTM[9] = row.y;	glTM[10] = row.z;	glTM[11] = 0.f;
	row = maxTM.GetRow(3);	glTM[12] = row.x;	glTM[13] = row.y;	glTM[14] = row.z;	glTM[15] = 1.f;
}



int LcmExpGL::TicksToFPS(TimeValue t)
{
	return (TicksToSec(t) * GetFrameRate());
}



BOOL LcmExpGL::IsMesh(INode *pNode)
{
	if(pNode == NULL)
		return FALSE;
	ObjectState os = pNode->EvalWorldState(0); 
	if(os.obj->SuperClassID() == GEOMOBJECT_CLASS_ID)
		return TRUE;

	return FALSE;
}


//test if is a bone or a biped controller
BOOL LcmExpGL::IsBone(INode *pNode)
{
	if(pNode == NULL)
		return FALSE;

	ObjectState os = pNode->EvalWorldState(0);

	if (!os.obj) return FALSE;

	if(os.obj->ClassID() == Class_ID(BONE_CLASS_ID, 0))
		return TRUE;

	if(os.obj->ClassID() == Class_ID(DUMMY_CLASS_ID, 0))
		return FALSE;

	Control *cont = pNode->GetTMController();
	if(cont->ClassID() == BIPSLAVE_CONTROL_CLASS_ID ||		//others biped parts
		cont->ClassID() == BIPBODY_CONTROL_CLASS_ID			//biped root "Bip01"
		
	) return TRUE;

	return FALSE;
}


//BOOL LcmExpGL::IsBipedBone(INode *pNode)
//{
//	if(pNode == NULL || pNode->IsRootNode())
//		return FALSE;
//
//	Control *cont = pNode->GetTMController();
//	if(cont->ClassID() == BIPSLAVE_CONTROL_CLASS_ID ||
//		cont->ClassID() == BIPBODY_CONTROL_CLASS_ID
//	) return TRUE;
//
//	return FALSE;
//}


// Get the number of direct child bones of a node
int LcmExpGL::GetChildBoneCount(INode *pNode)
{
	int count = 0;

	for(int i = 0; i < pNode->NumberOfChildren(); i++)
	{
		if(IsBone(pNode->GetChildNode(i)))
			count++;
	}
	return count;
}


// how many bones in the scene? Pass it the rootNode
int LcmExpGL::CountBones(INode* pNode)
{
	int count = 0;
	if(IsBone(pNode))
		count += 1;

	for(int i = 0; i < pNode->NumberOfChildren(); i++)
		count += CountBones(pNode->GetChildNode(i));
	return count;
}



// used by GetBoneByIndex
BOOL LcmExpGL::BuildIter (INode* pnode, INode** const Iterator, int& currIdx)
{
	
	if(IsBone(pnode)){
		Iterator[currIdx++] = pnode;
	}

	for(int i = 0; i < pnode->NumberOfChildren(); i++)
	{
		BuildIter (pnode->GetChildNode(i),Iterator,currIdx);
	}

	return TRUE;
}


// Get bone pointer from an index, this should get passed the root node
INode* LcmExpGL::GetBoneByIndex(INode* const pRoot, int index)
{
	INode* bone = NULL;
	const int bone_cnt = CountBones(pRoot);
	
	if (index>=bone_cnt) 
		return NULL;

	INode** const Iterator = new INode* [bone_cnt];
	int currIdx=0;
	
		BuildIter(pRoot,Iterator,currIdx);
	
	assert(currIdx==bone_cnt);

	bone = Iterator[index];
	
	assert (GetBoneIndex(pRoot,bone)==index);

	delete [] Iterator;
		
	assert (IsBone(bone));
	return bone;
}



// Recursive iterator to get a bone index, used with GetBoneIndex
int LcmExpGL::RecursiveGetBoneIndex(INode *pRoot, INode *pNodeTest, int &boneCount)
{
	int boneIdx = -1;

	if(IsBone(pRoot))
	{
		boneIdx = boneCount;
		boneCount++;

		if(pRoot == pNodeTest)
			return boneIdx;
	}

	// recurse child nodes
	for(int i = 0; i < pRoot->NumberOfChildren(); i++)
	{
		int boneIdx = RecursiveGetBoneIndex(pRoot->GetChildNode(i), pNodeTest, boneCount);
		if(boneIdx >= 0)
			return boneIdx;
	}

	return -1;
}


// Get an index from a node pointer
int LcmExpGL::GetBoneIndex(INode *pRoot, INode *pNode)
{
	if(!IsBone(pNode))
		return -1;

	int boneCount = 0;
	return RecursiveGetBoneIndex(pRoot, pNode, boneCount);
}



Matrix3 LcmExpGL::GetBoneTM(INode *pNode, TimeValue t)
{
	Matrix3 tm(1);

		tm = pNode->GetNodeTM(t);

	tm.NoScale();
	return tm;
}




Modifier* LcmExpGL::GetPhysiqueMod(INode *pNode)
{
	Object *pObj = pNode->GetObjectRef();

	if(!pObj)
		return NULL;

	// Is derived object ?
	while(pObj->SuperClassID() == GEN_DERIVOB_CLASS_ID)
	{
		// Yes -> Cast.
		IDerivedObject *pDerivedObj = static_cast<IDerivedObject*>(pObj);

		// Iterate over all entries of the modifier stack.
		int ModStackIndex = 0;
		while(ModStackIndex < pDerivedObj->NumModifiers())
		{
			// Get current modifier.
			Modifier* pMod = pDerivedObj->GetModifier(ModStackIndex);

			// Is this Physique ?
			if(pMod->ClassID() == Class_ID(PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B))
				return pMod;

			// Next modifier stack entry.
			ModStackIndex++;

		}
		pObj = pDerivedObj->GetObjRef();
	}
	// Not found.
	return NULL;
}


Modifier* LcmExpGL::GetSkinMod(INode *pNode)
{
	Object*	pObj = pNode->GetObjectRef();
	
	if(!pObj)
		return NULL;
	
	// Is derived object ?
	while(pObj->SuperClassID() == GEN_DERIVOB_CLASS_ID)
	{
		// Yes -> Cast.
		IDerivedObject *pDerObj = static_cast<IDerivedObject*>(pObj);

		// Iterate over all entries of the modifier stack.
		int ModStackIndex = 0;
		while (ModStackIndex < pDerObj->NumModifiers())
		{
			// Get current modifier.
			Modifier* mod = pDerObj->GetModifier(ModStackIndex);
			
			// Is this Skin ?
			if(mod->ClassID() == SKIN_CLASSID)
			{
				// Yes -> Exit.
				return mod;
			}
			
			// Next modifier stack entry.
			ModStackIndex++;
		}
		pObj = pDerObj->GetObjRef();
	}
	
	// Not found.
	return NULL;
}


BOOL LcmExpGL::ProcessBoneWeights (INode *pNode,INode *pRoot, MrcLink* BD)
{
	if ((!pNode)||(!IsMesh(pNode)))
		return FALSE;
			 
	
	Modifier *pMod = GetPhysiqueMod(pNode);

	if(!pMod)
	{
/*		pMod = GetSkinMod(pNode);
		if ( (!pMod) ||
			 (!GetSkinWeights(pNode,pRoot, pMod, BD)) )
*/			return FALSE;
	}
	else
			GetPhysiqueWeights(pNode, pRoot, pMod, BD);

	return TRUE;
}



int LcmExpGL::ProcessBoneStruct(INode *pNode, INode *pRoot,int parentIdx, MrcLink* BD)
{
	//test if the node is a bone		
	if (IsBone(pNode)) 
	{	
		int currIdx = GetBoneIndex (pRoot, pNode);

		assert(-1!=currIdx);
		
		//set bone header data
		MrcBone &bHdr = BD[currIdx].Hdr;
		// get the bones inverse base matrix at time 0		
		Matrix3 tm = GetBoneTM(pNode, 0);
		tm.Invert();

		MAXtoGL(tm, bHdr.inverseOrientationTM);
		bHdr.parentIdx = parentIdx;
		bHdr.childCnt = GetChildBoneCount(pNode);

		//set child's index vector
		if(bHdr.childCnt > 0)
		{
			BD[currIdx].childsVect.reserve(bHdr.childCnt);
			for (int i=0; i<pNode->NumberOfChildren(); ++i)
			{
				int chIdx = ProcessBoneStruct(pNode->GetChildNode(i),pRoot,currIdx, BD);		
				if (0<=chIdx)
					BD[currIdx].childsVect.push_back(chIdx);
			}
		}
		assert (BD[currIdx].childsVect.size() == BD[currIdx].Hdr.childCnt);
		return currIdx;
	}
	else 
	{
		for (int i=0; i<pNode->NumberOfChildren(); ++i)
			ProcessBoneStruct(pNode->GetChildNode(i),pRoot,-1, BD);
		return -1;
	}
}	


int LcmExpGL::ProcessBoneAnim (INode *pRoot, Interval range, ULONG sampleD, MrcLink* BD)
{
	int keyCnt=0;
	int totbones = CountBones(pRoot);
	const ULONG start = TicksToMilliSec(range.Start());
	const ULONG end   = TicksToMilliSec(range.End());
		
	if (!totbones) return 0;

	// cycle throught every bone
	for (int idx=0; idx<totbones; ++idx)
	{
		// get bone node pointer
		INode* pBone = GetBoneByIndex(pRoot, idx);
			assert (IsBone(pBone));

		ULONG msec=0;
		//BD[idx].keysVect.reserve(end/sampleD);
		// grab keys for this bone
		for(msec = start; msec < end+sampleD; msec += sampleD)
		{	
			MrcKey KeyHdr;
			memset(&KeyHdr, 0, sizeof(MrcKey));
			
			//grab key time (in millisecs)
			if(msec > end)
				KeyHdr.t = end;
			else
				KeyHdr.t = msec;

			//grab bone animation matrix at time t
			Matrix3 tm;
			TimeValue t;
			t = MilliSecToTicks(msec);
			tm = GetBoneTM(pBone, t);		
			MAXtoGL(tm);

			// grab translation
			Point3 pos = tm.GetTrans();		
			KeyHdr.p[0] = pos.x;
			KeyHdr.p[1] = pos.y;
			KeyHdr.p[2] = pos.z;

			// grab rotation union quaternion
			Quat quat(tm);
			quat.Normalize();
			KeyHdr.q[0] = quat.x;
			KeyHdr.q[1] = quat.y;
			KeyHdr.q[2] = quat.z;
			KeyHdr.q[3] = quat.w;

			BD[idx].keysVect.push_back(KeyHdr);
		}
	}
	keyCnt = BD[0].keysVect.size();
	return keyCnt;
}




BOOL LcmExpGL::GetPhysiqueWeights(INode *pNode, INode *pRoot, Modifier *pMod, MrcLink *BD)
{
	// create a Physique Export Interface for the given Physique Modifier
	IPhysiqueExport *phyInterface = (IPhysiqueExport*)pMod->GetInterface(I_PHYINTERFACE);
	
	if(phyInterface)
	{
		// create a ModContext Export Interface for the specific node of the Physique Modifier
		IPhyContextExport *modContextInt = (IPhyContextExport*)phyInterface->GetContextInterface(pNode);
		// needed by vertex interface (only Rigid one supported for now)
		modContextInt->ConvertToRigid(TRUE);
		// more than a single bone per vertex
		modContextInt->AllowBlending(TRUE);

		if(modContextInt)
		{
			int totalVtx = modContextInt->GetNumberVertices();
			for(int i = 0; i < totalVtx; i++)
			{
				//get i-th vertex
				IPhyVertexExport *vtxInterface = (IPhyVertexExport*)modContextInt->GetVertexInterface(i);
				if(vtxInterface)
				{
					int vtxType = vtxInterface->GetVertexType();
					if(vtxType == RIGID_TYPE)
					{
						//get bone
						INode *boneNode = ((IPhyRigidVertex*)vtxInterface)->GetNode();
						int boneIdx = GetBoneIndex(pRoot, boneNode);

						//Build vertex data
						MrcWgt wdata;
						wdata.vertIdx = i;
						wdata.weight = 1.0f;
						//Insert into proper bonedata
						BD[boneIdx].weightsVect.push_back(wdata);
						//update vertexWeightCnt for that bone
						BD[boneIdx].Hdr.vertexCnt = BD[boneIdx].weightsVect.size();
					}
					else if(vtxType == RIGID_BLENDED_TYPE)
					{
						IPhyBlendedRigidVertex *vtxBlendedInt = (IPhyBlendedRigidVertex*)vtxInterface;
						for(int j = 0; j < vtxBlendedInt->GetNumberNodes(); j++)
						{
							//get j-th bone
							INode *boneNode = vtxBlendedInt->GetNode(j);
							//whose index is boneIdx
							int boneIdx = GetBoneIndex(pRoot, boneNode);
	
							//Build vertex data
							MrcWgt wdata;
							wdata.vertIdx = i;
							wdata.weight = vtxBlendedInt->GetWeight(j);
							
							//check vertex existence for this bone
							BOOL notfound = TRUE;

							for (int v=0; notfound && v<BD[boneIdx].weightsVect.size();v++)
							{
								//update found vertex weight data for that bone
								if (BD[boneIdx].weightsVect[v].vertIdx == wdata.vertIdx)
								{
									BD[boneIdx].weightsVect[v].weight += wdata.weight;
									notfound = FALSE;
								}
							}

							if (notfound)
							{
								//Add a new vertex weight data into proper bonedata
								BD[boneIdx].weightsVect.push_back(wdata);
								//update vertexWeightCnt for that bone
								BD[boneIdx].Hdr.vertexCnt = BD[boneIdx].weightsVect.size();
							}
						}
					}
				}
			}
			phyInterface->ReleaseContextInterface(modContextInt);
		}
		pMod->ReleaseInterface(I_PHYINTERFACE, phyInterface);
	}

	return TRUE;
}



// export to MRC file every BoneData entry
BOOL LcmExpGL::ExportBoneData(MrcLink *BD,int boneCnt, int keyCnt, FILE *out)
{	
// write weights in bone major format
	//i-th bone
	for(int i = 0; i < boneCnt; i++)
	{
		LONG BoneHdrPos = ftell(out);

		fseek(out, sizeof(MrcBone), SEEK_CUR);
		
		// write the child bone index list
		for (int j=0; j<BD[i].Hdr.childCnt; j++)
		{			
			fwrite(&BD[i].childsVect[j], sizeof(int), 1, out);
		}			
		BD[i].Hdr.boneWeightsOfs = ftell(out);

		//BoneWeight data export
		int weightCnt = BD[i].Hdr.vertexCnt;
		
		//write data for j.th vertex who belongs to i-th bone
		for( j = 0; j < weightCnt; j++)
		{
			MrcWgt &vertWeight = BD[i].weightsVect[j];
			fwrite(&vertWeight, sizeof(MrcWgt), 1, out);
		}
		BD[i].Hdr.boneKeysOfs = ftell(out);
				
		//BoneANIMATION data export
		for( j = 0; j < keyCnt; j++)
		{
			MrcKey &boneKey = BD[i].keysVect[j];
			fwrite(&boneKey, sizeof(MrcKey), 1, out);
		}

		LONG endPos = ftell(out);
		// write down bone header
		fseek(out, BoneHdrPos, SEEK_SET);
		fwrite(&BD[i].Hdr, sizeof(MrcBone), 1, out);
		fseek(out, endPos, SEEK_SET);
	}
	return TRUE;
}


static NullView nullView;

// get and export mesh data to file
BOOL LcmExpGL::ExportMesh (INode* pNode, FILE *out)
{
	if ((!pNode)||(!IsMesh(pNode)))
		return FALSE;

	MrcMesh mHdr;
	memset(&mHdr, 0, sizeof(MrcMesh));

	Matrix3 tm = pNode->GetObjectTM(0);
	ObjectState os = pNode->EvalWorldState(0);
	int needDelete;
	Mesh& mesh = *(( (GeomObject*) os.obj)->GetRenderMesh(0,pNode,nullView,needDelete));
	
	LONG startPos = ftell(out);
	fseek(out, sizeof(MrcMesh), SEEK_CUR);

	// write the mesh vertices
	mHdr.vertCnt = mesh.getNumVerts();
	mHdr.vertOfs = ftell(out);
	for(int i = 0; i < mHdr.vertCnt; i++)
	{
		Point3 pnt = mesh.getVert(i) * tm;			//premultiply in MAX
		MAXtoGL(pnt);								//useful coordinate transformation
		fwrite(&pnt.x, sizeof(float), 3, out);
	}

	// write vertex normals
	mesh.buildNormals();
	mHdr.normCnt = mesh.getNumVerts();
	mHdr.normOfs = ftell(out);
	for(i = 0; i < mHdr.normCnt; i++)
	{ 
		// normals are taken from a unique smoothing group
		Point3 norm = Normalize(mesh.getNormal(i));
		MAXtoGL(norm);
		fwrite(&norm.x, sizeof(float), 3, out);
	}

	// build and write faces
	mHdr.faceCnt = mesh.getNumFaces();
	mHdr.faceOfs = ftell(out);
	for(i = 0; i < mHdr.faceCnt; i++)
	{
		MrcFace fHdr;
		memset(&fHdr, 0, sizeof(MrcFace));

		fHdr.vert[0] = mesh.faces[i].v[0];
		fHdr.vert[1] = mesh.faces[i].v[1];
		fHdr.vert[2] = mesh.faces[i].v[2];
		// TODO: insert normals
		fwrite(&fHdr, sizeof(MrcFace), 1, out);
	}

	LONG endPos = ftell(out);
	fseek(out, startPos, SEEK_SET);
	fwrite(&mHdr, sizeof(MrcMesh), 1, out);
	fseek(out, endPos, SEEK_SET);
	return TRUE;
}



