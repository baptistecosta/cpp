/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/
#ifdef _WIN32
	#include <Windows.h>
#endif
	#include <ctype.h>
	#include <stdarg.h>
	#include "log.h"
	#include "string.h"

	using namespace owl;

#ifdef _DEBUG		// Windows
	const bool	Log::log = true;
#else
	#ifndef NDEBUG	// Linux
		const bool	Log::log = true;
	#else
		const bool	Log::log = false;
	#endif
#endif

//-----------------------------------------------------------------------------
void			Log::i(const String& msg)
{	if (Log::log) Log::i(msg.cStr());	}
void			Log::i(const char* format, ...)
{
	if (Log::log)
	{
		va_list varg;
		va_start(varg, format);
		String str = StringTools::Format(format, varg);
		String out = String::Format("Info: %s\n", str.cStr());
		Print(out.cStr());
		va_end(varg);
	}
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void			Log::e(const String& msg)
{	if (Log::log) Log::e(msg.cStr());	}
void			Log::e(const char* format, ...)
{
	if (Log::log) 
	{
		va_list varg;
		va_start(varg, format);
		String str = StringTools::Format(format, varg);
		String out = String::Format("Error: %s\n", str.cStr());
		Print(out.cStr());
		va_end(varg);
	}
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void			Log::w(const String& msg)
{	if (Log::log) Log::w(msg.cStr());	}
void			Log::w(const char* format, ...)
{
	if (Log::log) 
	{
		va_list varg;
		va_start(varg, format);
		String str = StringTools::Format(format, varg);
		String out = String::Format("Warning: %s\n", str.cStr());
		Print(out.cStr());
		va_end(varg);
	}
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void			Log::Flat(const String& msg)
{	if (Log::log) Log::i(msg.cStr());	}
void			Log::Flat(const char* format, ...)
{
	if (Log::log)
	{
		va_list varg;
		va_start(varg, format);
		String str = StringTools::Format(format, varg);
		String out = String::Format("%s", str.cStr());
		Print(out.cStr());
		va_end(varg);
	}
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void			Log::Tee(FILE* f, const char* fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	vprintf(fmt, va);
	va_end(va);

	if (f)
	{
		va_start(va, fmt);
		vfprintf(f, fmt, va);
		va_end(va);
	}
}

//-----------------------------------------------------------------------------
void			Log::Hex(const unsigned char* data, int len, FILE* f)
{
	unsigned char* buf = (unsigned char*)data;
	for (int i = 0; i < len; i += 16)
	{
		Tee(f, "%06x: ", i);

		for (int j = 0; j < 16; j++)
			if (i + j < len)
				Tee(f, "%02x ", buf[i + j]);
			else
				Tee(f, "   ");
		Tee(f, " ");

		for (int j = 0; j < 16; j++)
			if (i + j < len)
				Tee(f, "%c", isprint(buf[i + j]) ? buf[i + j] : '.');
		Tee(f, "\n");
	}
}

//-----------------------------------------------------------------------------
void			Log::HorizontalLine()
{	if (Log::log) Print("\n===============================================================\n");	}
//-----------------------------------------------------------------------------
void			Log::NewLine()
{	if (Log::log) Print("\n");	}

//-----------------------------------------------------------------------------
void			Log::Print(const char* out)
{
#ifdef _WIN32
		OutputDebugStringA(out);
#elif __linux
		printf(out);
#endif
}
