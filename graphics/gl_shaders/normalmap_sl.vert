#version 330 core

struct	LightInfo
{
	vec3	Pos;
	vec4	La, Ld, Ls;
};

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec2 vertexUv;
layout(location = 2) in vec3 vertexNor;
layout(location = 3) in vec3 vertexTang;
layout(location = 4) in vec3 vertexBitang;

#define		N_LIGHT		1
uniform 	LightInfo	light;
uniform 	mat4		ViewProj, View, Proj, Model;

out			vec3		L;
out			vec3		V;
out			vec2		Uv;

void main()
{
	mat4 ModelView = View * Model;
	vec3 vertexPosEye = (ModelView * vec4(vertexPos, 1)).xyz;		// Position in eye space
	vec3 lightPosEye = (View * vec4(light.Pos, 1)).xyz;
		 
	vec3 n = normalize(ModelView * vec4(vertexNor, 0)).xyz;			// Normal in eye space
	vec3 t = normalize(ModelView * vec4(vertexTang, 0)).xyz;		// Tangent in eye space
	vec3 b = normalize(ModelView * vec4(vertexBitang, 0)).xyz;		// Bitangent in eye space
		 
	mat3 ToTangentSpace = mat3(t.x, b.x, n.x, t.y, b.y, n.y, t.z, b.z, n.z );
	
	L = ToTangentSpace * normalize(lightPosEye - vertexPosEye);
	V = ToTangentSpace * normalize(-vertexPosEye);
		
	Uv = vertexUv;

    gl_Position	= ViewProj * Model * vec4(vertexPos, 1);
}

