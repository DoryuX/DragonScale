#include <cstdlib>
#include <cstdio>

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include "Utils.h"
#include "Vector3.h"
#include "Matrix4.h"

static bool moving = false;
static float camera_pos[ 3 ] = { 0.0f, 0.0f, 5.0f };
static float target_pos[ 3 ] = { 0.0f, 0.0f, 0.0f };
static float up_pos[ 3 ]	 = { 0.0f, 1.0f, 0.0f };

static const int WINDOW_HEIGHT = 600;
static const int WINDOW_WIDTH = 800;

static const char* TITLE = "DragonScale";

GLuint vao[2];
GLuint buffers[2];

const int G_POSITION = 0;
const int G_COLOR = 1;

int PollKeys( void ) {
	int status = 0;
	SDL_Event event;

	while ( SDL_PollEvent( &event ) ) {
		switch( event.type ) {
			case SDL_KEYDOWN:	
				if ( event.key.keysym.sym == SDLK_a ) {
					camera_pos[ 0 ] -= 1.0f;
					moving = true;
				}
				if ( event.key.keysym.sym == SDLK_d ) {
					camera_pos[ 0 ] += 1.0f;
					moving = true;
				}
				if ( event.key.keysym.sym == SDLK_r ) {
					camera_pos[ 1 ] += 1.0f;
					moving = true;
				}
				if ( event.key.keysym.sym == SDLK_f ) {
					camera_pos[ 1 ] -= 1.0f;
					moving = true;
				}
				if ( event.key.keysym.sym == SDLK_w ) {
					camera_pos[ 2 ] -= 1.0f;
					moving = true;
				}
				if ( event.key.keysym.sym == SDLK_s ) {
					camera_pos[ 2 ] += 1.0f;
					moving = true;
				}
				if ( event.key.keysym.sym == SDLK_t ) {
					target_pos[ 1 ] += 1.0f;
					moving = true;
				}
				if ( event.key.keysym.sym == SDLK_g ) {
					target_pos[ 1 ] -= 1.0f;
					moving = true;
				}
				break;
			case SDL_KEYUP:
				if ( event.key.keysym.sym == SDLK_ESCAPE ) {
					status = SDL_EventType::SDL_QUIT;
				} else {
					moving = false;
				}
				break;
			case SDL_QUIT:
				status = SDL_EventType::SDL_QUIT;
				break;
		}
	}

	return status;
}

/*
	Assumes vertex and color data are the same size.
*/
void InitObject( const GLuint vao, 
				const GLuint vID, const GLvoid* vData,
				const GLuint cID, const GLvoid* cData,
				const GLuint size ) {
	GLuint buffers[ 2 ];

	// Load Triangle Data
	glBindVertexArray( vao );

	glGenBuffers( 2, buffers );

	// Vertex Position
	glEnableVertexAttribArray( vID );
	glBindBuffer( GL_ARRAY_BUFFER, buffers[ G_POSITION ] );
	glBufferData( GL_ARRAY_BUFFER, sizeof( GLfloat ) * 3 * size, vData, GL_STATIC_DRAW );	
	glVertexAttribPointer( vID, 3, GL_FLOAT, GL_FALSE, 0, 0 );

	// Color Data
	glEnableVertexAttribArray( cID );
	glBindBuffer( GL_ARRAY_BUFFER, buffers[ G_COLOR ] );
	glBufferData( GL_ARRAY_BUFFER, sizeof( GLfloat ) * 3 * size, cData, GL_STATIC_DRAW );
	glVertexAttribPointer( cID, 3, GL_FLOAT, GL_FALSE, 0, 0 );

	glBindVertexArray( 0 );
}

void Render( const GLuint vao, const GLuint size ) {
	glBindVertexArray( vao );
	glDrawArrays( GL_TRIANGLES, 0, size );
	glBindVertexArray( 0 );
}

