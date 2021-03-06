/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

	#include "externs/opengl/inc/glew.h"
	#include "externs/opengl/inc/GL/wglext.h"
	#include "vsync.h"

	using namespace owl;

//-----------------------------------------------------------------------------
void			VSync::Enable(bool b)
{
	typedef bool (APIENTRY *PFNWGLSWAPINTERVALFARPROC)(int);
	PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;
	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress("wglSwapIntervalEXT");
	if (wglSwapIntervalEXT)
		wglSwapIntervalEXT(b ? 1 : 0);
}
