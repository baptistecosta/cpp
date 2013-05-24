/**
 *	@file		Text 2D vertex shader
 *	@author		Baptiste Costa
 */

#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexUvs;

uniform mat4	Model;
uniform mat4	ViewProj;

out		vec2		uvs;

out		vec4		_mvp;

//-----------------------------------------------------------------------------
void				main()
{
	uvs = vertexUvs;

//	gl_Position =  vec4(vertexPosition, 1);

//	_mvp = ViewProj * Model * vec4(vertexPosition, 1);
//	_mvp = Model * vec4(1, 1, 1, 1);
//	_mvp = ViewProj * vec4(1, 1, 1, 1);
	gl_Position = ViewProj * Model * vec4(vertexPosition, 1);
//	gl_Position = ViewProj * vec4(vertexPosition, 1);
}
