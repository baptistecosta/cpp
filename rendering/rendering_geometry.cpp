/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

	#include "rendering_geometry.h"
	#include "glew.h"
	#include "opengl.h"
	#include "opengl_renderer.h"
	#include "core\math\math.h"
	#include "core\log.h"
	#include "resources\resource_manager.h"

using namespace owl;

//-----------------------------------------------------------------------------
RenderingGeometry::RenderingGeometry(const String& guid)
	:	Resource(guid)
{
	for (uint i = 0; i < NofBuffers; i++)
		m_buffer[i] = 0;
}

//-----------------------------------------------------------------------------
RenderingGeometry::~RenderingGeometry()
{
	if (m_buffer[0]) glDeleteBuffers(1, &m_buffer[0]);
	if (m_buffer[1]) glDeleteBuffers(1, &m_buffer[1]);
	if (m_buffer[2]) glDeleteBuffers(1, &m_buffer[2]);
	if (m_buffer[3]) glDeleteBuffers(1, &m_buffer[3]);
	if (m_buffer[4]) glDeleteBuffers(1, &m_buffer[4]);
	if (m_buffer[5]) glDeleteBuffers(1, &m_buffer[5]);
	if (m_buffer[6]) glDeleteBuffers(1, &m_buffer[6]);
	if (m_buffer[7]) glDeleteBuffers(1, &m_buffer[7]);
}

//-----------------------------------------------------------------------------
void			RenderingGeometriesLib::load(GLRenderer& renderer)
{
	Log::horizontalLine(); Log::i("Loading rendering geometries...");

	SharedHashMap<String, Geometry*>& map = Resources::geometries.getCache();
	map.forEachItem([&renderer](Geometry* g)
	{
		RenderingGeometry* rd = renderer.ConvertToRenderingGeometry(g);
		Log::i("Loading rendering geometry with id: %s", rd->getGuid());
		OpenGL::Bind3DGeometry(rd);
		Resources::render_geometries.registerRes(rd);
	});
}
