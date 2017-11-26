//
//////////////////////////////////////////////////////////////////////////////


#include <windows.h>
#include <math.h>

#include <d3dx9math.h>

#include "LcEuclid.h"




//2D, or 3D Math
INT2::INT2():x(0),y(0){}
INT2::INT2(INT X,INT Y):x(X),y(Y){}
INT2::operator INT* ()				{	return (INT *) &x;				}
INT2::operator const INT* () const	{	return (const INT *) &x;		}


VEC2i::VEC2i()									{	x = y = 0;							}
VEC2i::VEC2i(INT _x,INT _y)					{	x = _x;			y = _y;				}
VEC2i::VEC2i(const VEC2i& rhs )				{	x = rhs.x;		y = rhs.y;			}
VEC2i::VEC2i(const INT *_m )					{	if(_m){x=_m[0];y=_m[2];}else{x=y=0;}}
VEC2i::VEC2i(D3DXVECTOR2 p)							{	x = INT(p.x);	y = INT(p.y);		}

VEC2i::operator INT*()							{	return (INT *) &x;					}
VEC2i::operator const INT*() const				{	return (const INT *) &x;			}

bool VEC2i::operator==(const VEC2i& v) const	{	return x == v.x && y == v.y;		}
bool VEC2i::operator!=(const VEC2i& v) const	{	return x != v.x || y != v.y;		}

const VEC2i& VEC2i::operator=(const VEC2i& rhs) {	x = rhs.x;	y = rhs.y;	return *this;	}

// assignment operators
VEC2i& VEC2i::operator+=(const VEC2i& rhs){	x += rhs.x;	y += rhs.y;	return *this;	}
VEC2i& VEC2i::operator-=(const VEC2i& rhs){	x -= rhs.x;	y -= rhs.y;	return *this;	}
VEC2i& VEC2i::operator*=(INT f )			{	x *= f;		y *= f;		return *this;	}
VEC2i& VEC2i::operator/=(INT f )			{	x /= f;		y /= f;		return *this;	}

// unary operators
VEC2i VEC2i::operator+() const				{	return *this;					}
VEC2i VEC2i::operator-() const				{	return VEC2i(-x, -y);			}

// binary operators
VEC2i VEC2i::operator+(const VEC2i& rhs) const	{	return VEC2i(x+rhs.x, y+rhs.y);	}
VEC2i VEC2i::operator-(const VEC2i& rhs) const	{	return VEC2i(x-rhs.x, y-rhs.y);	}
VEC2i VEC2i::operator*(const INT& f) const		{	return VEC2i(x*f, y*f);			}
VEC2i VEC2i::operator/(const INT& f) const		{	return VEC2i(x/f, y/f);			}


INT3::INT3():x(0),y(0),z(0){}
INT3::INT3(INT X,INT Y,INT Z):x(X),y(Y),z(Z){}
INT3::operator INT* ()				{	return (INT *) &x;				}
INT3::operator const INT* () const	{	return (const INT *) &x;		}


VEC3i::VEC3i()								{	x =				y =			  z = 0;		}
VEC3i::VEC3i(const INT *_m)				{	if(_m){x=_m[0];y=_m[1];z=_m[2];}else{x=y=z=0;}}
VEC3i::VEC3i(const VEC3i& rhs)				{	x = rhs.x;		y= rhs.y;	  z = rhs.z;	}
VEC3i::VEC3i(INT _x,INT _y, INT _z)		{	x = _x;			y = _y;		  z = _z;		}
VEC3i::VEC3i(const D3DXVECTOR3 p)			{	x = INT(p.x);	y = INT(p.y); z = INT(p.z);	}
VEC3i::VEC3i(FLOAT _x, FLOAT _y, FLOAT _z)	{	x = INT(_x);	y = INT(_y);  z = INT(_z);	}

VEC3i::operator INT*()						{	return (INT *) &x;							}
VEC3i::operator const INT*() const			{	return (const INT *) &x;					}

bool VEC3i::operator==(const VEC3i& v) const	{	return x == v.x && y == v.y && z == v.z;	}
bool VEC3i::operator!=(const VEC3i& v) const	{	return x != v.x || y != v.y || z != v.z;	}

const VEC3i& VEC3i::operator=(const VEC3i& rhs) {	x = rhs.x;	y = rhs.y;	z = rhs.z;	return *this;	}

VEC3i& VEC3i::operator+=(const VEC3i& v)	{	x += v.x; y += v.y;	z += v.z; return *this;	}
VEC3i& VEC3i::operator-=(const VEC3i& v)	{	x -= v.x; y -= v.y;	z -= v.z; return *this;	}
VEC3i& VEC3i::operator*=(INT f )			{	x *= f;	 y *= f;	z *= f;	  return *this;	}
VEC3i& VEC3i::operator/=(INT f )			{	x /= f;	 y /= f;	z /= f;	  return *this;	}

VEC3i VEC3i::operator+() const				{	return *this;								}
VEC3i VEC3i::operator-() const				{	return VEC3i(-x,-y, -z);					}

VEC3i VEC3i::operator+(const VEC3i& v)const{	return VEC3i(x + v.x, y + v.y, z + v.z);	}
VEC3i VEC3i::operator-(const VEC3i& v)const{	return VEC3i(x - v.x, y - v.y, z - v.z);	}
VEC3i VEC3i::operator*(INT f) const		{	return VEC3i(x*f, y*f, z*f);				}
VEC3i VEC3i::operator/(INT f) const		{	return VEC3i(x/f, y/f, z/f);				}



INT4::INT4():x(0),y(0),z(0),w(0){}
INT4::INT4(INT X,INT Y,INT Z,INT W):x(X),y(Y),z(Z),w(W){}
INT4::operator INT* ()				{	return (INT *) &x;				}
INT4::operator const INT* () const	{	return (const INT *) &x;		}



VEC4i::VEC4i()								{	x=y=z=w=0;									}
VEC4i::VEC4i(const INT *_m )				{	if(_m){x=_m[0];y=_m[1];z=_m[2];w=_m[3];}else{x=y=z=w=0;}	}
VEC4i::VEC4i(const VEC4i& r)				{	x=r.x; y=r.y; z=r.z; w= r.w;				}
VEC4i::VEC4i(INT X,INT Y,INT Z,INT W)		{	x=X; y=Y; z=Z; w=W;							}
VEC4i::VEC4i(const D3DXVECTOR4 p)			{	x=INT(p.x);y=INT(p.y);z=INT(p.z);w=INT(p.w);}

