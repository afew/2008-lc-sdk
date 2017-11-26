//  For Mathematics for Euclid Field.
//
//////////////////////////////////////////////////////////////////////////////


#ifndef _LcEuclid_H_
#define _LcEuclid_H_


#if _MSC_VER >= 1200
	#pragma warning(disable:4201)		// anonymous unions warning
#endif

#pragma warning(disable:4786)


#include <vector>


typedef D3DXVECTOR2				VEC2;
typedef	D3DXVECTOR3				VEC3;
typedef D3DXVECTOR4				VEC4;
typedef D3DXMATRIX				MATA;
typedef D3DXQUATERNION			QUAT;
typedef D3DXPLANE				DPLN;



#define ONE_RADtoDEG	57.295779513082321f
#define ONE_DEGtoRAD	0.0174532925199433f
#define PI_RAD			3.1415926535897932f
#define DEG90toRAD		1.5707963267948966f
#define RADtoDEG(p)		( (p)*ONE_RADtoDEG)
#define DEGtoRAD(p)		( (p)*ONE_DEGtoRAD)


#define LC_ROUNDING_DELTA	0.0001f



//2D, or 3D Math
struct INT2
{
	union	{	struct	{	INT x, y;	};	INT m[2];	};
	INT2();
	INT2(INT X,INT Y);
	operator INT* ();
	operator const INT* () const;
};


struct VEC2i : public INT2
{
	VEC2i();
	VEC2i(INT _x,INT _y);
	VEC2i(const VEC2i& rhs );
	VEC2i(const INT *_m );
	VEC2i(D3DXVECTOR2 p);

	operator INT*();
	operator const INT*() const;
	
	bool operator==(const VEC2i& v) const;
	bool operator!=(const VEC2i& v) const;

	const VEC2i& operator=(const VEC2i& rhs);

	// assignment operators
	VEC2i& operator+=(const VEC2i& rhs);
	VEC2i& operator-=(const VEC2i& rhs);
	VEC2i& operator*=(INT f );
	VEC2i& operator/=(INT f );

	// unary operators
	VEC2i operator+() const;
	VEC2i operator-() const;

	// binary operators
	VEC2i operator+(const VEC2i& rhs) const;
	VEC2i operator-(const VEC2i& rhs) const;
	VEC2i operator*(const INT& f) const;
	VEC2i operator/(const INT& f) const;

	friend VEC2i operator*(INT f,const VEC2i& v){	return VEC2i(f * v.x, f * v.y);		}
};


typedef	std::vector<VEC2i>	lsVEC2i;
typedef	lsVEC2i::iterator	itVEC2i;

struct INT3
{
	union{	struct{	INT x, y, z;	};	INT m[3];};

	INT3();
	INT3(INT X,INT Y,INT Z);
	operator INT* ();
	operator const INT* () const;
};

struct VEC3i : public INT3
{
	VEC3i();
	VEC3i(const INT *_m);
	VEC3i(const VEC3i& rhs);
	VEC3i(INT _x,INT _y, INT _z);
	VEC3i(const D3DXVECTOR3 p);
	VEC3i(FLOAT _x, FLOAT _y, FLOAT _z);
	
	operator INT*();
	operator const INT*() const;

	bool operator==(const VEC3i& v) const;
	bool operator!=(const VEC3i& v) const;

	const VEC3i& operator=(const VEC3i& rhs);
	
	VEC3i& operator+=(const VEC3i& v);
	VEC3i& operator-=(const VEC3i& v);
	VEC3i& operator*=(INT f );
	VEC3i& operator/=(INT f );
	
	VEC3i operator+() const;
	VEC3i operator-() const;
	
	VEC3i operator+(const VEC3i& v)const;
	VEC3i operator-(const VEC3i& v)const;
	VEC3i operator*(INT f) const		;
	VEC3i operator/(INT f) const		;
	
	friend VEC3i operator*(INT f,const VEC3i& v)
	{	return VEC3i(f * v.x, f * v.y, f * v.z);	}
};


