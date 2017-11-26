// Implementation of the CLcXmsh class.
//
////////////////////////////////////////////////////////////////////////////////

#if _MSC_VER >= 1200
#pragma warning(disable : 4996)
#endif

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>

#include <d3d9.h>
#include <d3dx9.h>


#include "ILcXmsh.h"
#include "LcXmsh.h"

#include "ILcM3d.h"
#include "LcVtx.h"



#ifndef SAFE_DELETE
#	define SAFE_DELETE(p)	{ if(p) { delete (p);     (p)=NULL; } }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p){ if(p) { delete[] (p);   (p)=NULL; } }
#endif

#ifndef SAFE_RELEASE
#	define SAFE_RELEASE(p)  { if(p) { (p)->Release(); (p)=NULL; } }
#endif

#ifndef SAFE_FREE
#	define SAFE_FREE(p)  { if(p) { free(p); (p)=NULL; } }
#endif




CLcXmsh::LcXmsh::LcXmsh()
{
	pVB		= NULL;
	pIB		= NULL;
	pTx		= NULL;

	fmFc	= 0;
	fmVx	= 0;

	stFc	= 0;
	stVx	= 0;

	nFce	= 0;
	nVtx	= 0;
	

	memset(&Mtrl, 0, sizeof Mtrl);
	memset(sTxF, 0, sizeof sTxF);
}

CLcXmsh::LcXmsh::~LcXmsh()
{
	SAFE_FREE(	pVB		);
	SAFE_FREE(	pIB		);

	if(	pTx	)
	{
		LPDIRECT3DTEXTURE9 _pTxt = (LPDIRECT3DTEXTURE9)pTx;
		_pTxt->Release();
		pTx = NULL;
	}
}


CLcXmsh::CLcXmsh()
{
	m_pDev	= NULL;

	m_fmFc	= 0;
	m_fmVx	= 0;

	m_stFc	= 0;
	m_stVx	= 0;

	m_nFce	= 0;
	m_nVtx	= 0;

	m_nGeo	= 0;
	m_pGeo	= NULL;

	m_bMtl	= TRUE;
	m_bTxLoad = FALSE;
}

CLcXmsh::~CLcXmsh()
{
	Destroy();
}

void CLcXmsh::Destroy()
{
	// �ܺο��� �ؽ�ó �����͸� �������� ���
	if(!m_bTxLoad)
	{
		for(int i=0; i<m_nGeo; ++i)
		{
			m_pGeo[i].pTx = NULL;
		}
	}

	SAFE_DELETE_ARRAY(	m_pGeo	);


	m_fmFc	= 0;
	m_fmVx	= 0;

	m_stFc	= 0;
	m_stVx	= 0;

	m_nFce	= 0;
	m_nVtx	= 0;

	m_nGeo	= 0;

	m_bMtl	= TRUE;
}


