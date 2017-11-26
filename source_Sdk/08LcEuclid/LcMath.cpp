// Implementation of the Math Functions.
//
////////////////////////////////////////////////////////////////////////////////


#include <windows.h>
#include <stdio.h>
#include <d3dx9.h>

#include "LcEuclid.h"
#include "LcMath.h"


INT LcMath_MakePower2(INT a)
{
	int n=0;
	int t=a;

	while(t)
	{
		t>>=1;
		++n;
	}

	return (0x1<<(n-1) ^ a) ? 0x1<<n : 0x1<<(n-1);
};


void LcMath_InterpolateFloat2(FLOAT* pOut, FLOAT* p1, FLOAT* p2, FLOAT d)
{
	FLOAT inv = 1.f - d;
	
	pOut[0] = p1[0] * inv + p2[0] * d;
	pOut[1] = p1[1] * inv + p2[1] * d;
}


void LcMath_InterpolateFloat3(FLOAT* pOut, FLOAT* p1, FLOAT* p2, FLOAT d)
{
	FLOAT inv = 1.f - d;
	
	pOut[0] = p1[0] * inv + p2[0] * d;
	pOut[1] = p1[1] * inv + p2[1] * d;
	pOut[2] = p1[2] * inv + p2[2] * d;
}

void LcMath_InterpolateFloat4(FLOAT* pOut, FLOAT* p1, FLOAT* p2, FLOAT d)
{
	FLOAT inv = 1.f - d;
	
	pOut[0] = p1[0] * inv + p2[0] * d;
	pOut[1] = p1[1] * inv + p2[1] * d;
	pOut[2] = p1[2] * inv + p2[2] * d;
	pOut[3] = p1[3] * inv + p2[3] * d;
}


void LcMath_InterpolateFloat(FLOAT* pOut, FLOAT* p1, FLOAT* p2, FLOAT d, INT iCnt)
{
	FLOAT inv = 1.f - d;
	
	for(int i=0; i<iCnt; ++i)
		pOut[i] = p1[i] * inv + p2[i] * d;
}

INT	LcMath_Equals(FLOAT a, FLOAT b)
{
	return (
				(a + LC_ROUNDING_DELTA > b) &&
				(a - LC_ROUNDING_DELTA < b)
			) ? 1: -1;
}





FLOAT LcMath_Vec3Length(FLOAT *pV)
{
#ifdef __cplusplus
    return sqrtf(pV[0] * pV[0] + pV[1] * pV[1] + pV[2] * pV[2]);
#else
    return (FLOAT) sqrt(pV[0] * pV[0] + pV[1] * pV[1] + pV[2] * pV[2]);
#endif
}

FLOAT LcMath_Vec3LengthSq(FLOAT *pV)
{
	return  pV[0] * pV[0] + pV[1] * pV[1] + pV[2] * pV[2];
}


FLOAT LcMath_Vec3Dot(FLOAT *pV1, FLOAT *pV2 )
{
	return  pV1[0] * pV2[0] + pV1[1] * pV2[1] + pV1[2] * pV2[2];
}


void	LcMath_Vec3Cross(FLOAT* pOut, FLOAT *pV1, FLOAT *pV2 )
{
	pOut[0] = pV1[1] * pV2[2] - pV1[2] * pV2[1];
	pOut[1] = pV1[2] * pV2[0] - pV1[0] * pV2[2];
	pOut[2] = pV1[0] * pV2[1] - pV1[1] * pV2[0];
}


void LcMath_Vec3NormalVector(VEC3* pOut, VEC3* p1, VEC3* p2, VEC3* p3)
{
	VEC3 a, b;

	a = *p2 - *p1;
	b = *p3 - *p1;

	D3DXVec3Cross(pOut, &a, &b);
	D3DXVec3Normalize(pOut, pOut);
}



void	LcMath_Vec3Normalize(FLOAT* pOut)
{
	FLOAT fL = LcMath_Vec3Length(pOut);

	pOut[1] /= fL;
	pOut[1] /= fL;
	pOut[2] /= fL;
}



void LcMath_Vec3Minimize(FLOAT *pOut, FLOAT *pV1, FLOAT *pV2 )
{
	pOut[0] = (pV1[0] < pV2[0]) ? pV1[0] : pV2[0];
    pOut[1] = (pV1[1] < pV2[1]) ? pV1[1] : pV2[1];
    pOut[2] = (pV1[2] < pV2[2]) ? pV1[2] : pV2[2];
}


void LcMath_Vec3Maximize(FLOAT *pOut, FLOAT *pV1, FLOAT *pV2 )
{
	pOut[0] = (pV1[0] > pV2[0]) ? pV1[0] : pV2[0];
    pOut[1] = (pV1[1] > pV2[1]) ? pV1[1] : pV2[1];
    pOut[2] = (pV1[2] > pV2[2]) ? pV1[2] : pV2[2];
}


void LcMath_Mat4X4Identity(FLOAT* pM)
{
	pM[ 0] = 1.f;	pM[ 1] = 0.f;	pM[ 2] = 0.f;	pM[ 3] = 0.f;
	pM[ 4] = 0.f;	pM[ 5] = 1.f;	pM[ 6] = 0.f;	pM[ 7] = 0.f;
	pM[ 8] = 0.f;	pM[ 9] = 0.f;	pM[10] = 1.f;	pM[11] = 0.f;
	pM[12] = 0.f;	pM[13] = 0.f;	pM[14] = 0.f;	pM[15] = 1.f;
}


FLOAT LcMath_QuatLength(FLOAT *pV/*x,y,z,w*/)
{
#ifdef __cplusplus
    return sqrtf(pV[0] * pV[0] + pV[1] * pV[1] + pV[2] * pV[2] + pV[3] * pV[3]);
#else
    return (FLOAT)sqrt(pV[0] * pV[0] + pV[1] * pV[1] + pV[2] * pV[2] + pV[3] * pV[3]);
#endif
}


FLOAT LcMath_QuatLengthSq(FLOAT *pV/*x,y,z,w*/)
{
	return (pV[0] * pV[0] + pV[1] * pV[1] + pV[2] * pV[2] + pV[3] * pV[3]);
}


FLOAT LcMath_QuaternionDot(FLOAT *pV1, FLOAT *pV2)
{
	return (pV1[0] * pV2[0] + pV1[1] * pV2[1] + pV1[2] * pV2[2] + pV1[3] * pV2[3]);
}


void LcMath_QuaternionIdentity(FLOAT *pOut)
{
	pOut[0] = 0.f;
	pOut[1] = 0.f;
	pOut[2] = 0.f;
	pOut[3] = 1.f;
}




INT LcMath_LineCross2D(VEC2 * p)
{
	VEC2 L1 = p[1] - p[0];
	VEC2 L2 = p[3] - p[2];
	VEC2 L3 = p[2] - p[0];
	
	
	FLOAT fAlpha = L2.x * L1.y - L2.y * L1.x;
	
	if(0.f == fAlpha)
		return -1;
	
	FLOAT fBeta = fAlpha;
	
	fAlpha = (L2.x * L3.y - L2.y * L3.x)/fAlpha;
	
	if( (0.f > fAlpha) || (fAlpha > 1.f) )
		return -1;
	
	fBeta = (L2.x * L3.y - L1.y * L3.x)/fBeta;
	
	if( (0.f > fAlpha) || (fAlpha > 1.f) )
		return -1;
	
	return 0;
}

INT LcMath_Point3DtoScreen(	VEC3* pOut				// Output
						  ,	const VEC3* vcIn		// In put
						  , const VEC3* vcCam		// Camera pos
						  ,	const VEC3*	vcZ			// Camera Z vector
						  , const MATA*	mtVP		// View matrix * Projection Matrix
						  , FLOAT fScnW				// Screen Width
						  , FLOAT fScnH				// Screen Height
				  )
{
	FLOAT	fNear=1.f;
	FLOAT	beta;
	VEC3	vcB;
	VEC3	vcT	= (*vcIn - *vcCam);
	
	beta	= D3DXVec3Dot(vcZ, &vcT);
	
	if(beta<=fNear)
		return -1;			// 프로젝션 평면 뒤에 있음.

	beta = fNear/beta;
	vcB = beta * vcT;
	vcB -= fNear * (*vcZ);
	
	pOut->x	= vcB.x * mtVP->_11 + vcB.y * mtVP->_21 + vcB.z * mtVP->_31;			// 다음 Model view Matrix와 연산한다.
	pOut->y	= vcB.x * mtVP->_12 + vcB.y * mtVP->_22 + vcB.z * mtVP->_32;
	pOut->z	= vcB.x * mtVP->_13 + vcB.y * mtVP->_23 + vcB.z * mtVP->_33;

	pOut->x	=  fScnW * (pOut->x +1.f) * 0.5f;
	pOut->y	= -fScnH * (pOut->y -1.f) * 0.5f;

	return 0;
}





INT LcMath_MousePositionTo3D(VEC3* vcOut, MATA matView, MATA matProj, FLOAT fScnW, FLOAT fScnH, POINT ptMouse)
{
	MATA matViewProj;
	MATA matViewInv;
	
	MATA matViewProjInv;
	
	D3DXMatrixIdentity(&matViewProj);
	
	matViewProj = matView * matProj;
	D3DXMatrixInverse(&matViewProjInv, NULL, &matViewProj);
	
	
	VEC3 vecBefore( 2.f * ptMouse.x/fScnW - 1,  -2.f * ptMouse.y/fScnH + 1,	0);
	
	VEC3 vecP(0,0,0);
	
	
	// 다음 Model view Matrix와 연산한다.
	
	vecP.x = vecBefore.x * matViewProjInv._11 + vecBefore.y * matViewProjInv._21 + vecBefore.z * matViewProjInv._31;
	vecP.y = vecBefore.x * matViewProjInv._12 + vecBefore.y * matViewProjInv._22 + vecBefore.z * matViewProjInv._32;
	vecP.z = vecBefore.x * matViewProjInv._13 + vecBefore.y * matViewProjInv._23 + vecBefore.z * matViewProjInv._33;
	
	//camera position
	D3DXMatrixInverse(&matViewInv, NULL, &matView);
	VEC3 vecCamPos(matViewInv._41, matViewInv._42, matViewInv._43);
	
	//Normalvector
	VEC3 vecZ( matView._13, matView._23, matView._33);
	
	FLOAT beta =(vecZ.y + vecP.y);
	
	if (0.f == beta)
		return -1;
	
	beta =  -vecCamPos.y/beta;
	
	*vcOut = beta * (vecZ + vecP) + vecCamPos;
	
	return 0;
}




