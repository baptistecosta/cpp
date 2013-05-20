/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

	#include <Windows.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include "glew.h"
//	#include "platform/gl_ext.h"
	#include "glfw.h"
	
	#include "opengl.h"
	#include "rendering_geometry.h"
	#include "core/log.h"
	#include "platform/defines.h"

	#include "resources/picture.h"
	#include "resources/text2d.h"

	using namespace owl;

//---------------------------------------------------------------------------
void gllogout(GLenum source, GLenum type, uint id, GLenum severity, GLsizei length, const char* message, void* userParam)
{
	Log::i(message);
}

//---------------------------------------------------------------------------
void			OpenGL::Init()
{
	InitGlfw();
	InitGlew();

//	glDebugMessageCallback((GLDEBUGPROC)&gllogout, NULL);

	glfwSetWindowTitle("Tutorial 01");
	glfwEnable(GLFW_STICKY_KEYS);

	__GL_CALL(glClearColor (0.3f, 0.3f, 0.3f, 0.f))
	__GL_CALL(glEnable(GL_DEPTH_TEST))
	__GL_CALL(glDepthFunc(GL_LESS))

	__GL_CALL(glDisable(GL_CULL_FACE))
	__GL_CALL(glCullFace(GL_BACK))
	__GL_CALL(glFrontFace(GL_CW))

	__GL_CALL(glEnable(GL_BLEND))
	__GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA))
}

//---------------------------------------------------------------------------
void			OpenGL::InitGlfw()
{
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		::exit(1);
	}

	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 0);
//	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (!glfwOpenWindow(ScreenResolution_W, ScreenResolution_H, 0, 0, 0, 0, 32, 0, GLFW_WINDOW))
	{
		fprintf(stderr, "Failed to open GLFW window\n");
		glfwTerminate();
		DebugBreak();
	}
}

//---------------------------------------------------------------------------
void			OpenGL::InitGlew()
{
//	if (!GLExt::Init())
//		DebugBreak();

	const char* gl_version = (const char*)glGetString(GL_VERSION);
	Log::i(gl_version);
	
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		Log::e("%s\n", glewGetErrorString(err));
		DebugBreak();
	}
}

//---------------------------------------------------------------------------
void			OpenGL::ClearScreen()
{	__GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT))	}

//---------------------------------------------------------------------------
void			OpenGL::SwapBuffer()
{	glfwSwapBuffers();  }

//---------------------------------------------------------------------------
bool			OpenGL::Exit()
{	return glfwGetKey(GLFW_KEY_ESC ) != GLFW_PRESS && glfwGetWindowParam (GLFW_OPENED);	}

//---------------------------------------------------------------------------
void			OpenGL::CleanUp()
{	glfwTerminate();   }

//-----------------------------------------------------------------------------
void			OpenGL::Bind3DGeometry(RenderingGeometry* rdr_geom)
{
	if (rdr_geom == null_ptr)
		return;

	const RenderingVertices& vert = rdr_geom->getVertices();
	if (!vert.pos.empty()) CreateGLBuffer((void*)&vert.pos[0], sizeof(Vector3) * vert.pos.size(), rdr_geom->getBuffer(0), GL_STATIC_DRAW);
	if (!vert.uv.empty()) CreateGLBuffer((void*)&vert.uv[0], sizeof(Vector2) * vert.pos.size(), rdr_geom->getBuffer(1), GL_STATIC_DRAW);
	if (!vert.nor.empty()) CreateGLBuffer((void*)&vert.nor[0], sizeof(Vector3) * vert.pos.size(), rdr_geom->getBuffer(2), GL_STATIC_DRAW);
	if (!vert.tang.empty()) CreateGLBuffer((void*)&vert.tang[0], sizeof(Vector3) * vert.pos.size(), rdr_geom->getBuffer(3), GL_STATIC_DRAW);
	if (!vert.btan.empty()) CreateGLBuffer((void*)&vert.btan[0], sizeof(Vector3) * vert.pos.size(), rdr_geom->getBuffer(4), GL_STATIC_DRAW);
	if (!vert.joints.empty()) CreateGLBuffer((void*)&vert.joints[0], sizeof(SkinningJoints) * vert.pos.size(), rdr_geom->getBuffer(5), GL_STATIC_DRAW);
	if (!vert.weights.empty()) CreateGLBuffer((void*)&vert.weights[0], sizeof(SkinningWeights) * vert.pos.size(), rdr_geom->getBuffer(6), GL_STATIC_DRAW);
	if (!vert.indices.empty()) CreateGLBuffer((void*)&vert.indices[0], sizeof(unsigned short) * vert.indices.size(), rdr_geom->getBuffer(7), GL_STATIC_DRAW);
}

