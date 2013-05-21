/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __RENDERING_CONTEXT__
#define __RENDERING_CONTEXT__

	#include "core/containers/auto_array.h"
	#include "core/containers/vector.h"
	#include "resources/material.h"
	#include "resources/light.h"
	#include "typedefs.h"

namespace owl {

class	Matrix4;
class	GLRenderer;
class	RenderingGeometry;
class	Shader;
class	Text2D;

//!
class	RenderingContext
{
typedef	SharedVector<Light>::type Lights;

		Matrix4*				model_mtx;
		Matrix4*				proj_mtx;
		Matrix4*				view_mtx;
		Matrix4*				viewproj_mtx;
		Material*				material;
		int						texture_unit,
								normal_map_unit;
		Lights*					lights;
	
		// Test -- Make RenderingGeometry and Text2D and 3D sub-classes.
		RenderingGeometry*		rd_geom;
		Text2D*					text_2d;

public:

		RenderingContext();

		void					UseProgram(const Shader&);

		void					Draw(Shader&, GLRenderer&);
		void					DrawText(Shader&, GLRenderer&);

		const Matrix4&			GetModelMatrix() const							{	return *model_mtx;	}
		const Matrix4&			GetProjMatrix() const							{	return *proj_mtx;	}
		const Matrix4&			GetViewMatrix() const							{	return *view_mtx;	}
		const Matrix4&			GetViewProjMatrix() const						{	return *viewproj_mtx;	}
		const Material&			GetMaterial() const								{	return *material;	}
		int						GetTextureUnit()								{	return texture_unit;	}
		int						GetNormalMapUnit()								{	return normal_map_unit;	}
		Light&					GetLight(const int i);
		const Lights&			GetLights() const								{	return *lights;	}
		RenderingGeometry&		GetRenderingGeometry() const					{	return *rd_geom;	}
		Text2D&					GetText2D() const								{	return *text_2d;	}

		void					SetModelMatrix(Matrix4* m)						{	model_mtx = m;	}
		void					SetProjMatrix(Matrix4* m)						{	proj_mtx = m;	}
		void					SetViewMatrix(Matrix4* m)						{	view_mtx = m;	}
		void					SetViewProjMatrix(Matrix4* m)					{	viewproj_mtx = m;	}
		void					SetMaterial(Material* m)						{	material = m;	}
		void					SetTextureUnit(const uint t_u)					{	texture_unit = t_u;	}
		void					SetNormalMapUnit(const uint nm_u)				{	normal_map_unit = nm_u;	}
		void					SetLights(Lights* l)							{	lights = l;	}

		void					SetRenderingGeometry(RenderingGeometry* rg)		{	rd_geom = rg;	}
		void					SetText2D(Text2D* t)							{	text_2d = t;	}
};

}		//	owl
#endif	//	__RENDERING_CONTEXT__
