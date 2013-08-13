#include "Vector3.h"

#include <cassert>

using namespace Math;

Vector3::Vector3( void ) {
	x = y = z;
}

Vector3::Vector3( float xx = 0.0f, float yy = 0.0f, float zz = 0.0f )
	: x( xx ), y( yy ), z( zz ) {
}

Vector3::~Vector3( void ) {
}

float Vector3::LengthSquared( void ) const {
	float x2 = x * x;
	float y2 = y * y;
	float z2 = z * z;

	return x2 + y2 + z2;
}

float Vector3::Length( void ) const {
	return sqrtf( LengthSquared() );
}

Vector3 Vector3::operator+( const Vector3& v ) const {
	float tx = x + v.x;
	float ty = y + v.y;
	float tz = z + v.z;

	return Vector3( tx, ty, tz );
}

Vector3 Vector3::operator-( const Vector3& v ) const {
	float tx = x - v.x;
	float ty = y - v.y;
	float tz = z - v.z;

	return Vector3( tx, ty, tz );
}

Vector3 Vector3::operator*( const float s ) const {
	float tx = s * x;
	float ty = s * y;
	float tz = s * z;

	return Vector3( tx, ty, tz );
}

Vector3 Vector3::operator/( const float s ) const {
	assert( s != 0 );
	
	float inv = 1.0f / s;

	float tx = inv * x;
	float ty = inv * y;
	float tz = inv * z;

	return Vector3( tx, ty, tz );
}

Vector3& Vector3::operator+=( const Vector3& v ) {
	x += v.x;
	y += v.y;
	z += v.z;

	return *this;
}

Vector3& Vector3::operator-=( const Vector3& v ) {
	x -= v.x;
	y -= v.y;
	z -= v.z;

	return *this;
}

Vector3& Vector3::operator*=( const float s ) {
	x *= s;
	y *= s;
	z *= s;

	return *this;
}

Vector3& Vector3::operator/=( const float s ) {
	assert( s != 0 );

	float inv = 1.0f / s;

	x *= inv;
	y *= inv;
	z *= inv;

	return *this;
}

Vector3 Vector3::operator-( void ) const {
	return Vector3( -x, -y, -z );
}

float Vector3::operator[]( int i ) const {
	assert( i >= 0 && i <= 2 );

	return (&x)[i];
}

float& Vector3::operator[]( int i ) {
	assert( i >= 0 && i <= 2 );

	return (&x)[i];
}
