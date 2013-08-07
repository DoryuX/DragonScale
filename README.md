# DragonScale

This project is an exercise in building a working game engine. Additions to
this engine will grow as more research is done.

## Dependencies

[**Simple DirectMedia Layer**](http://www.libsdl.org/tmp/download-2.0.php) - Provides joystick support as well as other hardware devices and OpenGL.
#### In Solution Explorer
- Right-click project, go to properties.
- Under VC++ Directories
	- Add '<SDL2 Path>/SDL2-*/include' to 'Include Directories'
	- Add '<SDL2 Path>/SDL2-*/lib/x86' to 'Reference Directories'
	- Add '<SDL2 Path>/SDL2-*/lib/x86' to 'Library Directories'
- Under Linker > Input
	- Add 'SDL2.lib', 'SDL2main.lib', and 'SDL2test.lib' to 'Additional Dependencies'

---

[**GLEW**](http://glew.sourceforge.net/) - Extension loading library.
#### For Visual C++ 2012 
- Build from source in Visual Studio 2012.
- Break up comments to compile.
	- Starting at '<GLEW Path>/glew-*/build/glew.rc', line 59
	- Solved in [**this**](http://sourceforge.net/p/glew/bugs/201/) bug report.
	- For example:

> "The OpenGL Extension Wrangler Library\r\nCopyright (C) 2002-2008, Milan Ikits <milan ikits[]ieee org>\r\n"

Becomes

> "The OpenGL Extension Wrangler Library\r\n"
> 
> "Copyright (C) 2002-2008, Milan Ikits <milan ikits[]ieee org>\r\n"

#### In Solution Explorer
- Right-click project, go to properties.
- Under VC++ Directories
	- Add '<GLEW Path>/glew-*/include' to 'Include Directories'
	- Add '<GLEW Path>/glew-*/bin/Release/Win32' to 'Reference Directories'
	- Add '<GLEW Path>/glew-*/lib/Release/Win32' to 'Library Directories'
- Under Linker > Input
	- Add 'glew32.lib' to 'Additional Dependencies'