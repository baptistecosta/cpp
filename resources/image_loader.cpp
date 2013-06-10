/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#define STBI_HEADER_FILE_ONLY		//	Images loader library hack.

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "core/log.h"
	#include "externs/opengl/inc/glew.h"
	#include "externs/opengl/inc/glfw.h"
	#include "externs/stb_image/stb_image.c"
	#include "graphics/opengl.h"
	#include "image_loader.h"
	#include "picture.h"
	#include "resource_manager.h"
	
	using	namespace owl;

//-----------------------------------------------------------------------------
Picture*		ImageLoader::LoadBMP(const String& path)
{
	if (Resources::pictures.find(path))
		return	Resources::pictures.getResource(path);

	Picture* p = new Picture(path);

	// Data read from the header of the BMP file
	uchar	header[54];		// Each BMP file begins by a 54-bytes header
	uint	data_pos;		// Position in the file where the actual data begins
	uint	width, height;
	uint	image_size;		// width * height * 3

	// Actual RGB data
	uchar*	data;

	FILE* f = fopen(path.cStr(), "rb");

	if (!f)
		printf("Image could not be opened.\n");

	if (fread(header, 1, 54, f) != 54)
		printf("Not a correct BMP file.\n");

	if (header[0] != 'B' || header[1] != 'M')
		printf("Not a correct BMP file.\n");

	data_pos	= *(int*)&(header[0x0A]);
	image_size	= *(int*)&(header[0x22]);
	width		= *(int*)&(header[0x12]);
	height		= *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (image_size == 0)	image_size = width * height * 3;	// 3 : one byte for each R, G and B component
	if (data_pos == 0)		data_pos = 54;

	// Create a buffer
	data = new uchar [image_size];
	// Read the actual data from the file into the buffer
	fread(data, 1, image_size, f);
	fclose(f);

	// Create an opengl texture
	uint texture_name = 0;
	__GL_CALL(glGenTextures(1, &texture_name))
	p->SetGLTextureName(texture_name);

	__GL_CALL(glBindTexture(GL_TEXTURE_2D, p->GetGLTextureName()))
	__GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data))
	__GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR))
	__GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR))
	glGenerateMipmap(GL_TEXTURE_2D);
	OpenGL::CheckGLError();
	
	Resources::pictures.registerRes(p);
	return p;
}

//-----------------------------------------------------------------------------
Picture*		ImageLoader::loadTGA(const String& path)
{
	if (Resources::pictures.find(path))
		return	Resources::pictures.getResource(path);

	Picture* p = new Picture(path);

	uint texture_name = 0;
	__GL_CALL(glGenTextures(1, &texture_name))
	p->SetGLTextureName(texture_name);

	__GL_CALL(glBindTexture(GL_TEXTURE_2D, p->GetGLTextureName()))
	glfwLoadTexture2D(path.cStr(), 0);
		
	__GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT))
	__GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT))
	__GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR))
	__GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR))
	glGenerateMipmap(GL_TEXTURE_2D);
	OpenGL::CheckGLError();

	Resources::pictures.registerRes(p);
	return p;
}

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII
//-----------------------------------------------------------------------------
Picture*		ImageLoader::LoadDDS(const String& path)
{
	if (Resources::pictures.find(path))
		return	Resources::pictures.getResource(path);

	Picture* p = new Picture(path);

	uchar header[124];

	FILE *fp; 
	fp = fopen(path.cStr(), "rb"); 
	if (fp == NULL)
		exit(-1); 

	/* verify the type of file */ 
	char filecode[4]; 
	fread(filecode, 1, 4, fp); 
	if (strncmp(filecode, "DDS ", 4) != 0)	{	fclose(fp); exit(-1);   }

	/* get the surface desc */ 
	fread(&header, 124, 1, fp); 

	uint height = *(uint*)&(header[8]);
	uint width = *(uint*)&(header[12]);
	uint linearSize = *(uint*)&(header[16]);
	uint mipMapCount = *(uint*)&(header[24]);
	uint fourCC = *(uint*)&(header[80]);

	uchar* buffer;
	uint bufsize;
	/* how big is it going to be including all mipmaps? */ 
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize; 
	buffer = (uchar*)malloc(bufsize * sizeof(uchar)); 
	fread(buffer, 1, bufsize, fp); 
	fclose(fp);

	uint components = fourCC == FOURCC_DXT1 ? 3 : 4; 
	uint format;

	switch(fourCC) 
	{ 
		case FOURCC_DXT1: format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; break; 
		case FOURCC_DXT3: format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; break; 
		case FOURCC_DXT5: format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; break; 
		default: free(buffer); exit(-1);
	}

	// Create one OpenGL texture
	uint texture_name = 0;
	__GL_CALL(glGenTextures(1, &texture_name))
	p->SetGLTextureName(texture_name);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	__GL_CALL(glBindTexture(GL_TEXTURE_2D, p->GetGLTextureName()))
	__GL_CALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1))

	uint blockSize = format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT ? 8 : 16; 
	uint offset = 0;

	/* load the mipmaps */ 
	for (uint level = 0; level < mipMapCount && (width || height); ++level) 
	{ 
		uint	size = ((width+3)/4)*((height+3)/4)*blockSize; 
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, size, buffer + offset); 
		offset += size; 
		width  /= 2; 
		height /= 2; 
	} 

	free(buffer);

	Resources::pictures.registerRes(p);
	return p;
}