VEC4i::operator INT*()						{	return (INT *) &x;							}
VEC4i::operator const INT*() const			{	return (const INT *) &x;					}

bool VEC4i::operator==(const VEC4i& v) const{	return x==v.x&&y==v.y&&z==v.z&&w==v.w;		}
bool VEC4i::operator!=(const VEC4i& v) const{	return x!=v.x||y!=v.y||z!=v.z||w!=v.w;		}

const VEC4i& VEC4i::operator=(const VEC4i& rhs) {	x = rhs.x;	y = rhs.y;	z = rhs.z;	w = rhs.w;	return *this;	}

// assignment operators
VEC4i& VEC4i::operator+=(const VEC4i& v)	{	x+=v.x;y+=v.y;z+=v.z;w+=v.w;return *this;	}
VEC4i& VEC4i::operator-=(const VEC4i& v)	{	x-=v.x;y-=v.y;z-=v.z;w-=v.w;return *this;	}
VEC4i& VEC4i::operator*=(INT f )			{	x*=f; y*=f;	z*=f; w*=f;		return *this;	}
VEC4i& VEC4i::operator/=(INT f )			{	x/=f; y/=f;	z/=f; w/=f;		return *this;	}

// unary operators
VEC4i VEC4i::operator+() const				{	return *this;								}
VEC4i VEC4i::operator-() const				{	return VEC4i(-x,-y,-z,-w);				}

// binary operators
VEC4i VEC4i::operator+(const VEC4i& v)const{	return VEC4i(x+v.x,y+v.y,z+v.z,w+v.w);	}
VEC4i VEC4i::operator-(const VEC4i& v)const{	return VEC4i(x-v.x,y-v.y,z-v.z,w-v.w);	}
VEC4i VEC4i::operator*(INT f) const		{	return VEC4i(x*f,y*f,z*f, w*f);			}
VEC4i VEC4i::operator/(INT f) const		{	return VEC4i(x/f, y/f, z/f, w/f);		}



WORD2::WORD2():x(0),y(0){}
WORD2::WORD2(WORD X,WORD Y):x(X),y(Y){}
WORD2::operator WORD* ()				{	return (WORD *) &x;				}
WORD2::operator const WORD* () const	{	return (const WORD *) &x;		}


VEC2w::VEC2w()									{	x=y=0;		}
VEC2w::VEC2w(WORD X,WORD Y)					{	x=X; y=Y;	}



WORD3::WORD3():x(0),y(0),z(0){}
WORD3::WORD3(WORD X,WORD Y,WORD Z):x(X),y(Y),z(Z){}
WORD3::operator WORD* ()				{	return (WORD *) &x;				}
WORD3::operator const WORD* () const	{	return (const WORD *) &x;		}


VEC3w::VEC3w()									{	x=y=z=0;		}
VEC3w::VEC3w(WORD X,WORD Y, WORD Z, WORD W)	{	x=X; y=Y; z=Z;	}


WORD4::WORD4():x(0),y(0),z(0),w(0){}
WORD4::WORD4(WORD X,WORD Y,WORD Z,WORD W):x(X),y(Y),z(Z),w(W){}
WORD4::operator WORD* ()				{	return (WORD *) &x;				}
WORD4::operator const WORD* () const	{	return (const WORD *) &x;		}



VEC4w::VEC4w()									{	x=y=z=w=0;			}
VEC4w::VEC4w(WORD X,WORD Y, WORD Z, WORD W)	{	x=X; y=Y; z=Z; w=W;	}




DOUBLE2::DOUBLE2():x(0),y(0){}
DOUBLE2::DOUBLE2(DOUBLE X,DOUBLE Y):x(X),y(Y){}


DOUBLE3::DOUBLE3():x(0),y(0),z(0){}
DOUBLE3::DOUBLE3(DOUBLE X,DOUBLE Y,DOUBLE Z,DOUBLE W):x(X),y(Y),z(Z){}



DOUBLE4::DOUBLE4():x(0),y(0),z(0),w(0){}
DOUBLE4::DOUBLE4(DOUBLE X,DOUBLE Y,DOUBLE Z,DOUBLE W):x(X),y(Y),z(Z),w(W){}




LcRc::LcRc()									{	x0 = y0 = x1 = y1 = 0.f;										}
LcRc::LcRc(const FLOAT*_m)					{	if(!_m){x0=y0=x1=y1=0;}else{x0=_m[0];y0=_m[1];x1=_m[2];y1=_m[3];}}
LcRc::LcRc(const LcRc& _rh )						{	x0 = _rh.x0; y0 = _rh.y0; x1 = _rh.x1;	y1 = _rh.y1;			}

LcRc::LcRc( FLOAT fx0,FLOAT fy0,FLOAT fx1,FLOAT fy1)
{	x0 = fx0;	y0 = fy0;	x1 = fx1;	y1 = fy1;					}

LcRc::LcRc( D3DXVECTOR2 p0, D3DXVECTOR2 p1)	{	x0 = p0.x;	y0 = p0.y;	x1 = p1.x;	y1 = p1.y;					}

D3DXVECTOR2  LcRc::Get00()	 const			{	return D3DXVECTOR2(x0, y0);										}
D3DXVECTOR2  LcRc::Get10()	 const			{	return D3DXVECTOR2(x1, y0);										}
D3DXVECTOR2  LcRc::Get01()	 const			{	return D3DXVECTOR2(x0, y1);										}
D3DXVECTOR2  LcRc::Get11()	 const			{	return D3DXVECTOR2(x1, y1);										}

RECT   LcRc::GetRECT()						{	RECT r={LONG(x0),LONG(y0),LONG(x1),LONG(y1)};	return r;		}
D3DRECT LcRc::GetRectD3()						{	D3DRECT r={LONG(x0),LONG(y0),LONG(x1), LONG(y1)};	return r;	}

