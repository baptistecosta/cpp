/**
	Common	::	2011 - 2013
	@author		Baptiste Costa
*/

	#include <cctype>
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <assert.h>
	#include "string.h"
	#include "containers/vector.h"
	#include "defines.h"

	using namespace owl;

//---------------------------------------------------------------------------
int				StringTools::Len(const char* str)
{
	int n = 0;
	if (str)
		while (*str++)
			n++;
	return n;
}

//---------------------------------------------------------------------------
char*			StringTools::Cpy(char* dest, const char* src)
{
	const unsigned int src_len = StringTools::Len(src);
	memcpy(dest, src, src_len);
	dest[src_len] = 0;
	return dest;
}

//---------------------------------------------------------------------------
char*			StringTools::Cpy(char* dest, const char* src, const int size)
{
	memcpy(dest, src, size);
	dest[size] = 0;
	return dest;
}

//---------------------------------------------------------------------------
char*			StringTools::Cat(char* dest, const char* src)
{	return Cpy(dest + Len(dest), src);	}

//---------------------------------------------------------------------------
char*			StringTools::IToS(int n, char* str)
{
	bool is_neg = n < 0;
	n = abs(n);

	int k = 1;
	while (k <= n)
		k *= Decimal;

	char* itr = str;
	int i = 0;

	if (is_neg)
		++i, *itr++ = '-';

	while (n > 0 && i++ < MaxLen - 1)
	{
		k /= Decimal;
		int digit = n / k;
		*itr++ = '0' + digit;
		n -= k * digit;
	}
	*itr = 0;
	return str;
}

//---------------------------------------------------------------------------
String			StringTools::IToS(int n)
{
	char cstr[MaxLen];
	String str(IToS(n, cstr));
	return str;
}

//---------------------------------------------------------------------------
bool			StringTools::Cmp(const char* a, const char* b)
{
	if (!b)
		return false;

	bool res;
	while ((res = *a == *b) && *b)
		++a, ++b;
	return res;
}

//---------------------------------------------------------------------------
bool			StringTools::CaseCmp(const char* a, const char* b)
{
	if (!b)
		return false;

	bool res;
	while ((res = tolower(*a) == tolower(*b)) && *b)
		++a, ++b;
	return res;
}

//---------------------------------------------------------------------------
String			StringTools::Format(const char* str_format, va_list varg)
{
	char buf[4096];
	memset(buf, 0, sizeof(buf));
#ifdef _WIN32
	vsnprintf_s(buf, sizeof(buf), _TRUNCATE, str_format, varg);
#elif __linux
	vsnprintf(buf, sizeof(buf), str_format, varg);
#endif

	return String(buf);
}

//---------------------------------------------------------------------------
unsigned long int StringTools::Hash(const char* str)
{
	unsigned long int h = 0;
	int s = Len(str);
	for (int i = 0; i < s; i++)
		h += (i + 1) * str[0];
	return h;
}

//! String class
//---------------------------------------------------------------------------
const char		String::operator [] (const int i) const
{
	if (i < 0 || i >= Size())
		return 0;
	return ptr[i];
}

//---------------------------------------------------------------------------
String&			String::operator = (const char* str)
{
	if (!str)
	{
		ptr = 0;
		return *this;
	}

	Alloc(St::Len(str));
	St::Cpy(ptr, str);

	return *this;
}
String&			String::operator = (const String& str)
{	return operator = (str.ptr);	}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
String			String::operator + (const char c)		{	String r = *this; r += c; return r;	}
String			String::operator + (const char* str)	{	String r = *this; r += str; return r;	}
String			String::operator + (const String str)	{	String r = *this; r += str.ptr; return r;	}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void			String::operator += (const char c)
{
	const int size = Size() + 1;
	String tmp(size);
	St::Cpy(tmp.ptr, ptr);
	St::Cat(tmp.ptr, &c);
	delete [] ptr;
	ptr = tmp.Detach();
}
void			String::operator += (const char* str)
{
	assert(str != 0);
	const int size = St::Len(ptr) + St::Len(str);
	String tmp_str(size);
	St::Cpy(tmp_str.ptr, ptr);
	St::Cat(tmp_str.ptr, str);
	delete[] ptr;
	ptr = tmp_str.Detach();
}
void			String::operator += (const String& b)
{	*this += b.ptr;	}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
bool			String::operator == (const char* _str) const	{	return St::Cmp(ptr, _str);	}
bool			String::operator == (const String& b) const		{	return *this == b.cStr();	}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
bool			String::operator < (const String& _str) const
{	return strcmp(ptr, _str.cStr()) < 0;	}

