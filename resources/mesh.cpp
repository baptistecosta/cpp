/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

	#include <assert.h>
	#include "mesh.h"

using namespace owl;

//-----------------------------------------------------------------------------
Mesh::Mesh(const String& guid)
	:	Resource(guid)
	,	m_geometry(0)
	,	m_rnd_geom(0)
//	,	m_skeleton(0)
{
	//
}

//-----------------------------------------------------------------------------
Material*		Mesh::GetMaterial(int i)
{	assert(i >= 0 && i < m_material_list.size()); return m_material_list[i];	}

//-----------------------------------------------------------------------------
Picture*		Mesh::getPicture(int i)
{	assert(i >= 0 && i < m_picture_list.size()); return m_picture_list[i];	}
