# DragonScale Compile Guide

## Compiling Under Visual Studio 2012

I ran into issues with Visual Studio giving me linker errors when looking for the CRT Debug libs. There's probably a better way to solve this, but after
much searching I finally got the code to run.
#### Visual Studio 2012 - In Solution Explorer
- Right-click project, go to properties.
- Under C/C++ > Preprocessor
	- Remove '_DEBUG;' under 'Preprocessor Definitions'

## Adding the Dependencies

[**Simple DirectMedia Layer**](http://www.libsdl.org/tmp/download-2.0.php) - Provides joystick support as well as other hardware devices and OpenGL.
#### Visual Studio 2012 - In Solution Explorer
- Right-click project, go to properties.
- Under VC++ Directories
	- Add '<SDL2 Path>/SDL2-*/include' to 'Include Directories'
	- Add '<SDL2 Path>/SDL2-*/lib/x86' to 'Reference Directories'
	- Add '<SDL2 Path>/SDL2-*/lib/x86' to 'Library Directories'
- Under Linker > Input
	- Add 'SDL2.lib', 'SDL2main.lib', and 'SDL2test.lib' to 'Additional Dependencies'
- Copy and Paste 'SDL2.dll' in the directory that contains the source code for this project.

---

[**GLEW**](http://glew.sourceforge.net/) - Extension loading library.
#### Building Library in Visual C++ 2012 ( version 1.10.0 )
- Download the source.
- Go to '<GLEW Path>/glew-*/build/vc10'
- Open project in VS2012, update from VS2010.
- Break up comments to compile.
	- Starting at '<GLEW Path>/glew-*/build/glew.rc', line 59
	- Solved in [**this**](http://sourceforge.net/p/glew/bugs/201/) bug report.
	- For example:

> "The OpenGL Extension Wrangler Library\r\nCopyright (C) 2002-2008, Milan Ikits <milan ikits[]ieee org>\r\n"

Becomes

> "The OpenGL Extension Wrangler Library\r\n"
> 
> "Copyright (C) 2002-2008, Milan Ikits <milan ikits[]ieee org>\r\n"

etc.

#### Visual Studio 2012 - Back to original project, in Solution Explorer
- Right-click project, go to properties.
- Under VC++ Directories
	- Add '<GLEW Path>/glew-*/include' to 'Include Directories'
	- Add '<GLEW Path>/glew-*/bin/Release/Win32' to 'Reference Directories'
	- Add '<GLEW Path>/glew-*/lib/Release/Win32' to 'Library Directories'
- Under Linker > Input
	- Add 'glew32.lib' to 'Additional Dependencies'
- Copy and Paste 'glew32.dll' in the directory that contains the source code for this project.