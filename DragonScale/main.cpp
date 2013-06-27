#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

HGLRC		hRC = NULL;		// Permanent Rendering Context.
HDC			hDC = NULL;		// Private GDI Device Context.
HWND		hWnd = NULL;	// Window Handle.
HINSTANCE	hInstance;		// Instance of the application.

bool keys[256];
bool active = TRUE;
bool fullscreen = FALSE;

LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

GLvoid ReSizeGLScene(GLsizei width, GLsizei height) {	
	// Prevent divide by zero.
	if ( height == 0 ) {
		height = 1;
	}

	glViewport( 0, 0, width, height );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	gluPerspective( 45.0f, ( GLfloat ) width / ( GLfloat ) height, 0.1f, 100.0f );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

int InitGL( GLvoid ) {
	glShadeModel( GL_SMOOTH );

	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

	glClearDepth( 1.0f );
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );

	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

	return TRUE;
}

int DrawGLScene( GLvoid ) {
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();

	return TRUE;
}

GLvoid KillGLWindow( GLvoid ) {
	if ( fullscreen ) {
		ChangeDisplaySettings( NULL, 0 );
		ShowCursor( TRUE );
	}

	if ( hRC ) {
		if ( !wglMakeCurrent( NULL, NULL ) ) {
			MessageBox( NULL, L"Release of DC and RC Failed.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
		}

		if ( !wglDeleteContext( hRC ) ) {
			MessageBox( NULL, L"Release Rendering Context Failed.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
		}

		hRC = NULL;
	}

	if ( hDC && !ReleaseDC( hWnd, hDC ) ) {
		MessageBox( NULL, L"Release Device Context Failed.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
		hDC = NULL;
	}

	if ( hWnd && !DestroyWindow( hWnd ) ) {
		MessageBox( NULL, L"Could Not Release hWnd.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
		hWnd = NULL;
	}

	if ( !UnregisterClass( L"OpenGL", hInstance ) ) {
		MessageBox( NULL, L"Could Not Unregister Class.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
		hInstance = NULL;
	}
}

BOOL CreateGLWindow( LPCWSTR title, int width, int height, int bits, bool fullscreenflag ) {
	GLuint PixelFormat;
	WNDCLASS wc;
	DWORD dwExStyle;
	DWORD dwStyle;
	RECT WindowRect;

	WindowRect.left		= ( long ) 0;
	WindowRect.right	= ( long ) width;
	WindowRect.top		= ( long ) 0;
	WindowRect.bottom	= ( long ) height;

	fullscreen = fullscreenflag;

	hInstance = GetModuleHandle( NULL );
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = ( WNDPROC ) WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon( NULL, IDI_WINLOGO );
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"OpenGL";

	if ( !RegisterClass( &wc ) ) {
		MessageBox( NULL, L"Failed to Register The Window Class.", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
		return FALSE;
	}

	if ( fullscreen ) {
		DEVMODE dmScreenSettings;
		memset( &dmScreenSettings, 0, sizeof( dmScreenSettings ) );
		dmScreenSettings.dmSize = sizeof( dmScreenSettings );
		dmScreenSettings.dmPelsWidth = width;
		dmScreenSettings.dmPelsHeight = height;
		dmScreenSettings.dmBitsPerPel = bits;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if( ChangeDisplaySettings( &dmScreenSettings, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL ) {
			if( MessageBox( NULL, 
					L"The Requested Fullscreen Mode Is Not Supported By\n Your Video Card. Use Windowed Mode Instead?",
					L"GL", MB_YESNO | MB_ICONEXCLAMATION ) == IDYES ) {
						fullscreen = FALSE;
			} else {
				MessageBox( NULL, L"Program Will Now Close.", L"ERROR", MB_OK | MB_ICONSTOP );
				return FALSE;
			}
		}
	}

	if ( fullscreen ) {
		dwExStyle =	WS_EX_APPWINDOW;
		dwStyle = WS_POPUP;
		ShowCursor( FALSE );
	} else {
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle = WS_OVERLAPPEDWINDOW;
	}

	AdjustWindowRectEx( &WindowRect, dwStyle, FALSE, dwExStyle );

	if ( !( hWnd = CreateWindowEx( dwExStyle,				// Extended Style for Window
					L"OpenGL",								// Class Name
					title,							// Window Title
					WS_CLIPSIBLINGS |						// Required Window Style
					WS_CLIPCHILDREN |						//
					dwStyle,								// Selected Window Style
					0, 0,									// Window Position
					WindowRect.right  - WindowRect.left,	// Window Width
					WindowRect.bottom - WindowRect.top,		// Window Height
					NULL,									// No Parent Window
					NULL,									// No Window
					hInstance,								// Instance
					NULL ) ) ) {							// Don't Pass Anything to WM_CREATE

		KillGLWindow();
		MessageBox( NULL, L"Window Creation Error.", L"ERROR", MB_OK | MB_ICONEXCLAMATION );

		return FALSE;
	}

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof( PIXELFORMATDESCRIPTOR ),
		1,									// Version Number
		PFD_DRAW_TO_WINDOW |				// Format Support Window
		PFD_SUPPORT_OPENGL |				// Format Support OpenGL
		PFD_DOUBLEBUFFER,					// Format Support Double Buffer
		PFD_TYPE_RGBA,						// Request RGBA Format
		bits,								// Color Depth
		0, 0, 0, 0, 0, 0,					// Color Bits Ignored
		0,									// No Alpha Buffer
		0,									// Shift Bit Ignored
		0,									// No Accumulation Buffer
		0, 0, 0, 0,							// Accumulation Bits Ignored
		16,									// 16-Bit Z-Buffer
		0,									// No Stencil Buffer
		0,									// No Auxiliary Buffer
		PFD_MAIN_PLANE,						// Main Drawing Layer
		0,									// Reserved
		0, 0, 0								// Layer Masks Ignored
	};

	if ( !( hDC = GetDC( hWnd ) ) ) {
		KillGLWindow();
		MessageBox( NULL, L"Can't Create A GL Device Context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION );

		return FALSE;
	}

	if ( !( PixelFormat = ChoosePixelFormat( hDC, &pfd ) ) ) {
		KillGLWindow();
		MessageBox( NULL, L"Can't Find A Suitable PixelFormat.", L"ERROR", MB_OK | MB_ICONEXCLAMATION );

		return FALSE;
	}

	if ( !SetPixelFormat( hDC, PixelFormat, &pfd ) ) {
		KillGLWindow();
		MessageBox( NULL, L"Can't Set The PixelFormat.", L"ERROR", MB_OK | MB_ICONEXCLAMATION );

		return FALSE;
	}

	if ( !( hRC = wglCreateContext( hDC ) ) ) {
		KillGLWindow();
		MessageBox( NULL, L"Can't Create A GL Rendering Context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION );

		return FALSE;
	}

	if ( !wglMakeCurrent( hDC, hRC ) ) {
		KillGLWindow();
		MessageBox( NULL, L"Can't Activate The GL Rendering Context.", L"Error", MB_OK | MB_ICONEXCLAMATION );

		return FALSE;
	}

	ShowWindow( hWnd, SW_SHOW );
	SetForegroundWindow( hWnd );
	SetFocus( hWnd );
	ReSizeGLScene( width, height );

	if ( !InitGL() ) {
		KillGLWindow();
		MessageBox( NULL, L"Initialization Failed.", L"ERROR", MB_OK | MB_ICONEXCLAMATION );

		return FALSE;
	}

	return TRUE;
}

LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
	switch( uMsg ) {
		case WM_ACTIVATE:					// Window Activate Message
		{
			if ( !HIWORD( wParam ) ) {      // Minimization State
				active = TRUE;
			} else {
				active = FALSE;
			}

			return 0;						// Return to Message Loop
		}

		case WM_SYSCOMMAND:					// Intercept System Commands
		{
			switch( wParam ) {				// System Calls
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:
					return 0;
			}

			break;							// Exit
		}

		case WM_CLOSE:
		{
			PostQuitMessage( 0 );
			return 0;
		}

		case WM_KEYDOWN:
		{
			keys[ wParam ] = TRUE;
			return 0;
		}

		case WM_KEYUP:
		{
			keys[ wParam ] = FALSE;
			return 0;
		}

		case WM_SIZE:
		{
			ReSizeGLScene( LOWORD( lParam ), HIWORD( lParam ) );
			return 0;
		}
	}

	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {
	MSG msg;
	BOOL done = FALSE;

	if ( !CreateGLWindow( L"DragonScale", 640, 480, 16, fullscreen ) ) {
		return 0;
	}

	while ( !done ) {
		if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) {
			if ( msg.message == WM_QUIT ) {
				done = TRUE;
			} else {
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		} else {
			if ( active ) {
				if ( keys[ VK_ESCAPE ] ) {
					done = TRUE;
				} else {
					DrawGLScene();
					SwapBuffers( hDC );
				}
			}

			if ( keys[ VK_F1 ] ) {
				keys[ VK_F1 ] = FALSE;
				KillGLWindow();
				fullscreen = !fullscreen;

				if ( !CreateGLWindow( L"DragonScale", 640, 480, 16, fullscreen ) ) {
					return 0;
				}				
			}			
		}
	}

	KillGLWindow();
	return ( msg.wParam );
}

