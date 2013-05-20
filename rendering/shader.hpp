/*
	bEngine - 2011
	Baptiste Costa
*/

#ifndef	__SHADER__
#define	__SHADER__


	#include	"glew.h"
	#include	"core/string.h"
	
using	namespace	owl;

//!
struct	Shader
{
		String			vertex,
						fragment;
};

GLuint				LoadShaders(const String& vertex_file_path, const String& fragment_file_path);

#endif //__SHADER__
