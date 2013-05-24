/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __SPOT_LIGHT__
#define __SPOT_LIGHT__

	#include "core/math/vector.h"
	#include "light.h"

namespace owl {

//!
class	SpotLight	:	public	Light
{
private:

		Vector3				m_pos,
							m_dir;
		float				m_range,
							m_inner_q,
							m_outer_q;

public:

		SpotLight		(	const SpotLight& l	);
		SpotLight		(	
							const Vector3& pos = Vector3(-5.f, 5.f, 5.f),
							const Vector3& dir = Vector3(1.f, -1.f, -1.f),
							const float range = 25.f,
							const float inner_q = 15.f,
							const float outer_q = 10.f,
							const Color& La = Color::Black,
							const Color& Ld = Color::Sunlight,
							const Color& Ls = Color::Skyoc
						);

		void				Init();

		SpotLight&			operator =	(SpotLight& l);

		void				SetDirection(Vector3& v)							{	m_dir = v.normalize();	}

		Vector3&			GetPos()											{	return m_pos;	}
		Vector3&			GetDir()											{	return m_dir;	}
const	Vector3&			GetDir() const										{	return m_dir;	}
const	float				GetRange() const									{	return m_range;	}
const	float				GetInnerQ()	const									{	return m_inner_q;	}
const	float				GetOuterQ() const									{	return m_outer_q;	}

virtual	void				onMouseWheel(const bool is_forward);
virtual	void				onKeyEvent_Kp5();
virtual	void				onKeyEvent_Kp2();
virtual	void				onKeyEvent_Kp1();
virtual	void				onKeyEvent_Kp3();
virtual	void				onKeyEvent_Kp9();
virtual	void				onKeyEvent_Kp6();
};

}		//	owl
#endif	//	__SPOT_LIGHT__