//-----------------------------------------------------------------------------
void			OpenGL::Bind2DTextGeometry(Text2D& rdr_txt_geom)
{
	CreateGLBuffer((void*)&rdr_txt_geom.getPos()[0], sizeof(Vector2) * rdr_txt_geom.getPos().size(), rdr_txt_geom.getBuffer(0), GL_STATIC_DRAW);
	CreateGLBuffer((void*)&rdr_txt_geom.getUv()[0], sizeof(Vector2) * rdr_txt_geom.getPos().size(), rdr_txt_geom.getBuffer(1), GL_STATIC_DRAW);
}

//-----------------------------------------------------------------------------
void			OpenGL::CreateGLBuffer(void* data, size_t size, GLuint& buffer, GLenum usage)
{
	// void glGenBuffers(GLsizei n, GLuint* buffers);
	// Returns n buffer object names in buffers.
	__GL_CALL(glGenBuffers(1, &buffer))

	// void glBindBuffer(GLenum target, GLuint buffer);
	// Binds a buffer object to the specified buffer binding point.
	// Calling glBindBuffer with target set to one of the accepted symbolic constants and buffer set to the name
	// of a buffer object binds that buffer object name to the target.
	// If no buffer object with name buffer exists, one is created with that name.
	// When a buffer object is bound to a target, the previous binding for that target is automatically broken.
	__GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, buffer))

	// void glBufferData(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage);
	// Creates a new data store for the buffer object currently bound to "target".
	// Any pre-existing data store is deleted. The new data store is created with the specified size in bytes and usage.
	// If data is not NULL, the data store is initialized with data from this pointer.
	// In its initial state, the new data store is not mapped, it has a NULL mapped pointer, and its mapped access is GL_READ_WRITE.
	__GL_CALL(glBufferData(GL_ARRAY_BUFFER, size, data, usage))
}

//-----------------------------------------------------------------------------
void			OpenGL::GenerateTexture(Picture& p)
{
	// Retourne l'identifiant de l'objet texture créé par le driver opengl
	// (contient x, y, format, un pointeur null qui servira à stocker l'adresse des pixels de la texture en VRAM(RAM de la carte graphique))
	uint texture_name = 0;
	__GL_CALL(glGenTextures(1, &texture_name))
	p.setGLTextureName(texture_name);
}

//-----------------------------------------------------------------------------
void			OpenGL::BindTexture(uint gl_texture_unit, const uint& GLuint_named_texture)
{
	// Définie l'unité de texture "gl_texture_unit" comme courante. Voir GL_TEXTURE0.
	__GL_CALL(glActiveTexture(gl_texture_unit))
	// Lie la nouvelle texture via son identifiant à l'unité de texture courante.
	__GL_CALL(glBindTexture(GL_TEXTURE_2D, GLuint_named_texture))
}

//-----------------------------------------------------------------------------
void			OpenGL::CheckGLError()
{
#ifndef _DEBUG
	return;
#endif

	GLenum error = glGetError();
	switch (error)
	{
	case GL_NO_ERROR:
//		Log::i("No error has been recorded. The value of this symbolic constant is guaranteed to be 0.");
		break;
	case GL_INVALID_ENUM:
		Log::e("An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.");
		break;
	case GL_INVALID_VALUE:
		Log::e("A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.");
		break;
	case GL_INVALID_OPERATION:
		Log::e("The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.");
		break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		Log::e("The framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag.");
		break;
	case GL_OUT_OF_MEMORY:
		Log::e("There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.");
		break;
	case GL_STACK_UNDERFLOW:
		Log::e("An attempt has been made to perform an operation that would cause an internal stack to underflow.");
		break;
	case GL_STACK_OVERFLOW:
		Log::e("An attempt has been made to perform an operation that would cause an internal stack to overflow.");
		break;
	default:
		break;
	}

	if	(error != GL_NO_ERROR)
		DebugBreak();
}