FLOAT	LcRc::GetWidth()						{	return (x1 - x0);												}
FLOAT	LcRc::GetHeight()						{	return (y1 - y0);												}
D3DXVECTOR2	LcRc::GetCenter() const				{	return D3DXVECTOR2((x0 + x1) / 2.f,	(y0 + y1) / 2.f);				}

// casting
LcRc::operator FLOAT*()						{	return (FLOAT *) &x0;											}
LcRc::operator const FLOAT*() const			{	return (const FLOAT *) &x0;										}

//const LcRc& LcRc::operator=(const LcRc& v)	{	x0 = v.x0;	y0 = v.y0;	x1 = v.x1;	y1 = v.y1;	return *this;	}

bool LcRc::operator==(const LcRc& v) const	{	return x0 == v.x0 && y0 == v.y0 && x1 == v.x1 && y1 == v.y1;	}
bool LcRc::operator!=(const LcRc& v) const	{	return x0 != v.x0 || y0 != v.y0 || x1 != v.x1 || y0 != v.y1;	}

// assignment operators
LcRc& LcRc::operator+=(const LcRc& v)			{	x0 += v.x0;	y0 += v.y0;	x1 += v.x1;	y1 += v.y1;	return *this;	}
LcRc& LcRc::operator-=(const LcRc& v)			{	x0 -= v.x0;	y0 -= v.y0;	x1 -= v.x1;	y1 -= v.y1;	return *this;	}
LcRc& LcRc::operator*=(FLOAT f )				{	x0 *= f;	y0 *= f;	x1 *= f;	y1 *= f;	return *this;	}
LcRc& LcRc::operator/=(FLOAT f )				{	FLOAT fInv = 1.0f / f;	x0 *= fInv;	y0 *= fInv;	x1 *= fInv;	y1 *= fInv;		return *this;	}

// unary operators
LcRc LcRc::operator+() const					{	return *this;													}
LcRc LcRc::operator-() const					{	return LcRc(-x0, -y0, -x1, -y1);								}

// binary operators
LcRc LcRc::operator+(const LcRc& v)const		{	return LcRc(x0 + v.x0, y0 + v.y0, x1 + v.x1, y1 + v.y1);		}
LcRc LcRc::operator-(const LcRc& v)const		{	return LcRc(x0 - v.x0, y0 - v.y0, x1 - v.x1, y1 - v.y1);		}
LcRc LcRc::operator*(FLOAT f) const			{	return LcRc(x0 * f, y0 * f, x1 * f, y1 * f);				}
LcRc LcRc::operator/(FLOAT f) const			{	FLOAT fInv = 1.0f / f;	return LcRc(x0 * fInv, y0 * fInv, x1 * fInv, y1 * fInv);		}


LcRc LcRc::operator+(const D3DXVECTOR2& v)			{	return LcRc(x0 + v.x, y0 + v.y, x1 + v.x, y1 + v.y);		}
LcRc LcRc::operator-(const D3DXVECTOR2& v)			{	return LcRc(x0 - v.x, y0 - v.y, x1 - v.x, y1 - v.y);		}
LcRc LcRc::operator+=(const D3DXVECTOR2& v)			{	return LcRc(x0 += v.x, y0 += v.y, x1 += v.x, y1 += v.y);	}
LcRc LcRc::operator-=(const D3DXVECTOR2& v)			{	return LcRc(x0 -= v.x, y0 -= v.y, x1 -= v.x, y1 -= v.y);	}

bool LcRc::IsValid() const
{
	FLOAT tx = x1 - x0;
	FLOAT ty = y1 - y0;
	
	return !(tx <= 0.f || ty <= 0.f);
}


bool LcRc::IsCollided(const D3DXVECTOR2& v) const
{
	return	x0 <= v.x && y0 <= v.y &&
			x1 >= v.x && y1 >= v.y;
}

bool LcRc::IsCollided(const LcRc& v) const
{
	return (x0 < v.x1 && x1 > v.x0 &&
			y0 < v.y1 && y1 > v.y0);
}

void LcRc::ClipAgainst(const LcRc& v)
{
	if (v.x1 < x1)
		x1 = v.x1;

	if (v.y1 < y1)
		y1 = v.y1;
	
	if (v.x0 > x0)
		x0 = v.x0;

	if (v.y0 > y0)
		y0 = v.y0;
	
	if (y0 > y1)
		y0 = y1;

	if (x0 > x1)
		x0 = x1;
}

void LcRc::SetCorrection()
{
	if(x1 < x0)
	{
		FLOAT _t = x1;
		x1 = x0;
		x0 = _t;
	}
	
	if (y1 < y0)
	{
		FLOAT _t = y1;
		y1 = y0;
		y0 = _t;
	}
}


	
	
	
	
LcLine::LcLine(){}
LcLine::LcLine(const FLOAT*_m)		{	if(_m){m[0]=_m[0];m[1]=_m[1];m[2]=_m[2];m[3]=_m[3];m[4]=_m[4];m[5]=_m[5];}else{m[0]=m[1]=m[2]=m[3]=m[4]=m[5]=0;}}
LcLine::LcLine(const LcLine& rhs)	{	p = rhs.p; t = rhs.t;	}

LcLine::LcLine(FLOAT Px,FLOAT Py,FLOAT Pz,FLOAT Tx,FLOAT Ty,FLOAT Tz)
{
	p.x = Px;
	p.y = Py;
	p.z = Pz;
	t.x = Tx;
	t.y = Ty;
	t.z = Tz;
}

LcLine::LcLine(const D3DXVECTOR3& P, const D3DXVECTOR3& T)
{
	p = P;
	t = T;
}

// casting
LcLine::operator FLOAT*()						{	return (FLOAT *) &p.x;			}
LcLine::operator const FLOAT*() const			{	return (const FLOAT *) &p.x;	}

// unary operators
LcLine LcLine::operator+() const				{	return LcLine(p.x, p.y, p.z, t.x, t.y, t.z);		}
LcLine LcLine::operator-() const				{	return LcLine(-p.x, -p.y, -p.z, -t.x, -t.y, -t.z);	}