typedef	std::vector<VEC3i>	lsVEC3i;
typedef	lsVEC3i::iterator	itVEC3i;


struct INT4
{
	union	{	struct	{	INT x, y, z, w;	};	INT m[4];	};

	INT4();
	INT4(INT X,INT Y,INT Z,INT W);
	operator INT* ();
	operator const INT* () const;
};


struct VEC4i : public INT4
{
	VEC4i();
	VEC4i(const INT *_m );
	VEC4i(const VEC4i& r);
	VEC4i(INT X,INT Y,INT Z,INT W);
	VEC4i(const D3DXVECTOR4 p);
	
	operator INT*();
	operator const INT*() const;

	bool operator==(const VEC4i& v) const;
	bool operator!=(const VEC4i& v) const;

	const VEC4i& operator=(const VEC4i& rhs);
	
	// assignment operators
	VEC4i& operator+=(const VEC4i& v);
	VEC4i& operator-=(const VEC4i& v);
	VEC4i& operator*=(INT f )		;
	VEC4i& operator/=(INT f )		;
	
	// unary operators
	VEC4i operator+() const			;
	VEC4i operator-() const			;
	
	// binary operators
	VEC4i operator+(const VEC4i& v)const;
	VEC4i operator-(const VEC4i& v)const;
	VEC4i operator*(INT f) const		;
	VEC4i operator/(INT f) const		;
	
	friend VEC4i operator*(INT f,const VEC4i& v)
	{	return VEC4i(f*v.x,f*v.y,f*v.z,f*v.w);	}
};


typedef	std::vector<VEC4i>	lsVEC4i;
typedef	lsVEC4i::iterator	itVEC4i;



struct WORD2
{
	union{	struct{WORD x, y;	};	WORD m[2];	};

	WORD2();
	WORD2(WORD X,WORD Y);
	operator WORD* ();
	operator const WORD* () const;
};

struct VEC2w : public WORD2
{
	VEC2w();
	VEC2w(WORD X,WORD Y);

};

typedef	std::vector<VEC2w>	lsVEC2w;
typedef	lsVEC2w::iterator	itVEC2w;


struct WORD3
{
	union{	struct{	WORD x, y, z;	};	WORD m[3];};

	WORD3();
	WORD3(WORD X,WORD Y,WORD Z);
	operator WORD* ()			;
	operator const WORD* () const;
};

struct VEC3w : public WORD3
{
	VEC3w();
	VEC3w(WORD X,WORD Y, WORD Z, WORD W);

};

typedef	std::vector<VEC3w>	lsVEC3w;
typedef	lsVEC3w::iterator	itVEC3w;



struct WORD4
{
	union{	struct{	WORD x,y,z,w;	};	WORD m[4];	};

	WORD4();
	WORD4(WORD X,WORD Y,WORD Z,WORD W);
	operator WORD* ();
	operator const WORD* () const;
};

struct VEC4w : public WORD4
{
	VEC4w();
	VEC4w(WORD X,WORD Y, WORD Z, WORD W);

};

typedef	std::vector<VEC4w>	lsVEC4w;
typedef	lsVEC4w::iterator	itVEC4w;



struct DOUBLE2
{
	union	{	struct	{	DOUBLE x, y;	};	DOUBLE m[2];	};
	
	DOUBLE2();
	DOUBLE2(DOUBLE X,DOUBLE Y);
};


struct DOUBLE3
{
	union	{	struct	{	DOUBLE x, y, z;	};	DOUBLE m[3];	};

	DOUBLE3();
	DOUBLE3(DOUBLE X,DOUBLE Y,DOUBLE Z,DOUBLE W);
};


struct DOUBLE4
{
	union	{	struct	{	DOUBLE x, y, z, w;	};	DOUBLE m[4];	};

	DOUBLE4();
	DOUBLE4(DOUBLE X,DOUBLE Y,DOUBLE Z,DOUBLE W);
};


struct LcRc
{
	union	{	struct	{	FLOAT x0, y0, x1, y1;	};	FLOAT m[4];		};
	
	LcRc();
	LcRc(const FLOAT*_m);
	LcRc(const LcRc& _rh );
	
