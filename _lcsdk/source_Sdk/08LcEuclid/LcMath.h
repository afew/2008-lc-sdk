// Interface for the Math Functions.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LcMath_H_
#define _LcMath_H_


typedef D3DXVECTOR2				VEC2;
typedef D3DXVECTOR3				VEC3;
typedef D3DXMATRIX				MATA;


INT		LcMath_MakePower2(INT a);

void	LcMath_InterpolateFloat2(FLOAT* pOut, FLOAT* p1, FLOAT* p2, FLOAT d);
void	LcMath_InterpolateFloat3(FLOAT* pOut, FLOAT* p1, FLOAT* p2, FLOAT d);
void	LcMath_InterpolateFloat4(FLOAT* pOut, FLOAT* p1, FLOAT* p2, FLOAT d);
void	LcMath_InterpolateFloat	(FLOAT* pOut, FLOAT* p1, FLOAT* p2, FLOAT d, INT iCnt);
INT		LcMath_Equals			(FLOAT a, FLOAT b);

FLOAT	LcMath_Vec3Length		(FLOAT *pV1, FLOAT *pV2 );
FLOAT	LcMath_Vec3LengthSq		(FLOAT *pV1, FLOAT *pV2 );
FLOAT	LcMath_Vec3Dot			(FLOAT *pV1, FLOAT *pV2 );
void	LcMath_Vec3Cross		(FLOAT* pOut, FLOAT *pV1, FLOAT *pV2 );
void	LcMath_Vec3NormalVector	(VEC3* pOut, VEC3* p1, VEC3* p2, VEC3* p3);

void	LcMath_Vec3Normalize	(FLOAT* pOut, FLOAT *pV1, FLOAT *pV2 );
void	LcMath_Vec3Minimize		(FLOAT *pOut, FLOAT *pV1, FLOAT *pV2 );
void	LcMath_Vec3Maximize		(FLOAT *pOut, FLOAT *pV1, FLOAT *pV2 );

void	LcMath_Mat4X4Identity		(FLOAT* pM);
FLOAT	LcMath_QuaternionLength		(FLOAT *pV/*x,y,z,w*/);
FLOAT	LcMath_QuaternionLengthSq	(FLOAT *pV/*x,y,z,w*/);
FLOAT	LcMath_QuaternionDot		(FLOAT *pV1, FLOAT *pV2);
void	LcMath_QuaternionIdentity	(FLOAT *pV1);




// Collision

INT		LcMath_LineCross2D(VEC2 * p);

INT		LcMath_Point3DtoScreen(								// 3차원 점을 카메라의 Near 평면에 투영 하기
								VEC3* pOut					// Output
							,	const VEC3* vcIn			// Input
							,	const VEC3* vcCam			// Camera pos
							,	const VEC3*	vcZ				// Camera Z vector
							,	const MATA*	mtVP			// View matrix * Projection Matrix
							,	FLOAT fScnW					// Screen Width
							,	FLOAT fScnH					// Screen Height
						  );


INT		LcMath_MousePositionTo3D(VEC3* vcOut, MATA matView, MATA matProj, FLOAT fScnW, FLOAT fScnH, POINT ptMouse);

FLOAT	LcMath_PointToPlaneDistance(FLOAT* pPn/*VECTOR4*/, FLOAT* p1 /*VECTOR3*/);							// 점과 평면사이의 최단거리

FLOAT	LcMath_LineToPointDistance(const LcLine* pL, const D3DXVECTOR3* pP);								//  점과 직선의 최단거리


INT		LcMath_LineToSphere			(																		// 선과 구혹은 점의 충돌
										VEC3* pOut															// 선위에 있는 점과 선의 최단거리 지점
									,	FLOAT* fDst															// 선과 점의 최단거리
									,	const LcLine* pLine
									,	const VEC3* pC														// 점 혹은 구의 중심 점
									,	const FLOAT fR														// 구의 반경
									);


FLOAT	LcMath_LineToLineDistance(																			// 직선과 직선 사이의 최단 거리
									FLOAT* pSinTheta														// 직선과 직선 사이의 각도
								,	VEC3* pOut																// 직선과 직선사이의 노말벡터
								,	const LcLine* pV1														// 직선 1
								,	const LcLine* pV2														// 직선 2
								);


INT		LcMath_LineToLineIntersection(																		// 직선과 직선 사이의 관계 구하기: RETURN VALUE:겹처 있다면 -1, 평행 하면 -2 교차하면 1, 교차하지 않으면 최단거리의 점들을 돌려줌
										FLOAT* pSinTheta													// 직선과 직선 사이의 각도
									,	FLOAT* pDst															// 직선가 직선 사이의 최단 거리
									,	VEC3* pOut															// 최단 거리의 점들
									,	const LcLine* pV1													// 직선 1
									,	const LcLine* pV2													// 직선 2
									);