bool LcLine::operator==(const LcLine& v) const	{	return p.x == v.p.x && p.y == v.p.y && p.z == v.p.z && t.x == v.t.x && t.y == v.t.y && t.z == v.t.z;	}
bool LcLine::operator!=(const LcLine& v) const	{	return p.x != v.p.x || p.y != v.p.y || p.z != v.p.z || t.x != v.t.x || t.y != v.t.y || t.z != v.t.z;	}






LcPlane::LcPlane(){}
LcPlane::LcPlane(const FLOAT*_m)					{	if(!_m){m[0]=m[1]=m[2]=m[3]=0;}else{m[0]=_m[0];m[1]=_m[1];m[2]=_m[2];m[3]=_m[3];}}
LcPlane::LcPlane(const LcPlane& _rh)				{	n = _rh.n; d = _rh.d;	}
LcPlane::LcPlane( FLOAT a,FLOAT b,FLOAT c,FLOAT d){	m[0]=a;m[1]=b;m[2]=c;m[3]=d;}

// casting
LcPlane::operator FLOAT*()						{	return (FLOAT *) &n.x;				}
LcPlane::operator const FLOAT*() const			{	return (const FLOAT *) &n.x;		}

bool LcPlane::operator==(const LcPlane& v) const	{	return n.x == v.n.x && n.y == v.n.y && n.z == v.n.z && d == v.d;	}
bool LcPlane::operator!=(const LcPlane& v) const	{	return n.x != v.n.x || n.y != v.n.y || n.z != v.n.z || n.y != v.d;	}





LcSphere::LcSphere(){}
LcSphere::LcSphere(const FLOAT*_m)					{	if(!_m){m[0]=m[1]=m[2]=m[3]=0;}else{m[0]=_m[0];m[1]=_m[1];m[2]=_m[2];m[3]=_m[3];}}
LcSphere::LcSphere(const LcSphere& _rh)				{	p = _rh.p; r = _rh.r;	}
LcSphere::LcSphere( FLOAT X,FLOAT Y,FLOAT Z,FLOAT R){	m[0]=X;m[1]=Y;m[2]=Z;m[3]=R;}

// casting
LcSphere::operator FLOAT*()						{	return (FLOAT *) &p.x;				}
LcSphere::operator const FLOAT*() const			{	return (const FLOAT *) &p.x;		}

// unary operators
LcSphere LcSphere::operator+() const				{	return *this;													}
LcSphere LcSphere::operator-() const				{	return LcSphere(-p.x, -p.y, -p.z, -r);								}

bool LcSphere::operator==(const LcSphere& v) const{	return p.x == v.p.x && p.y == v.p.y && p.z == v.p.z && r == v.r;	}
bool LcSphere::operator!=(const LcSphere& v) const{	return p.x != v.p.x || p.y != v.p.y || p.z != v.p.z || p.y != v.r;	}












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


LcCubeH::LcCubeH()
{
}

LcCubeH::LcCubeH(const VEC3* _m)
{
	if(_m)
	{
		_0 = _m[0];
		_1 = _m[1];
		_2 = _m[2];
		_3 = _m[3];
		_4 = _m[4];
		_5 = _m[5];
		_6 = _m[6];
		_7 = _m[7];
	}
	else
	{
		_0.x = 0; _0.y = 0; _0.z = 0;
		_1.x = 0; _1.y = 0; _1.z = 0;
		_2.x = 0; _2.y = 0; _2.z = 0;
		_3.x = 0; _3.y = 0; _3.z = 0;
		_4.x = 0; _4.y = 0; _4.z = 0;
		_5.x = 0; _5.y = 0; _5.z = 0;
		_6.x = 0; _6.y = 0; _6.z = 0;
		_7.x = 0; _7.y = 0; _7.z = 0;
	}
}

LcCubeH::LcCubeH(	  const VEC3& __0
			, const VEC3& __1
			, const VEC3& __2
			, const VEC3& __3
			, const VEC3& __4
			, const VEC3& __5
			, const VEC3& __6
			, const VEC3& __7)
{
	_0 = __0;	_1 = __1;	_2 = __2;	_3 = __3;
	_4 = __4;	_5 = __5;	_6 = __6;	_7 = __7;
}


LcCubeH::operator FLOAT*()
{
	return (FLOAT *) &_0.x;
}

LcCubeH::operator const FLOAT*() const
{
	return (const FLOAT *) &_0.x;
}


BOOL LcCubeH::operator==(const LcCubeH& v) const
{
	return	_0 == v._0 &&
			_1 == v._1 &&
			_2 == v._2 &&
			_3 == v._3 &&
			_4 == v._4 &&
			_5 == v._5 &&
			_6 == v._6 &&
			_7 == v._7;
}

BOOL LcCubeH::operator !=(const LcCubeH& v) const
{
	return	_0 != v._0 ||
			_1 != v._1 ||
			_2 != v._2 ||
			_3 != v._3 ||
			_4 != v._4 ||
			_5 != v._5 ||
			_6 != v._6 ||
			_7 != v._7;
}

void LcCubeH::Set(const VEC3* _m)
{
	if(_m)
	{
		_0 = _m[0];
		_1 = _m[1];
		_2 = _m[2];
		_3 = _m[3];
		_4 = _m[4];
		_5 = _m[5];
		_6 = _m[6];
		_7 = _m[7];
	}
	else
	{
		_0.x = 0; _0.y = 0; _0.z = 0;
		_1.x = 0; _1.y = 0; _1.z = 0;
		_2.x = 0; _2.y = 0; _2.z = 0;
		_3.x = 0; _3.y = 0; _3.z = 0;
		_4.x = 0; _4.y = 0; _4.z = 0;
		_5.x = 0; _5.y = 0; _5.z = 0;
		_6.x = 0; _6.y = 0; _6.z = 0;
		_7.x = 0; _7.y = 0; _7.z = 0;
	}
}

void LcCubeH::Set(const LcCubeH& r)
{
	_0 = r._0;
	_1 = r._1;
	_2 = r._2;
	_3 = r._3;
	_4 = r._4;
	_5 = r._5;
	_6 = r._6;
	_7 = r._7;
}









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


