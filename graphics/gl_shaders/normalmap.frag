/**
 *	@file		Normal map fragment shader
 *	@author		Baptiste Costa
 */

#version 400 core

// Lights
#define	N_LIGHTS			20

#define	POINT_LIGHT			0
#define	DIRECTIONAL_LIGHT	1
#define	SPOT_LIGHT			2

struct	Light
{
	int				type;
	vec3			pos,
					dir;
	vec4			La,
					Ld,
					Ls;
	float			range,
					inner_q,
					outer_q;
};
uniform Light		lights[N_LIGHTS];
uniform int			nof_active_lights;

// Materials
uniform vec3		Ka, Kd, Ks;
uniform float		Ns;

// Textures
uniform sampler2D 	Texture;
uniform sampler2D 	NormalMap;

// Matrices
uniform mat4		View;

in		vec2		Uv;
in		vec3		vert_pos_eye;
in		vec3		view_dir_tang;
in		mat3		to_tangent_space;

out		vec4		frag_color;

//-----------------------------------------------------------------------------
float				dotL(const vec3 a, const vec3 b)
{	return clamp(dot(normalize(a), normalize(b)), 0.f, 1.f);	}

//-----------------------------------------------------------------------------
float				cosL(float q)
{	return cos(radians(clamp(q, 0.f, 90.f)));	}

//-----------------------------------------------------------------------------
vec3				getLightPositionEye(const int i)
{	return (View * vec4(lights[i].pos, 1.f)).xyz;	}

//-----------------------------------------------------------------------------
vec3				getLightToVertexPosition(const int i)
{
	switch (lights[i].type)
	{
		case POINT_LIGHT:
		case SPOT_LIGHT:
			return getLightPositionEye(i) - vert_pos_eye;

		case DIRECTIONAL_LIGHT:
			return mat3(View) * normalize(-lights[i].dir);
	}
}

//-----------------------------------------------------------------------------
float				computeSpotLightAttFactor(const int i, const vec3 light_to_vert)
{
	vec3 light_dir = normalize(mat3(View) * lights[i].dir);
	// Cosinus of the angle between light direction and the "light to vertex" vector.
	float cos_T = dotL(light_dir, -light_to_vert);
	// Spot inner and outer angles in radians.
	float cos_iC = cosL(lights[i].inner_q);
	float cos_oC = cosL(lights[i].outer_q);
	float cos_ioC = cosL(lights[i].inner_q + lights[i].outer_q);
	
	// The vertex is outside the spotlight.
	if (cos_T < cos_ioC)
		return 1.f;
	
	// The vertex is in the spotlight's outer cone.
	if (cos_T < cos_iC && cos_T >= cos_ioC)
		return 1.f - smoothstep(cos_ioC, cos_iC, cos_T);		
}

//-----------------------------------------------------------------------------
vec3				computeDiffSpecFactors(const int i, const vec3 light_to_vert, const vec3 text_coor, const vec3 normal_map_coord)
{
	// Light direction in tangent space coordinate.
	vec3 fL = to_tangent_space * normalize(light_to_vert);

	// Cosinus between light and normal vectors.
	float cosQ = dotL(fL, normal_map_coord);

	// Diffuse component
	vec3 d = lights[i].Ld.xyz * Kd * text_coor * cosQ;

	// Specular component
	vec3 s = vec3(0.f);
	if (cosQ > 0.f)
	{
		vec3 reflected_vec = normalize(reflect(-fL, normal_map_coord));
		// Cosinus of the angle formed by the reflected vector and the view direction vector.
		float spec_factor = dotL(reflected_vec, view_dir_tang);
		s = lights[i].Ls.xyz * Ks * pow(spec_factor, Ns);
	}

	return d + s;
}

//-----------------------------------------------------------------------------
vec3				processLight(const int i, const vec3 text_coor, const vec3 normal_map_coord)
{
	// Attenuation
	float att = 1.f;

	vec3 light_to_vert = getLightToVertexPosition(i);

	if (lights[i].type == SPOT_LIGHT)
		att -= computeSpotLightAttFactor(i, light_to_vert);

	att -= clamp((length(light_to_vert) / lights[i].range), 0.f, 1.f);

	// Ambient component
	vec3 a = lights[i].La.xyz * Ka;
	vec3 ds;
	if (att > 0.f)
		ds = computeDiffSpecFactors(i, light_to_vert, text_coor, normal_map_coord);

	return vec3(a + (ds * max(att, 0.f)));
}

//-----------------------------------------------------------------------------
void				main()
{
	// Texture color
	vec3 text_coor = texture2D(Texture, Uv).rgb;

	// * 2.0 - 1.0 : convert from color range [0, 1] to vector range [-1, 1]
	vec3 normal_map_coord = normalize(texture2D(NormalMap, Uv).xyz * 2.0 - 1.0);

	vec3 cl = vec3(0, 0, 0);
	for (int i = 0; i < nof_active_lights; i++)
		cl += processLight(i, text_coor, normal_map_coord);

	frag_color = vec4(cl, 1.0);
//	frag_color = vec4(text_coor, 1);
//	frag_color = vec4(normal_map_coord, 1);
}


