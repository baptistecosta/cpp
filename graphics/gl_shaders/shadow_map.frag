#version 330 core

//-----------------------------------------------------------------------------
// Lights
struct	Light
{
	vec3						dir;
	vec4						La
							,	Ld
							,	Ls;
};
uniform Light					lights[1];

// Materials uniforms
uniform vec3					Ka, Kd, Ks;
uniform float					Ns;

uniform	mat4					View;

in		vec3					vert_pos_eye;
in		vec3					vert_nor_eye;
out		vec3					frag_color;


// GLSL Programming/Specular Highlights
// https://en.wikibooks.org/wiki/GLSL_Programming/GLUT/Specular_Highlights

//	Forward declarations
vec3				computeLight(const int i, const vec3 normalized_vert_pos_eye, const vec3 normalized_vert_nor_eye);

//-----------------------------------------------------------------------------
void				main()
{
	// Normalized vertex position in eye coordinates.
	vec3 normalized_vert_pos_eye = normalize(-vert_pos_eye);
	// Normalized vertex normal in eye coordinates.
	vec3 normalized_vert_nor_eye = normalize(vert_nor_eye);

	for (int i = 0; i < nof_active_lights; i++)
		frag_color += computeLight(i, normalized_vert_pos_eye, normalized_vert_nor_eye);
}

//-----------------------------------------------------------------------------
vec3				computeLight(const int i, const vec3 normalized_vert_pos_eye, const vec3 normalized_vert_nor_eye)
{
	// Ambient component
	vec3 a = lights[i].La.xyz * Ka;

	// Attenuation factor
	float att = 1.f;

	// Light position/direction in eye coordinates.
	vec3 L;
	vec3 light_pos;
	vec3 light_to_vert;
	switch (lights[i].type)
	{
		case POINT_LIGHT:
			light_pos = (View * vec4(lights[i].pos, 1.f)).xyz;
			light_to_vert = light_pos - vert_pos_eye;
			L = normalize(light_to_vert);
			break;

		case DIRECTIONAL_LIGHT:
			vec3 light_dir_eye = mat3(View) * normalize(lights[i].dir);
			L = normalize(-light_dir_eye);
			break;

		case SPOT_LIGHT:
			light_pos = (View * vec4(lights[i].pos, 1.f)).xyz;
			// Vector from vertex to light in eye coordinates.
			// See if this normalization is necessary.
			light_to_vert = light_pos - vert_pos_eye;
			L = normalize(light_to_vert);
			vec3 light_dir = normalize(mat3(View) * lights[i].dir);
			
			// Cosinus of the angle between light direction and the "light to vertex" vector.
			float cos_T = clamp(dot(light_dir, -L), 0.f, 1.f);
			// Spot inner and outer angles in radians.
			float cos_iC = cos(radians(lights[i].inner_q));
			float cos_oC = cos(radians(lights[i].outer_q));
			float cos_ioC = cos(radians(lights[i].inner_q + lights[i].outer_q));

			// The vertex is outside the spotlight.
			if (cos_T < cos_ioC)
				return a;

			// The vertex is in the spotlight's outer cone.
			if (cos_T < cos_iC && cos_T >= cos_ioC)
				att -= 1.f - smoothstep(cos_ioC, cos_iC, cos_T);
		
			break;
	}

	float cosQ = clamp(dot(normalized_vert_nor_eye, L), 0, 1);

	// r stands for "reflect".
	// For a given incident vector I and surface normal N reflect returns the reflection direction calculated as I - 2.0 * dot(N, I) * N.
	// N should be normalized in order to achieve the desired result.
	vec3 r = reflect(-L, normalized_vert_nor_eye);
	
	// Diffuse component
	vec3 d = lights[i].Ld.xyz * Kd * cosQ;

	// Specular component
	vec3 s = lights[i].Ls.xyz * Ks * pow(max(dot(r, normalized_vert_pos_eye), 0.f), Ns);

	att -= clamp((length(light_to_vert) / lights[i].range), 0.f, 1.f);
	return a + ((d + s) * max(att, 0.f));
}