FLOAT LcMath_PointToPlaneDistance(FLOAT* pPn/*VECTOR4*/, FLOAT* p1 /*VECTOR3*/)
{
	return D3DXPlaneDotCoord((DPLN*)pPn, (VEC3*)p1);
}


FLOAT LcMath_LineToPointDistance(const LcLine* pL, const D3DXVECTOR3* pP)
{
	D3DXVECTOR3 L;
	D3DXVECTOR3 N;
	L = *pP - pL->p;
	D3DXVec3Cross(&N, &L, &pL->t);

	return D3DXVec3Length(&N);
}



INT	LcMath_LineToSphere(VEC3* pOut,FLOAT* fDst, const LcLine* pLine,const VEC3* pC,const FLOAT fR)
{
	VEC3	vcP;
	VEC3	vcT;
	FLOAT	fT;
	FLOAT	fN;

	vcT = *pC - pLine->p;
	fT = D3DXVec3Dot(&pLine->t, &vcT);

	vcP = pLine->p + fT * pLine->t;

	fN = D3DXVec3LengthSq(&vcT);
	fN -= fT * fT;

	if(fDst)
		*fDst = sqrtf(fN);

	if(pOut)
		*pOut = vcP;

	if(fN < fR*fR)
		return 0;
	
	return -1;
}





//FLOAT LcMath_LineToLineDistance(const LcLine* pL1, const LcLine* pL2)
//{
//	D3DXVECTOR3	n;
//	D3DXVec3Cross(&n, &pL1->t, &pL2->t);
//	D3DXVec3Normalize(&n,&n);
//
//	return fabs(D3DXVec3Dot(&(pL2->p - pL1->p), &n));
//}



// 두직선 사이의 최단 거리 구하기
FLOAT	LcMath_LineToLineDistance(																			// 직선과 직선 사이의 최단 거리
									FLOAT* pSinTheta														// 직선과 직선 사이의 각도
								,	VEC3* pOut																// 직선과 직선사이의 노말벡터
								,	const LcLine* pV1														// 직선 1
								,	const LcLine* pV2														// 직선 2
								)
{
	VEC3	vcN;
	VEC3	vcL1 = pV1->t;
	VEC3	vcL2 = pV2->t;
	VEC3	vcP1P2=pV1->p - pV2->p;

	FLOAT	fD =0.f;
	FLOAT	fL = 0.f;
	
	D3DXVec3Cross(&vcN, &vcL1, &vcL2);

	fL = D3DXVec3Length(&vcN);

	if(pSinTheta)
		*pSinTheta = fL;

	fL = 1.f/fL;

	vcN.x *= fL;
	vcN.y *= fL;
	vcN.z *= fL;

	fD = D3DXVec3Dot(&vcN, &vcP1P2);

	*pOut = vcN;

	return fD;
}




// 두 직선 사이의 상호 작용
INT LcMath_LineToLineIntersection(FLOAT* pSinTheta, FLOAT* pDst,VEC3* pOut,const LcLine* pV1,const LcLine* pV2)
{
	VEC3	vcN;
	VEC3	vcL1 = pV1->t;
	VEC3	vcL2 = pV2->t;
	VEC3	vcP1 = pV1->p;
	VEC3	vcP2 = pV2->p;
	
	VEC3	vcP2P1 = vcP2 - vcP1;
	
	FLOAT	fD		= 0.f;
	FLOAT	fL1L2   = D3DXVec3Dot(&vcL1, &vcL2);
	FLOAT	fP2P1L1 = D3DXVec3Dot(&vcP2P1, &vcL1);
	FLOAT	fP2P1L2 = D3DXVec3Dot(&vcP2P1, &vcL2);

	FLOAT	fDet1 = -1 + fL1L2 * fL1L2;
	FLOAT	fDet2 = fP2P1L2 - fL1L2 * fP2P1L1;
	FLOAT	fBeta =0.f;

	// 평행이다.
	if( fabsf(fL1L2)>0.9999f)
	{
		VEC3 vcT;
		
		D3DXVec3Cross(&vcT, &vcL1, &vcP2P1);
		fD = D3DXVec3LengthSq(&vcT);

		if(pDst)
			*pDst = fD;

		// 겹쳐 있다면
		// 부정( 정할 수 없다.)
		if(fD<0.0001f)
			return -1;

		// 겹쳐 있지 않다면
		// 불능(구할 수 없다.)
		return -2;
	}


	fD=LcMath_LineToLineDistance(pSinTheta, &vcN, pV1, pV2);

	if(pDst)
		*pDst = fD;


	// 충돌
	if(fabsf(fD)<0.0001f)
	{
		fBeta = fDet2/ fDet1;
		pOut[0] = vcP2 + fBeta * vcL2;
		pOut[1] = pOut[0];
		
		// 충돌한 점의 갯수를 돌려준다.
		return 1;
	}


	fBeta = fDet2/ fDet1;
	pOut[1] = vcP2 + fBeta * vcL2;
	pOut[0] = fD * vcN + pOut[1];
	
	// 충돌한 점의 갯수를 돌려준다.
	return 2;
}





// 무한 직선과 무한 평면사이..
// p = p1 + k * (p2 - p1)
// n . p + d = 0
// n . p1 + k *( p2 - p1) . p + d = 0
//
// k = - (d + n . p1)/( ( p2 - p1) . p )
//

FLOAT LcMath_LineToPlaneKValue(const DPLN* p1, const VEC3* p2, const VEC3* p3)
{
	VEC3 vcT = *p3 - *p2;
	FLOAT k = D3DXPlaneDotNormal(p1, &vcT);
	return -(D3DXPlaneDotCoord(p1, p2) / k);
}




INT	LcMath_LineToTriangle(VEC3* pOut, FLOAT* fD, VEC3* vcRayPos, VEC3* vcRayDir, VEC3* pTri, BOOL bUseCull)
{
	FLOAT	U, V;

	if(bUseCull)
	{
		VEC3 vcN;
		D3DXVec3Cross(&vcN, &(pTri[1]-pTri[0]), &(pTri[2]-pTri[0]));

		if( D3DXVec3Dot(&vcN, vcRayDir)>0)
			return -1;
	}

	if(D3DXIntersectTri( &pTri[0], &pTri[1], &pTri[2], vcRayPos, vcRayDir, &U, &V, fD))
	{
		if(pOut)
			*pOut = pTri[0] + U * (pTri[1] - pTri[0]) + V * (pTri[2] - pTri[0]);

		return 0;
	}

	return -1;
}


INT	LcMath_LineToTriangle(	VEC3* vcPck				// Pick Position
						,	FLOAT* D				// Distance from vcRayPos to pOut
						,	const VEC3* vcRayPos	// Starting Ray Position
						,	const VEC3* vcRayDir	// Starting Ray Position
						,	const VEC3* p0			// Triangle 0
						,	const VEC3* p1			// Triangle 1
						,	const VEC3* p2			// Triangle 2
						,	BOOL bUseCull)
{
	VEC3	vcCamPos= *vcRayPos;
	VEC3	vcA		= *p1-*p0;
	VEC3	vcB		= *p2-*p0;

	FLOAT	tU, tV, tD;

	if(bUseCull)
	{
		VEC3 vcN;
		D3DXVec3Cross(&vcN, &vcA, &vcB);

		if( D3DXVec3Dot(&vcN, vcRayDir)>0)
			return -1;
	}

	if(D3DXIntersectTri( p0, p1, p2, vcRayPos, vcRayDir, &tU, &tV, &tD))
	{
		if(vcPck)
			*vcPck = *p0 + tU * vcA + tV * vcB;
		
		if(D)
			*D = tD;
		
		return 0;
	}

	return -1;
}



INT LcMath_LineToTriangle(
							VEC3* vcOut
						,	const VEC3* pTri
						,	const LcLine* pLine
						,	BOOL bRectangle
						,	BOOL bPositiveDirection
						,	const VEC3* pBoundLine
						,	const VEC3* pTriNormal
						)
{
	VEC3		vcN;
	FLOAT		fD;

	VEC3 vcA = pTri[1] - pTri[0];
	VEC3 vcB = pTri[2] - pTri[0];

	if(NULL == pTriNormal)
	{
		D3DXVec3Cross(&vcN, &vcA, &vcB);
		D3DXVec3Normalize(&vcN,&vcN);
	}
	else
	{
		vcN = *pTriNormal;
	}

	fD = -D3DXVec3Dot(&vcN, &pTri[0]);

	if(pBoundLine)																// 직선이 유한일 경우
	{
		D3DXPLANE	plane = D3DXPLANE(vcN.x, vcN.y, vcN.z, fD);
		FLOAT fD1 = D3DXPlaneDotCoord(&plane, &pBoundLine[0]);
		FLOAT fD2 = D3DXPlaneDotCoord(&plane, &pBoundLine[1]);

		if(fD1 * fD2>0)																// Not Collision
			return -1;
	}

	VEC3		vcC;
	VEC3		vcD;

	FLOAT		fT;
	FLOAT		fAC;
	FLOAT		fBC;
	FLOAT		fAB;

	FLOAT		fA;
	FLOAT		fB;
	FLOAT		fU;
	FLOAT		fV;

	VEC3 vcL = pLine->t;

	fT = D3DXVec3Dot(&vcL, &vcN);
	fT = -( fD+ D3DXVec3Dot(&vcN, &pLine->p))/fT;

	if(bPositiveDirection && fT<=0)
		return -1;

	vcC = pLine->p + fT * vcL;
	vcC -= pTri[0];

	fA = D3DXVec3Length(&vcA);
	fB = D3DXVec3Length(&vcB);

	fAC = D3DXVec3Dot(&vcA, &vcC);
	fBC = D3DXVec3Dot(&vcB, &vcC);
	fAB = D3DXVec3Dot(&vcA, &vcB);

	fD = fA * fA * fB * fB - fAB*fAB+0.000001f;
	fU = (fAC * fB*  fB - fAB * fBC)/fD;
	fV = (fA * fA * fBC - fAB * fAC)/fD;

	vcD = fU * vcA + fV * vcB;
	vcD += pTri[0];
	*vcOut = vcD;

	if(bRectangle)
	{
		if(fU>=-0.0001f && fU<=1.0001f && fV>=-0.0001f && fV<=1.0001f)
			return 0;
	}
	else
	{
		if(fU>=-0.0001f && fU<=1.0001f && fV>=-0.0001f && fV<=1.0001f && (fU+fV)<1.0001f)
			return 0;
	}
	
	return -1;
}


