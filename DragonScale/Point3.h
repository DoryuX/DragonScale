#ifndef POINT3_H
#define POINT3_H

#include "Vector3.h"

namespace Math {

class Point3 {
public:
	Point3( void );
	Point3( float xx, float yy, float zz );
	~Point3( void );

	Point3 operator+( const Vector3& v ) const;
	Point3 operator-( const Vector3& v ) const;	
	Vector3 operator-( const Point3& p ) const;	
	Point3 operator*( const float f ) const;
	Point3 operator/( const float f ) const;

	Point3& operator+=( const Vector3& v );
	Point3& operator-=( const Vector3& v );
	Point3& operator*=( const float f );
	Point3& operator/=( const float f );

	Point3 operator-( void ) const;

	float operator[]( int i ) const;
	float& operator[]( int i );

	float x;
	float y;
	float z;
};

inline Point3 operator*( const float f, const Point3& p ) {
	return p * f;
}

inline float Distance( const Point3& p1, const Point3& p2 ) {
	return ( p1 - p2 ).Length();
}

inline float DistanceSquared( const Point3& p1, const Point3& p2 ) {
	return ( p1 - p2 ).LengthSquared();
}

}

#endif