INT CLcXmsh::Create(void* p1, void* p2, void* p3, void* p4)
{
	char	sMsh[MAX_PATH];

	m_pDev =(LPDIRECT3DDEVICE9)p1;
	strcpy(sMsh, (char*)p2);

	if(!strlen(sMsh))
		return -1;

	HRESULT hr = 0;
	
	ID3DXBuffer*	pBufAdj	= NULL;
	ID3DXBuffer*	pBufMtl = NULL;
	D3DXMATERIAL*	pMtrls	= NULL;
	DWORD			nMtrls	= 0;
	DWORD			n=0;

	ID3DXMesh*		pDxMesh = NULL;

	
	if(FAILED(hr = D3DXLoadMeshFromX(
		sMsh
		, D3DXMESH_MANAGED
		, m_pDev
		, &pBufAdj
		, &pBufMtl
		, 0
		, &nMtrls
		, &pDxMesh)))
	{
		return -1;
	}

	
	if( pBufMtl && nMtrls)
	{
		m_pGeo	= new CLcXmsh::LcXmsh[nMtrls];

		pMtrls	= (D3DXMATERIAL*)pBufMtl->GetBufferPointer();
		
		for(n = 0; n < nMtrls; ++n)
		{
			D3DXCOLOR	dxAmbient = pMtrls[n].MatD3D.Diffuse;					// ��Ƽ������ �ε��� �� Ambient ���� ���� ������ ������ ���� (�ɼ�)
			dxAmbient *=0.8f;

			pMtrls[n].MatD3D.Ambient =  dxAmbient;


			memcpy(&(m_pGeo[n].Mtrl), &(pMtrls[n].MatD3D), sizeof m_pGeo[n].Mtrl);	// ��Ƽ���� ����

			
			if( pMtrls[n].pTextureFilename)										// ��Ƽ������ �ؽ��縦 ������ �ִ°�?
			{
				char	sTx[MAX_PATH]={0};

				if(strlen(pMtrls[n].pTextureFilename))
				{
					strcpy(m_pGeo[n].sTxF, pMtrls[n].pTextureFilename);

//					sprintf(sTx, "Texture/%s", m_pGeo[n].sTxF);
//
//
//					LPDIRECT3DTEXTURE9	pTx = NULL;
//
//					if(SUCCEEDED( D3DXCreateTextureFromFile(
//						m_pDev,
//						sTx,
//						&pTx)))
//					{
//						m_pGeo[n].pTx = pTx;													// �ؽ��� �����͸� ����.
//					}
				}

			}

		}
	}
	
	SAFE_RELEASE(pBufMtl);														// ��Ƽ���� ���� ����

	DWORD*	d = (DWORD*)pBufAdj->GetBufferPointer();
	
	if(FAILED(hr = pDxMesh->OptimizeInplace(										//  �޽��� ����ȭ �Ѵ�.
		D3DXMESHOPT_ATTRSORT |													// �ε����� ���ؽ��� ��Ƽ���󿡵��� �����ϵ��� ������.
		D3DXMESHOPT_COMPACT  |													// �ʿ� ���� �ε����� ���ؽ��� �����͸� �����.
		D3DXMESHOPT_VERTEXCACHE,												// ĳ�� �������� ���δ�.
		d,
		0, 0, 0)))
	{
		return -1;
	}
	
	SAFE_RELEASE(pBufAdj);														// adjecence Buffer����


	// VertexBuffer Create
	D3DINDEXBUFFER_DESC		dscIB;
	D3DVERTEXBUFFER_DESC	dscVB;

	LPDIRECT3DINDEXBUFFER9	pSrcIB;
	LPDIRECT3DVERTEXBUFFER9	pSrcVB;

	pDxMesh->GetVertexBuffer(&pSrcVB);
	pDxMesh->GetIndexBuffer(&pSrcIB);

	pSrcIB->GetDesc(&dscIB);
	pSrcVB->GetDesc(&dscVB);

	m_fmFc	= dscIB.Format;														// Face Format
	m_fmVx	= pDxMesh->GetFVF();													// Format of vertex
	m_stFc	= (D3DFMT_INDEX16 == m_fmFc)? sizeof(WORD)*3 : sizeof(DWORD)*3;		// Stride of Face
	m_stVx	= D3DXGetFVFVertexSize(m_fmVx);										// Stride of Vertex
	m_nFce	= pDxMesh->GetNumFaces();
	m_nVtx	= pDxMesh->GetNumVertices();
	m_nGeo	= nMtrls;

	INT			i=0, j=0;
	
	char*		pIdxS = NULL;
	char*		pVtxS = NULL;

	char*		pIdxD = NULL;
	char*		pVtxD = NULL;

	DWORD		dAttr  =0;
	D3DXATTRIBUTERANGE*		pGeoT = NULL;

	
	
	hr = pDxMesh->GetAttributeTable( NULL, &dAttr);

	if(!m_nGeo || dAttr<=0)
		return -1;
	
	pGeoT	= new D3DXATTRIBUTERANGE[dAttr];
	pDxMesh->GetAttributeTable(pGeoT, &dAttr);
	
	pSrcIB->Lock(0, 0, (void**)&pIdxS, 0);
	pSrcVB->Lock(0, 0, (void**)&pVtxS, 0);
	
	for(i = 0; i < m_nGeo; ++i)
	{
		void*	pIB = NULL;
		void*	pVB = NULL;

		INT		nFce	= pGeoT[i].FaceCount;
		INT		nVtx	= pGeoT[i].VertexCount;
		INT		nBgnFce	= pGeoT[i].FaceStart;
		INT		nBgnVtx	= pGeoT[i].VertexStart;
		
		INT		nFceSize= m_stFc;
		INT		nVtxSize= m_stVx;
		
		pIB = malloc(  nFceSize * nFce);
		pVB = malloc(  nVtxSize * nVtx);
		
		pIdxD = (char*)pIB;
		

		if(D3DFMT_INDEX16 == m_fmFc)
		{
			CLcXmsh::VtxIdx16* pStd = (CLcXmsh::VtxIdx16*)pIdxS + nBgnFce;
			
			memcpy(pIdxD, pStd, nFce * nFceSize);
			
			for(j=0; j<nFce; ++j)
			{
				((CLcXmsh::VtxIdx16*)pIdxD)[j].a -= pStd->a;
				((CLcXmsh::VtxIdx16*)pIdxD)[j].b -= pStd->a;
				((CLcXmsh::VtxIdx16*)pIdxD)[j].c -= pStd->a;
			}
			
		}

		else if(D3DFMT_INDEX32 == m_fmFc)
		{
			CLcXmsh::VtxIdx32* pStd = (CLcXmsh::VtxIdx32*)pIdxS + nBgnFce;
			
			memcpy(pIdxD, pStd, nFce * nFceSize);
			
			for(j=0; j<nFce; ++j)
			{
				((CLcXmsh::VtxIdx32*)pIdxD)[j].a -= pStd->a;
				((CLcXmsh::VtxIdx32*)pIdxD)[j].b -= pStd->a;
				((CLcXmsh::VtxIdx32*)pIdxD)[j].c -= pStd->a;
			}
		}
		
		
		pVtxD = (char*)pVB;
		memcpy(pVtxD, pVtxS + nBgnVtx * nVtxSize, nVtx * nVtxSize);


		m_pGeo[i].fmFc	= m_fmFc;												// Format of Face
		m_pGeo[i].fmVx	= m_fmVx;												// Format of vertex

		m_pGeo[i].stFc	= m_stFc;												// Stride of Face
		m_pGeo[i].stVx	= m_stVx;												// Stride of Vertex

		m_pGeo[i].nFce	= nFce;
		m_pGeo[i].nVtx	= nVtx;
		
		m_pGeo[i].nFce	= nFce;
		m_pGeo[i].nVtx	= nVtx;
		
		m_pGeo[i].pIB	= pIB;
		m_pGeo[i].pVB	= pVB;

		m_nFce	+= nFce;
		m_nVtx	+= nVtx;
	}
	
	pSrcVB->Unlock();
	pSrcIB->Unlock();

	SAFE_DELETE_ARRAY(	pGeoT	);

	// Geometry�� ����� ���� ����

	SAFE_RELEASE(	pSrcVB	);
	SAFE_RELEASE(	pSrcIB	);
	SAFE_RELEASE(	pDxMesh	);

	return 0;
}


