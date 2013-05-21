/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

	#include <assert.h>
	#include <vector>
	#include "core/math/matrix4.h"
	#include "core/string.h"
	#include "core/log.h"
	#include "externs/opengl/inc/glew.h"
	#include "graphics_wrappers/opengl.h"
	#include "rendering_context.h"
	#include "material.h"
	#include "uniforms.h"
	#include "color.h"
	#include "light.h"
	#include "directional_light.h"
	#include "point_light.h"
	#include "spot_light.h"

	using namespace owl;

//-----------------------------------------------------------------------------
UniformWrapper::UniformWrapper(): m_uniform_location(0) {}

//-----------------------------------------------------------------------------
void			UniformWrapper::GetUniformLocation(const uint shader_program, const String& uniform_var_name)
{	
	// Retourne un entier qui représente l'emplacement d'une variable "uniform" à l'intérieur d'un shader (GLuint uniform_loc).
	m_uniform_location = glGetUniformLocation(shader_program, uniform_var_name.cStr());
	OpenGL::CheckGLError();
	if (Log::log && m_uniform_location == -1)
		Log::i("Shader program: %d, glGetUniformLocation returned -1 for uniform variable name: %s", shader_program, uniform_var_name);
}

//-----------------------------------------------------------------------------
void			UniformWrapper::SetUniform(int x)
{	if (m_uniform_location >= 0) __GL_CALL(glUniform1i(m_uniform_location, x))	}
void			UniformWrapper::SetUniform(uint x)
{	if (m_uniform_location >= 0) __GL_CALL(glUniform1ui(m_uniform_location, x))	}
void			UniformWrapper::SetUniform(float x)
{	if (m_uniform_location >= 0) __GL_CALL(glUniform1f(m_uniform_location, x))	}
void			UniformWrapper::SetUniform(float x, float y, float z)
{	if (m_uniform_location >= 0) __GL_CALL(glUniform3f(m_uniform_location, x, y, z))	}
void			UniformWrapper::SetUniform(Vector3& v)
{	if (m_uniform_location >= 0) __GL_CALL(glUniform3f(m_uniform_location, v.x, v.y, v.z))	}
void			UniformWrapper::SetUniform(const Vector3& v)
{	if (m_uniform_location >= 0) __GL_CALL(glUniform3f(m_uniform_location, v.x, v.y, v.z))	}
void			UniformWrapper::SetUniform(const Color& c)
{	if (m_uniform_location >= 0) __GL_CALL(glUniform4f(m_uniform_location, c.m_r, c.m_g, c.m_b, c.m_a))	}
void			UniformWrapper::SetUniform(const Matrix4& mtx)
{	if (m_uniform_location >= 0) __GL_CALL(glUniformMatrix4fv(m_uniform_location, 1, GL_TRUE, mtx.m))	}
void			UniformWrapper::SetUniform(float vmtx[], uint count)
{	if (m_uniform_location >= 0) __GL_CALL(glUniformMatrix4fv(m_uniform_location, count, GL_FALSE, &vmtx[0]))	}
void			UniformWrapper::SetUniform(Matrix4 vmtx[], uint count)
{	if (m_uniform_location >= 0) __GL_CALL(glUniformMatrix4fv(m_uniform_location, count, GL_FALSE, &vmtx[0].m[0]))	}
void			UniformWrapper::SetUniform(std::vector<Matrix4>& vmtx)
{	if (m_uniform_location >= 0) __GL_CALL(glUniformMatrix4fv(m_uniform_location, vmtx.size(), GL_FALSE, &vmtx[0].m[0]))	}

//-----------------------------------------------------------------------------
void			MatricesUniforms::GetUniformLocation(const uint shader_program, const int index)
{
	m_viewproj.GetUniformLocation(shader_program, "ViewProj");
	m_model.GetUniformLocation(shader_program, "Model");
	m_view.GetUniformLocation(shader_program, "View");
	m_proj.GetUniformLocation(shader_program, "Proj");
}

//---------------------------------------------------------------------------
void			MatricesUniforms::SetUniforms(RenderingContext& context)
{
	m_model.SetUniform(context.GetModelMatrix());
	m_view.SetUniform(context.GetViewMatrix());
	m_proj.SetUniform(context.GetProjMatrix());
	m_viewproj.SetUniform(context.GetViewProjMatrix());
}

//---------------------------------------------------------------------------
void			MaterialsUniforms::GetUniformLocation(const uint program, const int index)
{
	m_Ka.GetUniformLocation(program, "Ka");
	m_Kd.GetUniformLocation(program, "Kd");
	m_Ks.GetUniformLocation(program, "Ks");
	m_Ns.GetUniformLocation(program, "Ns");
}

//---------------------------------------------------------------------------
void			MaterialsUniforms::SetUniforms(RenderingContext& context)
{
	m_Ka.SetUniform(context.GetMaterial().getKa());
	m_Kd.SetUniform(context.GetMaterial().getKd());
	m_Ks.SetUniform(context.GetMaterial().getKs());
	m_Ns.SetUniform(context.GetMaterial().getNs());
}

//-----------------------------------------------------------------------------
void			TextureUniform::GetUniformLocation(const uint program, const int index)
{	m_texture.GetUniformLocation(program, "Texture");	}

