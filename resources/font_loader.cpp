/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

	#include <stdio.h>
	#include "font_loader.h"
	#include "font.h"
	#include "image_loader.h"
	#include "resource_manager.h"
	#include "core\log.h"
	#include "platform\file_system.h"
	#include "platform\auto_ptr.h"
	
using	namespace owl;

//---------------------------------------------------------------------------
Font*			FontLoader::load(const String& path)
{
	String font_id = path.FilenameFromPath();
	if (Resources::fonts.find(font_id))
		return Resources::fonts.getResource(font_id);

	FileSystem fs;
	if (!fs.open(path))
		exit(1);
/*
	FILE* f = fopen(path.cStr(), "r");
	if (!f)
	{
		printf("Fail to open %s.\n", path);
		return null_ptr;
	}
*/
	Font* font = new Font(font_id);
	String word, line;
	forever
	{
//		char word[48];
//		char line[256];

// 		int res = fscanf(f, "%s", word);
// 		if (res == EOF) break;
		
		if (!fs.scan(word))
			break;

//		else if (strcmp(word, "common") == 0)
		if (word == "common")
			fscanf(fs.getFile(), " lineHeight=%d base=%d scaleW=%d scaleH=%d pages=%d", &font->line_h, &font->base, &font->scale_w, &font->scale_h, &font->pages);

		if (word == "page")
		{
			char buff[96];
			fscanf(fs.getFile(), " id=%d file=%95s", &font->m_page_id, buff);
			String texture_filename(buff);
			texture_filename.Unquotify();

			AutoPtr<BaseImageLoader> loader = ImageLoaderFactory::create(String::Format("resources/fonts/%s", texture_filename));
			font->m_texture = loader->load();
		}

//		else if (strcmp(word, "chars") == 0)
		else if (word == "chars")
		{
/*
			int count = 0;
			char buff[256];

			fgets(buff, 256, fs.getFile());
			count = atoi(buff + 7);
			font->count = count;
*/
			fs.word(line);
			int count = atoi(&line.cStr()[7]);
			font->count = count;
		}

//		else if (strcmp(word, "char") == 0)
		else if (word == "char")
		{
// 			char buff[256] = {0};
// 			fgets(buff, 256, fs.getFile());
			
			fs.word(line);
			int id = atoi(&line.cStr()[4]);
// 			int id = atoi(buff + 4);
/*
			font->chars[id].pos.x = static_cast<float>(atoi(buff + 11));
			font->chars[id].pos.y = static_cast<float>(atoi(buff + 19));
			font->chars[id].size.x = static_cast<float>(atoi(buff + 31));
			font->chars[id].size.y = static_cast<float>(atoi(buff + 44));
			font->chars[id].offset.x = static_cast<float>(atoi(buff + 58));
			font->chars[id].offset.y = static_cast<float>(atoi(buff + 72));
			font->chars[id].x_advance = static_cast<float>(atoi(buff + 87));
*/
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
