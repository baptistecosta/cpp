/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

	#include "glew.h"
	#include "core/log.h"
	#include "graphics_wrappers/opengl.h"
	#include "object.h"
	#include "picture.h"
	#include "font.h"
	#include "text2d.h"

	using namespace owl;

//---------------------------------------------------------------------------
Text2D::Text2D(const String& guid, Font* font)
	: Resource(guid)
	, m_font(font)
{
	buffer[0] = buffer[1] = 0;
}

//---------------------------------------------------------------------------
Text2D::~Text2D()
{
	glDeleteBuffers(1, &buffer[0]);
	glDeleteBuffers(1, &buffer[1]);
}

//---------------------------------------------------------------------------
Text2D*			Text2D::UpdateTextBox(const Vector2& screen_size, const String& text, const Vector2& position)
{
	Vector2 gl_space_cursor(PixelCoordToOpenGLSpace(screen_size, position));
//	Log::i("Pixel coord in OpenGL space: x = %f, y = %f", gl_space_cursor.x, gl_space_cursor.y);
	float cur_x = 0;
	
	ClearVertexData();
	const int size = text.Size();
	for(int i = 0; i < size; ++i)
	{
		uint id = text[i];
		Vector2 c_size_ws ((m_font->chars[id].size.x * 2) / screen_size.x, (m_font->chars[id].size.y * 2) / screen_size.y);
				 
		Vector2 v_ul(m_font->chars[id].offset.x + cur_x								, m_font->line_h - m_font->chars[id].offset.y - m_font->chars[id].size.y);
		Vector2 v_ur(m_font->chars[id].offset.x + cur_x + m_font->chars[id].size.x	, m_font->line_h - m_font->chars[id].offset.y - m_font->chars[id].size.y);
		Vector2 v_dr(m_font->chars[id].offset.x + cur_x + m_font->chars[id].size.x	, m_font->line_h - m_font->chars[id].offset.y);
		Vector2 v_dl(m_font->chars[id].offset.x + cur_x								, m_font->line_h - m_font->chars[id].offset.y);

		v_ul = PixelToOpenGLSpace(screen_size, v_ul); v_ul.x += gl_space_cursor.x; v_ul.y += gl_space_cursor.y;
		v_ur = PixelToOpenGLSpace(screen_size, v_ur); v_ur.x += gl_space_cursor.x; v_ur.y += gl_space_cursor.y;
		v_dr = PixelToOpenGLSpace(screen_size, v_dr); v_dr.x += gl_space_cursor.x; v_dr.y += gl_space_cursor.y;
		v_dl = PixelToOpenGLSpace(screen_size, v_dl); v_dl.x += gl_space_cursor.x; v_dl.y += gl_space_cursor.y;

// 		Log::horizontalLine();
// 		Log::i("VUL Pixel coord in OpenGL space: x = %f, y = %f", v_ul.x, v_ul.y);
// 		Log::i("VUR Pixel coord in OpenGL space: x = %f, y = %f", v_ur.x, v_ur.y);
// 		Log::i("VDR Pixel coord in OpenGL space: x = %f, y = %f", v_dr.x, v_dr.y);
// 		Log::i("VDL Pixel coord in OpenGL space: x = %f, y = %f", v_dl.x, v_dl.y);

		pos.push_back(v_ul); pos.push_back(v_ur); pos.push_back(v_dl);
		pos.push_back(v_ur); pos.push_back(v_dr); pos.push_back(v_dl);

		Vector2 uv_ul(m_font->chars[id].pos.x / m_font->scale_w									, 1 - ((m_font->chars[id].pos.y + m_font->chars[id].size.y) / m_font->scale_h));
		Vector2 uv_ur((m_font->chars[id].pos.x + m_font->chars[id].size.x) / m_font->scale_w	, 1 - ((m_font->chars[id].pos.y + m_font->chars[id].size.y) / m_font->scale_h));
		Vector2 uv_dr((m_font->chars[id].pos.x + m_font->chars[id].size.x) / m_font->scale_w	, 1 - (m_font->chars[id].pos.y / m_font->scale_h));
		Vector2 uv_dl(m_font->chars[id].pos.x / m_font->scale_w									, 1 - (m_font->chars[id].pos.y / m_font->scale_h));

		uv.push_back(uv_ul); uv.push_back(uv_ur); uv.push_back(uv_dl);
		uv.push_back(uv_ur); uv.push_back(uv_dr); uv.push_back(uv_dl);

		cur_x += m_font->chars[id].x_advance;
	}
	return this;
}

//---------------------------------------------------------------------------
void			Text2D::ClearVertexData()
{
	pos.clear();
	uv.clear();
}

//---------------------------------------------------------------------------
const Vector2	Text2D::PixelCoordToOpenGLSpace(const Vector2& screen_size, const Vector2& v)
{	return Vector2((v.x / (screen_size.x / 2)) - 1, (v.y / (screen_size.y / 2)) - 1);	}

//---------------------------------------------------------------------------
const Vector2	Text2D::PixelToOpenGLSpace(const Vector2& screen_size, const Vector2& v)
{	return Vector2((v.x * 2) / screen_size.x, (v.y * 2) / screen_size.y);	}

//---------------------------------------------------------------------------
void			Text2D::Bind()
{
	OpenGL::CreateGLBuffer((void*)&pos[0], sizeof(Vector2) * pos.size(), buffer[0], GL_STATIC_DRAW);
	OpenGL::CreateGLBuffer((void*)&uv[0], sizeof(Vector2) * uv.size(), buffer[1], GL_STATIC_DRAW);
}
