/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __UNIFORMS__
#define __UNIFORMS__

	#include <vector>
	#include "core/containers/auto_array.h"
	#include "core/math/vector.h"
	#include "typedefs.h"

namespace owl {

class	String;
class	Camera;
struct	Color;
class	Light;
class	Material;
class	DirectionalLight;
class	PointLight;

class	RenderingContext;

//!
class	UniformWrapper
{
		int						m_uniform_location;

public:
		
		UniformWrapper();

		void					GetUniformLocation(const uint shader_program, const String& uniform_var_name);
		
		void					SetUniform(int x);
		void					SetUniform(uint x);
		void					SetUniform(float x);
		void					SetUniform(float x, float y, float z);
		void					SetUniform(Vector3& v);
		void					SetUniform(const Vector3& v);
		void					SetUniform(const Color& c);
		void					SetUniform(const Matrix4& mtx);
		void					SetUniform(float vmtx[], uint count);
		void					SetUniform(Matrix4 vmtx[], uint count);
		void					SetUniform(std::vector<Matrix4>& vmtx);
};


//!
struct	BaseUniform
{
virtual	void					GetUniformLocation(const uint program, const int index) = 0;
virtual	void					SetUniforms(RenderingContext& context) = 0;
};


//!
class	MatricesUniforms	:	public	BaseUniform
{
		UniformWrapper			m_viewproj
							,	m_model
							,	m_view
							,	m_proj;

public:

virtual	void					GetUniformLocation(const uint program, const int index);
virtual	void					SetUniforms(RenderingContext& context);
};


//!
class	MaterialsUniforms	:	public	BaseUniform
{
		UniformWrapper			m_Ka
							,	m_Kd
							,	m_Ks
							,	m_Ns;
public:

virtual	void					GetUniformLocation(const uint program, const int index);
virtual	void					SetUniforms(RenderingContext& context);
};


//!
class	TextureUniform	:	public	BaseUniform
{
		UniformWrapper			m_texture;

public:

virtual	void					GetUniformLocation(const uint program, const int index);
virtual	void					SetUniforms(RenderingContext &context);
};



//!
class	NormalMapUniform	:	public	BaseUniform
{
		UniformWrapper			m_normal_map;

public:

virtual	void					GetUniformLocation(const uint program, const int index);
virtual	void					SetUniforms(RenderingContext &context);
};


//!
class	LightsCountUniform	:	public	BaseUniform
{
protected:

		UniformWrapper			m_count;

virtual	void					GetUniformLocation(const uint program, const int index);
virtual	void					SetUniforms(RenderingContext& context);
};


//!
class	LightUniforms	:	public	BaseUniform
{
		int						m_index;
		UniformWrapper			m_La
							,	m_Ld
							,	m_Ls;

protected:

		LightUniforms(const uint index) : m_index(index) {}

virtual	void					GetUniformLocation(const uint program, const int index);
virtual	void					SetUniforms(RenderingContext& context);

const	int						GetIndex() const								{	return m_index;	}
};


//!
class	DirectionalLightUniforms	:	public	LightUniforms
{
		UniformWrapper			m_type
							,	m_dir;

public:
		
		DirectionalLightUniforms(const uint index) : LightUniforms(index) {}

virtual	void					GetUniformLocation(const uint program, const int index);
virtual	void					SetUniforms(RenderingContext& context);
};


//!
class	PointLightUniforms	:	public	LightUniforms
{
		UniformWrapper			m_type
							,	m_pos
							,	m_range;

public:

		PointLightUniforms(const uint index) : LightUniforms(index) {}
		
virtual	void					GetUniformLocation(const uint program, const int index);
virtual	void					SetUniforms(RenderingContext& context);
};


//!
class	SpotLightUniforms	:	public	LightUniforms
{
		UniformWrapper			m_type
							,	m_pos
							,	m_dir
							,	m_range
							,	m_inner_q
							,	m_outer_q;

public:
	
		SpotLightUniforms(const uint index) : LightUniforms(index) {}
		
virtual	void					GetUniformLocation(const uint program, const int index);
virtual	void					SetUniforms(RenderingContext &context);
};


//!
struct	LightUniformsFactory
{
static	LightUniforms*			Create(const Light& l, const uint index);
};


}		//	owl
#endif	//	__UNIFORMS__
