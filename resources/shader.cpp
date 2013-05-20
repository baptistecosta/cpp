/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

	#include "glew.h"
	#include "shader.h"
	#include "shader_loader.h"
	#include "rendering/opengl.h"
	#include "rendering/camera.h"
	#include "rendering/directional_light.h"
	#include "rendering/point_light.h"
	#include "rendering/spot_light.h"
	#include "rendering/rendering_context.h"
	#include "resources/resource_manager.h"

	using namespace owl;

//-----------------------------------------------------------------------------
Shader::Shader(Type type, uint program)
	:	m_type(type)
	,	m_program(program)
{
	//
}

//-----------------------------------------------------------------------------
Shader::~Shader()
{
	glDeleteProgram(m_program);
}

//-----------------------------------------------------------------------------
void			Shader::initUniformsBundles(RenderingContext& context)
{
	assert(!context.getLights().IsNull());
	const int n_lights = context.getLights().Size();
	switch (m_type)
	{
		case Shader::Text2D:
			m_uniform_bundles.Push(new TextureUniform);
			break;

		case Shader::NormalMap:
			m_uniform_bundles.Push(new NormalMapUniform);
			m_uniform_bundles.Push(new TextureUniform);
		case Shader::Phong:
			m_uniform_bundles.Push(new MaterialsUniforms);
		case Shader::Texture:
		case Shader::Flat:
			m_uniform_bundles.Push(new MatricesUniforms);
			m_uniform_bundles.Push(new LightsCountUniform);
			for (int i = 0; i < n_lights; i++)
				m_uniform_bundles.Push(LightUniformsFactory::Create(context.getLight(i), i));
			break;
	}
}

//-----------------------------------------------------------------------------
void			Shader::getUniformLocations()
{
	int size = m_uniform_bundles.Size();
	for (int i = 0; i < size; i++)
		m_uniform_bundles[i]->GetUniformLocation(m_program, i);
}

//-----------------------------------------------------------------------------
void			Shader::setUniforms(RenderingContext& context)
{
	int size = m_uniform_bundles.Size();
	for (int i = 0; i < size; i++)
		m_uniform_bundles[i]->SetUniforms(context);
}

//-----------------------------------------------------------------------------
const char*		Shader::GetShaderNameFromType(const Shader::Type type)
{
	switch (type)
	{
		case Shader::Text2D: return "text2d";
		case Shader::Texture: return "texture";
		case Shader::Flat: return "flat";
		case Shader::Phong: return "phong";
		case Shader::NormalMap: return "normalmap";
		default: return "";
	}
}

//-----------------------------------------------------------------------------
void			ShaderLib::Load(RenderingContext& context)
{
	uint vert_shader_id = glCreateShader(GL_VERTEX_SHADER);
	OpenGL::CheckGLError();

	for (int i = 0; i < Shader::ShaderCount; i++)
	{
		Shader::Type type = static_cast<Shader::Type>(i);
		const char* dir = "resources/shaders/";
		const char* name = Shader::GetShaderNameFromType(type);

		Log::HorizontalLine();
		Log::i("Loading shader: %s", name);
		Shader* shd = ShaderLoader::Load(type, String::Format("%s%s.vert", dir, name), String::Format("%s%s.frag", dir, name));
		assert(shd);

		shd->initUniformsBundles(context);
		shd->getUniformLocations();
		shaders.Push(shd);
		Log::i("Successfully load shader: %s", name);
	}
}

//-----------------------------------------------------------------------------
Shader*			ShaderLib::Get(Shader::Type type)
{
	assert(type >= Shader::Text2D && type < Shader::ShaderCount);
	return shaders[type];
}

/*
//-----------------------------------------------------------------------------
BaseShaderLoader::BaseShaderLoader(RenderingContext& context)
	:	m_type(Shader::InvalidShader)
	,	m_context(context)
{
	//
}

//-----------------------------------------------------------------------------
Shader*			BaseShaderLoader::load()
{
	assert(m_type != Shader::InvalidShader);
	assert(!m_vertex_shader_path.isEmpty());
	assert(!m_fragment_shader_path.isEmpty());

	Shader* shader = ShaderLoader::load(m_type, m_vertex_shader_path, m_fragment_shader_path);
	assert(shader);
	shader->initUniformsBundles(m_context);
	shader->getUniformLocations();

	return shader;
}

//-----------------------------------------------------------------------------
Shader*			Text2DShaderLoader::load()
{
	m_type = Shader::Text2D;
	m_vertex_shader_path = "resources/shaders/text2d.vert";
	m_fragment_shader_path = "resources/shaders/text2d.frag";

	return BaseShaderLoader::load();
}
//-----------------------------------------------------------------------------
Shader*			FlatShaderLoader::load()
{
	m_type = Shader::Flat;
	m_vertex_shader_path = "resources/shaders/flat.vert";
	m_fragment_shader_path = "resources/shaders/flat.frag";

	return BaseShaderLoader::load();
}
//-----------------------------------------------------------------------------
Shader*			PhongShaderLoader::load()
{
	m_type = Shader::Phong;
	m_vertex_shader_path = "resources/shaders/phong.vert";
	m_fragment_shader_path = "resources/shaders/phong.frag";

	return BaseShaderLoader::load();
}
//-----------------------------------------------------------------------------
Shader*			NormalMapShaderLoader::load()
{
	m_type = Shader::NormalMap;
	m_vertex_shader_path = "resources/shaders/normalmap.vert";
	m_fragment_shader_path = "resources/shaders/normalmap.frag";

	return BaseShaderLoader::load();
}
*/
