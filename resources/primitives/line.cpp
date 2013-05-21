/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

	#include "line.h"
	#include "core/math/vector.h"
	#include "resources/rendering_geometry.h"

using namespace owl;

//-----------------------------------------------------------------------------
void			Line::init(RenderingGeometry& g)
{
	for (uint i = 0; i < g.GetVertices().pos.size(); i++)
	{
		l.push_back(g.GetVertices().pos[i]);
		l.push_back(g.GetVertices().pos[i] + g.GetVertices().nor[i] * 0.3f);
		l.push_back(g.GetVertices().pos[i]);
		l.push_back(g.GetVertices().pos[i] + g.GetVertices().tang[i] * 0.3f);
		l.push_back(g.GetVertices().pos[i]);
		l.push_back(g.GetVertices().pos[i] + g.GetVertices().btan[i] * 0.3f);
		c.push_back(Vector3(0, 0, 1));
		c.push_back(Vector3(0, 0, 1));
		c.push_back(Vector3(1, 0, 0));
		c.push_back(Vector3(1, 0, 0));
		c.push_back(Vector3(0, 1, 0));
		c.push_back(Vector3(0, 1, 0));
	}
}

