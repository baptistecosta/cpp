/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

	#include "resource_manager.h"

using namespace owl;

//-----------------------------------------------------------------------------
ResourceManager<Font>					Resources::fonts;
ResourceManager<Geometry>				Resources::geometries;
ResourceManager<Controller>				Resources::controllers;
ResourceManager<LibraryGeometries>		Resources::lib_geometries;
ResourceManager<Mesh>					Resources::meshes;
ResourceManager<Material>				Resources::materials;
ResourceManager<Picture>				Resources::pictures;
ResourceManager<RenderingGeometry>		Resources::render_geometries;
ResourceManager<Scene>					Resources::scenes;
ResourceManager<Text2D>					Resources::texts2d;
ResourceManager<LibraryVisualScenes>	Resources::lib_visual_scenes;
//ResourceManager<Skeleton>				Resources::skeletons;

//-----------------------------------------------------------------------------
ObjLoader								ResourceLoaders::obj;
