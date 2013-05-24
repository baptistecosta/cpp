/**
 *	@file		Text 2D fragment shader
 *	@author		Baptiste Costa
 */

#version 330 core

uniform	sampler2D	Texture;

uniform mat4	Model;
uniform mat4	ViewProj;

in		vec2		uvs;
in		vec4		_mvp;
out		vec4 		color;

//-----------------------------------------------------------------------------
void				main()
{
	color = texture2D(Texture, uvs);
//	color = vec4(1, 0, 0, 1);
//	color = ViewProj * vec4(1, 1, 1, 1);
}