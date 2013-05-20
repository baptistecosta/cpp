/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

	#include <assert.h>
	#include "light.h"
	#include "directional_light.h"
	#include "point_light.h"
	#include "spot_light.h"

using namespace owl;

//-----------------------------------------------------------------------------
Light::Light(const Color& La, const Color& Ld, const Color& Ls)
	:	m_speed(0.1f)
	,	m_La(La)
	,	m_Ld(Ld)
	,	m_Ls(Ls)
{
	//
}

//-----------------------------------------------------------------------------
void			LightsLib::Load(Light* l)
{
	assert(l);
	lights.Push(l);
}

//-----------------------------------------------------------------------------
Light*			LightsLib::Get(const int i) const
{
	assert(i >= 0 && i < lights.Size());
	return lights[i];
}

