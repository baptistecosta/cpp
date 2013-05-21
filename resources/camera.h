/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef	__CAMERA__
#define	__CAMERA__

	#include "core/math/matrix4.h"
	#include "core/event_listener.h"

namespace owl {

//!
class	Camera	:	public	EventListener
{
private:

		enum	ProjectionState
		{
			PerspProj,
			OrthoProj
		};
		
		ProjectionState		proj_state;

		// Camera properties
static	const float			move_speed,
							orientation_speed;

		Vector3				m_pos,
							m_direction,
							m_right;
		float				m_hQ,			// Horizontal angle
							m_vQ,			// Vertical angle
							m_fov;			// Field of view

		// Transformation matrices
		Matrix4				m_view,
							m_proj,
							m_viewproj;

		// Mouse position
		int					mouse_x,
							mouse_y;

public:

		Camera();
		Camera(const Camera& camera);

		void				UpdateAngleFromBlockedMousePosition(const Vector2& screen_res);
		void				ComputeViewProjMatrices(const Vector2& mouse_pos = Vector2::origin);

private:

		void				operator = (const Camera& camera)	{}

public:

		Matrix4&			GetView()											{	return m_view;	}
		const Matrix4&		GetView() const										{	return m_view;	}
		Matrix4&			GetProj()											{	return m_proj;	}
		const Matrix4&		GetProj() const										{	return m_proj;	}
		Matrix4&			GetViewProj()										{	return m_viewproj;	}
		const Matrix4&		GetViewProj() const									{	return m_viewproj;	}
		const Vector3&		GetPosition() const									{	return m_pos;	}
		const Vector3&		GetDirection() const								{	return m_direction;	}
		const Vector2		GetMousePos() const									{	return Vector2((float)mouse_x, (float)mouse_y);	}

		void				SetPosition(const Vector3& p)						{	m_pos += p;	}
		void				SetDirection(const Vector2& d)						{	m_hQ += d.x; m_vQ += d.y;	}

private:

virtual	void				onMouseMoveEvent(int x, int y)						{	mouse_x = x, mouse_y = y;	}
virtual	void				onMouseWheel(const bool is_forward);
virtual	void				onKeyEvent_Up();
virtual	void				onKeyEvent_Down();
virtual	void				onKeyEvent_Left();
virtual	void				onKeyEvent_Right();

virtual	void				onKeyEvent_O()										{	proj_state = OrthoProj;	}
virtual	void				onKeyEvent_P()										{	proj_state = PerspProj;	}
};



}		// owl
#endif	// __CAMERA__
