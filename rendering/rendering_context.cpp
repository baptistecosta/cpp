/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

	#include <assert.h>
	#include "glew.h"
	#include "opengl.h"
	#include "opengl_renderer.h"
	#include "rendering_context.h"
	#include "resources\shader.h"
	#include "resources\text2d.h"
		
	using namespace owl;

//-----------------------------------------------------------------------------
RenderingContext::RenderingContext()
	:	model_mtx(null_ptr)
	,	proj_mtx(null_ptr)
	,	view_mtx(null_ptr)
	,	viewproj_mtx(null_ptr)
	,	material(null_ptr)
	,	texture_unit(0)
	,	normal_map_unit(0)
	,	lights(null_ptr)

	,	rd_geom(null_ptr)
	,	text_2d(null_ptr)
{
	//
}

//-----------------------------------------------------------------------------
void			RenderingContext::Draw(Shader& shader, GLRenderer& gl_renderer)
{
	__GL_CALL(glUseProgram(shader.getProgram()))
	
	shader.setUniforms(*this);
	gl_renderer.Draw(shader, *this);
}

//-----------------------------------------------------------------------------
void			RenderingContext::DrawText(Shader& shd, GLRenderer& gl_renderer)
{
//	assert(m_text_2d);
//	__GL_CALL(glUseProgram(shd->getProgram()))

// 	Font* font = m_text_2d->getFont();
// 	uint texture_name = font->m_texture->getGLTextureName();
// 	OpenGL::bindTexture(texture_name, GL_TEXTURE0);
// 
// 	setTexture(Picture::GLTextureUnit_0);
// 	setText2D(m_text_2d);

//	shd->setUniforms(*this);

// 	OpenGL::bind2DTextGeometry(m_text_2d);
// 	gl_renderer.drawText(m_text_2d);
}

//-----------------------------------------------------------------------------
Light&			RenderingContext::getLight(const int i)
{
	assert(i >= 0 && i < lights->Size());
	return *(*lights)[i];
}


//-----------------------------------------------------------------------------
void			RenderingContext::UseProgram(const Shader& s)
{	__GL_CALL(glUseProgram(s.getProgram()))	}
