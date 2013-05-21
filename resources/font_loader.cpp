/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

	#include <stdio.h>
	#include "core/containers/auto_ptr.h"
	#include "core/log.h"
	#include "core/filesys.h"
	#include "font_loader.h"
	#include "font.h"
	#include "image_loader.h"
	#include "resource_manager.h"
	
using	namespace owl;

//---------------------------------------------------------------------------
Font*			FontLoader::load(const String& path)
{
	String font_id = path.FilenameFromPath();
	if (Resources::fonts.find(font_id))
		return Resources::fonts.getResource(font_id);

	FileSystem fs;
	if (!fs.Open(path))
		exit(1);

	Font* font = new Font(font_id);
	String word, line;
	while (1)
	{
		if (!fs.Scan(word))
			break;

		if (word == "common")
			fscanf(fs.GetFile(), " lineHeight=%d base=%d scaleW=%d scaleH=%d pages=%d", &font->line_h, &font->base, &font->scale_w, &font->scale_h, &font->pages);

		if (word == "page")
		{
			char buff[96];
			fscanf(fs.GetFile(), " id=%d file=%95s", &font->m_page_id, buff);
			String texture_filename(buff);
			texture_filename.Unquotify();

			AutoPtr<BaseImageLoader> loader = ImageLoaderFactory::create(String::Format("resources/fonts/%s", texture_filename));
			font->m_texture = loader->load();
		}

		else if (word == "chars")
		{
			fs.Word(line);
			int count = atoi(&line.cStr()[7]);
			font->count = count;
		}

		else if (word == "char")
		{
			fs.Word(line);
			int id = atoi(&line.cStr()[4]);
			font->chars[id].pos.x = static_cast<float>(atoi(&line.cStr()[11]));
			font->chars[id].pos.y = static_cast<float>(atoi(&line.cStr()[19]));
			font->chars[id].size.x = static_cast<float>(atoi(&line.cStr()[31]));
			font->chars[id].size.y = static_cast<float>(atoi(&line.cStr()[44]));
			font->chars[id].offset.x = static_cast<float>(atoi(&line.cStr()[58]));
			font->chars[id].offset.y = static_cast<float>(atoi(&line.cStr()[72]));
			font->chars[id].x_advance = static_cast<float>(atoi(&line.cStr()[87]));
		}
	}
	return Resources::fonts.registerRes(font);;
}