LcCubeAA::LcCubeAA()
	: vcMin(FLT_MAX,FLT_MAX,FLT_MAX)
	, vcMax(-FLT_MAX,-FLT_MAX,-FLT_MAX)
{
}

LcCubeAA::LcCubeAA(const VEC3* _m)
{
	if(_m)
	{
		vcMin=_m[0];
		vcMax=_m[1];
	}
	else
	{
		vcMin = VEC3(FLT_MAX,FLT_MAX,FLT_MAX);
		vcMax = VEC3(-FLT_MAX,-FLT_MAX,-FLT_MAX);
	}
}

LcCubeAA::LcCubeAA(const VEC3& _min, const VEC3& _max)
	: vcMin(_min)
	, vcMax(_max)
{
	
}

LcCubeAA::LcCubeAA(const VEC3& vc0)
	: vcMin(vc0)
	, vcMax(vc0)
{
}


LcCubeAA::LcCubeAA(FLOAT* _p)
{
	vcMin.x =_p[0]; vcMin.y =_p[1]; vcMin.z =_p[2];
	vcMax.x =_p[3]; vcMax.y =_p[4]; vcMax.z =_p[5];
}


LcCubeAA::LcCubeAA(FLOAT x0, FLOAT y0, FLOAT z0, FLOAT x1, FLOAT y1, FLOAT z1)
	: vcMin(x0, y0, z0)
	, vcMax(x1, y1, z1)
{
}

void LcCubeAA::AddPoint(const VEC3* pV)
{
	AddPoint(pV->x, pV->y, pV->z);
}

void LcCubeAA::AddPoint(FLOAT x, FLOAT y, FLOAT z)
{
	if (x>vcMax.x) vcMax.x = x;
	if (y>vcMax.y) vcMax.y = y;
	if (z>vcMax.z) vcMax.z = z;
	
	if (x<vcMin.x) vcMin.x = x;
	if (y<vcMin.y) vcMin.y = y;
	if (z<vcMin.z) vcMin.z = z;
}

void LcCubeAA::AddPoint(const LcCubeAA* b)
{
	AddPoint(&b->vcMax);
	AddPoint(&b->vcMin);
}

void LcCubeAA::Set(FLOAT x, FLOAT y, FLOAT z)
{
	vcMax =VEC3(x,y,z);
	vcMin = vcMax;
}


void LcCubeAA::Set(const VEC3* pV)
{
	vcMax = *pV;
	vcMin = *pV;
}


void LcCubeAA::Set(const LcCubeAA& pV)
{
	*this = pV;
}


BOOL LcCubeAA::IsInside(const VEC3* pV)
{
	return (pV->x >= vcMin.x && pV->x <= vcMax.x &&
			pV->y >= vcMin.y && pV->y <= vcMax.y &&
			pV->z >= vcMin.z && pV->z <= vcMax.z);
}


VEC3 LcCubeAA::GetCenter() const
{
	return (vcMin + vcMax) / 2.f;
}

VEC3 LcCubeAA::GetExtent() const
{
	return vcMax - vcMin;
}


void LcCubeAA::GetEdge(VEC3* pvcEdge)
{
	FLOAT fX = vcMax.x - vcMin.x;
	FLOAT fY = vcMax.y - vcMin.y;
	FLOAT fZ = vcMax.z - vcMin.z;
	
	pvcEdge[0] = vcMin;
	pvcEdge[1] = vcMin;	pvcEdge[1].y +=fY;
	pvcEdge[2] = vcMin;	pvcEdge[2].x +=fX;
	pvcEdge[3] = vcMax;	pvcEdge[3].z -=fZ;
	pvcEdge[4] = vcMin;	pvcEdge[4].z +=fZ;
	pvcEdge[5] = vcMax;	pvcEdge[5].x -=fX;
	pvcEdge[6] = vcMax;	pvcEdge[6].y -=fY;
	pvcEdge[7] = vcMax;

}


void LcCubeAA::GetLine(VEC3* pvcLine)
{
	VEC3 vcEdge[8];

	this->GetEdge(vcEdge);

	pvcLine[ 0] = vcEdge[0], pvcLine[ 1] = vcEdge[1];
	pvcLine[ 2] = vcEdge[0], pvcLine[ 3] = vcEdge[2];
	pvcLine[ 4] = vcEdge[1], pvcLine[ 5] = vcEdge[3];
	pvcLine[ 6] = vcEdge[2], pvcLine[ 7] = vcEdge[3];
	
	pvcLine[ 8] = vcEdge[4], pvcLine[ 9] = vcEdge[5];
	pvcLine[10] = vcEdge[4], pvcLine[11] = vcEdge[6];
	pvcLine[12] = vcEdge[5], pvcLine[13] = vcEdge[7];
	pvcLine[14] = vcEdge[6], pvcLine[15] = vcEdge[7];

	pvcLine[16] = vcEdge[0], pvcLine[17] = vcEdge[4];
	pvcLine[18] = vcEdge[1], pvcLine[19] = vcEdge[5];
	pvcLine[20] = vcEdge[3], pvcLine[21] = vcEdge[7];
	pvcLine[22] = vcEdge[2], pvcLine[23] = vcEdge[6];
}

BOOL LcCubeAA::IsEmpty() const
{
	VEC3 d = vcMin - vcMax;
	if (d.x < 0) d.x = -d.x;
	if (d.y < 0) d.y = -d.y;
	if (d.z < 0) d.z = -d.z;
	
	return (d.x < LC_ROUNDING_DELTA &&
		d.y < LC_ROUNDING_DELTA &&
		d.z < LC_ROUNDING_DELTA);
}

void LcCubeAA::Repair()
{
	FLOAT t;
	
	if (vcMin.x > vcMax.x)
	{
		t=vcMin.x; vcMin.x = vcMax.x; vcMax.x=t;
	}
	
	if (vcMin.y > vcMax.y)
	{
		t=vcMin.y; vcMin.y = vcMax.y; vcMax.y=t;
	}
	
	if (vcMin.z > vcMax.z)
	{
		t=vcMin.z; vcMin.z = vcMax.z; vcMax.z=t;
	}
}





////////////////////////////////////////////////////////////////////////////////

LcTri::LcTri()
{
}
	
