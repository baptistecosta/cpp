/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

	#include <assert.h>
	#include "opengl_renderer.h"
	#include "rendering_geometry.h"
	#include "object.h"
	#include "mesh.h"

	using namespace owl;

//-----------------------------------------------------------------------------
Object::Object(Mesh* m, const Vector3& p, const Vector3& q, const Vector3& sc):
	Resource(m->getGuid()),
	mesh(m),
	t(p),
	r(q),
	s(sc)
{
	Init();
}

//-----------------------------------------------------------------------------
void			Object::Init()
{
	RenderingGeometry* rd = GLRenderer::ConvertToRenderingGeometry(mesh->getGeometry());
	rd->Bind();
	mesh->setRenderingGeometry(rd);
}