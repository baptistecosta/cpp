/**
 *	@file		Normal map vertex shader
 *	@author		Baptiste Costa
 */

#version 400 core

// Lights
#define	MAX_LIGHTS				20

#define	POINT_LIGHT				0
#define	DIRECTIONAL_LIGHT		1
#define	SPOT_LIGHT				2

struct	Light
{
	int							type;
	vec3						pos
							,	dir;
	vec4						La
							,	Ld
							,	Ls;
};
uniform Light					lights[MAX_LIGHTS];
uniform int						nof_active_lights;

// Vertices
layout(location = 0) in vec3	vertex_pos;
layout(location = 1) in vec2	vertex_uv;
layout(location = 2) in vec3	vertex_nor;
layout(location = 3) in vec3	vertex_tang;
layout(location = 4) in vec3	vertex_bitang;

uniform mat4					Model
							,	View
							,	Proj
							,	ViewProj;

out		vec2					Uv;
out		vec3					vert_pos_eye;
out		vec3					view_dir_tang;
out		mat3					to_tangent_space;


//-----------------------------------------------------------------------------
void							computeLights(const vec3 vert_pos_eye, const mat3 to_tangent_space, const int i);

//-----------------------------------------------------------------------------
void							main()
{
	mat4 model_view = View * Model;
	
	// Normalized normal, tangent and bitangent in eye coordinates.
	vec3 n = normalize(mat3(model_view) * vertex_nor);
	vec3 t = normalize(mat3(model_view) * vertex_tang);
	vec3 b = normalize(mat3(model_view) * vertex_bitang);
	
	// Matrix for transformation to tangent space.
	 to_tangent_space = mat3(	t.x, b.x, n.x,
								t.y, b.y, n.y,	
								t.z, b.z, n.z	);

	// Vertex position in eye coordinates.
	vert_pos_eye = (model_view * vec4(vertex_pos, 1.f)).xyz;
	// View direction to tangent space.
	view_dir_tang = to_tangent_space * normalize(-vert_pos_eye);

//	for (int i = 0; i < /*nof_active_lights || i < MAX_LIGHTS*/ 1; i++)
//		computeLights(vert_pos_eye, to_tangent_space, i);
	
	Uv = vertex_uv;

    gl_Position	= ViewProj * Model * vec4(vertex_pos, 1);
}
