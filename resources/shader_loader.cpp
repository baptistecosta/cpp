/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

	#include <fstream>
	#include "glew.h"
	#include "shader_loader.h"
	#include "resource_manager.h"
	#include "core/string.h"
	#include "core/log.h"
	#include "core/filesys.h"
	#include "rendering/rendering_context.h"
	#include "rendering/opengl.h"
	
using namespace owl;

//-----------------------------------------------------------------------------
int				ShaderLoader::m_compilation_res = 0;
int				ShaderLoader::m_info_log_len = 0;

//-----------------------------------------------------------------------------
Shader*			ShaderLoader::Load(Shader::Type type, const String& vshad_path, const String& fshad_path)
{
	FileSystem fs;

	// Vertex shader
	if (!fs.Open(vshad_path))
		exit(1);

	uint vert_shader_id = glCreateShader(GL_VERTEX_SHADER);
#if _DEBUG
	OpenGL::CheckGLError();
#endif

	String shd_src_code = fs.Read();
//	Log::horizontalLine(); Log::i("Vertex shader source code:\n%s", shd_src_code);

	CompileShader(vert_shader_id, shd_src_code.cStr());

	// Fragment shader
	if (!fs.Open(fshad_path)) exit(1);
	uint frag_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
	shd_src_code = fs.Read().cStr();

//	Log::horizontalLine(); Log::i("Fragment shader source code:\n%s", shd_src_code);
	CompileShader(frag_shader_id, shd_src_code.cStr());
	
	uint prog_id = LinkProgram(vert_shader_id, frag_shader_id);
	Log::i("PROGRAM = %d", prog_id);

	__GL_CALL(glDeleteShader(vert_shader_id))
	__GL_CALL(glDeleteShader(frag_shader_id))
	Log::NewLine();

	Shader* shader = new Shader(type, prog_id);
	return shader;
}

//-----------------------------------------------------------------------------
void			ShaderLoader::CompileShader(const uint shader_id, const char* shader_src_code)
{
	__GL_CALL(glShaderSource(shader_id, 1, &shader_src_code, NULL))
	__GL_CALL(glCompileShader(shader_id))

	__GL_CALL(glGetShaderiv(shader_id, GL_COMPILE_STATUS, &m_compilation_res))
	__GL_CALL(glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &m_info_log_len))
	Log::i(String::Format("Compile status: %s", m_compilation_res == GL_TRUE ? "succeed" : "failed"));

	// Check Vertex Shader
	LogGLInfo(shader_id);
}

//-----------------------------------------------------------------------------
uint			ShaderLoader::LinkProgram(const uint vert_shader_id, const uint frag_shader_id)
{
	Log::i("Linking program");
	uint prog_id = glCreateProgram();
#if _DEBUG
	OpenGL::CheckGLError();
#endif
	__GL_CALL(glAttachShader(prog_id, vert_shader_id))
	__GL_CALL(glAttachShader(prog_id, frag_shader_id))
	__GL_CALL(glLinkProgram(prog_id))

	__GL_CALL(glGetProgramiv(prog_id, GL_LINK_STATUS, &m_compilation_res))
	__GL_CALL(glGetProgramiv(prog_id, GL_INFO_LOG_LENGTH, &m_info_log_len))
	Log::i(String::Format("Compile status: %s", m_compilation_res == GL_TRUE ? "succeed" : "failed"));

	// Check the program
//	logGLInfo(prog_id);
	return prog_id;
}

//-----------------------------------------------------------------------------
void			ShaderLoader::LogGLInfo(uint id)
{
	String msg(m_info_log_len);
	glGetShaderInfoLog(id, m_info_log_len, NULL, msg.cStr());
	if (!msg.IsEmpty())
		Log::i(msg);
}

