/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __MESH__
#define __MESH__

	#include "resource.h"
	#include "core/containers/auto_array.h"

namespace owl {

class	Geometry;
class	Material;
class	Picture;
class	RenderingGeometry;
class	Skeleton;

//!
class Mesh :	public Resource
{
private:

		Geometry*				m_geometry;
		RenderingGeometry*		m_rnd_geom;
//		Skeleton*				m_skeleton;
		AutoArray<Material*>	m_material_list;
		AutoArray<Picture*>		m_picture_list;

public:

		Mesh(const String& guid);

		Geometry*				getGeometry()									{	return m_geometry;	}
		RenderingGeometry*		GetRenderingGeometry()							{	return m_rnd_geom;	}
//		Skeleton*				getSkeleton()									{	return m_skeleton;	}
		Material*				GetMaterial(int i);
		Picture*				getPicture(int i);

		void					setGeometry(Geometry* g)						{	m_geometry = g;	}
		void					setRenderingGeometry(RenderingGeometry* rg)		{	m_rnd_geom = rg;	}
//		void					setSkeleton(Skeleton* s)						{	m_skeleton = s;	}
		void					addMaterial(Material* mtl)						{	m_material_list.add(mtl);	}
		void					addPicture(Picture* t)							{	m_picture_list.add(t);	}
};

}		// owl
#endif	// __MESH__