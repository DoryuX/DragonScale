#ifndef MATRIX4_H
#define MATRIX4_H

namespace Math {

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

	float c[ 4 ][ 4 ];
};

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

}

#endif

