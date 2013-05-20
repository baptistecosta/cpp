/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

	#include "shadow_map_fbo.h"
	#include "glew.h"
	#include "core\log.h"
	
using namespace owl;

//-----------------------------------------------------------------------------
ShadowMapFBO::ShadowMapFBO()
	:	m_fbo_id(0)
	,	m_texture_id(0)
{
	//
}

//-----------------------------------------------------------------------------
ShadowMapFBO::~ShadowMapFBO()
{
	if (m_fbo_id)
		glDeleteFramebuffers(1, &m_fbo_id);

	if (m_texture_id)
		glDeleteTextures(1, &m_texture_id);
}

//-----------------------------------------------------------------------------
bool			ShadowMapFBO::init(uint window_w, uint window_h)
{
	// Create the FBO
	glGenFramebuffers(1, &m_fbo_id);

	// Create the depth buffer
	glGenTextures(1, &m_texture_id);
	glBindTexture(GL_TEXTURE_2D, m_texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, window_w, window_h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo_id);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_texture_id, 0);
	
	// Disable writes to the color buffer
	glDrawBuffer(GL_NONE);
	
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		Log::e("Frame buffer initialization failed with status 0x%x\n", status);
		return false;
	}
	return true;
}

//-----------------------------------------------------------------------------
void			ShadowMapFBO::bindForWriting()
{	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo_id);	}

//-----------------------------------------------------------------------------
void			ShadowMapFBO::bindForReading(GLenum texture_unit)
{
    glActiveTexture(texture_unit);
    glBindTexture(GL_TEXTURE_2D, m_texture_id);
}
