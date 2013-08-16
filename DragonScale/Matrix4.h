#ifndef MATRIX4_H
#define MATRIX4_H

#include "Vector3.h"

namespace Math {

const float PI_OVER_180 = 0.1745329251f;

class Matrix4
{
public:
	Matrix4( void );
	Matrix4( const float m[ 4 ][ 4 ] );
	Matrix4( float t00, float t01, float t02, float t03,
			 float t10, float t11, float t12, float t13,
			 float t20, float t21, float t22, float t23,
			 float t30, float t31, float t32, float t33 );
	~Matrix4( void );

	bool Compare( const Matrix4& m ) const;
	Matrix4 GetTranspose( void );
	Matrix4 GetInverse( void );

	bool operator==( const Matrix4& m ) const;
	bool operator!=( const Matrix4& m ) const;

	Matrix4 operator+( const Matrix4& v ) const;
	Matrix4 operator-( const Matrix4& v ) const;

	Matrix4& operator+=( const Matrix4& v );
	Matrix4& operator-=( const Matrix4& v );

	float c[ 4 ][ 4 ];
};

/**
	Math::Multiply - Matrix-Matrix Multiplication

	Multiply two matrices together.
**/
inline Matrix4 Multiply( const Matrix4& m1, const Matrix4& m2 ) {
	Matrix4 r;

	for( int i = 0; i < 4; ++i ) {
		for( int j = 0; j < 4; ++j ) {
			r.c[ i ][ j ] = m1.c[ i ][ 0 ] * m2.c[ 0 ][ j ] +
							m1.c[ i ][ 1 ] * m2.c[ 1 ][ j ] +
							m1.c[ i ][ 2 ] * m2.c[ 2 ][ j ] +
							m1.c[ i ][ 3 ] * m2.c[ 3 ][ j ];
		}
	}

	return r;
}

/**
	Math::Multiply - Matrix-Vector Multiplication

	Create a transform vector.
**/
inline Vector3 Multiply( const Matrix4& m, const Vector3& v ) {
	Vector3 r;

	for( int i = 0; i < 3; ++i ) {
		r[ i ] = m.c[ i ][ 0 ] * v.x +
					m.c[ i ][ 1 ] * v.y +
					m.c[ i ][ 2 ] * v.z;
	}

	return r;
}

inline Matrix4 Scale( const Vector3& v  ) {
	Matrix4 m;

	m.c[ 0 ][ 0 ] = v.x;
	m.c[ 1 ][ 1 ] = v.y;
	m.c[ 2 ][ 2 ] = v.z;

	return m;
}

inline Matrix4 Translate( const Vector3& v ) {
	Matrix4 m;

	m.c[ 0 ][ 3 ] = v.x;
	m.c[ 1 ][ 3 ] = v.y;
	m.c[ 2 ][ 3 ] = v.z;

	return m;
}

inline Matrix4 Shear( const Vector3& x, const Vector3& y, const Vector3& z ) {
	Matrix4 m;

	m.c[ 1 ][ 0 ] = x.y;
	m.c[ 2 ][ 0 ] = x.z;

	m.c[ 0 ][ 1 ] = y.x;
	m.c[ 2 ][ 1 ] = y.z;

	m.c[ 0 ][ 2 ] = z.x;
	m.c[ 1 ][ 2 ] = z.y;

	return m;
}

/**
	Math::Euler

	Creates a rotation matrix from pitch, yaw, and roll.
	Parameters are in Degrees
**/
inline Matrix4 Euler( const float angleX, const float angleY, const float angleZ ) {
	Matrix4 m;

	float radX = angleX * PI_OVER_180;
	float radY = angleY * PI_OVER_180;
	float radZ = angleZ * PI_OVER_180;

	float a = cosf( angleX );
	float b = sinf( angleX );
	float c = cosf( angleY );
	float d = sinf( angleY );
	float e = cosf( angleZ );
	float f = sinf( angleZ );

	float ad = a * d;
	float bd = b * d;

	m.c[ 0 ][ 0 ]  =  c * e;
	m.c[ 0 ][ 1 ]  = -c * f;
	m.c[ 0 ][ 2 ]  =  d;
	m.c[ 1 ][ 0 ]  =  bd * e + a * f;
	m.c[ 1 ][ 1 ]  = -bd * f + a * e;
	m.c[ 1 ][ 2 ]  = -b * c;
	m.c[ 2 ][ 0 ]  = -ad * e + b * f;
	m.c[ 2 ][ 1 ]  =  ad * f + b * e;
	m.c[ 2 ][ 2 ] =  a * c;

	m.c[ 0 ][ 3 ] = m.c[ 1 ][ 3 ] = m.c[ 2 ][ 3 ] =
	m.c[ 3 ][ 0 ] = m.c[ 3 ][ 1 ] = m.c[ 3 ][ 2 ] = 0.0f;

	m.c[ 3 ][ 3 ] = 1;

	return m;
}

}

#endif

