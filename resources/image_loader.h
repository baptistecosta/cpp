/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __IMAGE_LOADER__
#define __IMAGE_LOADER__

	#include "core/string.h"
	#include "core/containers/auto_array.h"

namespace owl {

class	Picture;

//!
struct	ImageLoader
{
static	Picture*		LoadBMP(const String& path);
static	Picture*		loadTGA(const String& path);
static	Picture*		LoadDDS(const String& path);
static	Picture*		LoadPNG(const String& path);
};

//!
class	BaseImageLoader 
{
		String			m_path;

public:

		BaseImageLoader(const String &path)	:	m_path(path)	{}
virtual	Picture*		load() = 0;

protected:

const	String&			getPath() const		{	return m_path;	}
const	void			log() const;
};

//!
struct	TGALoader	:	public	BaseImageLoader
{
		TGALoader(const String& path)	:	BaseImageLoader(path)	{}

virtual	Picture*		load();
};

//!
struct	PNGLoader	:	public	BaseImageLoader
{
		PNGLoader(const String& path)	:	BaseImageLoader(path)	{}

virtual	Picture*		load();
};

//!
struct	ImageLoaderFactory
{
static	BaseImageLoader* create(const String& path);
};

//!
class	ImagesLib
{
		AutoArray<BaseImageLoader*> loaders;

public:

		void			add(const String& path);
		void			load();
};

}		// owl
#endif	// __IMAGE_LOADER__