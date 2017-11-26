// Implementation of the CLcxMdl class.
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
#include "ILcmMdlX.h"

#include "LcxMdl.h"
#include "LcmSolid.h"
#include "LcmPack.h"


#include "LcmMesh.h"


namespace Lcm
{

//		Idx[ 0] = 0;	Idx[ 1] = 1;
//		Idx[ 2] = 1;	Idx[ 3] = 3;
//		Idx[ 4] = 3;	Idx[ 5] = 2;
//		Idx[ 6] = 2;	Idx[ 7] = 0;
//
//		Idx[ 8] = 0;	Idx[ 9] = 4;
//		Idx[10] = 1;	Idx[11] = 5;
//		Idx[12] = 3;	Idx[13] = 7;
//		Idx[14] = 2;	Idx[15] = 6;
//
//		Idx[16] = 4;	Idx[17] = 5;
//		Idx[18] = 5;	Idx[19] = 7;
//		Idx[20] = 7;	Idx[21] = 6;
//		Idx[22] = 6;	Idx[23] = 4;


static WORD	TBndAABB_nIdxLine[24]=
{
	0,	1,
	1,	3,
	3,	2,
	2,	0,

	0,	4,
	1,	5,
	3,	7,
	2,	6,

	4,	5,
	5,	7,
	7,	6,
	6,	4,
};


//					 
//		5------------7(Max)
//	y  /|           /|
//	| / |   z      / |
//	|/  |  /      /  |
//	1------------3   |
//	|   |/       |   |
//	|   4--------|---6
//	|  /         |  /
//	| /          | /
//	|/           |/
//	0(Min)-------2----- x

const static int TBndAABB_nIdxTri[][3] =
{
	{ 0, 1, 2},
	{ 3, 2, 1},

	{ 2, 3, 6},
	{ 7, 6, 3},

	{ 6, 7, 4},
	{ 5, 4, 7},

	{ 4, 5, 0},
	{ 1, 0, 5},

	{ 1, 5, 3},
	{ 7, 3, 5},

	{ 4, 0, 6},
	{ 2, 6, 0},
};



TbndAB::TbndAB()
{
	pOwner	= NULL;
}

TbndAB::TbndAB(const D3DXVECTOR3& _Min, const D3DXVECTOR3& _Max, void* pOwn)
{
	pOwner	= NULL;
	Set(_Min, _Max, pOwn);		
}

void TbndAB::Set(const D3DXVECTOR3& _Min, const D3DXVECTOR3& _Max, void* pOwn)
{
	vcN	= _Min;
	vcM = _Max;
	vcC = (vcM + vcN) * 0.5f;

	fX = vcM.x -_Min.x;
	fY = vcM.y -_Min.y;
	fZ = vcM.z -_Min.z;

	fR = sqrtf( fX * fX + fY * fY + fZ * fZ);

	vcE[0].p = vcN;
	vcE[1].p = vcN + D3DXVECTOR3( 0, fY,  0);
	vcE[2].p = vcN + D3DXVECTOR3(fX,  0,  0);
	vcE[3].p = vcN + D3DXVECTOR3(fX, fY,  0);
	vcE[4].p = vcN + D3DXVECTOR3( 0,  0, fZ);
	vcE[5].p = vcN + D3DXVECTOR3( 0, fY, fZ);
	vcE[6].p = vcN + D3DXVECTOR3(fX,  0, fZ);
	vcE[7].p = vcM;

	if(pOwn)
		pOwner = pOwn;
}


void TbndAB::RenderBox(void* _pDev)
{
	LPDIRECT3DDEVICE9 pDev = (LPDIRECT3DDEVICE9)_pDev;
	DWORD dFill;

	pDev->GetRenderState(D3DRS_FILLMODE, &dFill);
	pDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	pDev->SetTexture(0,0);
	pDev->SetFVF(TbndAB::_Tvtx::FVF);

	pDev->DrawIndexedPrimitiveUP(
				D3DPT_LINELIST, 0, 8, 12
				, TBndAABB_nIdxLine, D3DFMT_INDEX16
				, vcE, sizeof(TbndAB::_Tvtx));

	pDev->SetRenderState(D3DRS_FILLMODE, dFill);
}

void TbndAB::SetOwner(void* pOwn)
{
	pOwner = pOwn;
}

void* TbndAB::GetOwner()
{
	return pOwner;
}

void TbndAB::SetColor(DWORD dColor)
{
	for(int i=0 ;i<8; ++i)
		vcE[i].d = dColor;
}

D3DXVECTOR3* TbndAB::GetMin() const
{
	return (D3DXVECTOR3*)&(vcN);
}

D3DXVECTOR3* TbndAB::GetMax() const
{
	return (D3DXVECTOR3*)&(vcM);
}

D3DXVECTOR3* TbndAB::GetCenter() const
{
	return (D3DXVECTOR3*)&(vcC);
}

FLOAT TbndAB::GetWidth()	{	return  fX;	}
FLOAT TbndAB::GetHeight()	{	return	fY;	}
FLOAT TbndAB::GetDepth()	{	return	fZ;	}
FLOAT TbndAB::GetRadius()	{	return	fR;	}



TbndAABB::TbndAABB()
{
	pBnd=0;
	fStlSrtR = FLT_MAX;
}

TbndAABB::TbndAABB(const D3DXVECTOR3& vcP, TbndAB* _pBnd, FLOAT _fD)
: vPck(vcP), pBnd(_pBnd), fStlSrtR(_fD)
{
}



INT	LcxUtil_PickAABB(FLOAT* fDist												// Distance
					, D3DXVECTOR3* pvcOut										// In, Out: Picking Position
					, D3DXVECTOR3* pvcCamPos									// In: Camera Position
					, D3DXVECTOR3* pvcRayDir									// In: Ray Direction
					, TbndAB* pIn												// In: Bound Box for Test.
					)
{
	D3DXVECTOR3	vcCamPos= *pvcCamPos;
	D3DXVECTOR3	vcRayDir= *pvcRayDir;

	D3DXVECTOR3	vcBndCent= *pIn->GetCenter();
	D3DXVECTOR3	vcT		 = vcBndCent - vcCamPos;

	FLOAT	fRadius = pIn->GetRadius();
	FLOAT	fDot	= D3DXVec3Dot(&vcRayDir, &vcT);
	FLOAT	fL		= 0.F;

	// 디렉션과 반대이면 빠져나감
	if(fDot<0.f)
		return -1;


	// 충돌 바운드 구와 비교도 한다.
	fL		 =D3DXVec3LengthSq(&vcT);
	fDot	*=fDot;
	fRadius *=fRadius;

	if( fL >(fDot + fRadius))
		return -1;


	// 충돌 바운드 구의 반경보다 작으면 12 개의 삼각형 충돌을 진행한다.
	

	FLOAT	fMax= FLT_MAX;
	FLOAT	U;
	FLOAT	V;
	FLOAT	D;
	D3DXVECTOR3*	p0;
	D3DXVECTOR3*	p1;
	D3DXVECTOR3*	p2;
	D3DXVECTOR3		vcPck;

	TbndAB::_Tvtx* vcE = pIn->vcE;

	for(int i=0; i<12; ++i)
	{
		p0 = &vcE[TBndAABB_nIdxTri[i][0] ].p;
		p1 = &vcE[TBndAABB_nIdxTri[i][1] ].p;
		p2 = &vcE[TBndAABB_nIdxTri[i][2] ].p;

		if(D3DXIntersectTri( p0, p1, p2, &vcCamPos, &vcRayDir, &U, &V, &D))
		{
			if(D<fMax)
			{
				if(pvcOut)
					*pvcOut = *p0 + U * (*p1 - *p0) + V * (*p2 - *p0);

				fMax = D;
			}
		}
	}

	// 충돌하면...
	if(FLT_MAX != fMax)
	{
		*fDist = fMax;
		return 0;
	}

	return -1;
}



TmshInf::TmshInf()
{
	pTex		= NULL;
	memset(sTex, 0, sizeof(sTex));
	memset(sMdl, 0, sizeof(sMdl));

	iNix		= 0;
	iNvx		= 0;
	dFVF		= 0;
	iVxS		= 0;
	pIdx		= NULL;
	pVtx		= NULL;
}

TmshInf::~TmshInf()
{
	SAFE_FREE(	pIdx	);
	SAFE_FREE(	pVtx	);
}


INT		TmshInf::GetType()			{	return nType;		}
char*	TmshInf::GetMdlName()		{	return sMdl;		}
void	TmshInf::SetMdlName(char* s){	strcpy(sMdl, s);	}

INT		TmshInf::GetNumFce()		{	return iNix;		}
INT		TmshInf::GetNumVtx()		{	return iNvx;		}
DWORD	TmshInf::GetFVF()			{	return dFVF;		}
INT		TmshInf::GetZeroStrd()		{	return iVxS;		}

void*	TmshInf::GetBufFce()		{	return pIdx;		}

char*	TmshInf::GetTexName()		{	return sTex;		}
void	TmshInf::SetTexName(char* s){	strcpy(sTex, s);	}

void*	TmshInf::GetTex()			{	return pTex;		}
void	TmshInf::SetTex(void* _p)	{	pTex = _p;			}


INT MdlX_Create(char* sCmd, ILcmMdl** pData, void* p1, void* p2, void* p3, void* p4)
{
	(*pData) = NULL;

	if(0==_stricmp("billboard", sCmd))
	{
		CLcmBill* pMdB = NULL;

		pMdB = new CLcmBill;

		if(FAILED(pMdB->Create(p1, p2, p3, p4)))
		{
			// Return Error Notification
			delete pMdB;
			return -1;
		}

		(*pData) = pMdB;
		return 0;
	}

	else if(0==_stricmp("solid", sCmd))
	{
		CLcmSolid* pMdB = NULL;

		pMdB = new CLcmSolid;

		if(FAILED(pMdB->Create(p1, p2, p3, p4)))
		{
			// Return Error Notification
			delete pMdB;
			return -1;
		}

		(*pData) = pMdB;
		return 0;
	}

	return -1;
}


INT MdlX_CreatePack(char* sCmd, ILcmPack** pData, void* p1, void* p2, void* p3, void* p4)
{
	CLcmPack* pMdPck = NULL;

	pMdPck = new CLcmPack;

	if(FAILED(pMdPck->Create(p1, p2, p3, p4)))
	{
		// Return Error Notification
		delete pMdPck;
		return -1;
	}

	(*pData) = pMdPck;
	return 0;
}


INT MdlX_CreateMsh(char* sCmd, ILcmMesh** pData, void* p1, void* p2, void* p3, void* p4)
{
	(*pData) = NULL;

	CLcmMesh* pMsh = NULL;

	pMsh = new CLcmMesh;

	if(FAILED(pMsh->Create(p1, p2, p3, p4)))
	{
		// Return Error Notification
		delete pMsh;
		return -1;
	}

	(*pData) = pMsh;
	return 0;;
}

}// namespace Lcm