	LcRc( FLOAT fx0,FLOAT fy0,FLOAT fx1,FLOAT fy1);
	
	LcRc( D3DXVECTOR2 p0, D3DXVECTOR2 p1);
	
	D3DXVECTOR2  Get00()	 const	;
	D3DXVECTOR2  Get10()	 const	;
	D3DXVECTOR2  Get01()	 const	;
	D3DXVECTOR2  Get11()	 const	;

	RECT   GetRECT()				;
	D3DRECT GetRectD3()				;
	
	FLOAT	GetWidth()				;
	FLOAT	GetHeight()				;
	D3DXVECTOR2	GetCenter() const	;
	
	// casting
	operator FLOAT*()				;
	operator const FLOAT*() const	;

//	const LcRc& operator=(const LcRc& v)	;

	bool operator==(const LcRc& v) const	;
	bool operator!=(const LcRc& v) const	;
	
	// assignment operators
	LcRc& operator+=(const LcRc& v)			;
	LcRc& operator-=(const LcRc& v)			;
	LcRc& operator*=(FLOAT f )				;
	LcRc& operator/=(FLOAT f )				;
	
	// unary operators
	LcRc operator+() const					;
	LcRc operator-() const					;
	
	// binary operators
	LcRc operator+(const LcRc& v)const		;
	LcRc operator-(const LcRc& v)const		;
	LcRc operator*(FLOAT f) const			;
	LcRc operator/(FLOAT f) const			;
	
	friend LcRc operator*(FLOAT f,const LcRc& v){	return LcRc(f * v.x0, f * v.y0, f * v.x1, f * v.y1);			}
	
	LcRc operator+(const D3DXVECTOR2& v)		;
	LcRc operator-(const D3DXVECTOR2& v)		;
	LcRc operator+=(const D3DXVECTOR2& v)		;
	LcRc operator-=(const D3DXVECTOR2& v)		;

	bool IsValid() const;


	bool IsCollided(const D3DXVECTOR2& v) const;

	bool IsCollided(const LcRc& v) const;
	
	void ClipAgainst(const LcRc& v);
	
	void SetCorrection();

};

typedef	std::vector<LcRc>	lsLcRc;
typedef	lsLcRc::iterator	itLcRc;


struct LcLine																	// 직선의 방정식
{
	// Start Pointer, Tranverse Vector
	union	{	struct	{	D3DXVECTOR3 p;	D3DXVECTOR3 t;	};	FLOAT m[6];	};
	
	LcLine();
	LcLine(const FLOAT*_m)		;
	LcLine(const LcLine& rhs)	;

	LcLine(FLOAT Px,FLOAT Py,FLOAT Pz,FLOAT Tx,FLOAT Ty,FLOAT Tz);
	LcLine(const D3DXVECTOR3& P, const D3DXVECTOR3& T);
	
	// casting
	operator FLOAT*()						;
	operator const FLOAT*() const			;
	
	// unary operators
	LcLine operator+() const				;
	LcLine operator-() const				;

	bool operator==(const LcLine& v) const	;
	bool operator!=(const LcLine& v) const	;
};


typedef	std::vector<LcLine>	lsLcLine;
typedef	lsLcRc::iterator	itLcLine;


struct LcPlane																	// 평면의 방정식
{
	union	{	struct	{	D3DXVECTOR3 n;	FLOAT d;	};	FLOAT m[4];	};			// Normal Vector, -Distance
	
	LcPlane();
	LcPlane(const FLOAT*_m)					;
	LcPlane(const LcPlane& _rh)				;
	LcPlane( FLOAT a,FLOAT b,FLOAT c,FLOAT d);
	
	// casting
	operator FLOAT*()						;
	operator const FLOAT*() const			;

	bool operator==(const LcPlane& v) const	;
	bool operator!=(const LcPlane& v) const	;
};

typedef	std::vector<LcPlane>	lsLcPlane;
typedef	lsLcRc::iterator		itLcPlane;


