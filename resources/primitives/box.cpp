/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

	#include "box.h"

using namespace owl;

//-----------------------------------------------------------------------------
void			Box::create(Vector3& size, Vector3& subd)
{
/*
	std::vector<float> subdX, subdY, subdZ;				// Subdivisions
	Vector3 o(-size.x / 2, -size.y / 2, -size.z / 2);	// where we will begin to generate vertices

	// Store size of rows & columns sections
	for (uint n = 0; n <= subd.x; n++)	{	float	x = n * size.x / subd.x; subdX.push_back(x);   }
	for (uint n = 0; n <= subd.y; n++)	{	float	y = n * size.y / subd.y; subdY.push_back(y);   }
	for (uint n = 0; n <= subd.z; n++)	{	float	z = n * size.z / subd.z; subdZ.push_back(z);   }

	// Generate vertices position
	// Down face
	float x = 0.f, z = 0.f, y = o.y + subdY[0];
	
	for (uint j = 0; j <= subd.z; j++)
	{
		z = o.z + subdZ[j];
	
		for (uint i = 0; i <= subd.x; i++)
		{
			x = o.x + subdX[i];
			raw.pos.push_back(Vector3(x, y, z));
		}
	}

	// At each Y subdivisions starting 1 & finishing n - 1
	for (uint k = 1; k < subd.y; k++)
	{
		// Back vertices line
		y = o.y + subdY[k];
		z = o.z + subdZ[0];

		for (uint i = 0; i <= subd.x; i++)
		{
			x = o.x + subdX[i];
			raw.pos.push_back(Vector3(x, y, z));
		}

		// Side vertices
		x = o.x + subdX[subd.x];

		for (uint j = 1; j < subd.z; j++)
		{
			z = o.z + subdZ[j];

			x = o.x + subdX[0];
			raw.pos.push_back(Vector3(x, y, z));
			x = o.x + subdX[subd.x];
			raw.pos.push_back(Vector3(x, y, z));
		}

		// Front vertices line
		y = o.y + subdY[k];
		z = o.z + subdZ[subd.z];

		for (uint i = 0; i <= subd.x; i++)
		{
			x = o.x + subdX[i];
			raw.pos.push_back(Vector3(x, y, z));
		}
	}

	// Up face
	y = o.y + subdY[subd.y];

	for (uint j = 0; j <= subd.z; j++)
	{
		float z = o.z + subdZ[j];

		for (uint i = 0; i <= subd.x; i++)
		{
			float x = o.x + subdX[i];
			raw.pos.push_back(Vector3(x, y, z));
		}
	}
	
	// Triangles indexation
	// South face
	for (uint j = 0; j < subd.z; j++)
		for (uint i = 0; i < subd.x; i++)
		{
			raw.indices.push_back(j * (subd.x + 1) + i);
			raw.indices.push_back(j * (subd.x + 1) + (i + 1));
			raw.indices.push_back((j + 1) * (subd.x + 1) + i);

			raw.indices.push_back(j * (subd.x + 1) + (i + 1));
			raw.indices.push_back((j + 1) * (subd.x + 1) + (i + 1));
			raw.indices.push_back((j + 1) * (subd.x + 1) + i);
		}
*/
}