INT CLcXmsh::FrameMove()
{
	return 0;
}


void CLcXmsh::Render()
{
	for(int i = 0; i < m_nGeo; ++i)
	{
		if(m_bMtl)
			m_pDev->SetMaterial( &m_pGeo[i].Mtrl);

		LPDIRECT3DTEXTURE9	pTx = (LPDIRECT3DTEXTURE9)m_pGeo[i].pTx;

		m_pDev->SetFVF(m_pGeo[i].fmVx);
		m_pDev->SetTexture(0, pTx);

		m_pDev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST				// D3DPRIMITIVETYPE
										, 0								// Minimum vertex index
										, m_pGeo[i].nVtx				// Number of vertices
										, m_pGeo[i].nFce				// Number of primitives
										, m_pGeo[i].pIB					// User memory pointer to the index data
										, (D3DFORMAT)m_pGeo[i].fmFc		// IndexDataFormat
										, m_pGeo[i].pVB					// User memory pointer to vertex data
										, m_pGeo[i].stVx				// Stride between data for each vertex
										);
	}
}



INT CLcXmsh::Query(char* sCmd, void* pData)
{
	if(0==_stricmp("Use Material", sCmd))
	{
		BOOL bVal	= (BOOL)pData;
		m_bMtl		= bVal;
		
		return 0;
	}


	else if(0==_stricmp("Load Texture", sCmd))
	{
		char*	sFile=(char*)pData;

		if(FAILED(LoadTexture(sFile)))
			return -1;

		m_bTxLoad = TRUE;
		return 0;
	}

	else if(0==_stricmp("Get Texture Name", sCmd))
	{
		INT nIdx = *((INT*)pData);
		strcpy((char*)pData, m_pGeo[nIdx].sTxF);
		return 0;
	}

	else if(0==_stricmp("Set Texture Pointer", sCmd))
	{
		struct _T {	int n; void* p;	};

		_T*	t = (_T*)pData;

		INT		n = t->n;
		void*	p = t->p;
		m_pGeo[n].pTx = p;

		return 0;
	}

	else if(0==_stricmp("Number Geometry", sCmd))
	{
		*((INT*)pData)	= m_nGeo;
		return 0;
	}

	else if(0==_stricmp("Get FVF", sCmd))
	{
		*((DWORD*)pData)	= 0;

		if(!m_pGeo)
			return -1;

		*((DWORD*)pData)	= m_pGeo[0].fmVx;

		return 0;
	}

	else if(0==_stricmp("Get Vertex Number", sCmd))
	{
		if(!m_pGeo)
		{
			*((INT*)pData) = 0;
			return -1;
		}

		INT	nVtx = 0;
		INT	nIdx = *((INT*)pData);

		if(-1 == nIdx)		// �ε����� -1�̸� ��� ���ؽ� ���� ���ؼ� ������.
		{
			for(int i=0; i<m_nGeo; ++i)
				nVtx += m_pGeo[i].nVtx;
		}
		else				// �ε����� �ش��ϴ� ������Ʈ���� ���ؽ� ���ڸ� ������.
		{
			nVtx = m_pGeo[nIdx].nVtx;
		}

		*((INT*)pData)	= nVtx;

		return 0;
	}

	else if(0==_stricmp("Get Vertex Buffer", sCmd))
	{
		char* pVB = NULL;

		*((DWORD*)pData)	= 0;

		if(!m_pGeo)
			return -1;

		pVB = GetVertexBuffer();

		*((char**)pData)	= pVB;

		return 0;
	}

	else if(0==_stricmp("Set Device", sCmd))
	{
		m_pDev	=(LPDIRECT3DDEVICE9)pData;
		return 0;
	}
	
	else if(0==_stricmp("Save LcXmsh", sCmd))
	{
		char*	sFile=(char*)pData;

		return SaveLcXmsh(sFile);
	}

	else if(0==_stricmp("Open LcXmsh", sCmd))
	{
		char*	sFile=(char*)pData;

		return OpenLcXmsh(NULL, sFile);
	}
	

	return -1;
}




