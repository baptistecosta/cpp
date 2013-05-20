/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __OBJ_LOADER__
#define __OBJ_LOADER__

	#include <vector>
	#include "core/filesys.h"
	#include "core/string.h"
	#include "core/containers/vector.h"
	#include "core/math/vector.h"

	using namespace std;

namespace owl {

class	Geometry;
class	Material;
class	Mesh;

//!
class	ObjLoader
{
public:

		//	Load geometry from an .obj file.
		Mesh*			Load(const String& path);

private:

		FileSystem		m_fs;
		String			m_obj_path;

		void			loadGeometry(Mesh* mesh);
		
		//	Load vertex positions, uv and normals coordinates from a .obj file.
		bool			loadVertexData(FILE*, vector<Vector3>&);
		bool			loadVertexUv(FILE*, vector<Vector2>&);
		bool			loadVertexIndex(FILE*, vector<uint>& vpos_index, vector<uint>& vnor_index, vector<uint>& vuv_index);

		//	Load material from a .mtl file. Return a pointer on Material.
		//	\param	path : .mtl file path.
		void			loadMaterial(Mesh* mesh);
		void			storeMaterialPaths(Vector<String>& mtl_paths);
		Material*		processMaterialFile(const String& mtl_path);

		//	Convert an .obj file path to a .mtl file path.
		//	\param	path : .obj file path.
		String			getMaterialPath(String obj_path, String& mtl_path);
};

}		// owl
#endif	// __OBJ_LOADER__