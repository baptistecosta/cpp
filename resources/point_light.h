/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __POINT_LIGHT__
#define __POINT_LIGHT__

	#include "core/math/vector.h"
	#include "light.h"

namespace owl {

//!
class	PointLight	:	public	Light
{
private:

		Vector3				m_pos;
		float				m_range;

public:

		PointLight		(	const PointLight& l	);
		PointLight		(
							const Vector3& pos = Vector3::origin,
							const float range = 25.f,
							const Color& La = Color::Black,
							const Color& Ld = Color::Sunlight,
							const Color& Ls = Color::Skyoc
						);

		void				init();

		PointLight&			operator =	(const PointLight& l)					{	m_pos = l.m_pos; m_La = l.m_La, m_Ld = l.m_Ld, m_Ls = l.m_Ls; return *this;	}

		Vector3&			GetPos()											{	return m_pos;	}
const	Vector3&			GetPos() const										{	return m_pos;	}
const	float				getRange() const									{	return m_range;	}

virtual	void				onKeyEvent_Kp5()									{	m_pos.z -= m_speed;	}	// Forward
virtual	void				onKeyEvent_Kp2()									{	m_pos.z += m_speed;	}	// Backward
virtual	void				onKeyEvent_Kp1()									{	m_pos.x -= m_speed;	}	// Left
virtual	void				onKeyEvent_Kp3()									{	m_pos.x += m_speed;	}	// Right
virtual	void				onKeyEvent_Kp9()									{	m_pos.y += m_speed;	}	// Up
virtual	void				onKeyEvent_Kp6()									{	m_pos.y -= m_speed;	}	// Down
};

}		//	owl
#endif	//	__POINT_LIGHT__
