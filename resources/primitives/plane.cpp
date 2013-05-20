/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

	#include "plane.h"

	using namespace owl;

//-----------------------------------------------------------------------------
void			Plane::create(float size, uint rows, uint columns)
{
/*
	vector<float>	csection, rsection;		// Rows & columns sections cache arrays
	float			o = -size / 2;			// where we will begin to generate vertices

	// Store size of rows & columns sections
	for (uint n = 0; n <= columns; n++)	{	float	c = n * size / columns; csection.push_back(c);   }
	for (uint n = 0; n <= rows; n++)		{	float	r = n * size / rows; rsection.push_back(r);   }

	// Generate vertices position
	for (uint j = 0; j <= rows; j++)
	{
		float z = o + rsection[j];
		float v = (float)j * 1.f / rows;

		for (uint i = 0; i <= columns; i++)
		{
			float x = o + csection[i];
			float u = (float)i * 1.f / columns;

			raw.pos.push_back(bVector3(x, 0, z));
			raw.uv.push_back(bVector2(u, v));
			raw.nor.push_back(bVector3(0, 1, 0));
		}
	}

	// Triangles indexation
	for (uint j = 0; j < rows; j++)
		for (uint i = 0; i < columns; i++)
		{
			raw.indices.push_back(j * (columns + 1) + i);
			raw.indices.push_back(j * (columns + 1) + (i + 1));
			raw.indices.push_back((j + 1) * (columns + 1) + (i + 1));

			raw.indices.push_back(j * (columns + 1) + i);
			raw.indices.push_back((j + 1) * (columns + 1) + (i + 1));
			raw.indices.push_back((j + 1) * (columns + 1) + i);
		}
*/
}
