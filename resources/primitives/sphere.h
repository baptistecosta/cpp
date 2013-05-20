/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __SPHERE__
#define __SPHERE__

	#include "..\geometry.h"

namespace owl {

//!	@class	Sphere
class	Sphere	:	public	Geometry
{
		float			radius;
		
public:

		void			create(const Vector3& position, float radius, uint rings, uint sectors);

inline	float&			getRadius()												{	return	radius;   }
};

}		// owl
#endif	// __SPHERE__

