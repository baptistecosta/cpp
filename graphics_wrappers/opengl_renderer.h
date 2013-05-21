/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __OPENGL_RENDERER__
#define __OPENGL_RENDERER__
	
	#include "core/math/matrix4.h"
	#include "core/math/vector.h"
	#include "typedefs.h"

namespace owl {

class	Geometry;
class	Material;
class	RenderingContext;
class	RenderingGeometry;
struct	RenderingVertices;
class	Shader;
class	Text2D;
struct	Vertex;

//!
class	GLRenderer
{
public:

		enum
		{
			Max2DBuffer		=	2,
			Max3DBuffer		=	8
		};

		void					Draw(Shader&, RenderingContext&);
		void					DrawLines(RenderingGeometry*);
		void					DrawText(Shader&, RenderingContext&);

private:

		void					BindArrayBufferAndAttribPointer(uint buffer, uint index, int size, uint type/* = GL_FLOAT*/, uchar normalized = false, int stride = 0, const void* pointer = (void*)0);
		void					BindArrayBufferAndAttribIPointer(uint buffer, uint index, int size, uint type/* = GL_INT*/, int stride = 0, const void* pointer = (void*)0);
		void					BindAndDrawElement(uint buffer, uint index, uint mode, int count, uint type/* = GL_UNSIGNED_SHORT*/, const void* indices = (void*)0);

public:

static	RenderingGeometry*		ConvertToRenderingGeometry(Geometry*);
static	bool					GetSimilarVertexIndex(Vertex*, RenderingVertices*, uint&);
};

}		//	owl
#endif	//	__OPENGL_RENDERER__