//---------------------------------------------------------------------------
bool			String::Alloc(const int size)
{
	delete [] ptr;
	ptr = new char[size + 1];
	if (!ptr)
		return false;
	for (int i = 0; i < size + 1; i++)
		ptr[i] = 0;
	return true;
}

//---------------------------------------------------------------------------
char*			String::Begin() const
{	return &ptr[0];	}

//---------------------------------------------------------------------------
String			String::Append(const String& str)	{	return *this + str;	}
String			String::Prepend(const String& str)	{	String res(str); res += ptr; return res;	}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
bool			String::CaseCmp(const String& b) const
{	return St::CaseCmp(ptr, b.cStr());	}

//---------------------------------------------------------------------------
bool			String::Compare(const int pos, const int len, const char* code)
{
	int str_len = Size();
	if (pos >= str_len || pos + len >= str_len)
		return false;
	
	for (int i = 0; i < len; ++i)
	{
		if (ptr[pos + i] != code[i])
			return false;
	}
	return true;
}

//---------------------------------------------------------------------------
char*			String::Detach()
{
	char* tmp_str = ptr;
	ptr = 0;
	return tmp_str;
}

//---------------------------------------------------------------------------
char*			String::End() const
{	return &ptr[Size()];	}

//---------------------------------------------------------------------------
String			String::Erase(const int pos)
{
	String tmp_str;
	tmp_str.Alloc(pos);

	St::Cpy(tmp_str.ptr, ptr, pos);

	delete[] ptr;
	ptr = tmp_str.Detach();
	return *this;
}

//---------------------------------------------------------------------------
String			String::Erase(const int pos, int len)
{
	int str_len = Size();
	int len_a = pos;

	// If erase after the length of the string, return.
	if (len_a >= str_len)
		return *this;

	// Prevent erasing after the end of the string.
	if ((len_a + len) > str_len)
	{
		// Resize the erase length.
		len = str_len - len_a;
	}

	String	tmp_str;
	int len_b = str_len - len_a - len;
	tmp_str.Alloc(len_a + len_b);

	St::Cpy(tmp_str.ptr, ptr, len_a);
	if (len_b > 0)
	{
		int offset_b = len_a + len;
		St::Cat(tmp_str.ptr, ptr + offset_b);
	}

	delete [] ptr;
	ptr = tmp_str.Detach();
	return *this;
}

//---------------------------------------------------------------------------
const int		String::FindFirstOf(const char* code)
{
	int s = Size();
	int cs = St::Len(code);
	for (int i = 0; i < s; ++i)
	{
		if ((i + cs) > s)
			break;
		if (memcmp(&ptr[i], code, cs) == 0)
			return i;
	}
	return -1;
}

//---------------------------------------------------------------------------
const int		String::FindFirstOfNot(const char* code)
{
	int s = Size();
	int cs = St::Len(code);
	for (int i = 0; i < s; i += cs)
		if ((memcmp(&ptr[i], code, cs) != 0) || (i + cs) > s)
			return i;
	return -1;
}

//---------------------------------------------------------------------------
const int		String::FindLastOf(const char* code)
{
	int s = Size();
	int cs = St::Len(code);
	for (int i = s; i >= 0; --i)
	{
		int n = i - cs;
		if (n < 0)
			break;
		if (memcmp(&ptr[n], code, cs) == 0)
			return i;
	}
	return 0;
}

//---------------------------------------------------------------------------
const int		String::FindLastOfNot(const char* code)
{
	int s = Size();
	int cs = St::Len(code);
	for (int i = s; i >= 0; i -= cs)
	{
		int n = i - cs;
		if (memcmp(&ptr[n], code, cs) != 0 || n < 0)
			return i;
	}
	return 0;
}

