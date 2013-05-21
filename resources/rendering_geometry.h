/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __RENDERING_GEOMETRY__
#define __RENDERING_GEOMETRY__

	#include <string>
	#include <vector>
	#include "typedefs.h"
	#include "resources/geometry.h"

	using namespace std;

namespace owl {

class	Geometry;
class	GLRenderer;
struct	Vertex;

//!
struct	SkinningJoints
{
		int						j[MAX_BONES_PER_VERT];
		
		SkinningJoints(Vertex::Weights weights[MAX_BONES_PER_VERT])
		{
			for (uint i = 0; i < MAX_BONES_PER_VERT; i++)
				j[i] = weights[i].jid;
		}
};

//!
struct	SkinningWeights
{
		float					w[MAX_BONES_PER_VERT];
		
		SkinningWeights(Vertex::Weights weights[MAX_BONES_PER_VERT])
		{
			for (uint i = 0; i < MAX_BONES_PER_VERT; i++)
				w[i] = weights[i].w;
		}
};

//!
struct RenderingVertices
{
		vector<Vector3>			pos,
								nor,
								tang,
								btan;
		vector<Vector2>			uv;
		vector<SkinningJoints>	joints;
		vector<SkinningWeights>	weights;
		
		vector<ushort>			indices;
};

//!
class	RenderingGeometry	:	public	Resource
{
protected:
		
		enum
		{	NofBuffers		=	8	};

		uint					buffer[NofBuffers];
		RenderingVertices		vertices;

public:

		RenderingGeometry(const String& guid);
		~RenderingGeometry();

		void					Bind();

		uint&					GetBuffer(uint index)		{	return buffer[index];	}
		RenderingVertices&		GetVertices()				{	return vertices;	}
		const RenderingVertices& GetVertices() const		{	return vertices;	}
};

//!
struct RenderingGeometriesLib
{
static	void					Load(GLRenderer& renderer);
};

}		// owl
#endif	// __RENDERING_GEOMETRY__
