/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

	#include "directional_light.h"
	#include "core\string.h"
	
using namespace owl;

//-----------------------------------------------------------------------------
DirectionalLight::DirectionalLight()
	:	Light(Color::Black, Color::Sunlight, Color::Skyoc)
{
	init();
}

//-----------------------------------------------------------------------------
DirectionalLight::DirectionalLight(DirectionalLight& l)
	:	Light(l.m_La, l.m_Ld, l.m_Ls)
	,	m_direction(l.m_direction.normalize())
{
	init();
}

//-----------------------------------------------------------------------------
DirectionalLight::DirectionalLight(Vector3& dir, const Color& La, const Color& Ld, const Color& Ls)
	:	Light(La, Ld, Ls)
	,	m_direction(dir.normalize())
{
	init();
}

//-----------------------------------------------------------------------------
void			DirectionalLight::init()
{
	m_type = Directional;
}

