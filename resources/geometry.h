/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __GEOMETRY__
#define __GEOMETRY__

	#include "resource.h"
	#include "core/math/vector.h"
	#include "core/containers/auto_array.h"
	#include "core/containers/vector.h"
	#include "typedefs.h"

namespace owl {

#define	MAX_BONES_PER_VERT	4

//!
struct	Vertex
{
		struct	Weights
		{
			short			jid;
			float			w;

			Weights			() : jid(0), w(0) {}
		};

		Vector3				pos, nor, tang, btan;
		Vector2				uv;

		Weights				weights[MAX_BONES_PER_VERT];

		Vertex(const Vector3& _pos, const Vector3& _nor, const Vector2& _uv);
};

//!
class	Geometry:	public Resource
{
public:

		AutoVector<Vertex>::type vertices;
		bool				is_skinned;
	
		Geometry			(String& _guid);
	
		void				ComputeTangentBasis();
};

//!
struct	GeometriesLib
{
static	void				load();
};

}		//	owl
#endif	//	__GEOMETRY__