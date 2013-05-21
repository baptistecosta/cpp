/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

	#include "graphics_wrappers/opengl.h"
	#include "shader.h"
	#include "shader_loader.h"
	#include "camera.h"
	#include "directional_light.h"
	#include "point_light.h"
	#include "rendering_context.h"
	#include "resource_manager.h"
	#include "spot_light.h"

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
void			Shader::InitUniformsBundles(RenderingContext& context)
{
	assert(!context.GetLights().IsNull());
	const int n_lights = context.GetLights().Size();
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
				m_uniform_bundles.Push(LightUniformsFactory::Create(context.GetLight(i), i));
			break;
	}
}

//-----------------------------------------------------------------------------
void			Shader::GetUniformLocations()
{
	int size = m_uniform_bundles.Size();
	for (int i = 0; i < size; i++)
		m_uniform_bundles[i]->GetUniformLocation(m_program, i);
}

//-----------------------------------------------------------------------------
void			Shader::SetUniforms(RenderingContext& context)
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
		__LOG("Loading shader: %s", name);
		Shader* shd = ShaderLoader::Load(type, String::Format("%s%s.vert", dir, name), String::Format("%s%s.frag", dir, name));
		assert(shd);

		shd->InitUniformsBundles(context);
		shd->GetUniformLocations();
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
