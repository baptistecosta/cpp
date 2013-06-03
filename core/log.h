/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __LOG__
#define __LOG__

	#include <stdio.h>
	#include "core/string.h"
	#include "platform/debug.h"
	#include "platform/io.h"
	#include "defines.h"

namespace owl {

class	String;

//!
struct  Log
{
static	const bool		__log__;

static	void			i(const String& msg);
static	void			i(const char* format, ...);
static	void			e(const String& msg);
static	void			e(const char* format, ...);
static	void			w(const String& msg);
static	void			w(const char* format, ...);

static	void			Flat(const String& msg);
static	void			Flat(const char* format, ...);

static	void			Tee(FILE* f, const char* format, ...);
static	void			Hex(const uchar* data, int len, FILE* = 0);
static	void			Binary(const void* const ptr, size_t const size);

		// Horizontal line
static	void			HorizontalLine();
		// New line
static	void			NewLine();
};

}		// owl
#endif	// __LOG__
