/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __FONT_LOADER__
#define __FONT_LOADER__

	#include "core\string.h"

namespace owl {

class	Font;

//!
struct	FontLoader
{
static	Font			*load(const String &path);
};

}		//	owl
#endif	//	__FONT_LOADER__