INT LcMath_TriangleToTriangle(VEC3* pOut, VEC3* pV1, VEC3* pV2)
{
	//삼각형과 삼각형의 충돌은 최소1점이상 이어야...
	// 직선과 삼각형 충돌을 응용한다.

	INT		iPoint=0;
	VEC3*	pTri1 = NULL;
	VEC3*	pTri2 = NULL;

	VEC3	pLine[2];
	VEC3	vcPick;
	INT		i=0;
	

	pTri1 = pV1;
	pTri2 = pV2;

	for(i=0; i<3; ++i)
	{
		pLine[0] = pTri2[(i+0)%3];
		pLine[1] = pTri2[(i+1)%3];

		VEC3	vcLineT = pLine[1] - pLine[0];
		D3DXVec3Normalize(&vcLineT, &vcLineT);

		LcLine Line(pLine[0], vcLineT);

		if(SUCCEEDED(LcMath_LineToTriangle(&vcPick, pTri1, &Line, FALSE, FALSE, pLine)))
		{
			if(pOut)
				*(pOut + iPoint) = vcPick;

			++iPoint;
		}
	}

	if(2==iPoint)
		return iPoint;

	pTri1 = pV2;
	pTri2 = pV1;

	for(i=0; i<3; ++i)
	{
		pLine[0] = pTri2[(i+0)%3];
		pLine[1] = pTri2[(i+1)%3];

		VEC3	vcLineT = pLine[1] - pLine[0];
		D3DXVec3Normalize(&vcLineT, &vcLineT);

		LcLine Line(pLine[0], vcLineT);

		if(SUCCEEDED(LcMath_LineToTriangle(&vcPick, pTri1, &Line, FALSE, FALSE, pLine)))
		{
			if(pOut)
				*(pOut + iPoint) = vcPick;
			
			++iPoint;
		}
	}

	if(iPoint<1)
		return -1;

	return iPoint;
}


// 삼각형의 정교한 외접원
FLOAT LcMath_CircumcircleCorrect(VEC3* pOut,const VEC3* pTri)
{
	VEC3	p1, p2, p3;
	VEC3	t1, t2, t3;
	VEC3	 n, n1, n2;
	VEC3	q1, q2, q3;
	VEC3	s1;

	FLOAT	a1=0.f;
	FLOAT	fR=0.f;
	VEC3	vcT;

	FLOAT	fD=0.f;

	p1 = pTri[0];
	p2 = pTri[1];
	p3 = pTri[2];

	t1 = (p2 - p1) * .5f;
	t2 = (p3 - p2) * .5f;
	t3 = (p1 - p3) * .5f;

	D3DXVec3Cross(&n, &t1, &t2);
	n *=-1.f;
	D3DXVec3Normalize(&n, &n);

	D3DXVec3Cross(&n1, &n, &t1);
	D3DXVec3Cross(&n2, &n, &t2);

	q1 = p1 + t1;
	q2 = p2 + t2;
	q3 = p3 + t3;

	s1 = q2 - q1;

	FLOAT fN1 = D3DXVec3LengthSq(&n1);
	FLOAT fN2 = D3DXVec3LengthSq(&n2);

	FLOAT fN12 = D3DXVec3Dot(&n1, &n2);
	
	fD	= fN12 * fN12 - fN1 * fN2;



	FLOAT fN1S1 = D3DXVec3Dot(&n1, &s1);
	FLOAT fN2S1 = D3DXVec3Dot(&n2, &s1);

	a1 = fN12 * fN2S1 - fN2 * fN1S1;
	a1 /= fD;

	vcT		= t1 + a1 * n1;
	*pOut	= p1 + vcT;

	fR = D3DXVec3Length(&vcT);

	// 반경을 리턴
	return fR;
}



// 삼각형의 비정교한 외접원
// 무게중심을 가운데 두고, 이 점에서 각꼭지점 사이의 거리가 가장 큰 값을 반경으로 결정.
FLOAT LcMath_TriangleBoundSphere(VEC3* pOut,const VEC3* pTri)
{
	FLOAT	fR=0.f,	fR2=0.f, fR3=0.f;
	VEC3	q1, q2, q3;

	*pOut =pTri[0];
	*pOut +=pTri[1];
	*pOut +=pTri[2];

	*pOut /= 3.f;

	q1 = *pOut;
	q2 = *pOut;
	q3 = *pOut;

	q1 -= pTri[0];
	q2 -= pTri[1];
	q3 -= pTri[2];
	
	fR = D3DXVec3Length(&q1);
	fR2 = D3DXVec3Length(&q2);
	fR3 = D3DXVec3Length(&q3);

	if(fR2>fR)
		fR= fR2;

	if(fR3>fR)
		fR= fR3;

	// 반경을 리턴
	return fR;
}









INT LcMath_IsInPoints(const VEC3* p1, const VEC3* p2) 
{
	VEC3	_p = (*p1 - *p2);
	FLOAT f = D3DXVec3LengthSq(&_p);
	
	return (D3DXVec3LengthSq(p1) < f && D3DXVec3LengthSq(p2) < f) ? 1: -1;
}

INT LcMath_Vec3Greater(const VEC3* p1, const VEC3* p2)
{
	return (p1->x <= p2->x && p1->y <= p2->y && p1-> z <= p2->z) ? 1: -1;
}

INT	LcMath_Vec3Less(const VEC3* p1, const VEC3* p2)
{
	return (p1->x >= p2->x && p1->y >= p2->y && p1->z >= p2->z) ? 1: -1;
}


// 평면의 d값 세팅
void LcMath_PlaneSetD(DPLN* pOut, const VEC3* v)
{
	pOut->d = - D3DXVec3Dot(&VEC3(pOut->a, pOut->b, pOut->c), v);
}


// 법선 벡터와 원점에서의 최단거리로 평면 만들기
void LcMath_Plane(DPLN* pOut, const VEC3* n, FLOAT d)
{
	pOut->a = n->x;
	pOut->b = n->y;
	pOut->c = n->z;
	pOut->d = d;
}


// 법선벡터와 점으로 평면 만들기
void LcMath_PlaneSet(DPLN* pOut, const VEC3* p0, const VEC3* n)
{
	VEC3 nT = *n;
	D3DXVec3Normalize(&nT, &nT);
	
	pOut->a = nT.x;
	pOut->b = nT.y;
	pOut->c = nT.z;
	pOut->d = - D3DXVec3Dot(&nT, p0);
}


// 세점으로 평면 만들기
void LcMath_PlaneSet(DPLN* pOut, const VEC3* p0, const VEC3* p1, const VEC3* p2)
{
	VEC3 tp1 = *p1 - *p0;
	VEC3 tp2 = *p2 - *p0;
	
	VEC3 nT;
	D3DXVec3Cross(&nT, &tp1, &tp2);
	D3DXVec3Normalize(&nT, &nT);
	
	pOut->a = nT.x;
	pOut->b = nT.y;
	pOut->c = nT.z;
	pOut->d = - D3DXVec3Dot(&nT, p0);
}


// 직선과 평면과의 충돌점
INT LcMath_PlaneIntersectionWithLine(VEC3* pOut, const DPLN* pPn, const VEC3* p1, const VEC3* vcT)
{
	FLOAT k = D3DXPlaneDotNormal(pPn, vcT);
	
	if (k == 0)
		return -1;
	
	k =1.f/k;
	k *= (- D3DXPlaneDotCoord(pPn, p1) );
	
	if(pOut)
		*pOut = *p1 + k * (*vcT);
	
	return 0;
}





// 2점사이의 평면
INT LcMath_PlaneIntersectionWithPoints(VEC3* pOut, const DPLN* pPn, const VEC3* p1, const VEC3* p2)
{
	FLOAT d1 = D3DXPlaneDotCoord(pPn, p1);
	FLOAT d2 = D3DXPlaneDotCoord(pPn, p2);
	
	d1 *=  d2;
	
	if(pOut)
	{
		VEC3 vcT = *p2 - *p1;
		FLOAT k = D3DXPlaneDotNormal(pPn, &vcT);
		k =1.f/k;
		*pOut = *p1 + k * vcT;
	}
	
	return d1<0.f? 1: -1;
}




FLOAT	LcMath_PlaneToPlaneDistance(const LcPlane* pP1, const LcPlane* pP2)									// 평행한 평면사이의 최단거리
{
//	FLOAT fP1;
//	FLOAT fP2;
//
//	fP1 = D3DXVec3Length(&pP1->n);
//	fP1 = pP1->d/fP1;
//
//	fP2 = D3DXVec3Length(&pP2->n);
//	fP2 = pP2->d/fP2;
//
//	return fabs(fP1 - fP2);

	return fabs(pP1->d - pP2->d);
}



//	D3DXINLINE FLOAT D3DXPlaneDotCoord( const D3DXPLANE *pP, const VEC3 *pV)
//	{
//	#ifdef D3DX_DEBUG
//		if(!pP || !pV)
//			return 0.0f;
//	#endif
//
//		return pP->a * pV->x + pP->b * pV->y + pP->c * pV->z + pP->d;
//	}


// 임의의 점이 평면의 위(뒤)에 있는 가 아래(앞)에 있는가?
INT LcMath_PlaneToPointRelation(DPLN* pPn, const VEC3* p1)
{
	FLOAT d = D3DXPlaneDotCoord(pPn, p1);

	// 평면의 아래
	if (d < -LC_ROUNDING_DELTA)
		return -1;
	
	// 평면의 위에
	if (d > LC_ROUNDING_DELTA)
		return 1;
	
	// 평면 안에
	return 0;
}