struct LcSphere																	// 구
{
	union	{	struct	{	D3DXVECTOR3 p;	FLOAT r;};	FLOAT m[4];	};					// center, Radius
	
	LcSphere();
	LcSphere(const FLOAT*_m)				;
	LcSphere(const LcSphere& _rh)			;
	LcSphere( FLOAT X,FLOAT Y,FLOAT Z,FLOAT R);
	
	// casting
	operator FLOAT*()						;
	operator const FLOAT*() const			;

	// unary operators
	LcSphere operator+() const				;
	LcSphere operator-() const				;

	bool operator==(const LcSphere& v) const;
	bool operator!=(const LcSphere& v) const;
};

typedef	std::vector<LcSphere>	lsLcSphere;
typedef	lsLcRc::iterator		itLcSphere;







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
////////////////////////////////////////////////////////////////////////////////

struct LcCubeH
{
	union
	{
		struct
		{
			VEC3 _0;	VEC3 _1;	VEC3 _2;	VEC3 _3;
			VEC3 _4;	VEC3 _5;	VEC3 _6;	VEC3 _7;
		};// struct

		D3DVECTOR	m[8];

	}; // union

	
	LcCubeH();
	LcCubeH(const VEC3* _m);
	LcCubeH(	  const VEC3& __0
				, const VEC3& __1
				, const VEC3& __2
				, const VEC3& __3
				, const VEC3& __4
				, const VEC3& __5
				, const VEC3& __6
				, const VEC3& __7);

	operator FLOAT*();
	operator const FLOAT*() const;

//	VEC3* operator[](int nIdx) const
//	{
//		return (VEC3*)(&_0.x + nIdx * sizeof( VEC3));
//	}

	BOOL operator==(const LcCubeH& v) const;
	BOOL operator !=(const LcCubeH& v) const;

	void Set(const VEC3* _m);
	void Set(const LcCubeH& r);
};




////////////////////////////////////////////////////////////////////////////////
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

struct LcCubeAA
{
	union
	{
		struct
		{
			VEC3 vcMin;
			VEC3 vcMax;
		};// struct

		D3DVECTOR	m[2];

	}; // union

	
	LcCubeAA();
	LcCubeAA(const VEC3* _m);
	LcCubeAA(const VEC3& _min, const VEC3& _max);
	LcCubeAA(const VEC3& vc0);
	LcCubeAA(FLOAT* _p);
	LcCubeAA(FLOAT x0, FLOAT y0, FLOAT z0, FLOAT x1, FLOAT y1, FLOAT z1);
	
	BOOL operator==(const LcCubeAA& v) const { return vcMin == v.vcMin && v.vcMax == vcMax;}
	BOOL operator!=(const LcCubeAA& v) const { return vcMin != v.vcMin || v.vcMax != vcMax;}
	
	void AddPoint(const VEC3* pV);
	void AddPoint(FLOAT x, FLOAT y, FLOAT z);	
	void AddPoint(const LcCubeAA* b);
	
	void Set(FLOAT x, FLOAT y, FLOAT z);	
	void Set(const VEC3* pV);
	void Set(const LcCubeAA& pV);
	
	BOOL IsInside(const VEC3* pV);
	VEC3 GetCenter() const;
	VEC3 GetExtent() const;
	
	void GetEdge(VEC3* pvcEdge);
	void GetLine(VEC3* pvcLine);	
	BOOL IsEmpty() const;	
	void Repair();
};





////////////////////////////////////////////////////////////////////////////////
//
// Simple Triangle
//
////////////////////////////////////////////////////////////////////////////////

struct LcTri
{
	union
	{
		struct
		{
			VEC3	_0;
			VEC3	_1;
			VEC3	_2;
		};// struct

		D3DVECTOR	m[3];

	}; // union
	

	LcTri();	
	LcTri(const VEC3*_m);
	LcTri(VEC3 __0,VEC3 __1,VEC3 __2);

	LcTri(const LcTri& _r);
	LcTri(const FLOAT* _p);
	
