/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

	#include "line.h"
	#include "core/math/vector.h"
	#include "rendering/rendering_geometry.h"

using namespace owl;

//-----------------------------------------------------------------------------
void			Line::init(RenderingGeometry& g)
{
	for (uint i = 0; i < g.getVertices().pos.size(); i++)
	{
		l.push_back(g.getVertices().pos[i]);
		l.push_back(g.getVertices().pos[i] + g.getVertices().nor[i] * 0.3f);
		l.push_back(g.getVertices().pos[i]);
		l.push_back(g.getVertices().pos[i] + g.getVertices().tang[i] * 0.3f);
		l.push_back(g.getVertices().pos[i]);
		l.push_back(g.getVertices().pos[i] + g.getVertices().btan[i] * 0.3f);
		c.push_back(Vector3(0, 0, 1));
		c.push_back(Vector3(0, 0, 1));
		c.push_back(Vector3(1, 0, 0));
		c.push_back(Vector3(1, 0, 0));
		c.push_back(Vector3(0, 1, 0));
		c.push_back(Vector3(0, 1, 0));
	}
}

