#include "Vector.h"

using namespace Math;

Vector4::Vector4( void ) {
}

Vector4::Vector4( float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f ) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Vector4::~Vector4( void ) {
}
