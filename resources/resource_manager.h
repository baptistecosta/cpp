/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __RESOURCE_MANAGER__
#define __RESOURCE_MANAGER__

	#include "collada_loader.h"
	#include "font.h"
	#include "geometry.h"
	#include "material.h"
	#include "mesh.h"
	#include "obj_loader.h"
	#include "object.h"
	#include "picture.h"
	#include "rendering_geometry.h"
	#include "text2d.h"
	#include "core/containers/hash_table.h"

namespace owl {

//!	\brief	Store a resource into a std::map container using string key value accessor.
template <class T>	class	ResourceManager
{
	typedef	typename SharedHashMap<String, T>::type Map;
private:

	Map				cache;

public:

	//!	Return true if a resource is already store in the map container via "id".
	//!	\param	id, string key value of the resource you want to find.
	T*				find(const String& id)
	{
		if (Map::Pair* entry = cache.Find(id))
			return entry->GetValue();
		return NULL;
	}

	//!	Clean up the std::map container.
	void			cleanUp()
	{	cache.Clear();	}

	//!	Store a resource <T*> in the std::map container.
	//!	\param	m, the resource you want to add.
	T*				registerRes(T* m)
	{
		if (!find(m->getGuid()))
			cache.Insert(m->getGuid(), m);
		return m;
	}

	Map&			getCache()
	{	return cache;	}
	const Map&		getCache() const
	{	return cache;	}
	
	T*				getResource(const String& id)
	{
		if (Map::Pair* e = cache.Find(id))
			return e->GetValue();
		return NULL;
	}
};

//!
struct	Resources
{
	static	ResourceManager<Font>					fonts;
	static	ResourceManager<Geometry>				geometries;
	static	ResourceManager<Controller>				controllers;
	static	ResourceManager<LibraryGeometries>		lib_geometries;
	static	ResourceManager<Mesh>					meshes;
	static	ResourceManager<Material>				materials;
	static	ResourceManager<Object>					objects;
	static	ResourceManager<Picture>				pictures;
	static	ResourceManager<RenderingGeometry>		render_geometries;
	static	ResourceManager<Scene>					scenes;
	static	ResourceManager<Text2D>					texts2d;
	static	ResourceManager<LibraryVisualScenes>	lib_visual_scenes;
//	static	ResourceManager<Skeleton>				skeletons;
};

//!
struct	ResourceLoaders
{
	static	ObjLoader		obj;
};

}		// owl
#endif	// __RESOURCE_MANAGER__