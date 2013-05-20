/*
	bEngine ::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __PICTURE__
#define __PICTURE__

	#include "resource.h"

namespace owl {

//!
class	Picture	:	public Resource
{
		uint			gl_text_name;
		
public:

		Picture(const String& guid);

		const uint&		GetGLTextureName() const				{	return gl_text_name;	}
		void			SetGLTextureName(uint texture_name)		{	gl_text_name = texture_name;	}
};

//!
struct	PicturesLib 
{
static	void			load();
};

}		// owl
#endif	//__PICTURE__