//-----------------------------------------------------------------------------
Picture*		ImageLoader::LoadPNG(const String& path)
{
	if (path.IsEmpty())
	{
		Log::e("LoadPNG - Path is empty.");
		return false;
	}
	if (Resources::pictures.find(path))
		return Resources::pictures.getResource(path);

	Picture* p = new Picture(path);

	int x, y, comp;
	uchar* buffer = stbi_load(path.cStr(), &x, &y, &comp, 0);
	
	switch (comp)
	{
		case STBI_grey:
		case STBI_rgb:
			comp = GL_RGB;
			break;
		case STBI_grey_alpha:
		case STBI_rgb_alpha:
			comp = GL_RGBA;
			break;
	}

	p->SetGLTextureName(OpenGL::GenerateTexture());
//	OpenGL::GenerateTexture(*p);

	// Définie l'unité de texture 0 comme courante. (optionnel, == par défaut)
	__GL_CALL(glActiveTexture(GL_TEXTURE0))
	// Lie la nouvelle texture via son identifiant à l'unité de texture courante (unité 0)
	__GL_CALL(glBindTexture(GL_TEXTURE_2D, p->GetGLTextureName()))

	// Régle les parametres de filtrage de la texture courante.
	__GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT))
	__GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT))
	__GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR))
	__GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR))
	// Transfert les pixels de la png chargée dans 'buffer' sur la texture courante. Le driver se charge de les copier en VRAM.
	glTexImage2D(GL_TEXTURE_2D, 0, comp, x, y, 0, comp, GL_UNSIGNED_BYTE, buffer);
	OpenGL::CheckGLError();
	// Calcul les mipmap de la texture courante.
	glGenerateMipmap(GL_TEXTURE_2D);
	OpenGL::CheckGLError();

	Resources::pictures.registerRes(p);
	return p;
}

//-----------------------------------------------------------------------------
const void		BaseImageLoader::log() const
{	Log::i("Loading image with id: %s", getPath());	}

//-----------------------------------------------------------------------------
Picture*		TGALoader::load()
{	log(); return ImageLoader::loadTGA(getPath());	}

//-----------------------------------------------------------------------------
Picture*		PNGLoader::load()
{	log(); return ImageLoader::LoadPNG(getPath());	}

//-----------------------------------------------------------------------------
BaseImageLoader* ImageLoaderFactory::create(const String& path)
{
	String ext = path.FileExtension();

	if		(ext.IEquals("tga")) return new TGALoader(path);
	else if (ext.IEquals("png")) return new PNGLoader(path);

	Log::e("%s, image format not supported !", path);
	exit(1);
}

//-----------------------------------------------------------------------------
void			ImagesLib::add(const String& path)
{	loaders.add(ImageLoaderFactory::create(path));	}

//-----------------------------------------------------------------------------
void			ImagesLib::load()
{
	Log::HorizontalLine();
	for (int i = 0; i < loaders.size(); i++)
		loaders[i]->load();
}