INT CLcXmsh::SaveLcXmsh(char* sFile)
{
	INT		n		= 0;
	void*	pIdxS	= NULL;
	void*	pVtxS	= NULL;
	FILE*	fp		= NULL;

	if(m_nGeo<=0)
		return -1;

	fp = fopen(sFile, "wb");


	char	sType[128]="LpBinaryFile";
	char	sVer [ 32]="00.10.00";
	INT		nGeo = m_nGeo;


	fwrite(sType, 1, 128, fp);
	fwrite(sVer,  1,  32, fp);
	fwrite(&nGeo, sizeof(INT), 1, fp);

	fseek(fp, 1024, SEEK_SET);

	for(INT i=0; i<m_nGeo; ++i)
	{
		n = i;
		fwrite(&n, sizeof(INT), 1, fp);								// Index

		fwrite(&m_pGeo[n].Mtrl.Diffuse	, sizeof(float), 4, fp);	// Material
		fwrite(&m_pGeo[n].Mtrl.Ambient	, sizeof(float), 4, fp);
		fwrite(&m_pGeo[n].Mtrl.Specular	, sizeof(float), 4, fp);
		fwrite(&m_pGeo[n].Mtrl.Emissive	, sizeof(float), 4, fp);
		fwrite(&m_pGeo[n].Mtrl.Power	, sizeof(float), 1, fp);
		
		fwrite(m_pGeo[n].sTxF, sizeof(char), 260, fp);				// Texture FileName

		fwrite(&m_pGeo[n].fmFc , sizeof(DWORD), 1, fp);				// Format of Face
		fwrite(&m_pGeo[n].fmVx , sizeof(DWORD), 1, fp);				// Format of vertex
		fwrite(&m_pGeo[n].stFc , sizeof(UINT ), 1, fp);				// Stride of Face
		fwrite(&m_pGeo[n].stVx , sizeof(UINT ), 1, fp);				// Stride of Vertex
		fwrite(&m_pGeo[n].nFce , sizeof(UINT ), 1, fp);				// face number
		fwrite(&m_pGeo[n].nVtx , sizeof(UINT ), 1, fp);				// vertex number

		pIdxS = m_pGeo[n].pIB;
		fwrite(pIdxS, m_pGeo[n].stFc, m_pGeo[n].nFce, fp);			// Write faces
		
		pVtxS = m_pGeo[n].pVB;
		fwrite(pVtxS, m_pGeo[n].stVx, m_pGeo[n].nVtx, fp);			// Write vertices
	}


	fclose(fp);
	
	return 0;
}


INT CLcXmsh::OpenLcXmsh(void* pDev, char* sFile)
{
	INT		n = 0;
	void*	pIdxS = NULL;
	void*	pVtxS = NULL;

	LPDIRECT3DVERTEXBUFFER9	pVB = NULL;
	LPDIRECT3DINDEXBUFFER9	pIB = NULL;

	if(pDev)
		m_pDev	= (LPDIRECT3DDEVICE9)pDev;

	
	FILE*	fp = NULL;

	fp = fopen(sFile, "rb");

	if(!fp)
		return -1;


	char	sType[128]={0};
	char	sVer [ 32]={0};
	INT		nGeo =0;


	fread(sType, 1, 128, fp);
	fread(sVer,  1,  32, fp);
	fread(&nGeo, sizeof(INT), 1, fp);

	m_nGeo	= nGeo;
	m_pGeo	= new CLcXmsh::LcXmsh[nGeo];

	fseek(fp, 1024, SEEK_SET);

	for(INT i=0; i<m_nGeo; ++i)
	{
		fread(&n, sizeof(INT), 1, fp);								// Index

		fread(&m_pGeo[n].Mtrl.Diffuse	, sizeof(float), 4, fp);	// Material
		fread(&m_pGeo[n].Mtrl.Ambient	, sizeof(float), 4, fp);
		fread(&m_pGeo[n].Mtrl.Specular	, sizeof(float), 4, fp);
		fread(&m_pGeo[n].Mtrl.Emissive	, sizeof(float), 4, fp);
		fread(&m_pGeo[n].Mtrl.Power		, sizeof(float), 1, fp);
		
		fread(m_pGeo[n].sTxF, sizeof(char), 260, fp);				// Texture FileName

		fread(&m_pGeo[n].fmFc , sizeof(DWORD), 1, fp);				// Format of Face
		fread(&m_pGeo[n].fmVx , sizeof(DWORD), 1, fp);				// Format of vertex
		fread(&m_pGeo[n].stFc , sizeof(UINT ), 1, fp);				// Stride of Face
		fread(&m_pGeo[n].stVx , sizeof(UINT ), 1, fp);				// Stride of Vertex
		fread(&m_pGeo[n].nFce , sizeof(UINT ), 1, fp);				// face number
		fread(&m_pGeo[n].nVtx , sizeof(UINT ), 1, fp);				// vertex number

		m_pGeo[n].pIB = malloc(m_pGeo[n].stFc * m_pGeo[n].nFce);
		m_pGeo[n].pVB = malloc(m_pGeo[n].stVx * m_pGeo[n].nVtx);

		pIdxS = m_pGeo[n].pIB;
		fread(pIdxS, m_pGeo[n].stFc, m_pGeo[n].nFce, fp);		// Write faces

		pVtxS = m_pGeo[n].pVB;
		fread(pVtxS, m_pGeo[n].stVx, m_pGeo[n].nVtx, fp);		// Write vertices
	}

	fclose(fp);

	return 0;
}


