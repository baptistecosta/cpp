/*
	bEngine :: 2011 - 2012
	Baptiste Costa
*/

	#include "spot_light.h"
	#include "core/keymap.h"
	#include "core/string.h"
		
using namespace owl;

//-----------------------------------------------------------------------------
SpotLight::SpotLight(const SpotLight& l)
	:	Light(l.m_La, l.m_Ld, l.m_Ls)
	,	m_pos(l.m_pos)
	,	m_dir(l.m_dir)
	,	m_range(l.m_range)
	,	m_inner_q(l.m_inner_q)
	,	m_outer_q(l.m_outer_q)
{
	Init();
}

//-----------------------------------------------------------------------------
SpotLight::SpotLight(const Vector3& pos, const Vector3& dir, const float range, const float inner_q, const float outer_q, const Color& La, const Color& Ld, const Color& Ls)
	:	Light(La, Ld, Ls)
	,	m_pos(pos)
	,	m_dir(dir)
	,	m_range(range)
	,	m_inner_q(inner_q)
	,	m_outer_q(outer_q)
{
	Init();
}

//-----------------------------------------------------------------------------
void			SpotLight::Init()
{
	m_type = Spot;
	m_dir.normalize();
}

//-----------------------------------------------------------------------------
SpotLight&		SpotLight::operator= (SpotLight& l)
{
	m_pos = l.m_pos;
	m_dir = l.m_dir.normalize();
	m_La = l.m_La;
	m_Ld = l.m_Ld;
	m_Ls = l.m_Ls;
	m_inner_q = l.m_inner_q;
	m_outer_q = l.m_outer_q;

	return *this;
}

//-----------------------------------------------------------------------------
void			SpotLight::onMouseWheel(const bool is_forward)
{
	if (KeyPress::leftCtrl())
	{
		float step = 0.5f;
		m_range += is_forward ? step : -step;
	}
}

//-----------------------------------------------------------------------------
void			SpotLight::onKeyEvent_Kp5()
{
	if (KeyPress::leftCtrl())
		m_dir.z -= 0.02f, m_dir.normalize();
	else
		m_pos.z -= 0.1f;
}
//-----------------------------------------------------------------------------
void			SpotLight::onKeyEvent_Kp2()
{
	if (KeyPress::leftCtrl())
		m_dir.z += 0.02f, m_dir.normalize();
	else
		m_pos.z += 0.1f;
}
//-----------------------------------------------------------------------------
void			SpotLight::onKeyEvent_Kp1()
{
	if (KeyPress::leftCtrl())
		m_dir.x -= 0.02f, m_dir.normalize();
	else
		m_pos.x -= 0.1f;
}
//-----------------------------------------------------------------------------
void			SpotLight::onKeyEvent_Kp3()
{
	if (KeyPress::leftCtrl())
		m_dir.x += 0.02f, m_dir.normalize();
	else
		m_pos.x += 0.1f;
}
//-----------------------------------------------------------------------------
void			SpotLight::onKeyEvent_Kp9()
{
	if (KeyPress::leftCtrl())
		m_dir.y += 0.02f, m_dir.normalize();
	else
		m_pos.y += 0.1f;
}
//-----------------------------------------------------------------------------
void			SpotLight::onKeyEvent_Kp6()
{
	if (KeyPress::leftCtrl())
		m_dir.y -= 0.02f, m_dir.normalize();
	else
		m_pos.y -= 0.1f;
}
