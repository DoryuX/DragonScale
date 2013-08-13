#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>

namespace Math {

class Vector3 {
public:
	Vector3( void );
	~Vector3( void );
	Vector3( float x, float y, float z );

	float LengthSquared( void ) const;
	float Length( void ) const;

	Vector3 operator+( const Vector3& v ) const;
	Vector3 operator-( const Vector3& v ) const;
	Vector3 operator*( const float s ) const;
	Vector3 operator/( const float s ) const;

	Vector3& operator+=( const Vector3& v );
	Vector3& operator-=( const Vector3& v );
	Vector3& operator*=( const float s );
	Vector3& operator/=( const float s );

	Vector3 operator-( void ) const;

	float operator[]( int i ) const;
	float& operator[]( int i );

	float x;
	float y;
	float z;
};

inline float Dot( const Vector3& v1, const Vector3& v2 ) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline float AbsDot( const Vector3& v1, const Vector3& v2 ) {
	return fabsf( Dot( v1, v2 ) );
}

inline Vector3 Cross( const Vector3& v1, const Vector3& v2 ) {
	return Vector3( ( v1.y * v2.z ) - ( v1.z * v2.y ),
					( v1.z * v2.x ) - ( v1.x * v2.z ),
					( v1.x * v2.y ) - ( v1.y * v2.x ) );
}

inline Vector3 Normalize( const Vector3& v ) {
	return ( v / v.Length() );
}

inline void CoordinateSystem( const Vector3& v1, Vector3* v2, Vector3* v3 ) {
	if ( fabsf( v1.x ) > fabsf( v1.y ) ) {
		float invLen = 1.0f / sqrtf( v1.x * v1.x + v1.z * v1.z );
		*v2 = Vector3( -v1.z * invLen, 0.0f, v1.x * invLen );
	} else {
		float invLen = 1.0f / sqrtf( v1.y * v1.y + v1.z * v1.z );
		*v2 = Vector3( 0.0f, v1.z * invLen, -v1.y * invLen );
	}

	*v3 = Cross( v1, *v2 );
}

}

#endif