	LcTri(FLOAT _0x,FLOAT _0y,FLOAT _0z
		, FLOAT _1x,FLOAT _1y,FLOAT _1z
		, FLOAT _2x,FLOAT _2y,FLOAT _2z);
	
	// casting
	operator FLOAT*();
	operator const FLOAT*() const;
	
	bool operator==(const LcTri& v) const;
	bool operator!=(const LcTri& v) const;


	void Set(VEC3 __0, VEC3 __1, VEC3 __2);
};



////////////////////////////////////////////////////////////////////////////////
//
// Picking Trangle
//
////////////////////////////////////////////////////////////////////////////////


struct TpckTri
{
	union
	{
		struct
		{
			VEC3	p0;
			VEC3	p1;
			VEC3	p2;
		};

		D3DVECTOR	vcT3[3];			// Picking Triangle Positions		
	};

	VEC3		vcPck;					// Picking Position or Bound Center
	FLOAT		fStlSrtR;				// Distance from Camera for Sorting or Bound Radius
	
	// Attributessss.
	INT			nId;					// Triangle Index
	INT			nAt;					// Attribute. Floor, Stair, Wall, Ceiling...
	

	TpckTri();
	TpckTri(FLOAT fR, VEC3* vcC, VEC3* vcT0, VEC3* vcT1, VEC3* vcT2);
	TpckTri(INT nIdx, INT nAttrib, FLOAT fR, VEC3* vcC, VEC3* vcT0, VEC3* vcT1, VEC3* vcT2);

	FLOAT	GetBoundRadius();
	VEC3	GetBoundCenter();
};


typedef std::vector<TpckTri >	lsPckT;
typedef	TpckTri					BndTri;




////////////////////////////////////////////////////////////////////////////////
//
// Pikcing Quad from Outdoor Map
//
////////////////////////////////////////////////////////////////////////////////

struct TpckRc
{
	VEC3	vcQ[4];				// Picking Quad
};

typedef std::vector<TpckRc >	lsMcRect;
typedef lsMcRect::iterator		itMcRect;




////////////////////////////////////////////////////////////////////////////////
//
// Cylinder
//
////////////////////////////////////////////////////////////////////////////////

struct LcCylinder
{
	union{ struct{	VEC3 p;	VEC3 t;	FLOAT r; FLOAT h; };	FLOAT m[8];	};
	
	LcCylinder();	
	LcCylinder(const FLOAT*_m);
	LcCylinder(const LcCylinder& v);
	LcCylinder(FLOAT Px,FLOAT Py,FLOAT Pz,FLOAT Tx,FLOAT Ty,FLOAT Tz, FLOAT R, FLOAT H);
	LcCylinder(const VEC3& P, const VEC3& T, FLOAT R, FLOAT H);
	
	// casting
	operator FLOAT*();
	operator const FLOAT*() const;
	
	// unary operators
	bool operator==(const LcCylinder& v) const;
	bool operator!=(const LcCylinder& v) const;

	// 임의의 두점과 반경과 높이
	void Set(const VEC3& p0, const VEC3& p1, FLOAT R, FLOAT H);
};




struct LCXVECTOR2
{
	LCXVECTOR2();
	LCXVECTOR2( const FLOAT * );
	LCXVECTOR2( const LCXVECTOR2& );
	LCXVECTOR2( FLOAT x, FLOAT y );


	// casting
	operator FLOAT* ();
	operator const FLOAT* () const;

	// assignment operators
	LCXVECTOR2& operator += ( const LCXVECTOR2& );
	LCXVECTOR2& operator -= ( const LCXVECTOR2& );
	LCXVECTOR2& operator *= ( FLOAT );
	LCXVECTOR2& operator /= ( FLOAT );

	// unary operators
	LCXVECTOR2 operator + () const;
	LCXVECTOR2 operator - () const;

	// binary operators
	LCXVECTOR2 operator + ( const LCXVECTOR2& ) const;
	LCXVECTOR2 operator - ( const LCXVECTOR2& ) const;
	LCXVECTOR2 operator * ( FLOAT ) const;
	LCXVECTOR2 operator / ( FLOAT ) const;

