/*
	bEngine :: 2011 - 2012
	Baptiste Costa
*/

#ifndef __SHADER_LOADER__
#define __SHADER_LOADER__

	#include "shader.h"

namespace owl {

class	RenderingContext;
class	String;

//!
class	ShaderLoader
{
private:

static	int				m_compilation_res,
						m_info_log_len;

public:

static	Shader*			Load(Shader::Type type, const String& vert_shader_path, const String& frag_shader_path);

private:

static	void			CompileShader(const uint shader_id, const char* shader_src_code);
static	uint			LinkProgram(const uint vert_shader_id, const uint frag_shader_id);
static	void			LogGLInfo(uint id);
};

}		// owl
#endif	// __SHADER_LOADER__
