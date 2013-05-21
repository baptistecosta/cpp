/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

	#include "rendering_geometry.h"
	#include "core/math/math.h"
	#include "core/log.h"
	#include "graphics_wrappers/opengl.h"
	#include "graphics_wrappers/opengl_renderer.h"
	#include "resources/resource_manager.h"

	using namespace owl;

//-----------------------------------------------------------------------------
RenderingGeometry::RenderingGeometry(const String& guid): Resource(guid)
{
	for (uint i = 0; i < NofBuffers; i++)
		buffer[i] = 0;
}

//-----------------------------------------------------------------------------
RenderingGeometry::~RenderingGeometry()
{
	if (buffer[0]) glDeleteBuffers(1, &buffer[0]);
	if (buffer[1]) glDeleteBuffers(1, &buffer[1]);
	if (buffer[2]) glDeleteBuffers(1, &buffer[2]);
	if (buffer[3]) glDeleteBuffers(1, &buffer[3]);
	if (buffer[4]) glDeleteBuffers(1, &buffer[4]);
	if (buffer[5]) glDeleteBuffers(1, &buffer[5]);
	if (buffer[6]) glDeleteBuffers(1, &buffer[6]);
	if (buffer[7]) glDeleteBuffers(1, &buffer[7]);
}

//-----------------------------------------------------------------------------
void			RenderingGeometriesLib::Load(GLRenderer& renderer)
{
	Log::HorizontalLine(); Log::i("Loading rendering geometries...");

	SharedHashMap<String, Geometry*>& map = Resources::geometries.getCache();
	map.forEachItem([&renderer](Geometry* g)
	{
		RenderingGeometry* rd = renderer.ConvertToRenderingGeometry(g);
		Log::i("Loading rendering geometry with id: %s", rd->getGuid());
		rd->Bind();
//		OpenGL::Bind3DGeometry(rd);
		Resources::render_geometries.registerRes(rd);
	});
}

//-----------------------------------------------------------------------------
void			RenderingGeometry::Bind()
{
	if (!vertices.pos.empty()) OpenGL::CreateGLBuffer((void*)&vertices.pos[0], sizeof(Vector3) * vertices.pos.size(), buffer[0], GL_STATIC_DRAW);
	if (!vertices.uv.empty()) OpenGL::CreateGLBuffer((void*)&vertices.uv[0], sizeof(Vector2) * vertices.pos.size(), buffer[1], GL_STATIC_DRAW);
	if (!vertices.nor.empty()) OpenGL::CreateGLBuffer((void*)&vertices.nor[0], sizeof(Vector3) * vertices.pos.size(), buffer[2], GL_STATIC_DRAW);
	if (!vertices.tang.empty()) OpenGL::CreateGLBuffer((void*)&vertices.tang[0], sizeof(Vector3) * vertices.pos.size(), buffer[3], GL_STATIC_DRAW);
	if (!vertices.btan.empty()) OpenGL::CreateGLBuffer((void*)&vertices.btan[0], sizeof(Vector3) * vertices.pos.size(), buffer[4], GL_STATIC_DRAW);
	if (!vertices.joints.empty()) OpenGL::CreateGLBuffer((void*)&vertices.joints[0], sizeof(SkinningJoints) * vertices.pos.size(), buffer[5], GL_STATIC_DRAW);
	if (!vertices.weights.empty()) OpenGL::CreateGLBuffer((void*)&vertices.weights[0], sizeof(SkinningWeights) * vertices.pos.size(), buffer[6], GL_STATIC_DRAW);
	if (!vertices.indices.empty()) OpenGL::CreateGLBuffer((void*)&vertices.indices[0], sizeof(ushort) * vertices.indices.size(), buffer[7], GL_STATIC_DRAW);
}
