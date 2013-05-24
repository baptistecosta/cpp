/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

	#include "point_light.h"
	#include "core/string.h"
	
	using namespace owl;

//-----------------------------------------------------------------------------
PointLight::PointLight(const PointLight& l)
	:	Light(l.m_La, l.m_Ld, l.m_Ls)
	,	m_pos(l.m_pos)
{
	init();
}

//-----------------------------------------------------------------------------
PointLight::PointLight(const Vector3& pos, const float range, const Color& La, const Color& Ld, const Color& Ls)
	:	Light(La, Ld, Ls)
	,	m_pos(pos)
	,	m_range(range)
{
	init();
}

//-----------------------------------------------------------------------------
void			PointLight::init()
{
	m_type = Point;
}