FLOAT	LcMath_LineToPlaneKValue(const DPLN* p1, const VEC3* p2, const VEC3* p3);						// K 값 구하기.


INT		LcMath_LineToTriangle(
									VEC3* pOut				// Pick Position
								,	FLOAT* fD				// Distance from vcRayPos to pOut
								,	VEC3* vcRayPos			// Starting Ray Position
								,	VEC3* vcRayDir			// Ray Direction(It must be Normalization.)
								,	VEC3* pTri
								,	BOOL bUseCull=TRUE);


INT		LcMath_LineToTriangle(
									VEC3* vcPck				// Pick Position
								,	FLOAT* D				// Distance from vcRayPos to pOut
								,	const VEC3* vcRayPos	// Starting Ray Position
								,	const VEC3* vcRayDir	// Starting Ray Position
								,	const VEC3* p0			// Triangle 0
								,	const VEC3* p1			// Triangle 1
								,	const VEC3* p2			// Triangle 2
								,	BOOL bUseCull=TRUE);


INT		LcMath_LineToTriangle		(									// 직선과 삼각형의 충돌
										VEC3* pOut						// 충돌 점
									,	const	VEC3* pTri				// 삼각형
									,	const	LcLine* pLine			// 직선
									,	BOOL bRectangle=FALSE			// 충돌영역을 삼각형의 확장  평행사변형으로 쓸것인지..
									,	BOOL bPositiveDirection=FALSE	// 직선의 방향으로만 충돌점을 구하기
									,	const VEC3* pBoundLine = NULL	// 유한 직선인 경우 양 끝 지점
									,	const VEC3* pTriNormal= NULL	// 삼각형의 Normal Vector
									);

INT		LcMath_TriangleToTriangle	(VEC3* pOut, VEC3* pV1, VEC3* pV2);	//삼각형 VS 삼각형 충돌

FLOAT	LcMath_CircumcircleCorrect	(VEC3* pOut,const VEC3* pTri);		// 정확한 삼각형의 외접원
FLOAT	LcMath_TriangleBoundSphere	(VEC3* pOut,const VEC3* pTri);		// 부정확한 삼각형의 외접원

INT		LcMath_IsInPoints	(const VEC3* p1, const VEC3* p2);
INT		LcMath_Vec3Greater	(const VEC3* p1, const VEC3* p2);
INT		LcMath_Vec3Less		(const VEC3* p1, const VEC3* p2);

void	LcMath_PlaneSetD	(DPLN* pOut, const VEC3* v);				// 평면의 d값 세팅
void	LcMath_Plane		(DPLN* pOut, const VEC3* n, FLOAT d);		// 법선 벡터와 원점에서의 최단거리로 평면 만들기


void	LcMath_PlaneSet						(DPLN* pOut, const VEC3* p0, const VEC3* n);					// 법선벡터와 점으로 평면 만들기
void	LcMath_PlaneSet						(DPLN* pOut, const VEC3* p0, const VEC3* p1, const VEC3* p2);	// 세점으로 평면 만들기
INT		LcMath_PlaneIntersectionWithLine	(VEC3* pOut, const DPLN* pPn, const VEC3* p1, const VEC3* vcT);	// 직선과 평면과의 충돌점
INT		LcMath_PlaneIntersectionWithPoints	(VEC3* pOut, const DPLN* pPn, const VEC3* p1, const VEC3* p2);	// 2점사이의 평면


FLOAT	LcMath_PlaneToPlaneDistance			(const LcPlane* pP1, const LcPlane* pP2);						// 평행한 평면사이의 최단거리

INT		LcMath_PlaneToPointRelation			(DPLN* pPn, const VEC3* p1);									// 임의의 점이 평면의 위(뒤)에 있는 가 아래(앞)에 있는가?
INT		LcMath_PlaneToPointIsFront			(const DPLN* pPn, const VEC3* p1, FLOAT fEpsilon=0.f);			// 점(구)이 평면의 앞에 있는가?
VEC3	LcMath_PlaneMemberPoint				(DPLN* pPn);													// Return 값의 크기는 d값, 리턴값의 크기를 d로 나누면 평면의 법선벡터
INT		LcMath_PlaneToPlaneExistsInterSect	(const DPLN* p1, const DPLN* p2);								//평행하면 Dot product는 1

INT		LcMath_PlaneToPlaneIntersectLine(	VEC3* pOutP	// Line Start Point									// 두 평면이 교차하는 직선의 방정식 구하기...
										  , VEC3* pOutT	// Line Direction
										  , const DPLN* pPn1
										  , const DPLN* pPn2);