INT LcMath_PlaneToPointIsFront(const DPLN* pPn, const VEC3* p1, FLOAT fEpsilon)
{
	return (pPn->a * p1->x +
			pPn->b * p1->y +
			pPn->c * p1->z +
			pPn->d + fEpsilon ) > 0.f ? 1: -1;
}




VEC3 LcMath_PlaneMemberPoint(DPLN* pPn)
{
	return -pPn->d * VEC3(pPn->a, pPn->b, pPn->c);
}



//평행하면 Dot product는 1
INT LcMath_PlaneToPlaneExistsInterSect(const DPLN* p1, const DPLN* p2)
{
	FLOAT fD = D3DXVec3Dot(&VEC3(p1->a, p1->b, p1->c), &VEC3(p2->a, p2->b, p2->c));
	return fabsf(fD)>0.9999f? -1: 1;
}

// 두 평면이 교차하는 직선의 방정식 구하기...
// 이것은 의외로 쉽다.
//
// N1 . P + d1 = 0
// N2 . P + d2 = 0
//
// 에서
//
// P = a * N1 + b * N2
//
// 로 표현할 수 있으면 된다.
//
// N1 . P = a * N1 . N1 + b * N1 . N2
// N2 . P = a * N1 . N2 + b * N2 . N2
//
// 나머지는 행렬을 이용하면 끝!!!

INT LcMath_PlaneToPlaneIntersectLine(VEC3* pOutP/*Start Point*/, VEC3* pOutT/*Direction*/, const DPLN* pPn1, const DPLN* pPn2)
{
	VEC3	N1(pPn1->a, pPn1->b, pPn1->c);
	VEC3	N2(pPn2->a, pPn2->b, pPn2->c);
	VEC3	N;
	
	FLOAT f0 = D3DXVec3Length(&N1);
	FLOAT f2 = D3DXVec3Length(&N2);
	
	FLOAT f1 = D3DXVec3Dot(&N1,&N2);
	
	FLOAT D = f0*f2 - f1*f1;
	
	if (fabsf(D) < 0.0001f)
		return -1;
	
	D3DXVec3Cross(&N, &N1, &N2);
	D3DXVec3Normalize(&N, &N);
	*pOutT = N;
	
	D = 1.f / D;
	FLOAT _t0 = (-f2 * pPn1->d + f1 * pPn2->d) * D;
	FLOAT _t1 = (-f0 * pPn2->d + f1 * pPn1->d) * D;
	
	*pOutP = N1*_t0 + N2*_t1;
	
	return 0;
}



FLOAT LcMath_PlaneToPointMinDistance(const FLOAT* pPlane/*VECTOR4*/, const FLOAT* pPoint /*VECTOR3*/)
{
	// plane ==> n * x + d =0
	return pPlane[0] * pPoint[0] + pPlane[1] * pPoint[1] + pPlane[2] * pPoint[2] + pPlane[3];
}

FLOAT LcMath_PlaneToPointMinDistance(const DPLN* pPn, const VEC3* p1)
{
	return D3DXPlaneDotCoord(pPn, p1);
}





INT	LcMath_PointFromPlanes(VEC3* pOut, const DPLN* p1, const DPLN* p2, const DPLN* p3)
{
	FLOAT	X;
	FLOAT	Y;
	FLOAT	Z;
	FLOAT	D;

	MATA	mtX(	-p1->d,	p1->b,	p1->c,	0,
					-p2->d,	p2->b,	p2->c,	0,
					-p3->d,	p3->b,	p3->c,	0,
					0,		0,		0,		1);

	MATA	mtY(	p1->a,	-p1->d,	p1->c,	0,
					p2->a,	-p2->d,	p2->c,	0,
					p3->a,	-p3->d,	p3->c,	0,
					0,		0,		0,		1);

	MATA	mtZ(	p1->a,	p1->b,	-p1->d,	0,
					p2->a,	p2->b,	-p2->d,	0,
					p3->a,	p3->b,	-p3->d,	0,
					0,		0,		0,		1);

	MATA	mtD(	p1->a,	p1->b,	p1->c,	0,
					p2->a,	p2->b,	p2->c,	0,
					p3->a,	p3->b,	p3->c,	0,
					0,		0,		0,		1);

	D = D3DXMatrixDeterminant(&mtD);
	X = D3DXMatrixDeterminant(&mtX);
	Y = D3DXMatrixDeterminant(&mtY);
	Z = D3DXMatrixDeterminant(&mtZ);

	if(0.f == D)
		return -1;

	pOut->x = X/D;
	pOut->y = Y/D;
	pOut->z = Z/D;

	return 0;
}








void LcMath_MatrixRotationRadians(MATA* pOut, const VEC3* pRot)
{
	D3DXMatrixIdentity(pOut);
	
	FLOAT cr = cosf( pRot->x );
	FLOAT sr = sinf( pRot->x );
	FLOAT cp = cosf( pRot->y );
	FLOAT sp = sinf( pRot->y );
	FLOAT cy = cosf( pRot->z );
	FLOAT sy = sinf( pRot->z );
	
	pOut->m[0][0] =  cp*cy;
	pOut->m[0][1] =  cp*sy;
	pOut->m[0][2] =  -sp  ;
	
	FLOAT srsp = sr*sp;
	FLOAT crsp = cr*sp;
	
	pOut->m[1][0] = srsp*cy-cr*sy;
	pOut->m[1][1] = srsp*sy+cr*cy;
	pOut->m[1][2] = sr*cp;
	
	pOut->m[2][0] = crsp*cy+sr*sy;
	pOut->m[2][1] = crsp*sy-sr*cy;
	pOut->m[2][2] = cr*cp;
	
	//	D3DXMatrixRotationX(&mtX, pRot.x);
	//	D3DXMatrixRotationY(&mtY, pRot.y);
	//	D3DXMatrixRotationZ(&mtZ, pRot.z);
	//	*pOut = mtX * mtY * mtZ;
}


void LcMath_MatrixRotationDegrees(MATA* pOut, const VEC3* pRot)
{
	LcMath_MatrixRotationRadians(pOut, &((*pRot) * ONE_DEGtoRAD) );
}



void LcMath_MatrixInverseRotationRadians(MATA* pOut, const VEC3* pRot)
{
	D3DXMatrixIdentity(pOut);
	
	FLOAT cr = cosf( pRot->x );
	FLOAT sr = sinf( pRot->x );
	FLOAT cp = cosf( pRot->y );
	FLOAT sp = sinf( pRot->y );
	FLOAT cy = cosf( pRot->z );
	FLOAT sy = sinf( pRot->z );
	
	pOut->m[0][0] = cp*cy;
	pOut->m[1][0] = cp*sy;
	pOut->m[2][0] = -sp  ;
	
	FLOAT srsp = sr*sp;
	FLOAT crsp = cr*sp;
	
	pOut->m[0][1] = srsp*cy-cr*sy;
	pOut->m[1][1] = srsp*sy+cr*cy;
	pOut->m[2][1] = sr*cp ;
	
	pOut->m[0][2] = crsp*cy+sr*sy;
	pOut->m[1][2] = crsp*sy-sr*cy;
	pOut->m[2][2] = cr*cp ;
}


void LcMath_MatrixInverseRotationDegrees(MATA* pOut, const VEC3* pRot)
{
	LcMath_MatrixInverseRotationRadians(pOut, &((*pRot) * ONE_DEGtoRAD) );
}







//
void LcMath_Vec3RotationDegrees(VEC3* pOut, const MATA* pIn)
{
	FLOAT y = -asinf(pIn->m[2][0]);
	FLOAT D = y;
	FLOAT C = cosf(y);
	y *= ONE_RADtoDEG;
	
	FLOAT rotx, roty, x, z;
	
	if (fabsf(C)>0.0005f)
	{
		rotx = pIn->m[2][2] / C;
		roty = pIn->m[1][2] / C;
		x = atan2f( roty, rotx ) * ONE_RADtoDEG;
		rotx = pIn->m[0][0] / C;
		roty = pIn->m[0][1] / C;
		z = atan2f( roty, rotx ) * ONE_RADtoDEG;
	}
	else
	{
		x  = 0.f;
		rotx = pIn->m[1][1];
		roty =-pIn->m[1][0];
		z  = atan2f( roty, rotx ) * ONE_RADtoDEG;
	}
	
	// fix values that Get below zero
	// before it would set (!) values to 360
	// that where above 360:
	if (x < 0.f) x += 360.f;
	if (y < 0.f) y += 360.f;
	if (z < 0.f) z += 360.f;
	
	pOut->x = x;
	pOut->y = y;
	pOut->z = z;
}


// Matrix Transpose
void LcMath_MatrixTranspose(MATA* pOut, const MATA* pIn)
{
	for (INT i=0; i<4; ++i)
		for (INT j=0; j<4; ++j)
			pOut->m[i][j] = pIn->m[j][i];
}


void LcMath_Vec3Translate(VEC3* pOut, const MATA* pT, const VEC3* pIn)
{
	pOut->x = pIn->x + pT->_41;
	pOut->y = pIn->y + pT->_42;
	pOut->z = pIn->z + pT->_43;
}



void LcMath_Vec3InverseTranslate(VEC3* pOut, const MATA* pT, const VEC3* pIn)
{
	pOut->x = pIn->x - pT->_41;
	pOut->y = pIn->y - pT->_42;
	pOut->z = pIn->z - pT->_43;
}


void LcMath_Vec3Rotate(VEC3* pOut, const MATA* pR, const VEC3* pIn)
{
	VEC3 vcT = *pIn;
	pOut->x = vcT.x*pR->_11 + vcT.y*pR->_21 + vcT.z*pR->_31;
	pOut->y = vcT.x*pR->_12 + vcT.y*pR->_22 + vcT.z*pR->_32;
	pOut->z = vcT.x*pR->_13 + vcT.y*pR->_23 + vcT.z*pR->_33;
}

void LcMath_Vec3InverseRotate(VEC3* pOut, const MATA* pR, const VEC3* pIn)
{
	VEC3 vcT = *pIn;
	pOut->x = vcT.x*pR->_11 + vcT.y*pR->_12 + vcT.z*pR->_13;
	pOut->y = vcT.x*pR->_21 + vcT.y*pR->_22 + vcT.z*pR->_33;
	pOut->z = vcT.x*pR->_31 + vcT.y*pR->_32 + vcT.z*pR->_33;
}


