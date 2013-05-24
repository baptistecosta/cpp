/**
 *	@file		Text 2D vertex shader
 *	@author		Baptiste Costa
 */

#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexUvs;

out		vec2		uvs;

//-----------------------------------------------------------------------------
void				main()
{
	uvs = vertexUvs;
	gl_Position =  vec4(vertexPosition, 1);
}

