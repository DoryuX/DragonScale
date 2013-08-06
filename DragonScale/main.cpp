#include <cstdlib>
#include <cstdio>
#include <string>
#include <cmath>

#include <SDL.h>
#include <SDL_opengl.h>

void Display_InitGL( void ) {
	glShadeModel( GL_SMOOTH );

	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

	glClearDepth( 1.0f );

	glEnable( GL_DEPTH_TEST );

	glDepthFunc( GL_LEQUAL );

	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
}

void Perspective( GLfloat fovY, GLfloat aspect, GLfloat zNear, GLfloat zFar ) {
	GLfloat fw = 0.0f;
	GLfloat fh = 0.0f;

	fh = tanf( fovY / 360.0f * ( GLfloat ) M_PI ) * zNear;
	fw = fh * aspect;

	glFrustum( -fw, fw, -fh, fh, zNear, zFar );
}

int Display_SetViewport( int width, int height ) {
	GLfloat ratio;

	if ( height == 0 ) {
		height = 1;
	}

	ratio = ( GLfloat ) width / ( GLfloat ) height;

	glViewport( 0, 0, ( GLsizei ) width, ( GLsizei ) height );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	Perspective( 45.0f, ratio, 0.1f, 100.0f );

	glMatrixMode( GL_MODELVIEW );

	glLoadIdentity();

	return 1;
}

void Display_Render( SDL_Renderer* renderer ) {
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glLoadIdentity();
	glTranslatef( -1.5f, 0.0f, -6.0f );

	glColor3f( 1.0f, 0.0f, 0.0f );

	glBegin( GL_TRIANGLES );
		glVertex3f(  0.0f,  1.0f, 0.0f );
		glVertex3f( -1.0f, -1.0f, 0.0f );
		glVertex3f(  1.0f, -1.0f, 0.0f );
	glEnd();

	glTranslatef( 3.0f, 0.0f, 0.0f );

	glColor3f( 0.0f, 0.0f, 1.0f );
 
    glBegin( GL_QUADS );             
      glVertex3f( -1.0f,  1.0f, 0.0f ); 
      glVertex3f(  1.0f,  1.0f, 0.0f ); 
      glVertex3f(  1.0f, -1.0f, 0.0f ); 
      glVertex3f( -1.0f, -1.0f, 0.0f ); 
    glEnd( ); 

	SDL_RenderPresent( renderer );
}

int main( int argc, char* argv[] ) {
	SDL_Init( SDL_INIT_VIDEO );

	SDL_Window* displayWindow;
	SDL_Renderer* displayRenderer;
	SDL_RendererInfo displayRendererInfo;

	const int WINDOW_WIDTH = 800;
	const int WINDOW_HEIGHT = 600;

	SDL_CreateWindowAndRenderer( WINDOW_WIDTH, WINDOW_HEIGHT, 
								SDL_WINDOW_OPENGL, 
								&displayWindow, 
								&displayRenderer );

	SDL_GetRendererInfo( displayRenderer, &displayRendererInfo );

	Display_InitGL();

	Display_SetViewport( WINDOW_WIDTH, WINDOW_HEIGHT );

	Display_Render( displayRenderer );

	SDL_Delay( 5000 );

	SDL_Quit();
	return 0;
}