void LcMath_Vec3Transform(VEC3*pOut, const MATA* pT, const VEC3* pIn)
{
	VEC3 vcT = *pIn;
	pOut->x = vcT.x * pT->_11 + vcT.y * pT->_21 + vcT.z * pT->_31 + pT->_41;
	pOut->y = vcT.x * pT->_12 + vcT.y * pT->_22 + vcT.z * pT->_32 + pT->_42;
	pOut->z = vcT.x * pT->_13 + vcT.y * pT->_23 + vcT.z * pT->_33 + pT->_43;
}



void LcMath_PlaneTransform(DPLN*pOut, const MATA* pT, DPLN* pIn)
{
	VEC3 origin(0,0,0);
	VEC3 member=LcMath_PlaneMemberPoint(pIn);
	
	LcMath_Vec3Transform(&origin, pT, &origin);
	LcMath_Vec3Transform(&member, pT, &member);
	
	LcMath_Vec3Transform((VEC3*)&pOut->a, pT, (VEC3*)&pIn->a);
	pOut->a -= origin.x;
	pOut->b -= origin.y;
	pOut->c -= origin.z;
	
	pOut->d = - D3DXPlaneDotNormal(pOut, &member);
}



void LcMath_Vec4Transform(VEC4*pOut, const MATA* pT, const VEC4* pIn)
{
	pOut->x = pIn->x*pT->_11 + pIn->y*pT->_21 + pIn->z*pT->_31 + pIn->w*pT->_41;
	pOut->y = pIn->x*pT->_12 + pIn->y*pT->_22 + pIn->z*pT->_32 + pIn->w*pT->_42;
	pOut->z = pIn->x*pT->_13 + pIn->y*pT->_23 + pIn->z*pT->_33 + pIn->w*pT->_43;
	pOut->w = pIn->x*pT->_14 + pIn->y*pT->_24 + pIn->z*pT->_34 + pIn->w*pT->_44;
}







// Triangle
//////////////////////////////////////////////////////////////////////////////



void	LcMath_TriTransform	(LcTri* pOut,const MATA* pT,const LcTri* pIn)
{
	LcTri Tri = *pIn;
	LcMath_Vec3Transform(&pOut->_0, pT, &Tri._0);
	LcMath_Vec3Transform(&pOut->_1, pT, &Tri._1);
	LcMath_Vec3Transform(&pOut->_2, pT, &Tri._2);
}

void	LcMath_TriMove(LcTri* pOut,const VEC3* pV,const LcTri* pIn)
{
	LcTri Tri = *pIn;

	pOut->_0 = *pV + Tri._0;
	pOut->_1 = *pV + Tri._1;
	pOut->_2 = *pV + Tri._2;
}













void LcMath_CubeHMove(LcCubeH* pOut,const VEC3* pV,const LcCubeH* pIn)
{
	pOut->_0	= *pV + pIn->_0;
	pOut->_1	= *pV + pIn->_1;
	pOut->_2	= *pV + pIn->_2;
	pOut->_3	= *pV + pIn->_3;

	pOut->_4	= *pV + pIn->_4;
	pOut->_5	= *pV + pIn->_5;
	pOut->_6	= *pV + pIn->_6;
	pOut->_7	= *pV + pIn->_7;
}


void LcMath_CubeHTransform(LcCubeH* pOut,const MATA* pT, const LcCubeH* pIn)
{
	int i=0;

	LcCubeH pCubeT = *pIn;

	for(i=0; i<8; ++i)
		LcMath_Vec3Transform((VEC3*)&pOut->m[i], pT, (VEC3*)&pCubeT.m[i]);
}



void LcMath_CubeHBoxLine(void* pOut, INT nSizeChunk,const LcCubeH* pIn)
{
	*( (VEC3*) ((char*)(pOut) +  0 * nSizeChunk)  ) = pIn->_1;
	*( (VEC3*) ((char*)(pOut) +  1 * nSizeChunk)  ) = pIn->_0;
	*( (VEC3*) ((char*)(pOut) +  2 * nSizeChunk)  ) = pIn->_2;
	*( (VEC3*) ((char*)(pOut) +  3 * nSizeChunk)  ) = pIn->_0;
	*( (VEC3*) ((char*)(pOut) +  4 * nSizeChunk)  ) = pIn->_1;
	*( (VEC3*) ((char*)(pOut) +  5 * nSizeChunk)  ) = pIn->_3;
	*( (VEC3*) ((char*)(pOut) +  6 * nSizeChunk)  ) = pIn->_2;
	*( (VEC3*) ((char*)(pOut) +  7 * nSizeChunk)  ) = pIn->_3;
	
	*( (VEC3*) ((char*)(pOut) +  8 * nSizeChunk)  ) = pIn->_5;
	*( (VEC3*) ((char*)(pOut) +  9 * nSizeChunk)  ) = pIn->_4;
	*( (VEC3*) ((char*)(pOut) + 10 * nSizeChunk)  ) = pIn->_6;
	*( (VEC3*) ((char*)(pOut) + 11 * nSizeChunk)  ) = pIn->_4;
	*( (VEC3*) ((char*)(pOut) + 12 * nSizeChunk)  ) = pIn->_5;
	*( (VEC3*) ((char*)(pOut) + 13 * nSizeChunk)  ) = pIn->_7;
	*( (VEC3*) ((char*)(pOut) + 14 * nSizeChunk)  ) = pIn->_6;
	
	*( (VEC3*) ((char*)(pOut) + 15 * nSizeChunk)  ) = pIn->_7;
	*( (VEC3*) ((char*)(pOut) + 16 * nSizeChunk)  ) = pIn->_0;
	*( (VEC3*) ((char*)(pOut) + 17 * nSizeChunk)  ) = pIn->_4;
	*( (VEC3*) ((char*)(pOut) + 18 * nSizeChunk)  ) = pIn->_1;
	*( (VEC3*) ((char*)(pOut) + 19 * nSizeChunk)  ) = pIn->_5;
	*( (VEC3*) ((char*)(pOut) + 20 * nSizeChunk)  ) = pIn->_2;
	*( (VEC3*) ((char*)(pOut) + 21 * nSizeChunk)  ) = pIn->_6;
	*( (VEC3*) ((char*)(pOut) + 22 * nSizeChunk)  ) = pIn->_3;
	*( (VEC3*) ((char*)(pOut) + 23 * nSizeChunk)  ) = pIn->_7;
}


void LcMath_CubeHFromA(LcCubeH* pOut,const LcCubeAA* pIn)
{
	VEC3	vcP = pIn->vcMin;
	VEC3	vcT = pIn->vcMax - pIn->vcMin;
	FLOAT	w = vcT.x;
	FLOAT	h = vcT.y;
	FLOAT	d = vcT.z;

	pOut->_0	= vcP + VEC3(0, 0, 0);
	pOut->_1	= vcP + VEC3(0, h, 0);
	pOut->_2	= vcP + VEC3(w, 0, 0);
	pOut->_3	= vcP + VEC3(w, h, 0);

	pOut->_4	= vcP + VEC3(0, 0, d);
	pOut->_5	= vcP + VEC3(0, h, d);
	pOut->_6	= vcP + VEC3(w, 0, d);
	pOut->_7	= vcP + VEC3(w, h, d);
}



void LcMath_CubeAMove(LcCubeAA* pOut,const VEC3* pV,const LcCubeAA* pIn)
{
	pOut->vcMin	= *pV + pIn->vcMin;
	pOut->vcMax	= *pV + pIn->vcMax;
}



void LcMath_CubeATransform(LcCubeAA*pOut,const MATA* pT,const LcCubeAA* pIn)
{
	LcCubeAA CubeT = *pIn;
	LcMath_Vec3Transform(&pOut->vcMin, pT, &CubeT.vcMin);
	LcMath_Vec3Transform(&pOut->vcMax, pT, &CubeT.vcMax);
	pOut->Repair();
}


INT LcMath_CubeAToLine(VEC3* vcOut, LcCubeAA* pCube, LcLine* pLine, BOOL bPositiveDirection)
{
	VEC3 pVcBox[2];

	pVcBox[0] = pCube->vcMin;
	pVcBox[1] = pCube->vcMax;

	return LcMath_CubeAToLine(vcOut, pVcBox, pLine, bPositiveDirection);
}



INT LcMath_CubeAToLine(VEC3* vcOut, VEC3* pVcBox, LcLine* pLine, BOOL bPositiveDirection)
{
	BOOL	hr= -1;
	FLOAT	w;
	FLOAT	h;
	FLOAT	d;

	VEC3	vcP[8];
	VEC3	vcT;
	LcTri	Tri;
	VEC3	vcBuf[3];
	INT		nCollision = FALSE;
	BOOL	bRectangle = TRUE;
	
	vcP[0] = pVcBox[0];
	vcP[7] = pVcBox[1];

	vcT = vcP[7]- vcP[0];
	w = vcT.x;
	h = vcT.y;
	d = vcT.z;

	vcP[1] = vcP[0] + VEC3(w, 0, 0);
	vcP[2] = vcP[0] + VEC3(0, h, 0);
	vcP[3] = vcP[0] + VEC3(w, h, 0);
	vcP[4] = vcP[0] + VEC3(0, 0, d);
	vcP[5] = vcP[0] + VEC3(w, 0, d);
	vcP[6] = vcP[0] + VEC3(0, h, d);

	

	static const int nIdxTri[][3] =
	{
		{ 0, 1, 2},
		{ 2, 3, 6},
		{ 6, 7, 4},
		{ 4, 5, 0},
		
		{ 1, 5, 3},
		{ 4, 0, 6},
	};


	for(int i=0; i<6; ++i)
	{
		Tri.Set(vcP[ nIdxTri[i][0] ], vcP[ nIdxTri[i][1] ], vcP[ nIdxTri[i][2] ]);
		
		hr = LcMath_LineToTriangle(&vcT, (VEC3*)&Tri, pLine, bRectangle, bPositiveDirection);

		if(SUCCEEDED(hr))
		{
			vcBuf[nCollision] = vcT;
			++nCollision;
		}

		if(nCollision>=2)
			break;
	}

	if(1 == nCollision)
	{
		*vcOut = vcBuf[0];
		return nCollision;
	}

	else if(2 == nCollision)
	{
		FLOAT fD1 = D3DXVec3LengthSq( &(vcBuf[0] - pLine->p));
		FLOAT fD2 = D3DXVec3LengthSq( &(vcBuf[1] - pLine->p));

		if(fD1<fD2)
		{
			vcOut[0] = vcBuf[0];
			vcOut[1] = vcBuf[1];
		}
		else
		{
			vcOut[0] = vcBuf[1];
			vcOut[1] = vcBuf[0];
		}

		return nCollision;
	}


	return -1;
}