FLOAT	LcMath_PlaneToPointMinDistance	(const FLOAT* pPlane/*VECTOR4*/, const FLOAT* pPoint /*VECTOR3*/);	// 평면과 점사이의 최단 거리
FLOAT	LcMath_PlaneToPointMinDistance	(const DPLN* pPn, const VEC3* p1);									// 평면과 점사이의 최단 거리

INT		LcMath_PointFromPlanes(VEC3* pOut, const DPLN* p1, const DPLN* p2, const DPLN* p3);					// 세평면이 겹치는 점 구하기



void	LcMath_MatrixRotationRadians		(MATA* pOut, const VEC3* pRot);
void	LcMath_MatrixRotationDegrees		(MATA* pOut, const VEC3* pRot);
void	LcMath_MatrixInverseRotationRadians	(MATA* pOut, const VEC3* pRot);
void	LcMath_MatrixInverseRotationDegrees	(MATA* pOut, const VEC3* pRot);

void	LcMath_Vec3RotationDegrees	(VEC3* pOut, const MATA* pIn);

void	LcMath_MatrixTranspose		(MATA* pOut, const MATA* pIn);
void	LcMath_Vec3Translate		(VEC3* pOut, const MATA* pT, const VEC3* pIn);
void	LcMath_Vec3InverseTranslate	(VEC3* pOut, const MATA* pT, const VEC3* pIn);
void	LcMath_Vec3Rotate			(VEC3* pOut, const MATA* pR, const VEC3* pIn);
void	LcMath_Vec3InverseRotate	(VEC3* pOut, const MATA* pR, const VEC3* pIn);
void	LcMath_Vec3Transform		(VEC3*pOut, const MATA* pT, const VEC3* pIn);
void	LcMath_PlaneTransform		(DPLN*pOut, const MATA* pT, DPLN* pIn);
void	LcMath_Vec4Transform		(VEC4*pOut, const MATA* pT, const VEC4* pIn);



INT	LcMath_UVFromTri(															// 삼각형 내부에 있는 점의 U V 값구하기 U V는 P0에서의 상대적인 값이다.
						FLOAT* fU
					,	FLOAT* fV
					,	const VEC3* p
					,	const VEC3* p0
					,	const VEC3* p1
					,	const VEC3* p2);

INT LcMath_SphereToTri(		VEC3* pOut											// 구와 삼각형의 최단거리의 지점
						,	FLOAT* fDist										// 구와 삼각형의 최단거리
						,	const VEC3* pSphereCenter							// 구의 중앙
						,	const FLOAT* pSphereRadius							// 구의 반경
						,	const VEC3* pTri									// 삼각형의 세점
						,	VEC3* pTriCenter	= NULL							// 삼각형의 바운드구의 중심
						,	FLOAT* pTriRadius	= NULL							// 삼각형의 바운드구의 반경
						,	VEC3* pTriNormal	= NULL							// 삼각형의 노말멕터
						);


INT LcMath_SphereToSphere(	const VEC3* pSphereCenter1							// 구의 중앙1
						,	const FLOAT* pSphereRadius1							// 구의 반경1
						,	const VEC3* pSphereCenter2							// 구의 중앙2
						,	const FLOAT* pSphereRadius2							// 구의 반경2
						);












////////////////////////////////////////////////////////////////////////////////
//
// Simple Triangle
//
////////////////////////////////////////////////////////////////////////////////


void	LcMath_TriTransform	(LcTri* pOut,const MATA* pT,const LcTri* pIn);
void	LcMath_TriMove		(LcTri* pOut,const VEC3* pV,const LcTri* pIn);		// Move Triangle







////////////////////////////////////////////////////////////////////////////////
//
// Hexahedron
//
//					Max
//		5------------7
//	   /|           /|
//	  / |          / |
//	 /  |         /  |
//	1---+--------3   |
//	|   |        |   |
//	|   4--------+---6
//	|  /         |  /
//	| /          | /
//	|/           |/
//	0------------2
//	Min
//
//
//
// Cuboid Axis Aligned Bounding Box
//
//	Edges
//
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
//	
//
////////////////////////////////////////////////////////////////////////////////


void	LcMath_CubeHMove		(LcCubeH* pOut,const VEC3* pV,const LcCubeH* pIn);		// Move Hexagon
void	LcMath_CubeHTransform	(LcCubeH* pOut,const MATA* pT,const LcCubeH* pIn);		// Transform Hexagon
void	LcMath_CubeHBoxLine		(void* pOut, INT nSizeChunk,const LcCubeH* pIn);		// Get Box Line
void	LcMath_CubeHFromA		(LcCubeH* pOut,const LcCubeAA* pIn);					// Cube Hexa from AABB

