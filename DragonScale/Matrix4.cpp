#include "Matrix4.h"

#include <cstdlib>
#include <cmath>
#include <cstring>
#include <algorithm>

using namespace Math;

Matrix4::Matrix4( void ) {
	// Identity Matrix
	c[ 0 ][ 0 ] = c[ 1 ][ 1 ] = c[ 2 ][ 2 ] = c[ 3 ][ 3 ] = 1.0f;

	c[ 0 ][ 1 ] = c[ 0 ][ 2 ] = c[ 0 ][ 3 ] = 
	c[ 1 ][ 0 ] = c[ 1 ][ 2 ] = c[ 1 ][ 3 ] =
	c[ 2 ][ 0 ] = c[ 2 ][ 1 ] = c[ 2 ][ 3 ] = 
	c[ 3 ][ 0 ] = c[ 3 ][ 1 ] = c[ 3 ][ 2 ] = 0.0f;
}

Matrix4::Matrix4( const float m[ 4 ][ 4 ] ) {
	memcpy( c, m, 16 * sizeof( float ) );
}

Matrix4::Matrix4( float t00, float t01, float t02, float t03,
				  float t10, float t11, float t12, float t13,
				  float t20, float t21, float t22, float t23,
				  float t30, float t31, float t32, float t33 ) {
	c[ 0 ][ 0 ] = t00; c[ 0 ][ 1 ] = t01; c[ 0 ][ 2 ] = t02; c[ 0 ][ 3 ] = t03;
	c[ 1 ][ 0 ] = t10; c[ 1 ][ 1 ] = t11; c[ 1 ][ 2 ] = t12; c[ 1 ][ 3 ] = t13;
	c[ 2 ][ 0 ] = t20; c[ 2 ][ 1 ] = t21; c[ 2 ][ 2 ] = t22; c[ 2 ][ 3 ] = t23;
	c[ 3 ][ 0 ] = t30; c[ 3 ][ 1 ] = t31; c[ 3 ][ 2 ] = t23; c[ 3 ][ 3 ] = t33;
}

Matrix4::~Matrix4( void ) {
}

bool Matrix4::Compare( const Matrix4& m ) const {
	for( int i = 0; i < 4; ++i ) {
		for( int j = 0; j < 4; ++j ) {
			if ( m.c[ i ][ j ] != c[ i ][ j ] ) {
				return false;
			}
		}
	}

	return false;
}

Matrix4 Matrix4::GetTranspose( void ) {
	return Matrix4( c[ 0 ][ 0 ], c[ 1 ][ 0 ], c[ 2 ][ 0 ], c[ 3 ][ 0 ],
					c[ 0 ][ 1 ], c[ 1 ][ 1 ], c[ 2 ][ 1 ], c[ 3 ][ 1 ],
					c[ 0 ][ 2 ], c[ 1 ][ 2 ], c[ 2 ][ 2 ], c[ 3 ][ 2 ],
					c[ 0 ][ 3 ], c[ 1 ][ 3 ], c[ 2 ][ 3 ], c[ 3 ][ 3 ] );
}

// From Physically Based Rendering
Matrix4 Matrix4::GetInverse( void ) {
	int indxc[4], indxr[4];
    int ipiv[4] = { 0, 0, 0, 0 };
    float minv[4][4];
    memcpy(minv, c, 4*4*sizeof(float));
    for (int i = 0; i < 4; i++) {
        int irow = -1, icol = -1;
        float big = 0.;
        // Choose pivot
        for (int j = 0; j < 4; j++) {
            if (ipiv[j] != 1) {
                for (int k = 0; k < 4; k++) {
                    if (ipiv[k] == 0) {
                        if (fabsf(minv[j][k]) >= big) {
                            big = float(fabsf(minv[j][k]));
                            irow = j;
                            icol = k;
                        }
                    }
                    else if (ipiv[k] > 1)
                        printf("%s\n", "Singular matrix in MatrixInvert");
                }
            }
        }
        ++ipiv[icol];
        // Swap rows _irow_ and _icol_ for pivot
        if (irow != icol) {
            for (int k = 0; k < 4; ++k)
                std::swap(minv[irow][k], minv[icol][k]);
        }
        indxr[i] = irow;
        indxc[i] = icol;
        if (minv[icol][icol] == 0.)
            printf("%s\n", "Singular matrix in MatrixInvert");

        // Set $m[icol][icol]$ to one by scaling row _icol_ appropriately
        float pivinv = 1.f / minv[icol][icol];
        minv[icol][icol] = 1.f;
        for (int j = 0; j < 4; j++)
            minv[icol][j] *= pivinv;

        // Subtract this row from others to zero out their columns
        for (int j = 0; j < 4; j++) {
            if (j != icol) {
                float save = minv[j][icol];
                minv[j][icol] = 0;
                for (int k = 0; k < 4; k++)
                    minv[j][k] -= minv[icol][k]*save;
            }
        }
    }
    // Swap columns to reflect permutation
    for (int j = 3; j >= 0; j--) {
        if (indxr[j] != indxc[j]) {
            for (int k = 0; k < 4; k++)
                std::swap(minv[k][indxr[j]], minv[k][indxc[j]]);
        }
    }
    return Matrix4(minv);
}

bool Matrix4::operator==( const Matrix4& m ) const {
	return Compare( m );
}

bool Matrix4::operator!=( const Matrix4& m ) const {
	return !Compare( m );
}
