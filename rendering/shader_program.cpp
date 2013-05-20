/*
	bEngine - 2011
	Baptiste Costa
*/

	#include	"shader_program.h"
	#include	"core\math\matrix4.h"

void			ShaderProgram::setUniform(const char* name, int x)
{	int loc = GetUniformLocation(name); if (loc >= 0) glUniform1i(loc, x);  }

void			ShaderProgram::setUniform(const char* name, float x)
{	int loc = GetUniformLocation(name); if (loc >= 0) glUniform1f(loc, x);   }

void			ShaderProgram::setUniform(const char* name, float x, float y, float z)
{	int loc = GetUniformLocation(name);	if (loc >= 0) glUniform3f(loc, x, y, z);   }

void			ShaderProgram::setUniform(const char* name, Vector3& v)
{	int loc = GetUniformLocation(name);	if (loc >= 0) glUniform3f(loc, v.x, v.y, v.z);   }

void			ShaderProgram::setUniform(const char* name, Matrix4& mtx)
{	int loc = GetUniformLocation(name);	if (loc >= 0) glUniformMatrix4fv(loc, 1, GL_TRUE, mtx.m);   }

void			ShaderProgram::setUniform(const char* name, float vmtx[], uint count)
{	int loc = GetUniformLocation(name);	if (loc >= 0) glUniformMatrix4fv(loc, count, GL_FALSE, &vmtx[0]);   }

void			ShaderProgram::setUniform(const char* name, Matrix4 vmtx[], uint count)
{	int loc = GetUniformLocation(name);	if (loc >= 0) glUniformMatrix4fv(loc, count, GL_FALSE, &vmtx[0].m[0]);   }

void			ShaderProgram::setUniform(const char* name, vMatrix4& vmtx)
{	int loc = GetUniformLocation(name);	if (loc >= 0) glUniformMatrix4fv(loc, vmtx.size(), GL_FALSE, &vmtx[0].m[0]);   }

