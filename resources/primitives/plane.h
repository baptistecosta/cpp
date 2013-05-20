/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __PLANE__
#define __PLANE__

	#include "resources\geometry.h"

namespace owl {

//!	@struct	Plane
struct	Plane	:	public Geometry
{
		void			create(float size, uint rows, uint columns);
};

}		// owl
#endif	// __PLANE__