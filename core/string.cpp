/*
	bEngine :: 2011 - 2013
	Baptiste Costa
*/

	#include <ctype.h>
	#include <stdarg.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include "string.h"
	
	using namespace owl;

//---------------------------------------------------------------------------
String::String()										{	Clear();	}
String::String(const String& s)	: v(s.v)				{}
String::String(const char* s)	: v(Length(s + 1))		{	Clear(); *this += s;	}
String::String(const int size)	: v(size + 1)			{	Clear();	}

//---------------------------------------------------------------------------
String&			String::operator += (const char c)		{	*this += &c; return *this;	}
String&			String::operator += (const String& s)	{	*this += s.cStr(); return *this;	}
String&			String::operator += (const char* s)
{
	int len = Length(s);
	int inc = (Size() + (len + 1)) - v.GetCapacity();
	if (inc > 0)
		v.IncreaseCapacity(inc);
	Copy(*this, s, len);
	return *this;
}
String&			String::operator =	(const String& s)	{	v = s.v; return *this;	}
String&			String::operator =	(const char* s)
{
	Destroy();
	const int l = Length(s);
	v.Allocate(l);
	Copy(*this, s, l);
	return *this;
}
char			String::operator [] (const int i)		{	assert(i >= 0 && i < Size()); return v[i];	}
const char		String::operator [] (const int i) const	{	assert(i >= 0 && i < Size()); return v[i];	}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void			String::Clear()
{
	for (int i = 0; i < v.GetCapacity(); ++i)
		v[i] = 0;
}

//---------------------------------------------------------------------------
char*			String::Detach()
{	return v.Detach();	}

//---------------------------------------------------------------------------
int				String::Length(const char* s)
{
	int n = 0;
	if (s)
		while (*s++)
			n++;
	return n;
}

//---------------------------------------------------------------------------
String			String::Extract(const int pos, const int len) const
{
	assert((pos + len) <= Size());
	String s(len);
	Copy(s, &v[pos], len);
	return s;
}

//---------------------------------------------------------------------------
const String	String::FileExtension() const
{
	const int n = FindLastOf(".");
	return Extract(n, Size() - n);
}
const String	String::FilenameFromPath() const
{
	const int n = FindLastOf("/");
	return Extract(n, FindLastOf(".") - n - 1);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
const int		String::FindFirstOf(const char* s) const
{
	const int ss = Length(s);
	for (int i = 0; i < Size(); ++i)
	{
		if ((i + ss) > Size())
			break;
		if (memcmp(&v[i], s, ss) == 0)
			return i;
	}
	return -1;
}
const int		String::FindFirstOfNot(const char* s) const
{
	const int ss = Length(s);
	for (int i = 0; i < Size(); i += ss)
		if ((memcmp(&v[i], s, ss) != 0) || (i + ss) > Size())
			return i;
	return -1;
}
const int		String::FindLastOf(const char* s) const
{
	const int ss = Length(s);
	for (int i = Size(); i >= 0; --i)
	{
		int n = i - ss;
		if (n < 0)
			break;
		if (memcmp(&v[n], s, ss) == 0)
			return i;
	}
	return 0;
}
const int		String::FindLastOfNot(const char* s) const
{
	const int ss = Length(s);
	for (int i = Size(); i >= 0; i -= ss)
	{
		int n = i - ss;
		if (memcmp(&v[n], s, ss) != 0 || n < 0)
			return i;
	}
	return 0;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
const bool		String::IsNumeric() const
{
	for (int i = 0; i < Size(); ++i)
		if (v[i] < '0' || v[i] > '9')
			return false;
	return true;
}

const String&	String::RemoveOccurences(const Vector<String>& occs)
{
	const int size = Size();
	const int number_of_occurence = occs.Size();
	
	struct Occurence { int	count, size; Occurence() : count(0), size(0) {} };
	Vector<Occurence> occurences(number_of_occurence);
	
	for (int i = 0; i < number_of_occurence; ++i)
		occurences[i].size = occs[i].Size();

	// Compute the size without occurences
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < number_of_occurence; ++j)
		{
			if (memcmp(&v[i], occs[j].cStr(), occurences[j].size) == 0)
			{
				i += occurences[j].size - 1;
				occurences[j].count++;
			}
		}
	}
	int new_size = size;
	for (int i = 0; i < number_of_occurence; ++i)
		new_size -= occurences[i].size * occurences[i].count;
	
	if (size == new_size)
		return *this;
	
	Vector<char> str(new_size + 1);
	bool write = true, quoted = false;
	char ca = 0, cb = 0;
	for (int i = 0; i < size; ++i)
	{
		cb = v[i];
		if (ca != '\\' && cb == '"')
			quoted = !quoted;
	
		if (!quoted)
		{
			for (int j = 0; j < number_of_occurence; ++j)
			{
				if (memcmp(&cb, occs[j].cStr(), occurences[j].size) == 0)
				{
					i += occurences[j].size - 1;
					write = false;
					break;
				}
			}
		}
		if (write)
			str << cb;
		else
			write = true;
		ca = cb;
	}
	str[str.Size()] = 0;
	
	v = str;
	
	return *this;
}