INT CLcXmsh::LoadTexture(char* sTexPath)
{
	if( m_nGeo <=0)
		return -1;

	char	sPath[MAX_PATH]={0};
	char	sTx[MAX_PATH];

	strcpy(sPath, sTexPath);

	for(int n=0; n<m_nGeo; ++n)
	{
		if(strlen(m_pGeo[n].sTxF))												// ��Ƽ������ �ؽ��縦 ������ �ִ°�?
		{
			LPDIRECT3DTEXTURE9	pTx = NULL;

			sprintf(sTx, "%s/%s", sPath, m_pGeo[n].sTxF);

			if(SUCCEEDED( D3DXCreateTextureFromFile(
				m_pDev,
				sTx,
				&pTx)))
			{
				m_pGeo[n].pTx = pTx;													// �ؽ��� �����͸� ����.
			}
		}

	}

	return 0;
}



char* CLcXmsh::GetVertexBuffer()
{
	INT	n=0;
	INT	nVtx = 0;
	char*	pVtxS = NULL;
	char*	pVtxD = NULL;

	D3DPOOL d3dPoolVtx = D3DPOOL_MANAGED;

	for(n=0; n<m_nGeo; ++n)
		nVtx += m_pGeo[n].nVtx;
	
	pVtxD = (char*)malloc( m_pGeo[0].stVx * nVtx);

	

	INT	nPos=0;
	INT iSize=0;
		
	for(n=0; n<m_nGeo; ++n)
	{
		pVtxS = (char*)m_pGeo[n].pVB;

		iSize = m_pGeo[n].nVtx * m_pGeo[n].stVx;

		memcpy(pVtxD + nPos, pVtxS, iSize);		// Vertex Copy

		nPos += iSize;
	}		

	return pVtxD;
}


INT CLcXmsh::Lock(UINT nGeo, UINT OffsetToLock, UINT SizeToLock, void** ppbData)
{
	if(!m_pGeo[nGeo].pVB)
		return -1;

	if(ppbData)
		*ppbData = m_pGeo[nGeo].pVB;

	return 0;
}

INT CLcXmsh::Unlock(UINT nGeo)
{
	return 0;
}


void CLcXmsh::SetPos(INT nGeo, INT nIdx, FLOAT X, FLOAT Y, FLOAT Z, FLOAT W)
{
	if(!m_pGeo[nGeo].pVB)
		return;

	DWORD	dFVF = m_pGeo[nGeo].fmVx;

	if(LcVtx::VtxD::FVF == dFVF)
	{
		LcVtx::VtxD* pVtx = (LcVtx::VtxD*)m_pGeo[nGeo].pVB;
		pVtx[nIdx].p[0] = X;
		pVtx[nIdx].p[1] = Y;
		pVtx[nIdx].p[2] = Z;
	}

	else if(LcVtx::VtxDUV1::FVF == dFVF)
	{
		LcVtx::VtxDUV1* pVtx = (LcVtx::VtxDUV1*)m_pGeo[nGeo].pVB;
		pVtx[nIdx].p[0] = X;
		pVtx[nIdx].p[1] = Y;
		pVtx[nIdx].p[2] = Z;
	}

	else if(LcVtx::VtxN::FVF == dFVF)
	{
		LcVtx::VtxN* pVtx = (LcVtx::VtxN*)m_pGeo[nGeo].pVB;
		pVtx[nIdx].p[0] = X;
		pVtx[nIdx].p[1] = Y;
		pVtx[nIdx].p[2] = Z;
	}
	
	else if(LcVtx::VtxUV1::FVF == dFVF)
	{
		LcVtx::VtxUV1* pVtx = (LcVtx::VtxUV1*)m_pGeo[nGeo].pVB;
		pVtx[nIdx].p[0] = X;
		pVtx[nIdx].p[1] = Y;
		pVtx[nIdx].p[2] = Z;
	}

	else if(LcVtx::VtxNUV1::FVF == dFVF)
	{
		LcVtx::VtxNUV1* pVtx = (LcVtx::VtxNUV1*)m_pGeo[nGeo].pVB;
		pVtx[nIdx].p[0] = X;
		pVtx[nIdx].p[1] = Y;
		pVtx[nIdx].p[2] = Z;
	}

	else if(LcVtx::VtxNDUV1::FVF == dFVF)
	{
		LcVtx::VtxNDUV1* pVtx = (LcVtx::VtxNDUV1*)m_pGeo[nGeo].pVB;
		pVtx[nIdx].p[0] = X;
		pVtx[nIdx].p[1] = Y;
		pVtx[nIdx].p[2] = Z;
	}


	else if(LcVtx::VtxNDUV3::FVF == dFVF)
	{
		LcVtx::VtxNDUV3* pVtx = (LcVtx::VtxNDUV3*)m_pGeo[nGeo].pVB;
		pVtx[nIdx].p[0] = X;
		pVtx[nIdx].p[1] = Y;
		pVtx[nIdx].p[2] = Z;
	}


	else if(LcVtx::VtxRHWD::FVF == dFVF)
	{
		LcVtx::VtxRHWD* pVtx = (LcVtx::VtxRHWD*)m_pGeo[nGeo].pVB;
		pVtx[nIdx].p[0] = X;
		pVtx[nIdx].p[1] = Y;
		pVtx[nIdx].p[2] = Z;
		pVtx[nIdx].p[3] = W;
	}
}


