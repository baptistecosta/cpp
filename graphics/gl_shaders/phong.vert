/**
 *	@file		Phong vertex shader
 *	@author		Baptiste Costa
 */

#version 400 core

layout(location = 0) in vec3	vertex_pos;
layout(location = 1) in vec2	vertex_uv;
layout(location = 2) in vec3	vertex_nor;

uniform mat4					View,
								Proj,
								Model,
								ViewProj;

out		vec3					vert_pos_eye;
out		vec3					vert_nor_eye;

//-----------------------------------------------------------------------------
void				main()
{
	// Vertex position in eye coordinates.
	vert_pos_eye = (View * Model * vec4(vertex_pos, 1)).xyz;
	// Vertex normal in eye coordinates.
	vert_nor_eye = (View * Model * vec4(vertex_nor, 0)).xyz;
	    
    gl_Position =  ViewProj * Model * vec4(vertex_pos, 1);
}
