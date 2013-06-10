/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

	#include <fstream>
	#include "glew.h"
	#include "core/string.h"
	#include "core/log.h"
	#include "core/filesys.h"
	#include "graphics/opengl.h"
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
	uint vs_id = 0, fs_id = 0;
	String vs_src, fs_src;

	CreateShader(GL_VERTEX_SHADER, vshad_path.cStr(), vs_id, vs_src);
	CreateShader(GL_FRAGMENT_SHADER, fshad_path.cStr(), fs_id, fs_src);
	CompileShader(vs_id, vs_src.cStr());
	CompileShader(fs_id, fs_src.cStr());

	uint prog_id = LinkProgram(vs_id, fs_id);
	Shader* shader = new Shader(type, prog_id);

	__GL_CALL(glDeleteShader(vs_id))
	__GL_CALL(glDeleteShader(fs_id))

	Log::i("Program = %d", prog_id);
	Log::NewLine();

	return shader;
}

//-----------------------------------------------------------------------------
void			ShaderLoader::CreateShader(const uint shader_type, const char* filename, uint& shader_id, String& src)
{
	FileSystem fs;
	if (!fs.Open(filename))
	{
		Log::e("Could not open %s!", filename);
		exit(1);
	}
	shader_id = glCreateShader(shader_type);
#ifdef __debug__
	OpenGL::CheckGLError();
#endif
	src = fs.Read();
}

//-----------------------------------------------------------------------------
void			ShaderLoader::CompileShader(const uint shader_id, const char* shader_src_code)
{
	__GL_CALL(glShaderSource(shader_id, 1, &shader_src_code, NULL))
	__GL_CALL(glCompileShader(shader_id))

	__GL_CALL(glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compilation_res))
	__GL_CALL(glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_len))
	Log::i(compilation_res ? "Compilation succeed" : "Compilation failed");

	// Check Vertex Shader
	LogGLShaderInfo(shader_id);
}

//-----------------------------------------------------------------------------
uint			ShaderLoader::LinkProgram(const uint vert_shader_id, const uint frag_shader_id)
{
	Log::i("Linking program");
	uint prog_id = glCreateProgram();
#ifdef __debug__
	OpenGL::CheckGLError();
#endif
	__GL_CALL(glAttachShader(prog_id, vert_shader_id))
	__GL_CALL(glAttachShader(prog_id, frag_shader_id))
	__GL_CALL(glLinkProgram(prog_id))

	__GL_CALL(glGetProgramiv(prog_id, GL_LINK_STATUS, &compilation_res))
	__GL_CALL(glGetProgramiv(prog_id, GL_INFO_LOG_LENGTH, &info_log_len))
	Log::i(compilation_res ? "Compilation succeed" : "Compilation failed");

	// Check the program
	LogGLProgramInfo(prog_id);
	return prog_id;
}

//-----------------------------------------------------------------------------
void			ShaderLoader::LogGLShaderInfo(uint id)
{
	String msg(info_log_len);
	__GL_CALL(glGetShaderInfoLog(id, info_log_len, NULL, msg.cStr()))
	if (!msg.IsEmpty())
		Log::e(msg.cStr());
}
void			ShaderLoader::LogGLProgramInfo(uint id)
{
	String msg(info_log_len);
	__GL_CALL(glGetProgramInfoLog(id, info_log_len, NULL, msg.cStr()))
	if (!msg.IsEmpty())
		Log::e(msg.cStr());
}
//-----------------------------------------------------------------------------
