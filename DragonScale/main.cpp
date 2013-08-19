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

static const int WINDOW_HEIGHT = 800;
static const int WINDOW_WIDTH = 600;

static const char* TITLE = "DragonScale";

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
		DS::SDLDie( "Unable to initialize SDL." );
	}

	SDL_Window* mainWindow;
	SDL_GLContext mainContext;

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
		DS::SDLDie( "Unable to create a window." );
	}

	mainContext = SDL_GL_CreateContext( mainWindow );

	SDL_GL_SetSwapInterval( 1 );

	glewExperimental = true;
	if ( glewInit() != GLEW_OK ) {
		fprintf( stderr, "Failed to initialize GLEW!\n" );
		DS::SDLDie( "Glew could not be initialized." );
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
	GLuint programID = DS::LoadShaders( "simple.vert", "simple.frag" );
	glUseProgram( programID );

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

		Render( 0, vertexBuffer, 0, 3, 0 );

		SDL_GL_SwapWindow( mainWindow );		
	}

	// Delete the OpenGL context, destroy window, shutdown SDL.
	SDL_GL_DeleteContext( mainContext );
	SDL_DestroyWindow( mainWindow );
	SDL_Quit();

	return 0;
}