LcTri::LcTri(const VEC3*_m)
{
	if(_m)
	{
		_0=_m[0];
		_1=_m[1];
		_2=_m[2];
	}
	else
	{
		_0.x =0; _0.y =0; _0.z =0;
		_1.x =0; _1.y =0; _1.z =0;
		_2.x =0; _2.y =0; _2.z =0;
	}
}

LcTri::LcTri(const LcTri& _r)
{
	_0 = _r._0;
	_1 = _r._1;
	_2 = _r._2;
}

LcTri::LcTri(const FLOAT* _p)
{
	_0.x =_p[0]; _0.y =_p[1]; _0.z =_p[2];
	_1.x =_p[3]; _1.y =_p[4]; _1.z =_p[5];
	_2.x =_p[6]; _2.y =_p[7]; _2.z =_p[8];
}

LcTri::LcTri(VEC3 __0,VEC3 __1,VEC3 __2)
{
	_0 = __0;
	_1 = __1;
	_2 = __2;
}


LcTri::LcTri(FLOAT _0x,FLOAT _0y,FLOAT _0z
	, FLOAT _1x,FLOAT _1y,FLOAT _1z
	, FLOAT _2x,FLOAT _2y,FLOAT _2z)
{
	_0.x =_0x; _0.y =_0y; _0.z =_0z;
	_1.x =_1x; _1.y =_1y; _1.z =_1z;
	_2.x =_2x; _2.y =_2y; _2.z =_2z;
}



void LcTri::Set(VEC3 __0, VEC3 __1, VEC3 __2)
{
	_0 = __0;
	_1 = __1;
	_2 = __2;
}


// casting
LcTri::operator FLOAT*()
{
	return (FLOAT *) &_0.x;
}

LcTri::operator const FLOAT*() const
{
	return (const FLOAT *) &_0.x;
}

bool LcTri::operator==(const LcTri& v) const
{
	return	   _0.x == v._0.x && _0.y == v._0.y && _0.z == v._0.z
			&& _1.x == v._1.x && _1.y == v._1.y && _1.z == v._1.z
			&& _2.x == v._2.x && _2.y == v._2.y && _2.z == v._2.z
			;
}

bool LcTri::operator!=(const LcTri& v) const
{
	return	   _0.x != v._0.x || _0.y != v._0.y || _0.z != v._0.z
			|| _1.x != v._1.x || _1.y != v._1.y || _1.z != v._1.z
			|| _2.x != v._2.x || _2.y != v._2.y || _2.z != v._2.z
			;
}






////////////////////////////////////////////////////////////////////////////////

TpckTri::TpckTri()
{
	nId	= 0xFFFFFFFF;
	nAt	= 0xFFFFFFFF;
}


TpckTri::TpckTri(FLOAT fR, VEC3* vcC, VEC3* vcT0, VEC3* vcT1, VEC3* vcT2)
{
	nId	= 0xFFFFFFFF;
	nAt	= 0xFFFFFFFF;

	fStlSrtR	= fR;
	
	if(vcC)	vcPck	= *vcC;
	if(vcT0)	p0	= *vcT0;
	if(vcT1)	p1	= *vcT1;
	if(vcT2)	p2	= *vcT2;
}

TpckTri::TpckTri(INT nIdx, INT nAttrib, FLOAT fR, VEC3* vcC, VEC3* vcT0, VEC3* vcT1, VEC3* vcT2)
{
	 nId	= nIdx;
	 nAt	= nAttrib;

	fStlSrtR	= fR;
	
	if(vcC)	vcPck	= *vcC;
	if(vcT0)	p0	= *vcT0;
	if(vcT1)	p1	= *vcT1;
	if(vcT2)	p2	= *vcT2;
}

FLOAT TpckTri::GetBoundRadius()
{
	return fStlSrtR;
}

VEC3 TpckTri::GetBoundCenter()
{
	return vcPck;
}











////////////////////////////////////////////////////////////////////////////////
//
// Cylinder
//
////////////////////////////////////////////////////////////////////////////////



// Cylinder
LcCylinder::LcCylinder()
{
}

LcCylinder::LcCylinder(const FLOAT*_m)
{
	if(_m)
	{
		m[0]=_m[0];
		m[1]=_m[1];
		m[2]=_m[2];
		m[3]=_m[3];
		m[4]=_m[4];
		m[5]=_m[5];
		m[6]=_m[6];
		m[7]=_m[7];		
	}
	else
	{
		m[0]=m[1]=m[2]=m[3]=m[4]=m[5]=m[6]=m[7]=0;
	}
}

LcCylinder::LcCylinder(const LcCylinder& v)
{
	p = v.p;
	t = v.t;
	r = v.r;
	h = v.h;
}

LcCylinder::LcCylinder(FLOAT Px,FLOAT Py,FLOAT Pz,FLOAT Tx,FLOAT Ty,FLOAT Tz, FLOAT R, FLOAT H)
{
	p.x = Px;
	p.y = Py;
	p.z = Pz;
	t.x = Tx;
	t.y = Ty;
	t.z = Tz;
	r	= R;
	h	= H;
}

LcCylinder::LcCylinder(const VEC3& P, const VEC3& T, FLOAT R, FLOAT H)
{
	p = P;
	t = T;
	r = R;
	h = H;
}

LcCylinder::operator FLOAT*()
{
	return (FLOAT *) &p.x;
}

LcCylinder::operator const FLOAT*() const
{
	return (const FLOAT *) &p.x;
}

bool LcCylinder::operator==(const LcCylinder& v) const
{
	return	p.x == v.p.x && p.y == v.p.y && p.z == v.p.z &&
			t.x == v.t.x && t.y == v.t.y && t.z == v.t.z &&
			r == v.r &&
			h == v.h;
}

bool LcCylinder::operator!=(const LcCylinder& v) const
{
	return	p.x != v.p.x || p.y != v.p.y || p.z != v.p.z ||
			t.x != v.t.x || t.y != v.t.y || t.z != v.t.z ||
			r != v.r ||
			h != v.h;
}



void LcCylinder::Set(const VEC3& p0, const VEC3& p1, FLOAT R, FLOAT H)
{
	p = p0;
	t = p1 - p;
	r = R;
	h = H;

	D3DXVec3Normalize(&t, &t);
}




