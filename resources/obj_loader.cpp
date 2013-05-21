/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/
	#include <Windows.h>
	#include "core/log.h"
	#include "obj_loader.h"
	#include "resource_manager.h"

using namespace owl;

//-----------------------------------------------------------------------------
Mesh*			ObjLoader::Load(const String& path)
{
	if (path.IsEmpty())
	{
		Log::e("ObjLoader - Path is empty.");
		return false;
	}

	m_obj_path = path;
	Mesh* mesh = null_ptr;
	if (mesh = Resources::meshes.getResource(m_obj_path))
		return mesh;

	Log::HorizontalLine();

	mesh = new Mesh(path);
	loadGeometry(mesh);
	loadMaterial(mesh);
	
	return mesh;
}

//-----------------------------------------------------------------------------
void			ObjLoader::loadGeometry(Mesh* mesh)
{
	Geometry* geometry = null_ptr;
	if (geometry = Resources::geometries.find(m_obj_path))
	{
		mesh->setGeometry(geometry);
		return;
	}

	Log::i("Loading geometry with id: %s", m_obj_path);
	geometry = new Geometry(m_obj_path);

	std::vector<Vector3> vpos, vnor;
	std::vector<Vector2> vuv;
	std::vector<uint> vpos_index, vuv_index, vnor_index;

	if (!m_fs.Open(m_obj_path))
		DebugBreak();

	String str;
	while (true)
	{
		if (!m_fs.Scan(str))
			break;

		if		(str == "v")	loadVertexData(m_fs.GetFile(), vpos);
		else if (str == "vt")	loadVertexUv(m_fs.GetFile(), vuv);
		else if (str == "vn")	loadVertexData(m_fs.GetFile(), vnor);
		else if (str == "f")	loadVertexIndex(m_fs.GetFile(), vpos_index, vnor_index, vuv_index);
	}
	
	for (uint i = 0; i < vpos_index.size(); i++)
	{
		int pos_index = vpos_index[i], uv_index = vuv_index[i], nor_index = vnor_index[i];

		Vector3 _pos = vpos[pos_index - 1], _nor = vnor[nor_index - 1];
		Vector2 _uv = vuv[uv_index - 1];

		geometry->vertices.Push(new Vertex(_pos, _nor, _uv));
	}
	geometry->ComputeTangentBasis();
	mesh->setGeometry(Resources::geometries.registerRes(geometry));
}

//-----------------------------------------------------------------------------
bool			ObjLoader::loadVertexData(FILE* f, std::vector<Vector3>& vv)
{
	Vector3 _vd;
	if (fscanf(f, "%f %f %f\n", &_vd.x, &_vd.y, &_vd.z) != 3)
	{
		printf("Error reading OBJ. vertex data coordinates.\n");
		return false;
	}
	vv.push_back(_vd);
	return true;
}

//-----------------------------------------------------------------------------
bool			ObjLoader::loadVertexUv(FILE* f, std::vector<Vector2>& vv)
{
	Vector2 _uv;
	if (fscanf(f, "%f %f\n", &_uv.x, &_uv.y) != 2)
	{
		printf("Error reading OBJ. vertex uv coordinates.\n");
		return false;
	}
	vv.push_back(_uv);
	return true;
}

//-----------------------------------------------------------------------------
bool			ObjLoader::loadVertexIndex(FILE* f, vector<uint>& vpos_index, vector<uint>& vnor_index, vector<uint>& vuv_index)
{
	uint pos_index[3], uv_index[3], nor_index[3];

	int	n = fscanf(f, "%d/%d/%d %d/%d/%d %d/%d/%d\n",	&pos_index[0], &uv_index[0], &nor_index[0],
														&pos_index[1], &uv_index[1], &nor_index[1],
														&pos_index[2], &uv_index[2], &nor_index[2]	);
	if (n != 9)
	{
		printf ("OBJ file must contain vertex position, uv and normal coordinates.\n");
		return false;
	}

	vpos_index.push_back(pos_index[0]);
	vpos_index.push_back(pos_index[1]);
	vpos_index.push_back(pos_index[2]);
	vuv_index.push_back(uv_index[0]);
	vuv_index.push_back(uv_index[1]);
	vuv_index.push_back(uv_index[2]);
	vnor_index.push_back(nor_index[0]);
	vnor_index.push_back(nor_index[1]);
	vnor_index.push_back(nor_index[2]);

	return true;
}

//-----------------------------------------------------------------------------
void			ObjLoader::loadMaterial(Mesh* mesh)
{
	if (!m_fs.Open(m_obj_path))
		return;

	// Store material paths
	Vector<String> mtl_paths;
	storeMaterialPaths(mtl_paths);

	// Process material files
	for (int i = 0; i < mtl_paths.Size(); i++)
		mesh->addMaterial(processMaterialFile(mtl_paths[i]));
}

//-----------------------------------------------------------------------------
void			ObjLoader::storeMaterialPaths(Vector<String>& mtl_paths)
{
	String buff, mtl_filename;
	while (true)
	{
		if (!m_fs.Scan(buff))
			break;

		if (buff == "mtllib")
		{
			m_fs.Scan(mtl_filename);
			mtl_paths.Push(getMaterialPath(m_obj_path, mtl_filename));
		}
	}
	if (mtl_paths.Size() == 0)
		mtl_paths.Push("default material");
}

//-----------------------------------------------------------------------------
Material*		ObjLoader::processMaterialFile(const String& mtl_path)
{
	Material* mtl = Resources::materials.find(mtl_path);
	if (mtl)
		return mtl;

	Log::i("Loading material with path: %s", mtl_path);
	mtl = new Material(mtl_path);

	if (m_fs.Open(mtl_path))
	{
		String buff;
		while (true)
		{
			if (!m_fs.Scan(buff))
				break;

			if		(buff == "Ns") fscanf(m_fs.GetFile(), "%f", &mtl->getNs());
			else if (buff == "Ka") fscanf(m_fs.GetFile(), "%f %f %f", &mtl->getKa().x, &mtl->getKa().y, &mtl->getKa().z);
			else if (buff == "Kd") fscanf(m_fs.GetFile(), "%f %f %f", &mtl->getKd().x, &mtl->getKd().y, &mtl->getKd().z);
			else if (buff == "Ks") fscanf(m_fs.GetFile(), "%f %f %f", &mtl->getKs().x, &mtl->getKs().y, &mtl->getKs().z);
		}
	}
	return Resources::materials.registerRes(mtl);
}

//-----------------------------------------------------------------------------
String			ObjLoader::getMaterialPath(String obj_path, String& mtl_path)
{
	int slash_pos = obj_path.FindLastOf("/");
	obj_path.Erase(slash_pos);
	return obj_path + "/" + mtl_path;
}
