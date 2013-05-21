/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/
#ifndef __DIRECTIONAL_LIGHT__
#define __DIRECTIONAL_LIGHT__

	#include "core/math/vector.h"
	#include "light.h"

namespace owl {

//!
class	DirectionalLight	:	public	Light
{
private:

		Vector3					m_direction;
		
public:

		DirectionalLight();
		DirectionalLight(DirectionalLight& l);
		DirectionalLight(Vector3& dir, const Color& La = Color::Black, const Color& Ld = Color::Sunlight, const Color& Ls = Color::Skyoc);

		void					init();

		DirectionalLight&		operator =	(DirectionalLight& l)				{	m_direction = l.m_direction.normalize(); m_La = l.m_La, m_Ld = l.m_Ld, m_Ls = l.m_Ls; return *this;	}

		Vector3&				getDirection()									{	return m_direction;	}
const	Vector3&				getDirection() const							{	return m_direction;	}

virtual	void					onKeyEvent_Kp5()								{	m_direction.z -= 0.1f; m_direction.normalize();	}
virtual	void					onKeyEvent_Kp2()								{	m_direction.z += 0.1f; m_direction.normalize();	}
virtual	void					onKeyEvent_Kp1()								{	m_direction.x -= 0.1f; m_direction.normalize();	}
virtual	void					onKeyEvent_Kp3()								{	m_direction.x += 0.1f; m_direction.normalize();	}
virtual	void					onKeyEvent_Kp9()								{	m_direction.y += 0.1f; m_direction.normalize();	}
virtual	void					onKeyEvent_Kp6()								{	m_direction.y -= 0.1f; m_direction.normalize();	}
};

}		//	owl
#endif	//	__DIRECTIONAL_LIGHT__
