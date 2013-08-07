#include <cstdlib>
#include <cstdio>
#include <string>
#include <cmath>

#include <SDL.h>
#include <SDL_opengl.h>

void SDLDie( const char* msg ) {
	printf( "%s: %s\n", msg, SDL_GetError() );
	SDL_Quit();
	exit( 1 );
}

void Perspective( GLfloat fovY, GLfloat aspect, GLfloat zNear, GLfloat zFar ) {
	GLfloat fw = 0.0f;
	GLfloat fh = 0.0f;

	fh = tanf( fovY / 360.0f * ( GLfloat ) M_PI ) * zNear;
	fw = fh * aspect;

	glFrustum( -fw, fw, -fh, fh, zNear, zFar );
}

int main( int argc, char* argv[] ) {

	// Initialize video subsystem.
	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		SDLDie( "Unable to initialize SDL." );
	}

	SDL_Window* mainWindow;
	SDL_GLContext mainContext;

	SDL_Event event;
	int status = 0;

	const int WINDOW_WIDTH = 800;
	const int WINDOW_HEIGHT = 600;
	const char* TITLE = "DragonScale";

	// Request OpenGL Context
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 );

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

	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

	while ( true ) {
		glClear( GL_COLOR_BUFFER_BIT );

		SDL_GL_SwapWindow( mainWindow );

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

		if ( status == SDL_EventType::SDL_QUIT ) {
			break;
		}
	}

	// Delete the OpenGL context, destroy window, shutdown SDL.
	SDL_GL_DeleteContext( mainContext );
	SDL_DestroyWindow( mainWindow );
	SDL_Quit();

	return 0;
}
