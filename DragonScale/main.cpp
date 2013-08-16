#include <cstdlib>
#include <cstdio>
#include <string>
#include <cmath>
#include <fstream>
#include <vector>

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include "Vector3.h"
#include "Matrix4.h"

const float PI_OVER_360 = ( float ) M_PI / 360.0f;

void SDLDie( const char* msg ) {
	printf( "%s: %s\n", msg, SDL_GetError() );
	SDL_Quit();
	exit( 1 );
}

GLuint LoadShaders( const char* vsFile, const char* fsFile ) {
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

Math::Matrix4 Perspective( GLfloat fovY, GLfloat aspect, GLfloat zNear, GLfloat zFar ) {
	GLfloat fh = tanf( fovY * PI_OVER_360 ) * zNear;

	GLfloat xmin = -fh;
	GLfloat ymin = -fh;

	GLfloat width = fh - xmin;
	GLfloat height = fh - ymin;
	
	GLfloat depth = zFar - zNear;
	GLfloat q = -( zFar + zNear ) / depth;
	GLfloat qn = -2 * ( zFar * zNear ) / depth;

	GLfloat w = 2 * zNear / width;
	w = w / aspect;
	GLfloat h = 2 * zNear / height;

	return Math::Matrix4(    w, 0.0f, 0.0f,  0.0f,
						  0.0f,    h, 0.0f,  0.0f,
						  0.0f, 0.0f,    q, -1.0f,
						  0.0f, 0.0f,   qn,  0.0f );
}

Math::Matrix4 LookAt( const Math::Vector3& camera, const Math::Vector3& target, const Math::Vector3& up ) {
	Math::Vector3 axisZ = Math::Normalize( target - camera );
	Math::Vector3 axisX = Math::Normalize( Math::Cross( up, axisZ ) );
	Math::Vector3 axisY = Math::Cross( axisZ, axisX );

	// Right-handed Rotation Matrix
	/* 
	   TODO: Check Matrix Multiply vs. Dot product in right-most column?
	   -Dot( axisX, camera )
	   -Dot( axisY, camera )
	   -Dot( axisZ, camera )
	*/
	Math::Matrix4 rm = Math::Matrix4( axisX.x, axisX.y, axisX.z, 0.0f,
									  axisY.x, axisY.y, axisY.z, 0.0f,
									  axisZ.x, axisZ.y, axisZ.z, 0.0f,
										 0.0f,	  0.0f,	   0.0f, 1.0f );

	// Translation Matrix
	Math::Matrix4 tm = Math::Translate( -camera );

	return Math::Multiply( tm, rm );
}

int PollKeys( void ) {
	int status = 0;
	SDL_Event event;

	while ( SDL_PollEvent( &event ) ) {
		switch( event.type ) {
			case SDL_KEYDOWN:					
				break;
			case SDL_KEYUP:
				if ( event.key.keysym.sym == SDLK_ESCAPE ) {
					status = SDL_EventType::SDL_QUIT;
				}
				break;
			case SDL_QUIT:
				status = SDL_EventType::SDL_QUIT;
				break;
		}
	}

	return status;
}

void Render( const GLuint vertexID, const GLuint vertexBuffer, 
			 const GLuint attrib, const GLuint size, const GLuint stride ) {
	glClear( GL_COLOR_BUFFER_BIT );

	glEnableVertexAttribArray( vertexID );
	glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
	glVertexAttribPointer(
		attrib,
		size,
		GL_FLOAT,
		GL_FALSE,
		stride,
		( void* ) 0 );

	glDrawArrays( GL_TRIANGLES, 0, size );

	glDisableVertexAttribArray( vertexID );
}

int main( int argc, char* argv[] ) {

	// Initialize video subsystem.
	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		SDLDie( "Unable to initialize SDL." );
	}

	SDL_Window* mainWindow;
	SDL_GLContext mainContext;

	const int WINDOW_WIDTH = 800;
	const int WINDOW_HEIGHT = 600;
	const char* TITLE = "DragonScale";

	// Request OpenGL Context
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );

	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );	// 24-bit Z buffer.

	mainWindow = SDL_CreateWindow( TITLE,
								   SDL_WINDOWPOS_CENTERED,
								   SDL_WINDOWPOS_CENTERED,
								   WINDOW_WIDTH,
								   WINDOW_HEIGHT,
								   SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );

	if ( !mainWindow ) {
		SDLDie( "Unable to create a window." );
	}

	mainContext = SDL_GL_CreateContext( mainWindow );

	SDL_GL_SetSwapInterval( 1 );

	glewExperimental = true;
	if ( glewInit() != GLEW_OK ) {
		fprintf( stderr, "Failed to initialize GLEW!\n" );
		SDLDie( "Glew could not be initialized." );
	}

	glClearColor( 0.0f, 0.0f, 1.0f, 1.0f );

	GLuint vertexArrayID;
	glGenVertexArrays( 1, &vertexArrayID );
	glBindVertexArray( vertexArrayID );

	static const GLfloat vertexBufferData[] = {
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f
	};

	GLuint vertexBuffer;
	glGenBuffers( 1, &vertexBuffer );
	glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
	glBufferData( GL_ARRAY_BUFFER, 
				sizeof( vertexBufferData ), 
				vertexBufferData, 
				GL_STATIC_DRAW );	

	// GLSL Shaders
	GLuint programID = LoadShaders( "simple.vert", "simple.frag" );
	glUseProgram( programID );

	Math::Matrix4 projection = Perspective( 45.0f, 4.0f / 3.0f, 0.1f, 100.0f );
	Math::Matrix4 view = LookAt( Math::Vector3( 4.0f, 3.0f, 3.0f ),
								 Math::Vector3( 0.0f, 0.0f, 0.0f ),
								 Math::Vector3( 0.0f, 1.0f, 0.0f ) );
	Math::Matrix4 model = Math::Matrix4();
	Math::Matrix4 mvp = Math::Multiply( projection, Math::Multiply ( view, model ) );

	GLuint mvpID = glGetUniformLocation( programID, "MVP" );
	glUniformMatrix4fv( mvpID, 1, GL_TRUE, &mvp.c[ 0 ][ 0 ] );

	// Main Loop
	while ( true ) {
		if ( PollKeys() == SDL_EventType::SDL_QUIT ) {
			break;
		}

		Render( 0, vertexBuffer, 0, 3, 0 );

		SDL_GL_SwapWindow( mainWindow );		
	}

	// Delete the OpenGL context, destroy window, shutdown SDL.
	SDL_GL_DeleteContext( mainContext );
	SDL_DestroyWindow( mainWindow );
	SDL_Quit();

	return 0;
}