	friend LCXVECTOR2 operator * ( FLOAT, const LCXVECTOR2& );

	BOOL operator == ( const LCXVECTOR2& ) const;
	BOOL operator != ( const LCXVECTOR2& ) const;

	FLOAT x, y;
};



//    Quaternions

struct LCXQUATERNION
{
	LCXQUATERNION();
	LCXQUATERNION( const FLOAT * );
	LCXQUATERNION( const LCXQUATERNION& );
	LCXQUATERNION( FLOAT x, FLOAT y, FLOAT z, FLOAT w );

	// casting
	operator FLOAT* ();
	operator const FLOAT* () const;

	// assignment operators
	LCXQUATERNION& operator += ( const LCXQUATERNION& );
	LCXQUATERNION& operator -= ( const LCXQUATERNION& );
	LCXQUATERNION& operator *= ( const LCXQUATERNION& );
	LCXQUATERNION& operator *= ( FLOAT );
	LCXQUATERNION& operator /= ( FLOAT );

	// unary operators
	LCXQUATERNION  operator + () const;
	LCXQUATERNION  operator - () const;

	// binary operators
	LCXQUATERNION operator + ( const LCXQUATERNION& ) const;
	LCXQUATERNION operator - ( const LCXQUATERNION& ) const;
	LCXQUATERNION operator * ( const LCXQUATERNION& ) const;
	LCXQUATERNION operator * ( FLOAT ) const;
	LCXQUATERNION operator / ( FLOAT ) const;

	friend LCXQUATERNION operator * (FLOAT, const LCXQUATERNION& );

	BOOL operator == ( const LCXQUATERNION& ) const;
	BOOL operator != ( const LCXQUATERNION& ) const;

	FLOAT x, y, z, w;
};


// Planes
struct LCXPLANE
{
	LCXPLANE();
	LCXPLANE( const FLOAT* );
	LCXPLANE( const LCXPLANE& );
	LCXPLANE( FLOAT a, FLOAT b, FLOAT c, FLOAT d );

	// casting
	operator FLOAT* ();
	operator const FLOAT* () const;

	// unary operators
	LCXPLANE operator + () const;
	LCXPLANE operator - () const;

	// binary operators
	BOOL operator == ( const LCXPLANE& ) const;
	BOOL operator != ( const LCXPLANE& ) const;

	FLOAT a, b, c, d;
};



// Colors
struct LCXCOLOR
{
	LCXCOLOR();
	LCXCOLOR( DWORD argb );
	LCXCOLOR( const FLOAT * );
	LCXCOLOR( const LCXCOLOR& );
	LCXCOLOR( FLOAT r, FLOAT g, FLOAT b, FLOAT a );

	// casting
	operator DWORD () const;

	operator FLOAT* ();
	operator const FLOAT* () const;

	// assignment operators
	LCXCOLOR& operator += ( const LCXCOLOR& );
	LCXCOLOR& operator -= ( const LCXCOLOR& );
	LCXCOLOR& operator *= ( FLOAT );
	LCXCOLOR& operator /= ( FLOAT );

	// unary operators
	LCXCOLOR operator + () const;
	LCXCOLOR operator - () const;

	// binary operators
	LCXCOLOR operator + ( const LCXCOLOR& ) const;
	LCXCOLOR operator - ( const LCXCOLOR& ) const;
	LCXCOLOR operator * ( FLOAT ) const;
	LCXCOLOR operator / ( FLOAT ) const;

	friend LCXCOLOR operator * (FLOAT, const LCXCOLOR& );

	BOOL operator == ( const LCXCOLOR& ) const;
	BOOL operator != ( const LCXCOLOR& ) const;

	FLOAT r, g, b, a;
};



//   Redefinition
//typedef LCXVECTOR2			VEC2;
//typedef LCXQUATERNION		QUAT;
//typedef LCXPLANE			DPLN;
//typedef LCXCOLOR			DCLR;




#ifdef _DEBUG
	#pragma comment(lib, "LcEuclid_.lib")
#else
	#pragma comment(lib, "LcEuclid.lib")
#endif

#endif

