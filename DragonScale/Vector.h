#ifndef VECTOR_H
#define VECTOR_H

namespace Math {

class Vector4 {
public:
	Vector4( void );
	~Vector4( void );
	Vector4( float x, float y, float z, float w );

	float x;
	float y;
	float z;
	float w;
};

}

#endif

