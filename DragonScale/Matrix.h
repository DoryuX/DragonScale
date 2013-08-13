#ifndef MATRIX_H
#define MATRIX_H

namespace Math {

class Matrix4
{
public:
	Matrix4( void );
	~Matrix4( void );

	float r0c0, r0c1, r0c2, r0c3,
		  r1c0, r1c1, r1c2, r1c3,
		  r2c0, r2c1, r2c2, r2c3,
		  r3c0, r3c1, r3c2, r3c3;
};

}

#endif

