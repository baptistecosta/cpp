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
		int				j[MAX_BONES_PER_VERT];
		
		SkinningJoints(Vertex::Weights weights[MAX_BONES_PER_VERT])
		{
			for (uint i = 0; i < MAX_BONES_PER_VERT; i++)
				j[i] = weights[i].jid;
		}
};

//!
struct	SkinningWeights
{
		float			w[MAX_BONES_PER_VERT];
		
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

		uint					m_buffer[NofBuffers];
		RenderingVertices		m_vertices;

public:

		RenderingGeometry(const String& guid);
		~RenderingGeometry();

		uint&					getBuffer(uint index)		{	return m_buffer[index];	}
		RenderingVertices&		getVertices()				{	return m_vertices;	}
const	RenderingVertices&		getVertices() const			{	return m_vertices;	}
};

//!
struct RenderingGeometriesLib
{
static	void					load(GLRenderer& renderer);
};

}		// owl
#endif	// __RENDERING_GEOMETRY__