void LcMath_CubeABoxLine(void* pOut, INT nSizeChunk,const LcCubeAA* pIn)
{
	LcCubeH		pHexa;
	LcMath_CubeHFromA(&pHexa, pIn);

	*( (VEC3*) ((char*)(pOut) +  0 * nSizeChunk)  ) = pHexa._1;
	*( (VEC3*) ((char*)(pOut) +  1 * nSizeChunk)  ) = pHexa._0;
	*( (VEC3*) ((char*)(pOut) +  2 * nSizeChunk)  ) = pHexa._2;
	*( (VEC3*) ((char*)(pOut) +  3 * nSizeChunk)  ) = pHexa._0;
	*( (VEC3*) ((char*)(pOut) +  4 * nSizeChunk)  ) = pHexa._1;
	*( (VEC3*) ((char*)(pOut) +  5 * nSizeChunk)  ) = pHexa._3;
	*( (VEC3*) ((char*)(pOut) +  6 * nSizeChunk)  ) = pHexa._2;
	*( (VEC3*) ((char*)(pOut) +  7 * nSizeChunk)  ) = pHexa._3;
	
	*( (VEC3*) ((char*)(pOut) +  8 * nSizeChunk)  ) = pHexa._5;
	*( (VEC3*) ((char*)(pOut) +  9 * nSizeChunk)  ) = pHexa._4;
	*( (VEC3*) ((char*)(pOut) + 10 * nSizeChunk)  ) = pHexa._6;
	*( (VEC3*) ((char*)(pOut) + 11 * nSizeChunk)  ) = pHexa._4;
	*( (VEC3*) ((char*)(pOut) + 12 * nSizeChunk)  ) = pHexa._5;
	*( (VEC3*) ((char*)(pOut) + 13 * nSizeChunk)  ) = pHexa._7;
	*( (VEC3*) ((char*)(pOut) + 14 * nSizeChunk)  ) = pHexa._6;
	
	*( (VEC3*) ((char*)(pOut) + 15 * nSizeChunk)  ) = pHexa._7;
	*( (VEC3*) ((char*)(pOut) + 16 * nSizeChunk)  ) = pHexa._0;
	*( (VEC3*) ((char*)(pOut) + 17 * nSizeChunk)  ) = pHexa._4;
	*( (VEC3*) ((char*)(pOut) + 18 * nSizeChunk)  ) = pHexa._1;
	*( (VEC3*) ((char*)(pOut) + 19 * nSizeChunk)  ) = pHexa._5;
	*( (VEC3*) ((char*)(pOut) + 20 * nSizeChunk)  ) = pHexa._2;
	*( (VEC3*) ((char*)(pOut) + 21 * nSizeChunk)  ) = pHexa._6;
	*( (VEC3*) ((char*)(pOut) + 22 * nSizeChunk)  ) = pHexa._3;
	*( (VEC3*) ((char*)(pOut) + 23 * nSizeChunk)  ) = pHexa._7;
}







INT LcMath_CubeHToSphere(const LcCubeH* pHexa, const VEC3* pPoint,  /*In*/ FLOAT fRadius)
{
	INT		i=0, j=0;
	BOOL	bColl = FALSE;

	// 2. 두 육면체의 점이 각각 전부 안에 있는가 혹은 밖에 있는가?

	static const int nIdxNrm[][3] =
	{
		{ 0, 1, 2},
		{ 2, 3, 6},
		{ 6, 7, 4},

		{ 4, 5, 0},
		{ 1, 5, 3},
		{ 4, 0, 6},
	};

	D3DXPLANE	Plane1[6];
	INT			hr[6];


	for(i=0;i<6; ++i)
	{
		LcMath_PlaneSet(	&Plane1[i]
					, (VEC3*)&pHexa->m[ nIdxNrm[i][0] ]
					, (VEC3*)&pHexa->m[ nIdxNrm[i][1] ]
					, (VEC3*)&pHexa->m[ nIdxNrm[i][2] ]);
	}



	for(i=0;i<6; ++i)
	{
		hr[i] = LcMath_PlaneToPointIsFront(&Plane1[i], pPoint, -fRadius);
	}

	if( hr[0]<0 &&
		hr[1]<0 &&
		hr[2]<0 &&
		hr[3]<0 &&
		hr[4]<0 &&
		hr[5]<0
		)
	{
		bColl = TRUE;
	}

	if( TRUE == bColl)
		return 0;

	return -1;
}






INT LcMath_CubeHToTri(const LcCubeH* pHexa, const LcTri* pTri)
{
	INT		i=0;
	BOOL	bCollision = FALSE;

	
	// 2. 두 육면체의 점이 각각 전부 안에 있는 가 혹은 밖에 있는가?

	for(i=0; i<3; ++i)
	{
		VEC3	vcP = pTri->m[i];

		bCollision = LcMath_CubeHToSphere(pHexa, &vcP, 0);

		if( TRUE == bCollision)
			return 0;
	}


	// 3. 두 육면체가 교차하지만 포함하는 점이 없다.
	// 삼각형 vs 삼각형 충돌로 검사.

	const static int nIdxTri[][3] =
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


	for(i=0; i<12; ++i)
	{
		VEC3	pTri1[3];
		VEC3	pOut[3];


		pTri1[0] = pHexa->m[nIdxTri[i][0] ];
		pTri1[1] = pHexa->m[nIdxTri[i][1] ];
		pTri1[2] = pHexa->m[nIdxTri[i][2] ];


		INT hr = LcMath_TriangleToTriangle(pOut, pTri1, (VEC3*)pTri);

		if(SUCCEEDED(hr))
		{
			bCollision = TRUE;
			break;
		}
	}

	if(TRUE == bCollision)
		return 0;


	return -1;

}







INT LcMath_CubeHToH(const LcCubeH* pHexagon1, const LcCubeH* pHexagon2)
{
	INT		i=0, j=0;
	BOOL	bCollision = FALSE;

	// 1. 최대 Bound Sphere로 체크

	FLOAT	fBnd1 = 0.f;
	FLOAT	fBnd2 = 0.f;
	VEC3	vcCenter1(0,0,0);
	VEC3	vcCenter2(0,0,0);

	VEC3	vcT;
	FLOAT	fB=0;


	for(i=0;i<8; ++i)
	{
		vcCenter1 += pHexagon1->m[i];
		vcCenter2 += pHexagon2->m[i];
	}

	vcCenter1 /= 8.f;
	vcCenter2 /= 8.f;


	for(i=0;i<8; ++i)
	{
		vcT = vcCenter1 - pHexagon1->m[i];
		fB = D3DXVec3Length(&vcT);

		if(fB>fBnd1)
			fBnd1 = fB;


		vcT = vcCenter2 - pHexagon2->m[i];
		fB = D3DXVec3Length(&vcT);

		if(fB>fBnd2)
			fBnd2 = fB;
	}


	vcT = vcCenter1 - vcCenter2;
	fB = D3DXVec3Length(&vcT);

	// 박스의 중심 거리와 바운드 구와의 비교

	if(fB>=(fBnd1 + fBnd2))
		return -1;




	// 2. 두 육면체의 점이 각각 전부 안에 있는 가 혹은 밖에 있는가?

	static const int nIdxNrm[][3] =
	{
		{ 0, 1, 2},
		{ 2, 3, 6},
		{ 6, 7, 4},

		{ 4, 5, 0},
		{ 1, 5, 3},
		{ 4, 0, 6},
	};

	D3DXPLANE	Plane1[6];
	D3DXPLANE	Plane2[6];
	INT			hr[6];


	for(i=0;i<6; ++i)
	{
		LcMath_PlaneSet(	&Plane1[i]
					, (VEC3*)&pHexagon1->m[ nIdxNrm[i][0] ]
					, (VEC3*)&pHexagon1->m[ nIdxNrm[i][1] ]
					, (VEC3*)&pHexagon1->m[ nIdxNrm[i][2] ]);


		LcMath_PlaneSet(	&Plane2[i]
					, (VEC3*)&pHexagon2->m[ nIdxNrm[i][0] ]
					, (VEC3*)&pHexagon2->m[ nIdxNrm[i][1] ]
					, (VEC3*)&pHexagon2->m[ nIdxNrm[i][2] ]);
	}



	for(j=0; j<8; ++j)
	{
		for(i=0;i<6; ++i)
		{
			hr[i] = LcMath_PlaneToPointIsFront(&Plane1[i], (VEC3*)&pHexagon2->m[j]);
		}

		if( hr[0]<0 &&
			hr[1]<0 &&
			hr[2]<0 &&
			hr[3]<0 &&
			hr[4]<0 &&
			hr[5]<0
			)
		{
			bCollision = TRUE;
			break;
		}
	}

	if( TRUE == bCollision)
		return 0;


	for(j=0; j<8; ++j)
	{
		for(i=0;i<6; ++i)
		{
			hr[i] = LcMath_PlaneToPointIsFront(&Plane2[i], (VEC3*)&pHexagon1->m[j]);
		}

		if( hr[0]<0 &&
			hr[1]<0 &&
			hr[2]<0 &&
			hr[3]<0 &&
			hr[4]<0 &&
			hr[5]<0
			)
		{
			bCollision = TRUE;
			break;
		}
	}

	if( TRUE == bCollision)
		return 0;



	// 3. 두 육면체가 교차하지만 포함하는 점이 없다.
	// 삼각형 vs 삼각형 충돌로 검사.

	static const int nIdxTri[][3] =
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


	for(i=0; i<12; ++i)
	{
		VEC3	pTri1[3];
		VEC3	pTri2[3];

		VEC3	pOut[3];


		pTri1[0] = pHexagon1->m[nIdxTri[i][0] ];
		pTri1[1] = pHexagon1->m[nIdxTri[i][1] ];
		pTri1[2] = pHexagon1->m[nIdxTri[i][2] ];


		for(j=0; j<12; ++j)
		{
			pTri2[0] = pHexagon2->m[nIdxTri[j][0] ];
			pTri2[1] = pHexagon2->m[nIdxTri[j][1] ];
			pTri2[2] = pHexagon2->m[nIdxTri[j][2] ];

			INT hr = LcMath_TriangleToTriangle(pOut, pTri1, pTri2);

			if(SUCCEEDED(hr))
				bCollision = TRUE;
		}

		if(TRUE == bCollision)
			break;
	}

	if(TRUE == bCollision)
		return 0;


	return -1;

}










