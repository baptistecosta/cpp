/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __OBJECT__
#define __OBJECT__

	#include "picture.h"
	#include "core/math/vector.h"
	#include "core/math/matrix4.h"

namespace owl {

class	Mesh;

//!
class	Object	:	public	Resource
{
		Mesh*			mesh;
		Vector3			t, r, s,
						l_t;

		void			Init();

public:

		explicit Object	(
							Mesh*,
							const Vector3& pos = Vector3::origin,
							const Vector3& q = Vector3::origin,
							const Vector3& sc = Vector3::identity
						);

		Mesh*			GetMesh()												{	return mesh;	}
		Vector3&		GetPosition()											{	return t;	}
		Vector3&		GetLastPosition()										{	return l_t;	}
		Matrix4			GetMatrixTranslation()									{	return Matrix4::Translation(t);	}
		Matrix4			GetMatrixTransformation()								{	return Matrix4::Transform(t, r, s);	}
		
		void			Offset(const Vector3& o)								{	t += o;	}
		void			SetPosition(Vector3& p)									{	t = p;	}
		void			SetRotation(Vector3& q)									{	r = q;	}
		void			SetScale(Vector3& sc)									{	s = sc;	}
		void			SetLastPos(Vector3& l_p)								{	l_t = l_p;	}
		void			SetMesh(Mesh* m)										{	mesh = m;	}
};

}		// owl
#endif	// __OBJECT__