int main( int argc, char* argv[] ) {

	// Initialize video subsystem.
	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		DS::SDLDie( "Unable to initialize SDL." );
	}

	SDL_Window* mainWindow;
	SDL_GLContext mainContext;

	// Request OpenGL Context
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
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
		DS::SDLDie( "Unable to create a window." );
	}

	mainContext = SDL_GL_CreateContext( mainWindow );

	SDL_GL_SetSwapInterval( 1 );

	glewExperimental = GL_TRUE;
	if ( glewInit() != GLEW_OK ) {
		fprintf( stderr, "Failed to initialize GLEW!\n" );
		DS::SDLDie( "Glew could not be initialized." );
	}

	// Triangle Data
	static const GLfloat triangleBufferData[] = {
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f
	};

	static const GLfloat triangleColorData[] = { 
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	// Cube Data
	static const GLfloat cubeBufferData[] = {
		-1.0f,-1.0f,-1.0f, 
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, 
		 1.0f, 1.0f,-1.0f, 
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f
	};

	// One color for each vertex. They were generated randomly.
	static const GLfloat cubeColorData[] = {
		0.583f,  0.771f,  0.014f,
		0.609f,  0.115f,  0.436f,
		0.327f,  0.483f,  0.844f,
		0.822f,  0.569f,  0.201f,
		0.435f,  0.602f,  0.223f,
		0.310f,  0.747f,  0.185f,
		0.597f,  0.770f,  0.761f,
		0.559f,  0.436f,  0.730f,
		0.359f,  0.583f,  0.152f,
		0.483f,  0.596f,  0.789f,
		0.559f,  0.861f,  0.639f,
		0.195f,  0.548f,  0.859f,
		0.014f,  0.184f,  0.576f,
		0.771f,  0.328f,  0.970f,
		0.406f,  0.615f,  0.116f,
		0.676f,  0.977f,  0.133f,
		0.971f,  0.572f,  0.833f,
		0.140f,  0.616f,  0.489f,
		0.997f,  0.513f,  0.064f,
		0.945f,  0.719f,  0.592f,
		0.543f,  0.021f,  0.978f,
		0.279f,  0.317f,  0.505f,
		0.167f,  0.620f,  0.077f,
		0.347f,  0.857f,  0.137f,
		0.055f,  0.953f,  0.042f,
		0.714f,  0.505f,  0.345f,
		0.783f,  0.290f,  0.734f,
		0.722f,  0.645f,  0.174f,
		0.302f,  0.455f,  0.848f,
		0.225f,  0.587f,  0.040f,
		0.517f,  0.713f,  0.338f,
		0.053f,  0.959f,  0.120f,
		0.393f,  0.621f,  0.362f,
		0.673f,  0.211f,  0.457f,
		0.820f,  0.883f,  0.371f,
		0.982f,  0.099f,  0.879f
	};

	// GLSL Shaders
	GLuint programID = DS::LoadShaders( "simple.vert", "simple.frag" );
	glUseProgram( programID );

	GLuint vertexID = glGetAttribLocation( programID, "vPos_model" );
	GLuint colorID = glGetAttribLocation( programID, "vColor" );

	glGenVertexArrays( 2, &vao[ 0 ] );

	// Cube
	InitObject( vao[ 0 ], 
				vertexID, &cubeBufferData[ 0 ], 
				colorID, &cubeColorData[ 0 ], 
				36 );

	// Triangle
	InitObject( vao[ 1 ], 
				vertexID, &triangleBufferData[ 0 ], 
				colorID, &triangleColorData[ 0 ], 
				3 );

	Math::Matrix4 projection = DS::Perspective( 
								45.0f, 
								( float ) WINDOW_WIDTH / WINDOW_HEIGHT, 
								0.1f, 100.0f );

	Math::Matrix4 view = DS::LookAt( 
							Math::Vector3( camera_pos[ 0 ], camera_pos[ 1 ], camera_pos[ 2 ] ),
							Math::Vector3( target_pos[ 0 ], target_pos[ 1 ], target_pos[ 2 ] ),
							Math::Vector3( up_pos[ 0 ], up_pos[ 1 ], up_pos[ 2 ] ) );

	Math::Matrix4 model = Math::Matrix4();

	Math::Matrix4 mv = Math::Multiply( view, model );

	GLuint projID = glGetUniformLocation( programID, "PROJ" );
	GLuint mvID = glGetUniformLocation( programID, "MODELVIEW" );

	glUniformMatrix4fv( projID, 1, GL_FALSE, &projection.c[ 0 ][ 0 ] );
	glUniformMatrix4fv( mvID, 1, GL_FALSE, &mv.c[ 0 ][ 0 ] );

	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LESS );
	glClearColor( 0.0f, 0.0f, 1.0f, 1.0f );

	// Main Loop
	while ( true ) {
		if ( PollKeys() == SDL_EventType::SDL_QUIT ) {
			break;
		}

		if ( moving ) {
			view = DS::LookAt( 
							Math::Vector3( camera_pos[ 0 ], camera_pos[ 1 ], camera_pos[ 2 ] ),
							Math::Vector3( target_pos[ 0 ], target_pos[ 1 ], target_pos[ 2 ] ),
							Math::Vector3( up_pos[ 0 ], up_pos[ 1 ], up_pos[ 2 ] ) );

			mv = Math::Multiply( view, model );

			glUniformMatrix4fv( mvID, 1, GL_FALSE, &view.c[ 0 ][ 0 ] );
		}

		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		Render( vao[ 0 ], 36 );	// Cube
		Render( vao[ 1 ], 3 );	// Triangle
		
		SDL_GL_SwapWindow( mainWindow );		
	}

	// Delete the OpenGL context, destroy window, shutdown SDL.
	SDL_GL_DeleteContext( mainContext );
	SDL_DestroyWindow( mainWindow );
	SDL_Quit();

	return 0;
}
