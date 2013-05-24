/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __LOG__
#define __LOG__

#ifdef _WIN32
	#include <Windows.h>
#endif

	#include <stdio.h>
	#include "core/string.h"
	#include "platform/debug.h"

namespace owl {

class	String;

//!
struct	LogPolicyLevelInfo		{	static	String		Format(const char*);	};
struct	LogPolicyLevelError		{	static	String		Format(const char*);	};
struct	LogPolicyLevelWarning	{	static	String		Format(const char*);	};

//!
template <class LogPolicyLevel>
struct	Dump
{
	static void			Process(const char* str_format, ...)
	{
		va_list varg;
		va_start(varg, str_format);
		String str = StringTools::Format(str_format, varg);
		String out = LogPolicyLevel::Format(str.cStr());

#ifdef _WIN32
		OutputDebugStringA(out.cStr());
#elif __linux
		printf("%s", out.cStr());
#endif
		va_end(varg);
	}
};

#ifdef __debug__
	#define	__LOG(_FORMAT_, ...)	Dump<LogPolicyLevelInfo>::Process(_FORMAT_, __VA_ARGS__);
	#define	__LOG_E(_FORMAT_, ...)	Dump<LogPolicyLevelError>::Process(_FORMAT_, __VA_ARGS__);
	#define	__LOG_W(_FORMAT_, ...)	Dump<LogPolicyLevelWarning>::Process(_FORMAT_, __VA_ARGS__);
	#define __LOG_NL()				Dump<LogPolicyLevelInfo>::Process("");
#else
	#define	__LOG(_FORMAT_, ...)
	#define	__LOG_E(_FORMAT_, ...)
	#define	__LOG_W(_FORMAT_, ...)
	#define __LOG_NL()
#endif

//!
struct  Log
{
static	const bool		log;

static	void			i(const String& msg);
static	void			i(const char* format, ...);

static	void			e(const String& msg);
static	void			e(const char* format, ...);

static	void			Flat(const String& msg);
static	void			Flat(const char* format, ...);

static	void			Tee(FILE* f, const char* format, ...);
static	void			Hex(const unsigned char* data, int len, FILE*);
static	void			Binary(const void* const ptr, size_t const size);

		// Horizontal line
static	void			HorizontalLine();
		// New line
static	void			NewLine();

static	void			Print(const char*);
};

}		// owl
#endif	// __LOG__