//-----------------------------------------------------------------------------
void			TextureUniform::SetUniforms(RenderingContext& context)
{	m_texture.SetUniform(context.GetTextureUnit());	}

//-----------------------------------------------------------------------------
void			NormalMapUniform::GetUniformLocation(const uint program, const int index)
{	m_normal_map.GetUniformLocation(program, "NormalMap");	}

//-----------------------------------------------------------------------------
void			NormalMapUniform::SetUniforms(RenderingContext &context)
{	m_normal_map.SetUniform(context.GetNormalMapUnit());	}

//-----------------------------------------------------------------------------
void			LightsCountUniform::GetUniformLocation(const uint program, const int index)
{	m_count.GetUniformLocation(program, "nof_active_lights");	}

//-----------------------------------------------------------------------------
void			LightsCountUniform::SetUniforms(RenderingContext& context)
{	m_count.SetUniform(context.GetLights().Size());	}

//-----------------------------------------------------------------------------
void			LightUniforms::GetUniformLocation(const uint shader_program, const int i)
{
	m_La.GetUniformLocation(shader_program, String::Format("lights[%d].La", i));
	m_Ld.GetUniformLocation(shader_program, String::Format("lights[%d].Ld", i));
	m_Ls.GetUniformLocation(shader_program, String::Format("lights[%d].Ls", i));
}

//-----------------------------------------------------------------------------
void			LightUniforms::SetUniforms(RenderingContext& context)
{
	m_La.SetUniform(context.GetLight(m_index).getLa());
	m_Ld.SetUniform(context.GetLight(m_index).getLd());
	m_Ls.SetUniform(context.GetLight(m_index).getLs());
}

//-----------------------------------------------------------------------------
void			DirectionalLightUniforms::GetUniformLocation(const uint shader_program, const int i)
{
	LightUniforms::GetUniformLocation(shader_program, GetIndex());

	m_type.GetUniformLocation(shader_program, String::Format("lights[%d].type", GetIndex()));
	m_dir.GetUniformLocation(shader_program, String::Format("lights[%d].dir", GetIndex()));
}

//-----------------------------------------------------------------------------
void			DirectionalLightUniforms::SetUniforms(RenderingContext& context)
{
	LightUniforms::SetUniforms(context);

	DirectionalLight* l = static_cast<DirectionalLight*>(&context.GetLight(GetIndex()));
	m_type.SetUniform(static_cast<int>(Light::Directional));
	m_dir.SetUniform(l->getDirection());
}

//-----------------------------------------------------------------------------
void			PointLightUniforms::GetUniformLocation(const uint shader_program, const int i)
{
	LightUniforms::GetUniformLocation(shader_program, GetIndex());

	m_type.GetUniformLocation(shader_program, String::Format("lights[%d].type", GetIndex()));
	m_pos.GetUniformLocation(shader_program, String::Format("lights[%d].pos", GetIndex()));
	m_range.GetUniformLocation(shader_program, String::Format("lights[%d].range", GetIndex()));
}

//-----------------------------------------------------------------------------
void			PointLightUniforms::SetUniforms(RenderingContext& context)
{
	LightUniforms::SetUniforms(context);
		
	PointLight* l = static_cast<PointLight*>(&context.GetLight(GetIndex()));
	m_type.SetUniform(static_cast<int>(Light::Point));
	m_pos.SetUniform(l->GetPos());
	m_range.SetUniform(l->getRange());
}

//-----------------------------------------------------------------------------
void			SpotLightUniforms::GetUniformLocation(const uint shader_program, const int i)
{
	LightUniforms::GetUniformLocation(shader_program, GetIndex());

	m_type.GetUniformLocation(shader_program, String::Format("lights[%d].type", GetIndex()));
	m_dir.GetUniformLocation(shader_program, String::Format("lights[%d].dir", GetIndex()));
	m_pos.GetUniformLocation(shader_program, String::Format("lights[%d].pos", GetIndex()));
	m_range.GetUniformLocation(shader_program, String::Format("lights[%d].range", GetIndex()));
	m_inner_q.GetUniformLocation(shader_program, String::Format("lights[%d].inner_q", GetIndex()));
	m_outer_q.GetUniformLocation(shader_program, String::Format("lights[%d].outer_q", GetIndex()));
}

//-----------------------------------------------------------------------------
void			SpotLightUniforms::SetUniforms(RenderingContext& context)
{
	LightUniforms::SetUniforms(context);
	
	SpotLight* l = static_cast<SpotLight*>(&context.GetLight(GetIndex()));
	m_type.SetUniform(static_cast<int>(Light::Spot));
	m_pos.SetUniform(l->GetPos());
	m_dir.SetUniform(l->GetDir());
	m_inner_q.SetUniform(l->GetInnerQ());
	m_outer_q.SetUniform(l->GetOuterQ());
	m_range.SetUniform(l->GetRange());
}

//-----------------------------------------------------------------------------
LightUniforms* LightUniformsFactory::Create(const Light& l, const uint index)
{
	switch (l.GetType())
	{
		case Light::Directional:
			return new DirectionalLightUniforms(index);
		case Light::Point:
			return new PointLightUniforms(index);
		case Light::Spot:
			return new SpotLightUniforms(index);
		default:
			return null_ptr;
	}
}