INT LcMath_CylinderToLineIntersection(	VEC3* pOut,	const LcCylinder* pV1, const LcLine* pV2, const VEC3* pV3)
{
	INT iInsc=0;

	VEC3 vcInsc[2];			// 실린더 중심선과 직선이 교차하는 지점
	FLOAT	fD=0.f;					// 실린더와 직선의 거리
	FLOAT	fR=pV1->r;				// 실린더의 반지름
	FLOAT	fA=0.f;					// 장반경
	VEC3	vcL1 = pV2->t;
	VEC3	vcL2 = pV1->t;
	FLOAT	fSinTheta=0.f;

	iInsc = LcMath_LineToLineIntersection(&fSinTheta, &fD, vcInsc, pV2, (LcLine*)pV1);

	// 부정이나 불능 두가지 중 하나.
	if(FAILED(iInsc))
	{
		// 원통의 반경과 비교한다.
		// 직선이 원통 안에 완전히 있다.
		if(fD <= pV1->r)
			return 0;	// 점을 정할 수가 없다.

		// 직선이 원통 바깥에 있다.
		else
			return -1;
	}

	// 직선이 원통 바깥에 있다.
	if(fabsf(fD)>=fR)
		return -1;


	// 충돌한 두 점의 위치를 찾는다.
	fA = fR/fabsf(fSinTheta);

	FLOAT fPi = fD/fR;
	fPi = asinf(fPi);

	FLOAT fK = fA * cosf( fPi);

	pOut[0] = vcInsc[0] - fK * vcL1;
	pOut[1] = vcInsc[0] + fK * vcL1;

	
	if(NULL == pV3)
		return 2;


	//
	FLOAT fDot[2];

	VEC3 vcDot0;
	VEC3 vcDot1;

	vcDot0 = pV3[0] - pOut[0];
	vcDot1 = pV3[1] - pOut[0];
	fDot[0] = D3DXVec3Dot(&vcDot0, &vcDot1);

	vcDot0 = pV3[0] - pOut[1];
	vcDot1 = pV3[1] - pOut[1];
	fDot[1] = D3DXVec3Dot(&vcDot0, &vcDot1);


	// 0번째만 영역 안에 있다.
	if( fDot[0] <0 && fDot[1] >0)
	{
		return 0;
	}

	// 1번째만 영역 안에 있다.
	else if( fDot[0] >0 && fDot[1] <0)
	{
		return 1;
	}

	// 둘다 영역 안에 있다.
	else if( fDot[0] <0 && fDot[1] <0)
	{
		return 2;
	}



	return -1;
}





// 원통과 삼각형의 충돌
// 삼각형이 원통안에 있으면 삼ㄱ각형과 원통의 최단거리 지점을 반환
// 원통의 중심선이 삼각형과 교차하면 교차 지점을 반환
// 나머지... 삼각형과 원통의 충돌 지점을 반환
INT LcMath_CylinderToTriIntersection(	VEC3* pOut				// Intersection point
									  ,	const LcCylinder* pV1	// Input Cylinder
									  , const VEC3* pV2			// Input Triangle
									  )
{

	// 1. 충돌하지 않는 삼각형들은 제외시키낟.
	// 간단하게 삼각형을 둘러싸고 있는 구와 원통의 충돌을 구한다.
	// 원통 반지름 + 삼각형 충돌 반지름  < 원통와 삼각형의 최단 거리 이면 충돌이 아님.

	VEC3	TriCenter;
	FLOAT	TriRadius = LcMath_TriangleBoundSphere(&TriCenter, pV2);
	VEC3	pTriToSphere(0,0,0);

	// 원통과 삼각형 중심의 최단거리
	FLOAT	fDstLineFromPoint;

	LcMath_LineToSphere(NULL, &fDstLineFromPoint, (LcLine*)pV1, &TriCenter, TriRadius);

	if( fDstLineFromPoint>= (TriRadius + pV1->r))
		return -1;


	// 2. 삼각형이 원통 내부에 있을 경우
	// 완전히 있다면 충돌이며 중돌 포인트는 삼각형의 중심으로 한다.

	if(TriRadius<pV1->r)
	{
		// 삼각형이 원통 안에 완전히 있는가?
		if(fDstLineFromPoint<= (pV1->r - TriRadius))
		{
			*pOut = TriCenter;
			return 0;
		}
	}


	// 나머지 충돌은 두개가 남아 있다. 하나는 원통의 중심 직선이
	// 삼각형과 충돌 여부,  다른 하나는 삼각형의 세 변과 원통이 충돌하는 경우다.


	// 3. 원통의 중심 직선이 삼각형과 충돌 하는가?
	if(SUCCEEDED(LcMath_LineToTriangle(pOut, pV2, (LcLine*)pV1)))
		return 0;


	// 4. 나머지...
	// 삼각형을 이루고 있는 세 유한 직선이 원동과 충돌하는 지점을 구한다.

	VEC3	vcCol[6];
	INT		nCnt=-1;

	for(int i=0; i<3; ++i)
	{
		VEC3	vcOutT[2];
		INT		hr;
		VEC3	vcBnd[2];
		LcLine	Line;


		INT		nTriIdx0 = (i+0)%3;
		INT		nTriIdx1 = (i+1)%3;

		vcBnd[0] = pV2[nTriIdx0];
		vcBnd[1] = pV2[nTriIdx1];

		Line.p = vcBnd[0];
		Line.t = vcBnd[1] - vcBnd[0];
		D3DXVec3Normalize(&Line.t, &Line.t);

		hr = LcMath_CylinderToLineIntersection(vcOutT, pV1, &Line, vcBnd);

		if(FAILED(hr))
			continue;

		if(0 == hr)
		{
			++nCnt;
			vcCol[nCnt] = vcOutT[0];
		}

		else if(  1== hr)
		{
			++nCnt;
			vcCol[nCnt] = vcOutT[1];
		}

		else if( 2 == hr)
		{
			++nCnt;
			vcCol[nCnt] = vcOutT[0];

			++nCnt;
			vcCol[nCnt] = vcOutT[1];
		}
	}

	if(nCnt>=0)
	{
		memcpy(pOut, vcCol, (nCnt+1) * sizeof (VEC3));
		return (nCnt+1);
	}

	return -1;
}




// 원통과 삼각형의 충돌
// 삼각형이 원통안에 있으면 삼각형과 원통의 최단거리 지점을 반환
// 원통의 중심선이 삼각형과 교차하면 교차 지점을 반환
// 나머지... 삼각형과 원통의 충돌 지점을 반환
INT LcMath_CylinderToTriIntersection(	VEC3* pOut				// Intersection point
									,	const LcCylinder* pV1	// Input Cylinder
									,	BndTri* pV2				// Input Bound Triangle
									,	BOOL bRectangle			// 충돌영역을 삼각형의 확장 평행사변형으로 쓸것인지..
									,	BOOL bPositiveDirection	// 직선의 방향으로만 충돌점을 구하기
									,	const VEC3* pBoundLine	// 유한 직선인 경우 양 끝 지점
									,	const VEC3* pTriNormal	// 삼각형의 Normal Vector
									)
{

	// 1. 충돌하지 않는 삼각형들은 제외시키낟.
	// 간단하게 삼각형을 둘러싸고 있는 구와 원통의 충돌을 구한다.
	// 원통 반지름 + 삼각형 충돌 반지름  < 원통와 삼각형의 최단 거리 이면 충돌이 아님.

	VEC3	TriCenter = pV2->GetBoundCenter();
	FLOAT	TriRadius = pV2->GetBoundRadius();
	VEC3	pTriToSphere(0,0,0);

	// 원통과 삼각형 중심의 최단거리
	FLOAT	fDstLineFromPoint;

	LcMath_LineToSphere(NULL, &fDstLineFromPoint, (LcLine*)pV1, &TriCenter, TriRadius);

	if( fDstLineFromPoint>= (TriRadius + pV1->r))
		return -1;


	// 2. 삼각형이 원통 내부에 있을 경우
	// 완전히 있다면 충돌이며 중돌 포인트는 삼각형의 중심으로 한다.

	if(TriRadius<pV1->r)
	{
		// 삼각형이 원통 안에 완전히 있는가?
		if(fDstLineFromPoint<= (pV1->r - TriRadius))
		{
			*pOut = TriCenter;
			return 0;
		}
	}


	// 나머지 충돌은 두개가 남아 있다. 하나는 원통의 중심 직선이
	// 삼각형과 충돌 여부,  다른 하나는 삼각형의 세 변과 원통이 충돌하는 경우다.


	// 3. 원통의 중심 직선이 삼각형과 충돌 하는가?
	if(SUCCEEDED(LcMath_LineToTriangle(pOut, &pV2->p0, (LcLine*)pV1, bRectangle, bPositiveDirection, pBoundLine, pTriNormal)))
		return 0;


	// 4. 나머지...
	// 삼각형을 이루고 있는 세 유한 직선이 원동과 충돌하는 지점을 구한다.

	VEC3	vcCol[6];
	INT		nCnt=-1;

	for(int i=0; i<3; ++i)
	{
		VEC3	vcOutT[2];
		INT		hr;
		VEC3	vcBnd[2];
		LcLine	Line;


		INT		nTriIdx0 = (i+0)%3;
		INT		nTriIdx1 = (i+1)%3;

		vcBnd[0] = pV2->vcT3[nTriIdx0];
		vcBnd[1] = pV2->vcT3[nTriIdx1];

		Line.p = vcBnd[0];
		Line.t = vcBnd[1] - vcBnd[0];
		D3DXVec3Normalize(&Line.t, &Line.t);

		hr = LcMath_CylinderToLineIntersection(vcOutT, pV1, &Line, vcBnd);

		if(FAILED(hr))
			continue;

		if(0 == hr)
		{
			++nCnt;
			vcCol[nCnt] = vcOutT[0];
		}

		else if(  1== hr)
		{
			++nCnt;
			vcCol[nCnt] = vcOutT[1];
		}

		else if( 2 == hr)
		{
			++nCnt;
			vcCol[nCnt] = vcOutT[0];

			++nCnt;
			vcCol[nCnt] = vcOutT[1];
		}
	}

	if(nCnt>=0)
	{
		memcpy(pOut, vcCol, (nCnt+1) * sizeof (VEC3));

		// 다른 것과 구분하기 위해서 <<2한다.
		return (nCnt+1)<<2;
	}

	return -1;
}