//---------------------------------------------------------------------------
String			String::Format(const char* str_format, ...)
{
	va_list varg;
	va_start(varg, str_format);

	char buff[4096];
	memset(buff, 0, sizeof(buff));
#ifdef _WIN32
	vsnprintf_s(buff, sizeof(buff), _TRUNCATE, str_format, varg);
#elif __linux
	vsnprintf(buff, sizeof(buff), str_format, varg);
#endif
	va_end(varg);

	return String(buff);
}

//---------------------------------------------------------------------------
const String	String::FileExtension() const
{
	String p(ptr);
	int n = p.FindLastOf(".") + 1;
	p.Keep(n, p.Size() - n);
	return p;
}

//---------------------------------------------------------------------------
const String	String::FilenameFromPath() const
{
	String buf(ptr);
	int slash = buf.FindLastOf("/") + 1;
	int ext = buf.FindLastOf(".");
	buf.Keep(slash, ext - slash);
	return buf;
}

//---------------------------------------------------------------------------
String			String::Extract(const int pos, const int len)
{
	assert((pos + len) <= Size());
	String s(len);
	St::Cpy(s.ptr, ptr + pos, len);
	return s;
}

//---------------------------------------------------------------------------
void			String::Keep(const int pos, const int len)
{
	String s = Extract(pos, len);
	delete [] ptr;
	ptr = s.Detach();
}

//---------------------------------------------------------------------------
const bool		String::IsEmpty() const
{	return Size() == 0;	}

//---------------------------------------------------------------------------
const bool		String::IsNull() const
{	return ptr == 0;	}


//---------------------------------------------------------------------------
const bool		String::IsNumeric() const
{
	const int size = Size();
	for (int i = 0; i < size; ++i)
		if (ptr[i] < '0' || ptr[i] > '9')
			return false;
	return true;
}

//---------------------------------------------------------------------------
const String&	String::RemoveOccurences(const Vector<String>& occs)
{
	const int size = Size();
	const int number_of_occurence = occs.Size();

	struct Occurence { int	count, size; Occurence() : count(0), size(0) {} };
	Vector<Occurence> occurences(number_of_occurence);

	for (int i = 0; i < number_of_occurence; ++i)
		occurences[i].size = occs[i].Size();

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < number_of_occurence; ++j)
		{
			if (memcmp(&ptr[i], occs[j].cStr(), occurences[j].size) == 0)
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
	char ca = '\0', cb = '\0';
	for (int i = 0, it = 0; i < size; ++i)
	{
		cb = ptr[i];
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
			str[it++] = cb;
		else
			write = true;
		ca = cb;
	}

	delete [] ptr;
	ptr = str.Detach();

	return *this;
}

//---------------------------------------------------------------------------
const int		String::Size() const
{	return St::Len(ptr);	}

//---------------------------------------------------------------------------
void			String::ToLower()
{
	if (ptr)
		for (int i = 0; i < Size(); i++)
			ptr[i] = tolower(ptr[i]);
}

//---------------------------------------------------------------------------
void			String::ToUpper()
{
	if (ptr)
		for (int i = 0; i < Size(); i++)
			ptr[i] = toupper(ptr[i]);
}

//---------------------------------------------------------------------------
bool			String::IsCharInArray(char needle, const char* haystack)
{
	int s = St::Len(haystack);
	for (int i = 0; i < s; ++i)
		if (needle == haystack[i])
			return true;
	return false;
}

//! Trimmed character policy
//---------------------------------------------------------------------------
struct			TrimmedCharactersPolicy_All		{	static const char* val;	};
struct			TrimmedCharactersPolicy_Spaces	{	static const char* val;	};

const char*		TrimmedCharactersPolicy_All::val	= " \t\n\r\v";
const char*		TrimmedCharactersPolicy_Spaces::val	= " ";
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void			String::TrimAll()
{	Trim<TrimmedCharactersPolicy_All>();	}
void			String::TrimSpaces()
{	Trim<TrimmedCharactersPolicy_Spaces>();	}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
const String&	String::Unquotify()
{	return *this = Extract(1, Size() - 2);	}
