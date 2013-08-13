#include "Point3.h"

#include <cassert>

using namespace Math;

Point3::Point3( void ) {
	x = y = z = 0.0f;
}

Point3::Point3( float xx = 0.0f, float yy = 0.0f, float zz = 0.0f ) 
	: x( xx ), y( yy ), z( zz ) {
}

Point3::~Point3( void ) {
}

Point3 Point3::operator+( const Vector3& v ) const {
	float tx = x + v.x;
	float ty = y + v.y;
	float tz = z + v.z;

	return Point3( tx, ty, tz );
}

Point3 Point3::operator-( const Vector3& v ) const {
	float tx = x - v.x;
	float ty = y - v.y;
	float tz = z - v.z;

	return Point3( tx, ty, tz );
}

Vector3 Point3::operator-( const Point3& p ) const {
	float tx = x - p.x;
	float ty = y - p.y;
	float tz = z - p.z;

	return Vector3( tx, ty, tz );
}

Point3 Point3::operator*( const float s ) const {
	float tx = s * x;
	float ty = s * y;
	float tz = s * z;

	return Point3( tx, ty, tz );
}

Point3 Point3::operator/( const float s ) const {
	assert( s != 0 );
	
	float inv = 1.0f / s;

	float tx = inv * x;
	float ty = inv * y;
	float tz = inv * z;

	return Point3( tx, ty, tz );
}

Point3& Point3::operator+=( const Vector3& v ) {
	x += v.x;
	y += v.y;
	z += v.z;

	return *this;
}

Point3& Point3::operator-=( const Vector3& v ) {
	x -= v.x;
	y -= v.y;
	z -= v.z;

	return *this;
}

Point3& Point3::operator*=( const float s ) {
	x *= s;
	y *= s;
	z *= s;

	return *this;
}

Point3& Point3::operator/=( const float s ) {
	assert( s != 0 );

	float inv = 1.0f / s;

	x *= inv;
	y *= inv;
	z *= inv;

	return *this;
}

Point3 Point3::operator-( void ) const {
	return Point3( -x, -y, -z );
}

float Point3::operator[]( int i ) const {
	assert( i >= 0 && i <= 2 );

	return (&x)[i];
}

float& Point3::operator[]( int i ) {
	assert( i >= 0 && i <= 2 );

	return (&x)[i];
}
