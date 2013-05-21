/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/
	
	#include <map>
	#include "core/containers/stl.h"
	#include "core/log.h"
	#include "geometry.h"
	#include "obj_loader.h"
	#include "resource_manager.h"

	using namespace owl;

//-----------------------------------------------------------------------------
Vertex::Vertex(const Vector3& _pos, const Vector3& _nor, const Vector2& _uv)
{	pos = _pos; nor = _nor; uv = _uv;	}


//-----------------------------------------------------------------------------
Geometry::Geometry(String& _guid):
	Resource(_guid),
	is_skinned(false)
{}

//-----------------------------------------------------------------------------
void			Geometry::ComputeTangentBasis()
{
	for (int i = 0; i < vertices.Size(); i += 3)
	{
		Vector3& v0 = vertices[i]->pos;
		Vector3& v1 = vertices[i + 1]->pos;
		Vector3& v2 = vertices[i + 2]->pos;

		Vector2& uv0 = vertices[i]->uv;
		Vector2& uv1 = vertices[i + 1]->uv;
		Vector2& uv2 = vertices[i + 2]->uv;

		Vector3 dt_pos1 = v1 - v0;
		Vector3 dt_pos2 = v2 - v0;

		Vector2 dt_uv1	= uv1 - uv0;
		Vector2 dt_uv2	= uv2 - uv0;

		float r	= 1.f / (dt_uv1.x * dt_uv2.y - dt_uv1.y * dt_uv2.x);
		Vector3 tang = (dt_pos1 * dt_uv2.y - dt_pos2 * dt_uv1.y) * r;
		Vector3 btan = (dt_pos2 * dt_uv1.x - dt_pos1 * dt_uv2.x) * r;

		tang.normalize();
		btan.normalize();

		vertices[i]->tang = tang;
		vertices[i + 1]->tang = tang;
		vertices[i + 2]->tang = tang;
		vertices[i]->btan = btan;
		vertices[i + 1]->btan = btan;
		vertices[i + 2]->btan = btan;
	}
}

//-----------------------------------------------------------------------------
void			GeometriesLib::load()
{
// 	ObjLoader::load(PathManager::get("obj_cave"));
// 	ObjLoader::load(PathManager::get("obj_floor"));
// 	ObjLoader::load(PathManager::get("obj_joint"));
// 	ObjLoader::load(PathManager::get("obj_suzan"));

	Log::HorizontalLine(); Log::i("Loading geometries...");
	ObjLoader loader;
	loader.Load(String("resources/meshes/cave.obj"));
	loader.Load(String("resources/meshes/floor.obj"));
	loader.Load(String("resources/meshes/joint.obj"));
	loader.Load(String("resources/meshes/suzan.obj"));
}