INT	LcMath_UVFromTri(	FLOAT* fU
					,	FLOAT* fV
					,	const VEC3* p
					,	const VEC3* p0
					,	const VEC3* p1
					,	const VEC3* p2)
{
	VEC3	vcA = *p1 - *p0;
	VEC3	vcB = *p2 - *p0;
	VEC3	vcP = *p  - *p0;

	FLOAT	fPA = D3DXVec3Dot( &vcA, &vcP);
	FLOAT	fPB = D3DXVec3Dot( &vcB, &vcP);
	FLOAT	fAB = D3DXVec3Dot( &vcA, &vcB);
	FLOAT	fAA = D3DXVec3Dot( &vcA, &vcA);
	FLOAT	fBB = D3DXVec3Dot( &vcB, &vcB);

	FLOAT fD = fAA*fBB - fAB*fAB;

	if(fabsf(fD)<0.00001f)
		return -1;

	*fU = (fPA*fBB - fPB*fAB)/fD;
	*fV = (fPB*fAA - fPA*fAB)/fD;

	return 0;
}



INT LcMath_SphereToTri(		VEC3* pOut					// 구와 삼각형의 최단거리의 지점
						,	FLOAT* fDist				// 구와 삼각형의 최단거리
						,	const VEC3* pSphereCenter	// 구의 중앙
						,	const FLOAT* pSphereRadius	// 구의 반경
						,	const VEC3* pTri			// 삼각형의 세점
						,	VEC3* pTriCenter			// 삼각형의 바운드구의 중심
						,	FLOAT* pTriRadius			// 삼각형의 바운드구의 반경
						,	VEC3* pTriNormal			// 삼각형의 노말멕터
						)
{
	VEC3	vTbndR;
	FLOAT	fTbndR;

	int		i=0;

	// 1. 삼각형의 충돌 바운드가 있다면..
	if(NULL == pTriCenter)
	{
		fTbndR = LcMath_TriangleBoundSphere(&vTbndR, pTri);
	}
	else
	{
		vTbndR	= *pTriCenter;
		fTbndR	= *pTriRadius;
	}

	VEC3	vcR = vTbndR - *pSphereCenter;
	FLOAT	fRadiusA = D3DXVec3LengthSq( &vcR);


	FLOAT fRadius12 = *pSphereRadius + fTbndR;

	fRadius12 = fRadius12 * fRadius12;

	if(fRadius12 < fRadiusA)
	{
		return -1;
	}


	// 1. 삼각형 만드는 평면에서 구의 중심까지의 최단거리를 만드는 지점과 거리를 구한다.
	// 최단거리가 구의 반경보다 크면 충돌을 안함.
	DPLN plane;
	LcMath_PlaneSet(&plane, &pTri[0], &pTri[1], &pTri[2]);

	*fDist = LcMath_PlaneToPointMinDistance((FLOAT*)&plane, (FLOAT*)pSphereCenter);

	if( fRadiusA < (*fDist) * (*fDist))
		return -1;

	FLOAT k = D3DXVec3Dot( (VEC3*)&plane, &vcR);

	*pOut = *pSphereCenter + k *  *((VEC3*)&plane);






	FLOAT fU, fV;

	if(FAILED(LcMath_UVFromTri(&fU, &fV, pOut, &pTri[0], &pTri[1], &pTri[2])))
		return -1;

	// 2. 앞서 구한 지점이 삼각형 내부에 있다면 무조건 충돌.
	if(fU>=-0.0001f && fU<=1.0001f && fV>=-0.0001f && fV<=1.0001f && (fU+fV)<1.0001f)
		return 0;

	// 3. 삼각형의 세 점우 하나라도 구 안에 있는가 검사.
	for(i=0; i<3; ++i)
	{
		VEC3	vcTc = pTri[i] - *pSphereCenter;
		FLOAT	fTr	= D3DXVec3LengthSq( &vcTc);

		if( fTr< (*pSphereRadius) * (*pSphereRadius))
			return 0;
	}


	// 4. 나머지는 삼각형이 구에 걸치는 형태
	// 앞서 구한 지점이 삼각형 외부에 존재한다면 삼각형이 만드는 세직선과 구의 충돌을 구한다.

	for(i=0; i<3; ++i)
	{
		VEC3 vcTstOut;
		FLOAT fTstDst;
		LcLine pLine;

		INT n0 = (i+0)%3;
		INT n1 = (i+1)%3;

		pLine.p = pTri[n0];
		pLine.t = pTri[n1] - pTri[n0];

		D3DXVec3Normalize(&pLine.t, &pLine.t);

		// 구와 충돌한 지점을 구하고...
		if(SUCCEEDED(LcMath_LineToSphere(&vcTstOut, &fTstDst, &pLine, pSphereCenter, *pSphereRadius)))
		{
			// 이 점이 유한 직선 안에 있는가 검사.

			VEC3 vcTA = pTri[n0] - vcTstOut;
			VEC3 vcTB = pTri[n1] - vcTstOut;

			FLOAT fTst = D3DXVec3Dot(&vcTA, &vcTB);

			//만약 안에 있다면 fTst<0이어야 한다.
			if(fTst<0.f)
				return 0;

		}
	}



	return -1;
}




INT LcMath_SphereToSphere(	const VEC3* pSphereCenter1							// 구의 중앙1
						,	const FLOAT* pSphereRadius1							// 구의 반경1
						,	const VEC3* pSphereCenter2							// 구의 중앙2
						,	const FLOAT* pSphereRadius2							// 구의 반경2
						)
{
	FLOAT fDst;
	VEC3 vcAB = *pSphereCenter2 - *pSphereCenter1;
	fDst = D3DXVec3Length(&vcAB);

	if( fDst < ( *pSphereRadius1 + *pSphereRadius2))
		return 0;

	return -1;
}




BOOL LcMath_RcCollision(LcRc* v1, LcRc* v2)
{
	return (v1->x0 <= v2->x1 && v1->x1 >= v2->x0 &&
			v1->y0 <= v2->y1 && v1->y1 >= v2->y0);
}









////////////////////////////////////////////////////////////////////////////////
// Color
////////////////////////////////////////////////////////////////////////////////


DWORD	LcColor_GetAlpha(DWORD c)	{	return (c>>24) & 0xff;	}
DWORD	LcColor_GetRed(DWORD c)		{	return (c>>16) & 0xff;	}
DWORD	LcColor_GetGreen(DWORD c)	{	return (c>>8) & 0xff;	}
DWORD	LcColor_GetBlue(DWORD c)	{	return c & 0xff;		}

void	LcColor_SetAlpha(DWORD& c, DWORD a)		{ c = ((a & 0xff)<<24) | (((c>>16)& 0xff)<<16) | ((c>>8 & 0xff)<<8) | (c & 0xff); }
void	LcColor_SetRed(DWORD& c,DWORD r)		{ c = (((c>>24) & 0xff)<<24) | ((r & 0xff)<<16) | ((c>>8 & 0xff)<<8) | (c & 0xff); }
void	LcColor_SetGreen(DWORD& c,DWORD g)		{ c = (((c>>24) & 0xff)<<24) | (((c>>16)& 0xff)<<16) | ((g & 0xff)<<8) | (c & 0xff); }
void	LcColor_SetBlue(DWORD& c,DWORD b)		{ c = (((c>>24) & 0xff)<<24) | (((c>>16)& 0xff)<<16) | ((c>>8 & 0xff)<<8) | (b & 0xff); }

WORD	LcColor_RGB16(INT r,INT g,INT b)		{	return (((r>>3) & 0x1F)<<10) | (((g>>3) & 0x1F)<<5) | ((b>>3) & 0x1F);							}
WORD	LcColor_RGBA16(INT r,INT g,INT b,INT a)	{	return  (((a>>7) & 0x1)<<15) | (((r>>3) & 0x1F)<<10) | (((g>>3) & 0x1F)<<5) | ((b>>3) & 0x1F);	}
WORD	LcColor_GetA1R5G5B5(DWORD c)			{	return LcColor_RGBA16(c>>16, c>>8, c, c>>24);													}
WORD	LcColor_X8R8G8B8toA1R5G5B5(INT color)	{	return LcColor_RGB16(color>>16, color>>8, color);												}

// Returns value from A1R5G5B5 color	
INT		LcColor_GetAlpha16(WORD color)			{	return ((color >> 11)&0x1);		}
INT		LcColor_GetRed16(WORD color)			{	return ((color >> 10)&0x1F);	}
INT		LcColor_GetGreen16(WORD color)			{	return ((color >> 5)&0x1F);		}
INT		LcColor_GetBlue16(WORD color)			{	return ((color)&0x1F);			}

INT		LcColor_GetLuminance(WORD color)		{	return ((LcColor_GetRed16(color)<<3) + (LcColor_GetGreen16(color)<<3) + (LcColor_GetBlue16(color)<<3)) / 3;		}
INT		LcColor_GetA1R5G5B5toA8R8G8B8(WORD color){	return	(((color >> 15)&0x1)<<31) |	(((color >> 10)&0x1F)<<19) |(((color >> 5)&0x1F)<<11) |	(color&0x1F)<<3;	}
INT		LcColor_GetR5G6B5toA8R8G8B8(WORD color)	{	return	0xFF000000 & ((((color >> 11)&0x1F)<<19) |	(((color >> 5)&0x3F)<<11) |	(color&0x1F)<<3);				}	


