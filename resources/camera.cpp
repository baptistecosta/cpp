/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

	#include "camera.h"
	#include <glfw.h>
	#include "core/framerate.h"
	#include "core/keymap.h"
	#include "core/string.h"

	using namespace owl;
	using namespace owl::math;

//-----------------------------------------------------------------------------
Vector2				Camera::resolution;
const float			Camera::move_speed = 0.4f;
const float			Camera::orientation_speed = 0.001f;

//-----------------------------------------------------------------------------
Camera::Camera()
	:	proj_state(Camera::PerspProj)
	,	m_pos(0, 0, 0)
	,	m_hQ(0.f)
	,	m_vQ(0.f)
	,	m_fov(45.f)
	,	mouse_x(0)
	,	mouse_y(0)
{
	//
}

//-----------------------------------------------------------------------------
Camera::Camera(const Camera& camera)
	:	proj_state(camera.proj_state)
	,	m_pos(camera.m_pos)
	,	m_hQ(camera.m_hQ)
	,	m_vQ(camera.m_vQ)
	,	m_fov(camera.m_fov)
	,	mouse_x(camera.mouse_x)
	,	mouse_y(camera.mouse_x)
{
	//
}

//-----------------------------------------------------------------------------
void			Camera::UpdateAngleFromBlockedMousePosition(const Vector2& screen_res)
{
	glfwSetMousePos(static_cast<int>(screen_res.x * 0.5f), static_cast<int>(screen_res.y * 0.5f));

	float sf = FrameRate::framerate.GetSpeedFactor();
	m_hQ += orientation_speed * sf * (screen_res.x * 0.5f - mouse_x);
	m_vQ -= orientation_speed * sf * (screen_res.y * 0.5f - mouse_y);
}

//-----------------------------------------------------------------------------
void			Camera::ComputeViewProjMatrices(const Vector2& offset)
{
	m_hQ += offset.x * orientation_speed * FrameRate::framerate.GetSpeedFactor();
	m_vQ += offset.y * orientation_speed * FrameRate::framerate.GetSpeedFactor();

	// Front vector
	m_direction = Vector3(cos(m_vQ) * sin(m_hQ), sin(m_vQ), cos(m_vQ) * cos(m_hQ));
	m_right = Vector3(sin(m_hQ - pi2), 0.f, cos(m_hQ - pi2));

	Vector3 up;
	Vector3::Cross(up, m_direction, m_right);
	up.normalize();

	m_proj.LoadIdentity();
	m_view.LoadIdentity();

	switch (proj_state)
	{
		case OrthoProj: ortho(m_proj, -4.f, 4.f, -3.f, 3.f, 0.1f, 100.f); break;
		case PerspProj: persp(m_proj, m_fov, 4.f / 3.f, 0.1f, 100.f); break;
	}
	lookAt(m_view, m_pos, m_direction, up);

	// Compute view proj matrix
	m_viewproj = m_proj * m_view;
}

//-----------------------------------------------------------------------------
void			Camera::onMouseWheel(const bool is_forward)
{
	if (KeyPress::leftShift())
	{
		float step = 2.f;
		m_fov += is_forward ? -step : step;
	}
}

//-----------------------------------------------------------------------------
void			Camera::onKeyEvent_Up()
{	m_pos += m_direction * FrameRate::framerate.GetSpeedFactor() * move_speed;	}
void			Camera::onKeyEvent_Down()
{	m_pos -= m_direction * FrameRate::framerate.GetSpeedFactor() * move_speed;	}
void			Camera::onKeyEvent_Left()
{	m_pos -= m_right * FrameRate::framerate.GetSpeedFactor() * move_speed;	}
void			Camera::onKeyEvent_Right()
{	m_pos += m_right * FrameRate::framerate.GetSpeedFactor() * move_speed;	}


//-----------------------------------------------------------------------------
void			Camera::onMouseMoveEvent(int x, int y)
{
	ComputeViewProjMatrices(Vector2(-(x - (resolution.x * 0.5f)), y - (resolution.y * 0.5f)));
}
