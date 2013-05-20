/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __FONT__
#define __FONT__

	#include "core/containers/shared_pointer.h"
	#include "core/math/vector.h"
	#include "typedefs.h"
	#include "picture.h"
	#include "resource.h"
	
namespace owl {

//!
struct	CharInfo 
{
		Vector2				pos, size, offset;
		float				x_advance;
		int					page;

		CharInfo()
			:	pos(Vector2::origin)
			,	x_advance(0)
			,	page(0)
		{
			//
		}
};

//!
class	Font:	public	Resource
{
public:

		int					m_page_id;
		SharedPtr<Picture>	m_texture;

		CharInfo			chars[256];
		int					line_h,	base, pages, count, scale_w, scale_h;

		Font(const String& guid)
			:	Resource(guid)
			,	m_page_id(0)
			,	line_h(0)
			,	base(0)
			,	pages(0)
			,	count(0)
			,	scale_w(0)
			,	scale_h(0)
		{
			//
		}
};

}		//	owl
#endif	//	__FONT__