/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __TEXT_2D__
#define __TEXT_2D__

	#include <vector>
	#include "core/containers/shared_pointer.h"
	#include "font.h"
	#include "picture.h"
	#include "resource.h"
	#include "typedefs.h"

	using namespace std;

namespace owl {

//!
class	Text2D	:	public	Resource
{
		vector<Vector2>		pos, uv;
		uint				buffer[2];

		SharedPtr<Font>		m_font;

public:
		
		Text2D(const String& guid, Font* font);
		~Text2D();

		Text2D*				UpdateTextBox(const Vector2& screen_size, const String& text, const Vector2& cursor);

private:

		void				ClearVertexData();
		const Vector2		PixelCoordToOpenGLSpace(const Vector2& screen_size, const Vector2& v);
		const Vector2		PixelToOpenGLSpace(const Vector2& screen_size, const Vector2& v);

public:

		void				Bind();

		const vector<Vector2> GetPos() const				{	return pos;	}
		const vector<Vector2> GetUv() const					{	return uv;	}
		uint&				GetBuffer(uint index)			{	return buffer[index];   }
		const uint&			GetBuffer(uint index) const		{	return buffer[index];   }
		const uint			GetSize() const 				{	return pos.size();   }
		Font*				GetFont()						{	return m_font.Raw();	}

		void				SetFont(Font* font)				{	m_font = font;	}
};

}		// owl
#endif	// __TEXT_2D__
