/**
 *	@file		Flat vertex shader
 *	@author		Baptiste Costa
 */

#version 400 core

layout(location = 0) in vec3 vertex_pos;

uniform mat4	Model;
uniform mat4	View;
uniform mat4	Proj;
uniform mat4	ViewProj;

//-----------------------------------------------------------------------------
void			main()
{
	gl_Position =  ViewProj * Model * vec4(vertex_pos, 1.0);
//	gl_Position = vec4(vertex_pos, 1.0);
}

