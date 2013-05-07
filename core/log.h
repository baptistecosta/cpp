/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __LOG__
#define __LOG__

	#include <stdio.h>

namespace owl {

class	String;

//!	@struct	Log
//!	@brief	Log info(Log::i), error (Log::e), warning (Log::w).
struct  Log
{
static	const bool		log;

static	void			i(const String& msg);
static	void			i(const char* format, ...);

static	void			e(const String& msg);
static	void			e(const char* format, ...);

static	void			w(const String& msg);
static	void			w(const char* format, ...);

static	void			Flat(const String& msg);
static	void			Flat(const char* format, ...);

static	void			Tee(FILE* f, const char* format, ...);
static	void			Hex(const unsigned char* data, int len, FILE*);

		// Horizontal line
static	void			HorizontalLine();
		// New line
static	void			NewLine();
};

}		// owl
#endif	// __LOG__