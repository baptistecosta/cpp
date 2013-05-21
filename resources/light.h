/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __LIGHT__
#define __LIGHT__

	#include <vector>	
	#include "core/containers/auto_array.h"
	#include "core/containers/shared_pointer.h"
	#include "core/containers/vector.h"
	#include "core/event_listener.h"
	#include "typedefs.h"
	#include "color.h"

namespace owl {

class	LightUniforms;

//!
class	Light
	:	public	EventListener
	,	public	SharedObject
{
public:

		enum	Type
		{
			Point		=	0,
			Directional,
			Spot
		};

protected:

		Type					m_type;
		Color					m_La,			// Ambient
								m_Ld,			// Diffuse
								m_Ls;			// Specular
		float					m_speed;
		
		Light(const Color& La, const Color& Ld, const Color& Ls);

public:

virtual	~Light() {}

const	Type					GetType() const									{	return m_type;	}
const	Color&					getLa() const									{	return m_La;	}
const	Color&					getLd() const									{	return m_Ld;	}
const	Color&					getLs() const									{	return m_Ls;	}
		
		void					setLa(const Color& La)							{	m_La = La;	}
		void					setLd(const Color& Ld)							{	m_Ld = Ld;	}
		void					setLs(const Color& Ls)							{	m_Ls = Ls;	}
};

//!
class	LightsLib
{
		typedef SharedVector<Light>::type	SharedLights;

		SharedLights			lights;

public:

		void					Load(Light* l);
		SharedLights&			Get()											{	return lights;	}
		Light*					Get(const int i) const;

const	int						Size() const									{	return lights.Size();	}
};

}		//	owl
#endif	//	__LIGHT_MGNT__