void	LcMath_CubeAMove		(LcCubeAA* pOut,const VEC3* pV,const LcCubeAA* pIn);	// Move Hexagon
void	LcMath_CubeATransform	(LcCubeAA*pOut,const MATA* pT,const LcCubeAA* pIn);

INT		LcMath_CubeAToLine		(	VEC3* vcOut
								 ,	VEC3* pVcBox
								 ,	LcLine* pLine
								 ,	BOOL bPositiveDirection=TRUE						// Line의 진행 방항으로 충돌할 것인가?
								 );

INT		LcMath_CubeAToLine		(	VEC3* vcOut
								 ,	LcCubeAA* pCube
								 ,	LcLine* pLine
								 ,	BOOL bPositiveDirection=TRUE
								 );

void	LcMath_CubeABoxLine		(void* pOut, INT nSizeChunk,const LcCubeAA* pIn);		// Get Hexagon Line

INT		LcMath_CubeHToSphere	(	const LcCubeH* pHexa								// Hexgagon To Sphere
								,	const VEC3* pPoint									// 중심점
								,	FLOAT fRadius=0.f									// 구의 반경
								);

INT		LcMath_CubeHToTri		(const LcCubeH* pHexa, const LcTri* pTri);				// Collision Hexa to Tri

INT		LcMath_CubeHToH			(const LcCubeH* pH1, const LcCubeH* pH2);				// Collision Hexagon to Hexagon







////////////////////////////////////////////////////////////////////////////////
//
//
// Cylinder
//
////////////////////////////////////////////////////////////////////////////////



INT		LcMath_CylinderToLineIntersection(										// 원통과 직선의 충돌
												VEC3* pOut						// Intersection point
											,	const LcCylinder* pV1			// Input Cylinder
											,	const LcLine* pV2				// Input Line
											,	const VEC3* pV3 = NULL			// Input Line Boundary
										  );


INT		LcMath_CylinderToTriIntersection(										// 원통과 삼각형의 충돌
											VEC3* pOut							// Intersection point
										,	const LcCylinder* pV1				// Input Cylinder
										,	const VEC3* pV2						// Input Triangle
									  );


INT		LcMath_CylinderToTriIntersection(										// 원통과 삼각형의 충돌 원통의 중심선이 삼각형과 충돌안하면 return (nCnt+1)<<2;을 함으로 주의!!!
											VEC3* pOut							// Intersection point
										,	const LcCylinder* pV1				// Input Cylinder
										,	BndTri* pV2							// Input Bound Triangle
										,	BOOL bRectangle=FALSE				// 충돌영역을 삼각형의 확장 평행사변형으로 쓸것인지..
										,	BOOL bPositiveDirection=FALSE		// 직선의 방향으로만 충돌점을 구하기
										,	const VEC3* pBoundLine = NULL		// 유한 직선인 경우 양 끝 지점
										,	const VEC3* pTriNormal= NULL		// 삼각형의 Normal Vector
									  );




BOOL LcMath_RcCollision(LcRc* v1, LcRc* v2);

////////////////////////////////////////////////////////////////////////////////
//
// Color
//
////////////////////////////////////////////////////////////////////////////////

DWORD	LcColor_GetAlpha	(DWORD c);
DWORD	LcColor_GetRed		(DWORD c);
DWORD	LcColor_GetGreen	(DWORD c);
DWORD	LcColor_GetBlue		(DWORD c);

void	LcColor_SetAlpha	(DWORD& c, DWORD a);
void	LcColor_SetRed		(DWORD& c,DWORD r);
void	LcColor_SetGreen	(DWORD& c,DWORD g);
void	LcColor_SetBlue		(DWORD& c,DWORD b);

WORD	LcColor_RGB16		(INT r,INT g,INT b);
WORD	LcColor_RGBA16		(INT r,INT g,INT b,INT a);
WORD	LcColor_GetA1R5G5B5	(DWORD c);

WORD	LcColor_X8R8G8B8toA1R5G5B5(INT color);

// Returns value from A1R5G5B5 color	
INT		LcColor_GetAlpha16	(WORD color);
INT		LcColor_GetRed16	(WORD color);
INT		LcColor_GetGreen16	(WORD color);
INT		LcColor_GetBlue16	(WORD color);

INT		LcColor_GetLuminance(WORD color);

INT		LcColor_GetA1R5G5B5toA8R8G8B8(WORD color);
INT		LcColor_GetR5G6B5toA8R8G8B8(WORD color);



#endif



