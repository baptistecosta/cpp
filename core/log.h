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

extern const bool __log__;

//!
struct	LogPolicyLevelInfo		{	static	String		Format(const char*);	};
struct	LogPolicyLevelError		{	static	String		Format(const char*);	};
struct	LogPolicyLevelWarning	{	static	String		Format(const char*);	};

//!
template <class LogPolicyLevel>
struct  TLog
{
	static void o(const char* str_format, ...)
	{
#ifndef __debug__
		return;
#endif

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

typedef TLog<LogPolicyLevelInfo>	LogI;
typedef TLog<LogPolicyLevelError>	LogE;
typedef TLog<LogPolicyLevelWarning>	LogW;

//!
struct  Log
{
static	void			Flat(const String& msg);
static	void			Flat(const char* format, ...);

static	void			Tee(FILE* f, const char* format, ...);
static	void			Hex(const unsigned char* data, int len, FILE* = 0);
static	void			Binary(const void* const ptr, size_t const size);

		// Horizontal line
static	void			HorizontalLine();
		// New line
static	void			NewLine();

static	void			Print(const char*);
};

}		// owl
#endif	// __LOG__
