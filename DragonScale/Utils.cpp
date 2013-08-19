#include "Utils.h"

#include <string>
#include <fstream>
#include <vector>

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

namespace DS {

void SDLDie( const char* msg ) {
	printf( "%s: %s\n", msg, SDL_GetError() );
	SDL_Quit();
	exit( 1 );
}

Math::Matrix4 Frustum( float xNeg, float xPos, 
							  float yNeg, float yPos, 
							  float zNear, float zFar ) {
	Math::Matrix4 m;

	m.c[ 0 ][ 0 ] = 2.0f * zNear / ( xPos - xNeg );
	m.c[ 0 ][ 1 ] = 0.0f;
	m.c[ 0 ][ 2 ] = 0.0f;
	m.c[ 0 ][ 3 ] = 0.0f;

	m.c[ 1 ][ 0 ] = 0.0f;
	m.c[ 1 ][ 1 ] = 2.0f * zNear / ( yPos - yNeg );
	m.c[ 1 ][ 2 ] = 0.0f;
	m.c[ 1 ][ 3 ] = 0.0f;

	m.c[ 2 ][ 0 ] =  ( xPos + xNeg )  / ( xPos - xNeg );
	m.c[ 2 ][ 1 ] =  ( yPos + yNeg )  / ( yPos - yNeg );
	m.c[ 2 ][ 2 ] = -( zFar + zNear ) / ( zFar - zNear );
	m.c[ 2 ][ 3 ] = -1.0f;

	m.c[ 3 ][ 0 ] = 0.0f;
	m.c[ 3 ][ 1 ] = 0.0f;
	m.c[ 3 ][ 2 ] = -2.0f * zFar * zNear / ( zFar - zNear );
	m.c[ 3 ][ 3 ] = 0.0f;

	return m;
}

Math::Matrix4 Perspective( float fovY, float aspect, float zNear, float zFar ) {
	float scale = tanf( fovY * Math::PI_OVER_360 ) * zNear;
	float right = aspect * scale;
	float left = -right;
	float top = scale;
	float bottom = -top;

	return Frustum( left, right, bottom, top, zNear, zFar );
}

Math::Matrix4 LookAt( const Math::Vector3& eye, const Math::Vector3& center, const Math::Vector3& up ) {
	Math::Vector3 axisZ = Math::Normalize( center - eye );
	Math::Vector3 axisY = Math::Normalize( up );
	Math::Vector3 axisX = Math::Normalize( Math::Cross( axisZ, axisY ) );
	axisY = Math::Cross( axisX, axisZ );

	Math::Matrix4 m;

	m.c[ 0 ][ 0 ] = axisX.x;
	m.c[ 1 ][ 0 ] = axisX.y;
	m.c[ 2 ][ 0 ] = axisX.z;

	m.c[ 0 ][ 1 ] = axisY.x;
	m.c[ 1 ][ 1 ] = axisY.y;
	m.c[ 2 ][ 1 ] = axisY.z;

	m.c[ 0 ][ 2 ] = -axisZ.x;
	m.c[ 1 ][ 2 ] = -axisZ.y;
	m.c[ 2 ][ 2 ] = -axisZ.z;

	m.c[ 3 ][ 0 ] = -Math::Dot( axisX, eye );
	m.c[ 3 ][ 1 ] = -Math::Dot( axisY, eye );
	m.c[ 3 ][ 2 ] = Math::Dot( axisZ, eye );

	return m;
}

unsigned int LoadShaders( const char* vsFile, const char* fsFile ) {
	// Create Shaders
	GLuint vsID = glCreateShader( GL_VERTEX_SHADER );
	GLuint fsID = glCreateShader( GL_FRAGMENT_SHADER );

	// Read Vertex Shader
	
	std::string vsCode;
	std::fstream vsStream;
	vsStream.open( vsFile, std::ios::in );
	
	if ( vsStream.is_open() ) {
		std::string line = "";

		while ( std::getline( vsStream, line ) ) {
			vsCode += "\n" + line;
		}

		vsStream.close();
	}
	

	// Read Fragment Shader
	std::string fsCode;
	std::ifstream fsStream( fsFile, std::ios::in );

	if ( fsStream.is_open() ) {
		std::string line = "";

		while ( std::getline( fsStream, line ) ) {
			fsCode += "\n" + line;
		}

		fsStream.close();
	}

	GLint result = GL_FALSE;
	int infoLogLength;

	// Compile Vertex Shader
	char const* vSourcePointer = vsCode.c_str();
	glShaderSource( vsID, 1, &vSourcePointer, NULL );
	glCompileShader( vsID );

	// Check Vertex Shader
	glGetShaderiv( vsID, GL_COMPILE_STATUS, &result );
	glGetShaderiv( vsID, GL_INFO_LOG_LENGTH, &infoLogLength );
	std::vector< char > vsErrorMessage( infoLogLength );
	glGetShaderInfoLog( vsID, infoLogLength, NULL, &vsErrorMessage[0] );
	fprintf( stdout, "%s\n", &vsErrorMessage[0] );

	// Compile Fragment Shader
	char const* fSourcePointer = fsCode.c_str();
	glShaderSource( fsID, 1, &fSourcePointer, NULL );
	glCompileShader( fsID );

	// Check Fragment Shader
	glGetShaderiv( fsID, GL_COMPILE_STATUS, &result );
	glGetShaderiv( fsID, GL_INFO_LOG_LENGTH, &infoLogLength );
	std::vector< char > fsErrorMessage( infoLogLength );
	glGetShaderInfoLog( fsID, infoLogLength, NULL, &fsErrorMessage[0] );
	fprintf( stdout, "%s\n", &fsErrorMessage[0] );

	// Link the Program
	GLuint programID = glCreateProgram();
	glAttachShader( programID, vsID );
	glAttachShader( programID, fsID );
	glLinkProgram( programID );

	// Check the Program
	glGetProgramiv( programID, GL_LINK_STATUS, &result );
	glGetProgramiv( programID, GL_INFO_LOG_LENGTH, &infoLogLength );
	std::vector< char > programErrorMessage( std::max( infoLogLength, int(1) ) );
	glGetProgramInfoLog( programID, infoLogLength, NULL, &programErrorMessage[0] );
	fprintf( stdout, "%s\n", &programErrorMessage[0] );

	glDeleteShader( vsID );
	glDeleteShader( fsID );

	return programID;
}

}