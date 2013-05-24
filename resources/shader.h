/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __SHADER__
#define __SHADER__

	#include "resource.h"
	#include "core/containers/auto_array.h"
	#include "core/containers/vector.h"
	#include "uniforms.h"
	
namespace owl {

class	Camera;
class	Light;
class	Material;
class	Matrix4;
class	String;

//!
class	Shader	:	public	SharedObject
{
public:

		typedef	AutoVector<BaseUniform>::type	AutoUniforms;

		enum	Type
		{
			Text2D		=	0,
			Texture,
			Flat,
			Phong,
			NormalMap,
			
			ShaderCount,
			InvalidShader
		};

private:

		const Type			m_type;
		const uint			m_program;
		AutoUniforms		m_uniform_bundles;

public:

		Shader(Type type, uint program);
		~Shader();

		void				InitUniformsBundles(RenderingContext&);

		const uint&			GetProgram() const			{	return m_program;   }

		void				GetUniformLocations();
		void				SetUniforms(RenderingContext&);

static	const char*			GetShaderNameFromType(const Shader::Type);
};


//!
class	ShaderLib
{
		typedef	SharedVector<Shader>::type	SharedShaders;

		SharedShaders		shaders;

public:

		void				Load(const char* path, RenderingContext&);
		Shader*				Get(Shader::Type);
};

}		// owl
#endif	// __SHADER__
