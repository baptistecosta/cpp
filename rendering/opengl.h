/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __OPENGL__
#define __OPENGL__

	#include "typedefs.h"

#if _DEBUG
#define	__GL_CALL(_C_)	{	_C_; OpenGL::CheckGLError();	}
#else
#define	__GL_CALL(_C_)	{	_C_	};
#endif

namespace owl {

class	Picture;
class	RenderingGeometry;
class	Text2D;

//!
struct	OpenGL
{
		enum
		{
			Texture_0	=	0x84C0,
			Texture_1	=	0x84C1,
			Texture_2	=	0x84C2,
			Texture_3	=	0x84C3,
			Texture_4	=	0x84C4,
			Texture_5	=	0x84C5,
			Texture_6	=	0x84C6,
			Texture_7	=	0x84C7,
			Texture_8	=	0x84C8,
			Texture_9	=	0x84C9
		};

		void			Init();
		void			InitGlfw();
		void			InitGlew();

		void			ClearScreen();
		void			SwapBuffer();

		bool			Exit();
		void			CleanUp();

static	void			Bind3DGeometry(RenderingGeometry* rdr_geom);
static	void			Bind2DTextGeometry(Text2D& rdr_txt_geom);
static	void			CreateGLBuffer(void* data, size_t size, uint& buffer, uint usage/* = GL_STATIC_DRAW*/);
static	void			GenerateTexture(Picture& picture);
static	void			BindTexture(uint gl_texture_unit, const uint& GLuint_named_texture);
static	void			CheckGLError();
};

}		// owl
#endif	// __OPENGL__
