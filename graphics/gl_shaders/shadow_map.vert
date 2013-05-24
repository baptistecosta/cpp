#version 330 core

layout(location = 0) in vec3	vertex_pos;
layout(location = 1) in vec2	vertex_uv;
layout(location = 2) in vec3	vertex_nor;

uniform mat4					View,
								Proj,
								Model,
								ViewProj;


//-----------------------------------------------------------------------------
void				main()
{
    gl_Position =  ViewProj * Model * vec4(vertex_pos, 1);
}
