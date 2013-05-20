/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __SHADER__
#define __SHADER__

	#include "resource.h"
	#include "core/containers/auto_array.h"
	#include "core/containers/vector.h"
	#include "rendering/uniforms.h"
	
struct	SkinningJoints;
struct	SkinningWeights;

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

const	Type				m_type;
const	uint				m_program;
		AutoUniforms		m_uniform_bundles;

public:

		Shader(Type type, uint program);
		~Shader();

		void				initUniformsBundles(RenderingContext& context);

const	uint&				getProgram() const									{	return m_program;   }

		void				getUniformLocations();
		void				setUniforms(RenderingContext& context);

static	const char*			GetShaderNameFromType(const Shader::Type type);
};


//!
class	ShaderLib
{
		typedef	SharedVector<Shader>::type	SharedShaders;

		SharedShaders		shaders;

public:

		void				Load(RenderingContext& context);
		Shader*				Get(Shader::Type type);
};

/**
 *	@class	BaseShaderLoader
 */
/*
class	BaseShaderLoader
{
protected:

		Shader::Type				m_type;
		String						m_vertex_shader_path
								,	m_fragment_shader_path;
		RenderingContext&			m_context;

public:

		BaseShaderLoader(RenderingContext& context);

virtual	Shader*						load();
};
*/

/**
 *	@struct	Text2DShaderLoader
 */
/*
struct	Text2DShaderLoader	:	public	BaseShaderLoader
{
		Text2DShaderLoader(RenderingContext& context)	:	BaseShaderLoader(context)	{}

virtual	Shader*						load();
};
*/

/**
 *	@struct	FlatShaderLoader
 */
/*
struct	FlatShaderLoader	:	public	BaseShaderLoader
{
		FlatShaderLoader(RenderingContext& context)	:	BaseShaderLoader(context)	{}

virtual	Shader*						load();
};
*/

/**
 *	@struct	PhongShaderLoader
 */
/*
struct	PhongShaderLoader	:	public	BaseShaderLoader
{
		PhongShaderLoader(RenderingContext& context)	:	BaseShaderLoader(context)	{}

virtual	Shader*						load();
};
*/

/**
 *	@struct	NormalMapShaderLoader
 */
/*
struct	NormalMapShaderLoader	:	public	BaseShaderLoader
{
		NormalMapShaderLoader(RenderingContext& context)	:	BaseShaderLoader(context)	{}

virtual	Shader*						load();
};
*/
}		// owl
#endif	// __SHADER__