//---------------------------------------------------------------------------
void			String::ToLower()
{
	for (int i = 0; i < Size(); i++)
		v[i] = tolower(v[i]);
}
void			String::ToUpper()
{
	for (int i = 0; i < Size(); i++)
		v[i] = toupper(v[i]);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void			String::Copy(String& d, const char* s, const int len)
{
	for (int i = 0; i < len; ++i)
		d.v << s[i];
	d.v[d.Size()] = 0;
}
void			String::Copy(char* d, const char* s)
{	Copy(d, s, Length(s));	}
void			String::Copy(char* d, const char* s, const int len)
{
	for (int i = 0; i < len; ++i)
		d[i] = s[i];
	d[len] = 0;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
const bool		String::Equals(const char* a, const char* b)
{
	if (!b)
		return false;
	bool r;
	while ((r = *a == *b) && *b)
		++a, ++b;
	return r;
}
const bool		String::IEquals(const char* a, const char* b)
{
	if (!b)
		return false;
	bool r;
	while ((r = tolower(*a) == tolower(*b)) && *b)
		++a, ++b;
	return r;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void			String::Concatenate(char* dest, const char* src)
{	Copy(dest + Length(dest), src, Length(src));	}
String			String::Concatenate(const char* a, const char* b)
{
	String s = a;
	s += b;
	return s;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
String			String::Format(const char* format, ...)
{
	va_list varg;
	va_start(varg, format);
	String r = FormatVarg(format, varg);
	va_end(varg);
	return r;
}
String			String::FormatVarg(const char* format, va_list varg)
{
	char buf[2048] = {};
	memset(buf, 0, sizeof(buf));
#ifdef _WIN32
	vsnprintf_s(buf, sizeof(buf), _TRUNCATE, format, varg);
#elif __linux
	vsnprintf(buf, sizeof(buf), format, varg);
#endif
	return String(buf);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
ulong			String::Hash(const char* str)
{
	ulong h = 0;
	int s = Length(str);
	for (int i = 0; i < s; i++)
		h += (i + 1) * str[0];
	return h;
}

//---------------------------------------------------------------------------
bool			String::IsCharInArray(char needle, const char* haystack)
{
	int s = Length(haystack);
	for (int i = 0; i < s; ++i)
		if (needle == haystack[i])
			return true;
	return false;
}

//---------------------------------------------------------------------------
char*			String::IToS(int n, char* str)
{
	enum { MaxLength = 64, Decimal = 10 };

	bool is_neg = n < 0;
	n = abs(n);

	int k = 1;
	while (k <= n)
		k *= Decimal;

	char* itr = str;
	int i = 0;

	if (is_neg)
		++i, *itr++ = '-';

	while (n > 0 && i++ < MaxLength - 1)
	{
		k /= Decimal;
		int digit = n / k;
		*itr++ = '0' + digit;
		n -= k * digit;
	}
	*itr = 0;
	return str;
}
String			String::IToS(const int n)
{
	enum { MaxLength = 64 };
	char c[MaxLength] = {};
	return String(IToS(n, c));
}
//---------------------------------------------------------------------------

//! Trimmed character policy
//---------------------------------------------------------------------------
struct			bTrimmedCharactersPolicy_All		{	static const char* val;	};
struct			bTrimmedCharactersPolicy_Spaces		{	static const char* val;	};

const char*		bTrimmedCharactersPolicy_All::val	= " \t\n\r\v";
const char*		bTrimmedCharactersPolicy_Spaces::val	= " ";
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void			String::TrimAll()
{	Trim<bTrimmedCharactersPolicy_All>();	}
void			String::TrimSpaces()
{	Trim<bTrimmedCharactersPolicy_Spaces>();	}
//---------------------------------------------------------------------------
