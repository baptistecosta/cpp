/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __MATERIAL__
#define __MATERIAL__

	#include "resource.h"
	#include "../core/math/vector.h"

namespace owl {

//!
class	Material	:	public	Resource
{
		Vector3			m_Ka, m_Kd, m_Ks;	// Ambient, diffuse & specular coefficient of a 3D mesh material.
		float			m_Ns;				// Shininess of a 3D mesh material (1 - 200).

public:
		
		Material(String guid = "default_material");

		Material&		operator =	(const Material& mtl);

const	Vector3&		getKa() const					{	return m_Ka;	}
const	Vector3&		getKd() const					{	return m_Kd;	}
const	Vector3&		getKs() const					{	return m_Ks;	}
const	float&			getNs() const					{	return m_Ns;	}

		void			setKa(const Vector3& Ka)		{	m_Ka = Ka;	}
		void			setKd(const Vector3& Kd)		{	m_Kd = Kd;	}
		void			setKs(const Vector3& Ks)		{	m_Ks = Ks;	}
		void			setNs(const float Ns)			{	m_Ns = Ns;	}

static	const Vector3	def_Ka, def_Kd, def_Ks;
static	const float		def_Ns;


const	void			log();
};

}		// owl
#endif	// __MATERIAL__
