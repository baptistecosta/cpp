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
	,	m_geometry(null_ptr)
	,	m_rnd_geom(null_ptr)
//	,	m_skeleton(null_ptr)
{
	//
}

//-----------------------------------------------------------------------------
Material*		Mesh::GetMaterial(int i)
{	assert(i >= 0 && i < m_material_list.size()); return m_material_list[i];	}

//-----------------------------------------------------------------------------
Picture*		Mesh::getPicture(int i)
{	assert(i >= 0 && i < m_picture_list.size()); return m_picture_list[i];	}
