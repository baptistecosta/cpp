/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

	#include <fstream>
	#include "glew.h"
	#include "core/string.h"
	#include "core/log.h"
	#include "core/filesys.h"
	#include "graphics_wrappers/opengl.h"
	#include "rendering_context.h"
	#include "resource_manager.h"
	#include "shader_loader.h"

	using namespace owl;

//-----------------------------------------------------------------------------
int				ShaderLoader::compilation_res = 0;
int				ShaderLoader::info_log_len = 0;

//-----------------------------------------------------------------------------
Shader*			ShaderLoader::Load(Shader::Type type, const String& vshad_path, const String& fshad_path)
{
	FileSystem fs;

	// Vertex shader
	if (!fs.Open(vshad_path))
	{
		__LOG_E("Could not open %s!", vshad_path);
		exit(1);
	}

	uint vert_shader_id = glCreateShader(GL_VERTEX_SHADER);
#ifdef __debug__
	OpenGL::CheckGLError();
#endif

	String shd_src_code = fs.Read();
//	__LOG("Vertex shader source code:\n%s", shd_src_code)

	CompileShader(vert_shader_id, shd_src_code.cStr());

	// Fragment shader
	if (!fs.Open(fshad_path)) exit(1);
	uint frag_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
	shd_src_code = fs.Read().cStr();

//	__LOG("Fragment shader source code:\n%s", shd_src_code)
	CompileShader(frag_shader_id, shd_src_code.cStr());
	
	uint prog_id = LinkProgram(vert_shader_id, frag_shader_id);
	__LOG("Program = %d", prog_id)

	__GL_CALL(glDeleteShader(vert_shader_id))
	__GL_CALL(glDeleteShader(frag_shader_id))
	__LOG_NL()

	Shader* shader = new Shader(type, prog_id);
	return shader;
}

//-----------------------------------------------------------------------------
void			ShaderLoader::CompileShader(const uint shader_id, const char* shader_src_code)
{
	__GL_CALL(glShaderSource(shader_id, 1, &shader_src_code, NULL))
	__GL_CALL(glCompileShader(shader_id))

	__GL_CALL(glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compilation_res))
	__GL_CALL(glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_len))
	__LOG(compilation_res ? "Compilation succeed" : "Compilation failed")

	// Check Vertex Shader
	LogGLInfo(shader_id);
}

//-----------------------------------------------------------------------------
uint			ShaderLoader::LinkProgram(const uint vert_shader_id, const uint frag_shader_id)
{
	__LOG("Linking program")
	uint prog_id = glCreateProgram();
#if _DEBUG
	OpenGL::CheckGLError();
#endif
	__GL_CALL(glAttachShader(prog_id, vert_shader_id))
	__GL_CALL(glAttachShader(prog_id, frag_shader_id))
	__GL_CALL(glLinkProgram(prog_id))

	__GL_CALL(glGetProgramiv(prog_id, GL_LINK_STATUS, &compilation_res))
	__GL_CALL(glGetProgramiv(prog_id, GL_INFO_LOG_LENGTH, &info_log_len))
	__LOG(compilation_res ? "Compilation succeed" : "Compilation failed")

	// Check the program
//	logGLInfo(prog_id);
	return prog_id;
}

//-----------------------------------------------------------------------------
void			ShaderLoader::LogGLInfo(uint id)
{
	String msg(info_log_len);
	glGetShaderInfoLog(id, info_log_len, NULL, msg.cStr());
	if (!msg.IsEmpty())
		__LOG(msg.cStr())
}

