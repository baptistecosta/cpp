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

static	int				compilation_res,
						info_log_len;

public:

static	Shader*			Load(Shader::Type, const String& vert_shader_path, const String& frag_shader_path);

private:

static	void			CreateShader(const uint shader_type, const char* filename, uint& shader_id, String& src);
static	void			CompileShader(const uint shader_id, const char* shader_src_code);
static	uint			LinkProgram(const uint vert_shader_id, const uint frag_shader_id);
static	void			LogGLShaderInfo(uint id);
static	void			LogGLProgramInfo(uint id);
};

}		//	owl
#endif	//	__SHADER_LOADER__