// 2D Vector
LCXVECTOR2::LCXVECTOR2() : x(0), y(0)
{
}


LCXVECTOR2::LCXVECTOR2( const LCXVECTOR2& rhs)
{
	x = rhs.x;
	y = rhs.y;
}



LCXVECTOR2::LCXVECTOR2( const FLOAT *pf )
{
	x = pf[0];
	y = pf[1];
}



LCXVECTOR2::LCXVECTOR2( FLOAT fx, FLOAT fy )
{
	x = fx;
	y = fy;
}

// casting

LCXVECTOR2::operator FLOAT* ()
{
	return (FLOAT *) &x;
}


LCXVECTOR2::operator const FLOAT* () const
{
	return (const FLOAT *) &x;
}

// assignment operators
LCXVECTOR2& LCXVECTOR2::operator += ( const LCXVECTOR2& v )
{
	x += v.x;
	y += v.y;
	return *this;
}

LCXVECTOR2& LCXVECTOR2::operator -= ( const LCXVECTOR2& v )
{
	x -= v.x;
	y -= v.y;
	return *this;
}

LCXVECTOR2& LCXVECTOR2::operator *= ( FLOAT f )
{
	x *= f;
	y *= f;
	return *this;
}

LCXVECTOR2& LCXVECTOR2::operator /= ( FLOAT f )
{
	FLOAT fInv = 1.0f / f;
	x *= fInv;
	y *= fInv;
	return *this;
}

// unary operators
LCXVECTOR2 LCXVECTOR2::operator + () const
{
	return *this;
}

LCXVECTOR2 LCXVECTOR2::operator - () const
{
	return LCXVECTOR2(-x, -y);
}

// binary operators
LCXVECTOR2 LCXVECTOR2::operator + ( const LCXVECTOR2& v ) const
{
	return LCXVECTOR2(x + v.x, y + v.y);
}

LCXVECTOR2 LCXVECTOR2::operator - ( const LCXVECTOR2& v ) const
{
	return LCXVECTOR2(x - v.x, y - v.y);
}

LCXVECTOR2 LCXVECTOR2::operator * ( FLOAT f ) const
{
	return LCXVECTOR2(x * f, y * f);
}

LCXVECTOR2 LCXVECTOR2::operator / ( FLOAT f ) const
{
	FLOAT fInv = 1.0f / f;
	return LCXVECTOR2(x * fInv, y * fInv);
}


LCXVECTOR2 operator * ( FLOAT f, const LCXVECTOR2& v )
{
	return LCXVECTOR2(f * v.x, f * v.y);
}

BOOL LCXVECTOR2::operator == ( const LCXVECTOR2& v ) const
{
	return x == v.x && y == v.y;
}


BOOL LCXVECTOR2::operator != ( const LCXVECTOR2& v ) const
{
	return x != v.x || y != v.y;
}



// Quaternion

LCXQUATERNION::LCXQUATERNION() : x(0), y(0), z(0), w(0)
{
}


LCXQUATERNION::LCXQUATERNION( const FLOAT* pf )
{
	x = pf[0];
	y = pf[1];
	z = pf[2];
	w = pf[3];
}


LCXQUATERNION::LCXQUATERNION( const LCXQUATERNION& rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	w = rhs.w;
}



LCXQUATERNION::LCXQUATERNION( FLOAT fx, FLOAT fy, FLOAT fz, FLOAT fw )
{
	x = fx;
	y = fy;
	z = fz;
	w = fw;
}


// casting
LCXQUATERNION::operator FLOAT* ()
{
	return (FLOAT *) &x;
}


LCXQUATERNION::operator const FLOAT* () const
{
	return (const FLOAT *) &x;
}


// assignment operators
LCXQUATERNION& LCXQUATERNION::operator += ( const LCXQUATERNION& q )
{
	x += q.x;
	y += q.y;
	z += q.z;
	w += q.w;
	return *this;
}

LCXQUATERNION& LCXQUATERNION::operator -= ( const LCXQUATERNION& q )
{
	x -= q.x;
	y -= q.y;
	z -= q.z;
	w -= q.w;
	return *this;
}

LCXQUATERNION& LCXQUATERNION::operator *= ( const LCXQUATERNION& q )
{
//	LCXQuaternionMultiply(this, this, &q);
	return *this;
}

LCXQUATERNION& LCXQUATERNION::operator *= ( FLOAT f )
{
	x *= f;
	y *= f;
	z *= f;
	w *= f;
	return *this;
}

LCXQUATERNION& LCXQUATERNION::operator /= ( FLOAT f )
{
	FLOAT fInv = 1.0f / f;
	x *= fInv;
	y *= fInv;
	z *= fInv;
	w *= fInv;
	return *this;
}


// unary operators
LCXQUATERNION LCXQUATERNION::operator + () const
{
	return *this;
}

LCXQUATERNION LCXQUATERNION::operator - () const
{
	return LCXQUATERNION(-x, -y, -z, -w);
}


// binary operators
LCXQUATERNION LCXQUATERNION::operator + ( const LCXQUATERNION& q ) const
{
	return LCXQUATERNION(x + q.x, y + q.y, z + q.z, w + q.w);
}

LCXQUATERNION LCXQUATERNION::operator - ( const LCXQUATERNION& q ) const
{
	return LCXQUATERNION(x - q.x, y - q.y, z - q.z, w - q.w);
}

LCXQUATERNION LCXQUATERNION::operator * ( const LCXQUATERNION& q ) const
{
	LCXQUATERNION qT;
//	LCXQuaternionMultiply(&qT, this, &q);
	return qT;
}

LCXQUATERNION LCXQUATERNION::operator * ( FLOAT f ) const
{
	return LCXQUATERNION(x * f, y * f, z * f, w * f);
}

LCXQUATERNION LCXQUATERNION::operator / ( FLOAT f ) const
{
	FLOAT fInv = 1.0f / f;
	return LCXQUATERNION(x * fInv, y * fInv, z * fInv, w * fInv);
}


LCXQUATERNION operator * (FLOAT f, const LCXQUATERNION& q )
{
	return LCXQUATERNION(f * q.x, f * q.y, f * q.z, f * q.w);
}