void CLcXmsh::SetNormal(INT nGeo, INT nIdx, FLOAT X, FLOAT Y, FLOAT Z)
{
	if(!m_pGeo[nGeo].pVB)
		return;

	DWORD	dFVF = m_pGeo[nGeo].fmVx;

	if(LcVtx::VtxN::FVF == dFVF)
	{
		LcVtx::VtxN* pVtx = (LcVtx::VtxN*)m_pGeo[nGeo].pVB;
		pVtx[nIdx].n[0] = X;
		pVtx[nIdx].n[1] = Y;
		pVtx[nIdx].n[2] = Z;
	}
	
	else if(LcVtx::VtxNUV1::FVF == dFVF)
	{
		LcVtx::VtxNUV1* pVtx = (LcVtx::VtxNUV1*)m_pGeo[nGeo].pVB;
		pVtx[nIdx].n[0] = X;
		pVtx[nIdx].n[1] = Y;
		pVtx[nIdx].n[2] = Z;
	}

	else if(LcVtx::VtxNDUV1::FVF == dFVF)
	{
		LcVtx::VtxNDUV1* pVtx = (LcVtx::VtxNDUV1*)m_pGeo[nGeo].pVB;
		pVtx[nIdx].n[0] = X;
		pVtx[nIdx].n[1] = Y;
		pVtx[nIdx].n[2] = Z;
	}

	else if(LcVtx::VtxNDUV3::FVF == dFVF)
	{
		LcVtx::VtxNDUV3* pVtx = (LcVtx::VtxNDUV3*)m_pGeo[nGeo].pVB;
		pVtx[nIdx].n[0] = X;
		pVtx[nIdx].n[1] = Y;
		pVtx[nIdx].n[2] = Z;
	}
}


void CLcXmsh::SetDiffuse(INT nGeo, INT nIdx, FLOAT R, FLOAT G, FLOAT B, FLOAT A)
{
	DWORD dColor = D3DXCOLOR(R,G, B, A);

	if(!m_pGeo[nGeo].pVB)
		return;

	DWORD	dFVF = m_pGeo[nGeo].fmVx;

	if(LcVtx::VtxD::FVF == dFVF)
	{
		LcVtx::VtxD* pVtx = (LcVtx::VtxD*)m_pGeo[nGeo].pVB;
		pVtx[nIdx].d = dColor;
	}

	else if(LcVtx::VtxDUV1::FVF == dFVF)
	{
		LcVtx::VtxDUV1* pVtx = (LcVtx::VtxDUV1*)m_pGeo[nGeo].pVB;
		pVtx[nIdx].d = dColor;
	}

	else if(LcVtx::VtxNDUV1::FVF == dFVF)
	{
		LcVtx::VtxNDUV1* pVtx = (LcVtx::VtxNDUV1*)m_pGeo[nGeo].pVB;
		pVtx[nIdx].d = dColor;
	}

	else if(LcVtx::VtxNDUV3::FVF == dFVF)
	{
		LcVtx::VtxNDUV3* pVtx = (LcVtx::VtxNDUV3*)m_pGeo[nGeo].pVB;
		pVtx[nIdx].d = dColor;
	}

	else if(LcVtx::VtxRHWD::FVF == dFVF)
	{
		LcVtx::VtxRHWD* pVtx = (LcVtx::VtxRHWD*)m_pGeo[nGeo].pVB;
		pVtx[nIdx].d = dColor;
	}
}


