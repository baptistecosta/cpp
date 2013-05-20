/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __BOX__
#define __BOX__

	#include "..\geometry.h"

namespace owl {

//!	@struct	Box
struct	Box	:	public	Geometry
{
		void			create(Vector3& size, Vector3& subd);
};

}		// owl
#endif	// __BOX__