BOOL LCXQUATERNION::operator == ( const LCXQUATERNION& q ) const
{
	return x == q.x && y == q.y && z == q.z && w == q.w;
}

BOOL LCXQUATERNION::operator != ( const LCXQUATERNION& q ) const
{
	return x != q.x || y != q.y || z != q.z || w != q.w;
}



// Plane


LCXPLANE::LCXPLANE() : a(0), b(0), c(0), d(0)
{
}



LCXPLANE::LCXPLANE( const FLOAT* pf )
{
	a = pf[0];
	b = pf[1];
	c = pf[2];
	d = pf[3];
}



LCXPLANE::LCXPLANE( const LCXPLANE& rhs )
{
	a = rhs.a;
	b = rhs.b;
	c = rhs.c;
	d = rhs.d;
}




LCXPLANE::LCXPLANE( FLOAT fa, FLOAT fb, FLOAT fc, FLOAT fd )
{
	a = fa;
	b = fb;
	c = fc;
	d = fd;
}


// casting

LCXPLANE::operator FLOAT* ()
{
	return (FLOAT *) &a;
}


LCXPLANE::operator const FLOAT* () const
{
	return (const FLOAT *) &a;
}


// unary operators
LCXPLANE LCXPLANE::operator + () const
{
	return *this;
}

LCXPLANE LCXPLANE::operator - () const
{
	return LCXPLANE(-a, -b, -c, -d);
}


// binary operators
BOOL LCXPLANE::operator == ( const LCXPLANE& p ) const
{
	return a == p.a && b == p.b && c == p.c && d == p.d;
}

BOOL LCXPLANE::operator != ( const LCXPLANE& p ) const
{
	return a != p.a || b != p.b || c != p.c || d != p.d;
}


// Color


LCXCOLOR::LCXCOLOR() : r(1), g(1), b(1), a(1)
{
}



LCXCOLOR::LCXCOLOR( DWORD dw )
{
	const FLOAT f = 1.0f / 255.0f;
	r = f * (FLOAT) (unsigned char) (dw >> 16);
	g = f * (FLOAT) (unsigned char) (dw >>  8);
	b = f * (FLOAT) (unsigned char) (dw >>  0);
	a = f * (FLOAT) (unsigned char) (dw >> 24);
}


LCXCOLOR::LCXCOLOR( const FLOAT* pf )
{
	r = pf[0];
	g = pf[1];
	b = pf[2];
	a = pf[3];
}


LCXCOLOR::LCXCOLOR( const LCXCOLOR& rhs )
{
	r = rhs.r;
	g = rhs.g;
	b = rhs.b;
	a = rhs.a;
}



LCXCOLOR::LCXCOLOR( FLOAT fr, FLOAT fg, FLOAT fb, FLOAT fa )
{
	r = fr;
	g = fg;
	b = fb;
	a = fa;
}


// casting

LCXCOLOR::operator DWORD () const
{
	DWORD dwR = r >= 1.0f ? 0xff : r <= 0.0f ? 0x00 : (DWORD) (r * 255.0f + 0.5f);
	DWORD dwG = g >= 1.0f ? 0xff : g <= 0.0f ? 0x00 : (DWORD) (g * 255.0f + 0.5f);
	DWORD dwB = b >= 1.0f ? 0xff : b <= 0.0f ? 0x00 : (DWORD) (b * 255.0f + 0.5f);
	DWORD dwA = a >= 1.0f ? 0xff : a <= 0.0f ? 0x00 : (DWORD) (a * 255.0f + 0.5f);

	return (dwA << 24) | (dwR << 16) | (dwG << 8) | dwB;
}



LCXCOLOR::operator FLOAT * ()
{
	return (FLOAT *) &r;
}


LCXCOLOR::operator const FLOAT * () const
{
	return (const FLOAT *) &r;
}


// assignment operators
LCXCOLOR& LCXCOLOR::operator += ( const LCXCOLOR& c )
{
	r += c.r;
	g += c.g;
	b += c.b;
	a += c.a;
	return *this;
}

LCXCOLOR& LCXCOLOR::operator -= ( const LCXCOLOR& c )
{
	r -= c.r;
	g -= c.g;
	b -= c.b;
	a -= c.a;
	return *this;
}

LCXCOLOR& LCXCOLOR::operator *= ( FLOAT f )
{
	r *= f;
	g *= f;
	b *= f;
	a *= f;
	return *this;
}

LCXCOLOR& LCXCOLOR::operator /= ( FLOAT f )
{
	FLOAT fInv = 1.0f / f;
	r *= fInv;
	g *= fInv;
	b *= fInv;
	a *= fInv;
	return *this;
}


// unary operators
LCXCOLOR LCXCOLOR::operator + () const
{
	return *this;
}

LCXCOLOR LCXCOLOR::operator - () const
{
	return LCXCOLOR(-r, -g, -b, -a);
}


// binary operators
LCXCOLOR LCXCOLOR::operator + ( const LCXCOLOR& c ) const
{
	return LCXCOLOR(r + c.r, g + c.g, b + c.b, a + c.a);
}

LCXCOLOR LCXCOLOR::operator - ( const LCXCOLOR& c ) const
{
	return LCXCOLOR(r - c.r, g - c.g, b - c.b, a - c.a);
}

LCXCOLOR LCXCOLOR::operator * ( FLOAT f ) const
{
	return LCXCOLOR(r * f, g * f, b * f, a * f);
}

LCXCOLOR LCXCOLOR::operator / ( FLOAT f ) const
{
	FLOAT fInv = 1.0f / f;
	return LCXCOLOR(r * fInv, g * fInv, b * fInv, a * fInv);
}


LCXCOLOR operator * (FLOAT f, const LCXCOLOR& c )
{
	return LCXCOLOR(f * c.r, f * c.g, f * c.b, f * c.a);
}


BOOL LCXCOLOR::operator == ( const LCXCOLOR& c ) const
{
	return r == c.r && g == c.g && b == c.b && a == c.a;
}

BOOL LCXCOLOR::operator != ( const LCXCOLOR& c ) const
{
	return r != c.r || g != c.g || b != c.b || a != c.a;
}


