/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

	#include <assert.h>
	#include "object.h"
	#include "mesh.h"
	#include "rendering/opengl.h"
	#include "rendering/opengl_renderer.h"

	using namespace owl;

//-----------------------------------------------------------------------------
Object::Object(Mesh* m, const Vector3& p, const Vector3& q, const Vector3& sc)
	:	Resource(m->getGuid())
	,	mesh(m)
	,	t(p)
	,	r(q)
	,	s(sc)
{
	Init();
}

//-----------------------------------------------------------------------------
void			Object::Init()
{
	assert(mesh);
	RenderingGeometry* rd = GLRenderer::ConvertToRenderingGeometry(mesh->getGeometry());
	OpenGL::Bind3DGeometry(rd);
	assert(rd);
	mesh->setRenderingGeometry(rd);
}