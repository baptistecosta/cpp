/*
	bEngine - 2011
	Baptiste Costa
*/
	
	#include	"obj_loader.h"
	#include	"..\platform\defines.h"

Mesh*		Load( const char* path )
{
	if (Resources::resources.GetMesh().Find(path))
		return	Resources::resources.GetMesh().GetResource(path);

	Mesh* mesh = new Mesh(path);
	LoadOBJGeometry(path)
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool		LoadOBJGeometry (Geometry* m, Material* mtl)
{
	bool		r[3] = {false};		// anti-repeat vertContent;
	const char*	path = m->GetGuid();

	vUint		p_n, uv_n, n_n;
	vVec3		tempPos, tempNor;
	vVec2		tempUvs;

	FILE*		f = fopen (path, "r");

	if (!f)
	{
		printf ("Fail to open %s\n", path);
		return false;
	}

	while (1)
	{
		char	line_header[128];
		int		res = fscanf (f, "%s", line_header);

		if (res == EOF) break;	// EOF = End of file

		// Vertices
		if (strcmp(line_header, "v") == 0)
		{
			bVector3	v;
			fscanf (f, "%f %f %f\n", &v.x, &v.y, &v.z);
			tempPos.push_back (v);
			if (!r[0]) r[0] = true;
		}

		else if (strcmp(line_header, "vt") == 0)
		{
			bVector2	uv;
			fscanf (f, "%f %f\n", &uv.x, &uv.y);
			tempUvs.push_back (uv);
			if (!r[1]) r[1] = true;
		}

		else if (strcmp (line_header, "vn") == 0)
		{
			bVector3	normal;
			fscanf (f, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			tempNor.push_back (normal);
			if (!r[2]) r[2] = true;
		}

		else if (strcmp (line_header, "f") == 0)
		{
			if (r[0] && r[1] && r[2]) // Positions, uvs & normals
			{
				string	v1, v2, v3;
				unsigned int v_id[3], uv_id[3], nor_id[3];

				int	m = fscanf(f, "%d/%d/%d %d/%d/%d %d/%d/%d\n",	&v_id[0], &uv_id[0], &nor_id[0],
					&v_id[1], &uv_id[1], &nor_id[1],
					&v_id[2], &uv_id[2], &nor_id[2]);
				if (m != 9){	printf ("%s can't be read.\n", path); return false;   }

				p_n.push_back(v_id[0]); p_n.push_back(v_id[1]); p_n.push_back(v_id[2]);
				uv_n.push_back(uv_id[0]); uv_n.push_back(uv_id[1]); uv_n.push_back(uv_id[2]);
				n_n.push_back(nor_id[0]); n_n.push_back(nor_id[1]); n_n.push_back(nor_id[2]);
			}

			else if (r[0] && r[1] && !r[2]) // Positions & uvs
			{
				string	v1, v2;
				unsigned int v_id[3], uv_id[3];

				int	m = fscanf(f, "%d/%d %d/%d %d/%d\n",	&v_id[0], &uv_id[0], &v_id[1], &uv_id[1], &v_id[2], &uv_id[2]);
				if (m != 6){	printf ("%s can't be read.\n", path); return false;   }

				p_n.push_back(v_id[0]); p_n.push_back(v_id[1]); p_n.push_back(v_id[2]);
				uv_n.push_back(uv_id[0]); uv_n.push_back(uv_id[1]); uv_n.push_back(uv_id[2]);
			}

			else if (r[0] && !r[1] && r[2]) // Positions & normals
			{
				string	v1, v3;
				unsigned int v_id[3], nor_id[3];

				int	m = fscanf(f, "%d//%d %d//%d %d//%d\n", &v_id[0], &nor_id[0], &v_id[1], &nor_id[1], &v_id[2], &nor_id[2]);
				if (m != 6){	printf ("%s can't be read.\n", path); return false;   }

				p_n.push_back(v_id[0]); p_n.push_back(v_id[1]); p_n.push_back(v_id[2]);
				n_n.push_back(nor_id[0]); n_n.push_back(nor_id[1]); n_n.push_back(nor_id[2]);
			}

			else if (r[0] && !r[1] && !r[2]) // Positions
			{
				string	v1;
				unsigned int v_id[3];

				int	m = fscanf(f, "%d %d %d\n", &v_id[0], &v_id[1], &v_id[2]);
				if (m != 3){	printf ("%s can't be read.\n", path); return false;   }

				p_n.push_back(v_id[0]); p_n.push_back(v_id[1]); p_n.push_back(v_id[2]);
			}
		}
	}
	fclose(f);

	// Fill Vertices informations
	if (r[0] && r[1] && r[2]) // Positions, uvs & normals
	{
		for (int i = 0; i < p_n.size(); i++)
		{
			int posIndex = p_n[i], uvIndex = uv_n[i], norIndex = n_n[i];

			bVector3 _pos = tempPos[posIndex - 1], _nor = tempNor[norIndex - 1];
			bVector2 _uv = tempUvs[uvIndex - 1];

			m->ver.push_back(_pos); m->uvs.push_back(_uv); m->nor.push_back(_nor);
		}
	}

	else if (r[0] && r[1] && !r[2]) // Positions & uvs
	{
		for (int i = 0; i < p_n.size(); i++)
		{
			int posIndex = p_n[i], uvIndex = uv_n[i];

			bVector3 _pos = tempPos[posIndex - 1];
			bVector2 _uv = tempUvs[uvIndex - 1];

			m->ver.push_back(_pos); m->uvs.push_back(_uv);;
		}
	}

	else if (r[0] && !r[1] && r[2]) // Positions & normals
	{
		for (int i = 0; i < p_n.size(); i++)
		{
			int posIndex = p_n[i], norIndex = n_n[i];
			bVector3 _pos = tempPos[posIndex - 1], _nor = tempNor[norIndex - 1];
			m->ver.push_back(_pos); m->nor.push_back(_nor);
		}
	}

	else if (r[0] && !r[1] && !r[2]) // Positions
	{
		for (int i = 0; i < p_n.size(); i++)
		{
			int posIndex = p_n[i];
			bVector3 _pos = tempPos[posIndex - 1];
			m->ver.push_back(_pos);
		}
	}

	return true;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool	LoadOBJMaterial( Material* mtl )
{
	FILE*		f = fopen (mtl->GetGuid(), "r");

	if (!f)
	{
		printf ("Fail to open %s\n", mtl->GetGuid());
		return false;
	}

	while (1)
	{
		char	line[128];
		int		res = fscanf(f, "%s", line);

		if (res == EOF) break;
		
		if (strcmp(line, "newmtl") == 0) 
		{
			char	mtl_id[128];
			fscanf(f, "%s", mtl_id);
			mtl->SetGuid(mtl_id);
		}
		else if	(strcmp(line, "Ns") == 0) fscanf(f, "%f", &mtl->Ns);
		else if (strcmp(line, "Ka") == 0) fscanf(f, "%f %f %f", &mtl->Ka.x, &mtl->Ka.y, &mtl->Ka.z);
		else if (strcmp(line, "Kd") == 0) fscanf(f, "%f %f %f", &mtl->Kd.x, &mtl->Kd.y, &mtl->Kd.z);
		else if (strcmp(line, "Ks") == 0) fscanf(f, "%f %f %f", &mtl->Ks.x, &mtl->Ks.y, &mtl->Ks.z);
	}
	fclose(f);

	return true;
}
