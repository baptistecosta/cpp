/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef _LINE_
#define _LINE_

	#include <vector>
	#include "core\math\vector.h"

namespace owl {

class	RenderingGeometry;

//!	@class	Line
class	Line
{
		std::vector<Vector3> l, c;

public:

		void			init(RenderingGeometry&);
};

}		// owl
#endif	// _LINE_