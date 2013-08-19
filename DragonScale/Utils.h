#ifndef UTILS_H
#define UTILS_H

#include "Vector3.h"
#include "Matrix4.h"

namespace DS {

	Math::Matrix4 Frustum( float xNeg, float xPos, 
								  float yNeg, float yPos, 
								  float zNear, float zFar );

	Math::Matrix4 Perspective( float fovY, 
									  float aspect, 
									  float zNear, float zFar );

	Math::Matrix4 LookAt( const Math::Vector3& eye, 
								 const Math::Vector3& center, 
								 const Math::Vector3& up );

	unsigned int LoadShaders( const char* vsFile, const char* fsFile );

	void SDLDie( const char* msg );

}

#endif

