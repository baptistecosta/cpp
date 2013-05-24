#version 400 core

struct	LightInfo
{
	vec3	Pos;
	vec4	La, Ld, Ls;
};

#define	MAX_LIGHT	1
uniform int			NofLights;
uniform LightInfo	light[MAX_LIGHT];

uniform vec3		Ka, Kd, Ks;
uniform float		Ns;

uniform sampler2D 	Texture;
uniform sampler2D 	NormalMap;

in		vec3		L;
in		vec3		V;
in		vec2		Uv;

out		vec3		fragColor;

void main()
{
	vec3 T = texture2D(Texture, Uv).rgb;
	vec3 N = texture2D(NormalMap, Uv).rgb;

	N.x = (N.x - 0.5) * 2.0;
	N.y = (N.y - 0.5) * 2.0;
	N.z = sqrt(1 - (N.x * N.x) - (N.y * N.y));
	
	vec3 R = normalize(reflect(-L, N));

	float cosQ = clamp(dot(N, L), 0, 1);
		
	vec3 d = light[0].Ld.xyz * Kd * T * cosQ;
	vec3 s = light[0].Ls.xyz * Ks * pow(max(dot(R, normalize(V)), 0.f), Ns);
		
	fragColor = d + s;
}