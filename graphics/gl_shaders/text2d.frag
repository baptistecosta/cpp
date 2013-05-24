/**
 *	@file		Text 2D fragment shader
 *	@author		Baptiste Costa
 */

#version 330 core

uniform	sampler2D	Texture;

in		vec2		uvs;
out		vec4 		color;

//-----------------------------------------------------------------------------
void				main()
{
	color = texture2D(Texture, uvs);
}