void CLcXmsh::SetUV(INT nGeo, INT nIdx, INT nUV, FLOAT U, FLOAT V)
{
	if(!m_pGeo[nGeo].pVB)
		return;

	DWORD	dFVF = m_pGeo[nGeo].fmVx;

	if(LcVtx::VtxDUV1::FVF == dFVF)
	{
		LcVtx::VtxDUV1* pVtx = (LcVtx::VtxDUV1*)m_pGeo[nGeo].pVB;
		pVtx[nIdx].u = U;
		pVtx[nIdx].v = V;
	}

	else if(LcVtx::VtxNDUV1::FVF == dFVF)
	{
		LcVtx::VtxNDUV1* pVtx = (LcVtx::VtxNDUV1*)m_pGeo[nGeo].pVB;
		pVtx[nIdx].u = U;
		pVtx[nIdx].v = V;
	}

	else if(LcVtx::VtxNDUV3::FVF == dFVF)
	{
		LcVtx::VtxNDUV3* pVtx = (LcVtx::VtxNDUV3*)m_pGeo[nGeo].pVB;

		if(0==nUV)
		{
			pVtx[nIdx].u0 = U;
			pVtx[nIdx].v0 = V;
		}
		else if(1==nUV)
		{
			pVtx[nIdx].u1 = U;
			pVtx[nIdx].v1 = V;
		}
		else if(2==nUV)
		{
			pVtx[nIdx].u2 = U;
			pVtx[nIdx].v2 = V;
		}
	}

}



void CLcXmsh::GetPos(INT nGeo, INT nIdx, FLOAT* X, FLOAT* Y, FLOAT* Z, FLOAT* W)
{
	if(!m_pGeo[nGeo].pVB)
		return;

	DWORD	dFVF = m_pGeo[nGeo].fmVx;
	
	if(LcVtx::VtxD::FVF == dFVF)
	{
		LcVtx::VtxD* pVtx = (LcVtx::VtxD*)m_pGeo[nGeo].pVB;
		*X = pVtx[nIdx].p[0];
		*Y = pVtx[nIdx].p[1];
		*Z = pVtx[nIdx].p[2];
	}

	else if(LcVtx::VtxDUV1::FVF == dFVF)
	{
		LcVtx::VtxDUV1* pVtx = (LcVtx::VtxDUV1*)m_pGeo[nGeo].pVB;
		*X = pVtx[nIdx].p[0];
		*Y = pVtx[nIdx].p[1];
		*Z = pVtx[nIdx].p[2];
	}

	else if(LcVtx::VtxN::FVF == dFVF)
	{
		LcVtx::VtxN* pVtx = (LcVtx::VtxN*)m_pGeo[nGeo].pVB;
		*X = pVtx[nIdx].p[0];
		*Y = pVtx[nIdx].p[1];
		*Z = pVtx[nIdx].p[2];
	}
	
	else if(LcVtx::VtxUV1::FVF == dFVF)
	{
		LcVtx::VtxUV1* pVtx = (LcVtx::VtxUV1*)m_pGeo[nGeo].pVB;
		*X = pVtx[nIdx].p[0];
		*Y = pVtx[nIdx].p[1];
		*Z = pVtx[nIdx].p[2];
	}

	else if(LcVtx::VtxNUV1::FVF == dFVF)
	{
		LcVtx::VtxNUV1* pVtx = (LcVtx::VtxNUV1*)m_pGeo[nGeo].pVB;
		*X = pVtx[nIdx].p[0];
		*Y = pVtx[nIdx].p[1];
		*Z = pVtx[nIdx].p[2];
	}

	else if(LcVtx::VtxNDUV1::FVF == dFVF)
	{
		LcVtx::VtxNDUV1* pVtx = (LcVtx::VtxNDUV1*)m_pGeo[nGeo].pVB;
		*X = pVtx[nIdx].p[0];
		*Y = pVtx[nIdx].p[1];
		*Z = pVtx[nIdx].p[2];
	}

	else if(LcVtx::VtxNDUV3::FVF == dFVF)
	{
		LcVtx::VtxNDUV3* pVtx = (LcVtx::VtxNDUV3*)m_pGeo[nGeo].pVB;
		*X = pVtx[nIdx].p[0];
		*Y = pVtx[nIdx].p[1];
		*Z = pVtx[nIdx].p[2];
	}

	else if(LcVtx::VtxRHWD::FVF == dFVF)
	{
		LcVtx::VtxRHWD* pVtx = (LcVtx::VtxRHWD*)m_pGeo[nGeo].pVB;
		*X = pVtx[nIdx].p[0];
		*Y = pVtx[nIdx].p[1];
		*Z = pVtx[nIdx].p[2];
		*W = pVtx[nIdx].p[3];
	}
}





