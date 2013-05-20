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

		Text2D*				updateTextBox(const Vector2& screen_size, const String& text, const Vector2& cursor);

private:

		void				clearVertexData()									{	pos.clear(); uv.clear();   }
		const Vector2		pixelCoordToOpenGLSpace(const Vector2& screen_size, const Vector2& v);
		const Vector2		pixelToOpenGLSpace(const Vector2& screen_size, const Vector2& v);

public:

		const vector<Vector2> getPos() const									{	return pos;	}
		const vector<Vector2> getUv() const										{	return uv;	}
		uint&				getBuffer(uint index)								{	return buffer[index];   }
		const uint&			getBuffer(uint index) const							{	return buffer[index];   }
		const uint			getSize() const 									{	return pos.size();   }
		Font*				getFont()											{	return m_font.cPtr();	}

		void				setFont(Font* font)									{	m_font = font;	}
};

}		// owl
#endif	// __TEXT_2D__
