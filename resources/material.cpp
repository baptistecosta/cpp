/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

	#include "material.h"
	#include "resource_manager.h"
	#include "core\log.h"

//-----------------------------------------------------------------------------
Material::Material(String guid)
	:	Resource(guid)
	,	m_Ka(def_Ka)
	,	m_Kd(def_Kd)
	,	m_Ks(def_Ks)
	,	m_Ns(def_Ns)
{
	if (!Resources::materials.find(guid))
		Resources::materials.registerRes(this);
}

//-----------------------------------------------------------------------------
Material&		Material::operator = (const Material& mtl)
{
	m_guid = mtl.m_guid;
	m_Ka = mtl.m_Ka;
	m_Kd = mtl.m_Kd;
	m_Ks = mtl.m_Ks;
	m_Ns = mtl.m_Ns;
	return *this;
}

//-----------------------------------------------------------------------------
const void		Material::log()
{
	Log::HorizontalLine();
	Log::i("Material name: %s", m_guid);
	Log::i("Ka: %d, Kd: %d, Ks %d, Ns %f", m_Ka, m_Kd, m_Ks, m_Ns);
	Log::NewLine();
}

//-----------------------------------------------------------------------------
const Vector3	Material::def_Ka = Vector3(0.f, 0.f, 0.f);
const Vector3	Material::def_Kd = Vector3(0.8f, 0.8f, 0.8f);
const Vector3	Material::def_Ks = Vector3(0.8f, 0.8f, 0.8f);
const float		Material::def_Ns = 96.f;
