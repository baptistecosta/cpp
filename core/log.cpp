/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

	#include <ctype.h>
	#include <stdarg.h>
	#include "log.h"

	using namespace owl;

//---------------------------------------------------------------------------
#ifdef __debug__
const bool		Log::__log__ = true;
#else
const bool		Log::__log__ = false;
#endif

//---------------------------------------------------------------------------
void			Log::i(const String& msg)
{	Log::i(msg.cStr());	}
void			Log::i(const char* format, ...)
{
	if (__log__)
	{
		va_list varg;
		va_start(varg, format);
		String str = String::FormatVarg(format, varg);
		String out = String::Format("%s\n", str.cStr());
		va_end(varg);

		IO::Print(out.cStr());
	}
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void			Log::e(const String& msg)
{	Log::e(msg.cStr());	}
void			Log::e(const char* format, ...)
{
	if (__log__)
	{
		va_list varg;
		va_start(varg, format);
		String str = String::FormatVarg(format, varg);
		String out = String::Format("Error: %s\n", str.cStr());
		va_end(varg);

		IO::Print(out.cStr());
	}
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void			Log::w(const String& msg)
{	Log::w(msg.cStr());	}
void			Log::w(const char* format, ...)
{
	if (__log__)
	{
		va_list varg;
		va_start(varg, format);
		String str = String::FormatVarg(format, varg);
		String out = String::Format("Warning: %s\n", str.cStr());
		va_end(varg);

		IO::Print(out.cStr());
	}
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void			Log::Flat(const String& msg)
{	Log::Flat(msg.cStr());	}
void			Log::Flat(const char* format, ...)
{
	if (__log__)
	{
		va_list varg;
		va_start(varg, format);
		String str = String::FormatVarg(format, varg);
		String out = String::Format("%s", str.cStr());
		IO::Print(out.cStr());
		va_end(varg);
	}
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void			Log::Tee(FILE* f, const char* fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	IO::Print(String::FormatVarg(fmt, va).cStr());
	va_end(va);

	if (f)
	{
		va_start(va, fmt);
		vfprintf(f, fmt, va);
		va_end(va);
	}
}

//---------------------------------------------------------------------------
void			Log::Hex(const uchar* data, int len, FILE* f)
{
	uchar* buf = (uchar*)data;
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

//---------------------------------------------------------------------------
//! Little endian
void			Log::Binary(const void* const ptr, const size_t len)
{
	uchar* b = (uchar*)ptr;
	for (unsigned int i = 0; i < len; i++)
	{
		Log::Flat("Byte %03d: ", i);
		for (int j = 7; j >= 0; --j)
		{
			uchar byte = b[i] & (1<<j);
			byte >>= j;
			Log::Flat("%u", byte);
		}
		Log::NewLine();
	}
	Log::NewLine();
}

//---------------------------------------------------------------------------
void			Log::HorizontalLine()
{	if (__log__) IO::Print("\n===============================================================\n");	}
void			Log::NewLine()
{	if (__log__) IO::Print("\n");	}
//---------------------------------------------------------------------------
