/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

	#include "sphere.h"

using namespace owl;

//-----------------------------------------------------------------------------
void			Sphere::create(const Vector3& position, float _radius, uint rings, uint sectors)
{
/*
	radius = _radius;
	
	vector<float>	sinQ, cosQ;
	vector<float>	sinP, cosP;
	
	float	Q = 2.f * Pi / sectors;
	float	P = Pi / rings;

	// Theta & Phi cache
	for (uint i = 0; i < sectors; i++)		
	{	
		sinQ.push_back(sin(Q * i));
		cosQ.push_back(cos(Q * i));
	}

	for (uint j = 0; j < rings; j++)	
	{	
		sinP.push_back(sin(P * j));
		cosP.push_back(cos(P * j));   
	}

	// Positive Z pole
	raw.pos.push_back(bVector3(0.f, 0.f, radius));
	raw.nor.push_back(bVector3(0.f, 0.f, 1.f));

	// Stacks
	for (uint j = 1; j < rings; j++)
		for (uint i = 0; i < sectors; i++)
		{
			float nx = sinQ[i] * sinP[j];
			float ny = cosQ[i] * sinP[j];
			float nz = cosP[j];
			
			float x = radius * nx;
			float y = radius * ny;
			float z = radius * nz;

			raw.pos.push_back(bVector3(x, y, z));
			raw.nor.push_back(bVector3(nx, ny, nz));

		}

	// Negative Z pole
	raw.pos.push_back(bVector3(0.f, 0.f, -radius));
	raw.nor.push_back(bVector3(0.f, 0.f, -1.f));

	// Positive Z pole
	uint	ringA = 0, ringB = 1, i = 0;

	for(i = 0; i < sectors - 1; i++)
	{
		raw.indices.push_back(ringA);
		raw.indices.push_back(ringB + i + 1);
		raw.indices.push_back(ringB + i);
	}

	raw.indices.push_back(ringA);
	raw.indices.push_back(ringB);
	raw.indices.push_back(ringB + i);

	// Interior stacks
	for (uint j = 1; j < rings - 1; j++)
	{
		ringA = 1 + (j - 1) * sectors;
		ringB = ringA + sectors;

		for (i = 0; i < sectors - 1; i++)
		{
			raw.indices.push_back(ringA + i);
			raw.indices.push_back(ringA + i + 1);
			raw.indices.push_back(ringB + i);

			raw.indices.push_back(ringA + i + 1);
			raw.indices.push_back(ringB + i + 1);
			raw.indices.push_back(ringB + i);
		}
		// Last triangle of the ring
		raw.indices.push_back(ringA + i);
		raw.indices.push_back(ringA);
		raw.indices.push_back(ringB + i);

		raw.indices.push_back(ringA);
		raw.indices.push_back(ringB);
		raw.indices.push_back(ringB + i);
	}

	// Negative Z pole
	ringA = 1 + (rings - 2) * sectors; ringB = ringA + sectors;

	for (i = 0; i < sectors - 1; i++)
	{
		raw.indices.push_back(ringA + i);
		raw.indices.push_back(ringA + i + 1);
		raw.indices.push_back(ringB);
	}

	raw.indices.push_back(ringA + i);
	raw.indices.push_back(ringA);
	raw.indices.push_back(ringB);
*/
}

