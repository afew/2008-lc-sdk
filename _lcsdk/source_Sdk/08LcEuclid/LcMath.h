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

INT		LcMath_Point3DtoScreen(								// 3���� ���� ī�޶��� Near ��鿡 ���� �ϱ�
								VEC3* pOut					// Output
							,	const VEC3* vcIn			// Input
							,	const VEC3* vcCam			// Camera pos
							,	const VEC3*	vcZ				// Camera Z vector
							,	const MATA*	mtVP			// View matrix * Projection Matrix
							,	FLOAT fScnW					// Screen Width
							,	FLOAT fScnH					// Screen Height
						  );


INT		LcMath_MousePositionTo3D(VEC3* vcOut, MATA matView, MATA matProj, FLOAT fScnW, FLOAT fScnH, POINT ptMouse);

FLOAT	LcMath_PointToPlaneDistance(FLOAT* pPn/*VECTOR4*/, FLOAT* p1 /*VECTOR3*/);							// ���� �������� �ִܰŸ�

FLOAT	LcMath_LineToPointDistance(const LcLine* pL, const D3DXVECTOR3* pP);								//  ���� ������ �ִܰŸ�


INT		LcMath_LineToSphere			(																		// ���� ��Ȥ�� ���� �浹
										VEC3* pOut															// ������ �ִ� ���� ���� �ִܰŸ� ����
									,	FLOAT* fDst															// ���� ���� �ִܰŸ�
									,	const LcLine* pLine
									,	const VEC3* pC														// �� Ȥ�� ���� �߽� ��
									,	const FLOAT fR														// ���� �ݰ�
									);


FLOAT	LcMath_LineToLineDistance(																			// ������ ���� ������ �ִ� �Ÿ�
									FLOAT* pSinTheta														// ������ ���� ������ ����
								,	VEC3* pOut																// ������ ���������� �븻����
								,	const LcLine* pV1														// ���� 1
								,	const LcLine* pV2														// ���� 2
								);


INT		LcMath_LineToLineIntersection(																		// ������ ���� ������ ���� ���ϱ�: RETURN VALUE:��ó �ִٸ� -1, ���� �ϸ� -2 �����ϸ� 1, �������� ������ �ִܰŸ��� ������ ������
										FLOAT* pSinTheta													// ������ ���� ������ ����
									,	FLOAT* pDst															// ������ ���� ������ �ִ� �Ÿ�
									,	VEC3* pOut															// �ִ� �Ÿ��� ����
									,	const LcLine* pV1													// ���� 1
									,	const LcLine* pV2													// ���� 2
									);


FLOAT	LcMath_LineToPlaneKValue(const DPLN* p1, const VEC3* p2, const VEC3* p3);						// K �� ���ϱ�.


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


INT		LcMath_LineToTriangle		(									// ������ �ﰢ���� �浹
										VEC3* pOut						// �浹 ��
									,	const	VEC3* pTri				// �ﰢ��
									,	const	LcLine* pLine			// ����
									,	BOOL bRectangle=FALSE			// �浹������ �ﰢ���� Ȯ��  ����纯������ ��������..
									,	BOOL bPositiveDirection=FALSE	// ������ �������θ� �浹���� ���ϱ�
									,	const VEC3* pBoundLine = NULL	// ���� ������ ��� �� �� ����
									,	const VEC3* pTriNormal= NULL	// �ﰢ���� Normal Vector
									);

INT		LcMath_TriangleToTriangle	(VEC3* pOut, VEC3* pV1, VEC3* pV2);	//�ﰢ�� VS �ﰢ�� �浹

FLOAT	LcMath_CircumcircleCorrect	(VEC3* pOut,const VEC3* pTri);		// ��Ȯ�� �ﰢ���� ������
FLOAT	LcMath_TriangleBoundSphere	(VEC3* pOut,const VEC3* pTri);		// ����Ȯ�� �ﰢ���� ������

INT		LcMath_IsInPoints	(const VEC3* p1, const VEC3* p2);
INT		LcMath_Vec3Greater	(const VEC3* p1, const VEC3* p2);
INT		LcMath_Vec3Less		(const VEC3* p1, const VEC3* p2);

void	LcMath_PlaneSetD	(DPLN* pOut, const VEC3* v);				// ����� d�� ����
void	LcMath_Plane		(DPLN* pOut, const VEC3* n, FLOAT d);		// ���� ���Ϳ� ���������� �ִܰŸ��� ��� �����


void	LcMath_PlaneSet						(DPLN* pOut, const VEC3* p0, const VEC3* n);					// �������Ϳ� ������ ��� �����
void	LcMath_PlaneSet						(DPLN* pOut, const VEC3* p0, const VEC3* p1, const VEC3* p2);	// �������� ��� �����
INT		LcMath_PlaneIntersectionWithLine	(VEC3* pOut, const DPLN* pPn, const VEC3* p1, const VEC3* vcT);	// ������ ������ �浹��
INT		LcMath_PlaneIntersectionWithPoints	(VEC3* pOut, const DPLN* pPn, const VEC3* p1, const VEC3* p2);	// 2�������� ���


FLOAT	LcMath_PlaneToPlaneDistance			(const LcPlane* pP1, const LcPlane* pP2);						// ������ �������� �ִܰŸ�

INT		LcMath_PlaneToPointRelation			(DPLN* pPn, const VEC3* p1);									// ������ ���� ����� ��(��)�� �ִ� �� �Ʒ�(��)�� �ִ°�?
INT		LcMath_PlaneToPointIsFront			(const DPLN* pPn, const VEC3* p1, FLOAT fEpsilon=0.f);			// ��(��)�� ����� �տ� �ִ°�?
VEC3	LcMath_PlaneMemberPoint				(DPLN* pPn);													// Return ���� ũ��� d��, ���ϰ��� ũ�⸦ d�� ������ ����� ��������
INT		LcMath_PlaneToPlaneExistsInterSect	(const DPLN* p1, const DPLN* p2);								//�����ϸ� Dot product�� 1

INT		LcMath_PlaneToPlaneIntersectLine(	VEC3* pOutP	// Line Start Point									// �� ����� �����ϴ� ������ ������ ���ϱ�...
										  , VEC3* pOutT	// Line Direction
										  , const DPLN* pPn1
										  , const DPLN* pPn2);




FLOAT	LcMath_PlaneToPointMinDistance	(const FLOAT* pPlane/*VECTOR4*/, const FLOAT* pPoint /*VECTOR3*/);	// ���� �������� �ִ� �Ÿ�
FLOAT	LcMath_PlaneToPointMinDistance	(const DPLN* pPn, const VEC3* p1);									// ���� �������� �ִ� �Ÿ�

INT		LcMath_PointFromPlanes(VEC3* pOut, const DPLN* p1, const DPLN* p2, const DPLN* p3);					// ������� ��ġ�� �� ���ϱ�



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



INT	LcMath_UVFromTri(															// �ﰢ�� ���ο� �ִ� ���� U V �����ϱ� U V�� P0������ ������� ���̴�.
						FLOAT* fU
					,	FLOAT* fV
					,	const VEC3* p
					,	const VEC3* p0
					,	const VEC3* p1
					,	const VEC3* p2);

INT LcMath_SphereToTri(		VEC3* pOut											// ���� �ﰢ���� �ִܰŸ��� ����
						,	FLOAT* fDist										// ���� �ﰢ���� �ִܰŸ�
						,	const VEC3* pSphereCenter							// ���� �߾�
						,	const FLOAT* pSphereRadius							// ���� �ݰ�
						,	const VEC3* pTri									// �ﰢ���� ����
						,	VEC3* pTriCenter	= NULL							// �ﰢ���� �ٿ�屸�� �߽�
						,	FLOAT* pTriRadius	= NULL							// �ﰢ���� �ٿ�屸�� �ݰ�
						,	VEC3* pTriNormal	= NULL							// �ﰢ���� �븻����
						);


INT LcMath_SphereToSphere(	const VEC3* pSphereCenter1							// ���� �߾�1
						,	const FLOAT* pSphereRadius1							// ���� �ݰ�1
						,	const VEC3* pSphereCenter2							// ���� �߾�2
						,	const FLOAT* pSphereRadius2							// ���� �ݰ�2
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
								 ,	BOOL bPositiveDirection=TRUE						// Line�� ���� �������� �浹�� ���ΰ�?
								 );

INT		LcMath_CubeAToLine		(	VEC3* vcOut
								 ,	LcCubeAA* pCube
								 ,	LcLine* pLine
								 ,	BOOL bPositiveDirection=TRUE
								 );

void	LcMath_CubeABoxLine		(void* pOut, INT nSizeChunk,const LcCubeAA* pIn);		// Get Hexagon Line

INT		LcMath_CubeHToSphere	(	const LcCubeH* pHexa								// Hexgagon To Sphere
								,	const VEC3* pPoint									// �߽���
								,	FLOAT fRadius=0.f									// ���� �ݰ�
								);

INT		LcMath_CubeHToTri		(const LcCubeH* pHexa, const LcTri* pTri);				// Collision Hexa to Tri

INT		LcMath_CubeHToH			(const LcCubeH* pH1, const LcCubeH* pH2);				// Collision Hexagon to Hexagon







////////////////////////////////////////////////////////////////////////////////
//
//
// Cylinder
//
////////////////////////////////////////////////////////////////////////////////



INT		LcMath_CylinderToLineIntersection(										// ����� ������ �浹
												VEC3* pOut						// Intersection point
											,	const LcCylinder* pV1			// Input Cylinder
											,	const LcLine* pV2				// Input Line
											,	const VEC3* pV3 = NULL			// Input Line Boundary
										  );


INT		LcMath_CylinderToTriIntersection(										// ����� �ﰢ���� �浹
											VEC3* pOut							// Intersection point
										,	const LcCylinder* pV1				// Input Cylinder
										,	const VEC3* pV2						// Input Triangle
									  );


INT		LcMath_CylinderToTriIntersection(										// ����� �ﰢ���� �浹 ������ �߽ɼ��� �ﰢ���� �浹���ϸ� return (nCnt+1)<<2;�� ������ ����!!!
											VEC3* pOut							// Intersection point
										,	const LcCylinder* pV1				// Input Cylinder
										,	BndTri* pV2							// Input Bound Triangle
										,	BOOL bRectangle=FALSE				// �浹������ �ﰢ���� Ȯ�� ����纯������ ��������..
										,	BOOL bPositiveDirection=FALSE		// ������ �������θ� �浹���� ���ϱ�
										,	const VEC3* pBoundLine = NULL		// ���� ������ ��� �� �� ����
										,	const VEC3* pTriNormal= NULL		// �ﰢ���� Normal Vector
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