void CLcXmsh::GetNormal(INT nGeo, INT nIdx, FLOAT* X, FLOAT* Y, FLOAT* Z)
{
	if(!m_pGeo[nGeo].pVB)
		return;

	DWORD	dFVF = m_pGeo[nGeo].fmVx;

	if(LcVtx::VtxN::FVF == dFVF)
	{
		LcVtx::VtxN* pVtx = (LcVtx::VtxN*)m_pGeo[nGeo].pVB;
		*X = pVtx[nIdx].n[0];
		*Y = pVtx[nIdx].n[1];
		*Z = pVtx[nIdx].n[2];
	}
	
	else if(LcVtx::VtxNUV1::FVF == dFVF)
	{
		LcVtx::VtxNUV1* pVtx = (LcVtx::VtxNUV1*)m_pGeo[nGeo].pVB;
		*X = pVtx[nIdx].n[0];
		*Y = pVtx[nIdx].n[1];
		*Z = pVtx[nIdx].n[2];
	}

	else if(LcVtx::VtxNDUV1::FVF == dFVF)
	{
		LcVtx::VtxNDUV1* pVtx = (LcVtx::VtxNDUV1*)m_pGeo[nGeo].pVB;
		*X = pVtx[nIdx].n[0];
		*Y = pVtx[nIdx].n[1];
		*Z = pVtx[nIdx].n[2];
	}

	else if(LcVtx::VtxNDUV3::FVF == dFVF)
	{
		LcVtx::VtxNDUV3* pVtx = (LcVtx::VtxNDUV3*)m_pGeo[nGeo].pVB;
		*X = pVtx[nIdx].n[0];
		*Y = pVtx[nIdx].n[1];
		*Z = pVtx[nIdx].n[2];
	}
}




void CLcXmsh::GetDiffuse(INT nGeo, INT nIdx, FLOAT* R, FLOAT* G, FLOAT* B, FLOAT* A)
{
	D3DXCOLOR dColor(1,1, 1, 1);

	if(!m_pGeo[nGeo].pVB)
		return;

	DWORD	dFVF = m_pGeo[nGeo].fmVx;

	if(LcVtx::VtxD::FVF == dFVF)
	{
		LcVtx::VtxD* pVtx = (LcVtx::VtxD*)m_pGeo[nGeo].pVB;
		dColor = pVtx[nIdx].d;
		*R = dColor.r;
		*G = dColor.g;
		*B = dColor.b;
		*A = dColor.a;
	}

	else if(LcVtx::VtxDUV1::FVF == dFVF)
	{
		LcVtx::VtxDUV1* pVtx = (LcVtx::VtxDUV1*)m_pGeo[nGeo].pVB;
		pVtx[nIdx].d = dColor;
		*R = dColor.r;
		*G = dColor.g;
		*B = dColor.b;
		*A = dColor.a;
	}

	else if(LcVtx::VtxNDUV1::FVF == dFVF)
	{
		LcVtx::VtxNDUV1* pVtx = (LcVtx::VtxNDUV1*)m_pGeo[nGeo].pVB;
		pVtx[nIdx].d = dColor;
		*R = dColor.r;
		*G = dColor.g;
		*B = dColor.b;
		*A = dColor.a;
	}

	else if(LcVtx::VtxNDUV3::FVF == dFVF)
	{
		LcVtx::VtxNDUV3* pVtx = (LcVtx::VtxNDUV3*)m_pGeo[nGeo].pVB;
		pVtx[nIdx].d = dColor;
		*R = dColor.r;
		*G = dColor.g;
		*B = dColor.b;
		*A = dColor.a;
	}

	else if(LcVtx::VtxRHWD::FVF == dFVF)
	{
		LcVtx::VtxRHWD* pVtx = (LcVtx::VtxRHWD*)m_pGeo[nGeo].pVB;
		pVtx[nIdx].d = dColor;
		*R = dColor.r;
		*G = dColor.g;
		*B = dColor.b;
		*A = dColor.a;
	}
}




void CLcXmsh::GetUV(INT nGeo, INT nIdx, INT nUV, FLOAT* U, FLOAT* V)
{
	if(!m_pGeo[nGeo].pVB)
		return;

	DWORD	dFVF = m_pGeo[nGeo].fmVx;

	if(LcVtx::VtxDUV1::FVF == dFVF)
	{
		LcVtx::VtxDUV1* pVtx = (LcVtx::VtxDUV1*)m_pGeo[nGeo].pVB;
		*U = pVtx[nIdx].u;
		*V = pVtx[nIdx].v;
	}

	else if(LcVtx::VtxNDUV1::FVF == dFVF)
	{
		LcVtx::VtxNDUV1* pVtx = (LcVtx::VtxNDUV1*)m_pGeo[nGeo].pVB;
		*U = pVtx[nIdx].u;
		*V = pVtx[nIdx].v;
	}

	else if(LcVtx::VtxNDUV3::FVF == dFVF)
	{
		LcVtx::VtxNDUV3* pVtx = (LcVtx::VtxNDUV3*)m_pGeo[nGeo].pVB;

		if(0==nUV)
		{
			*U = pVtx[nIdx].u0;
			*V = pVtx[nIdx].v0;
		}
		else if(1==nUV)
		{
			*U = pVtx[nIdx].u1;
			*V = pVtx[nIdx].v1;
		}
		else if(2==nUV)
		{
			*U = pVtx[nIdx].u2;
			*V = pVtx[nIdx].v2;
		}
	}

}




