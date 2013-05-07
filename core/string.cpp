/**
	Common	::	2011 - 2013
	@author		Baptiste Costa
*/

	#include <cctype>
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <assert.h>
	#include "typedefs.h"
	#include "string.h"

using	namespace owl;

//---------------------------------------------------------------------------
int				StringTools::Len(const char* str)
{
	int n = 0;
	if (str)
	{
		while (*str++ != 0)
			n++;
	}

	//	Test it
	// 	while (str[n++])
	// 		;

	return n;
}

//---------------------------------------------------------------------------
char*			StringTools::Cpy(char* dest, const char* src)
{
	const unsigned int src_len = StringTools::Len(src);
	memcpy(dest, src, src_len);
	dest[src_len] = StringTools::NullChar;
	return dest;
}

//---------------------------------------------------------------------------
char*			StringTools::Cpy(char* dest, const char* src, const int size)
{
	memcpy(dest, src, size);
	dest[size] = StringTools::NullChar;
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
	*itr = StringTools::NullChar;
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

//-----------------------------------------------------------------------------
String			StringTools::Format(const char* str_format, va_list varg)
{
	char buff[4096];
	memset(buff, 0, sizeof(buff));
	vsnprintf_s(buff, sizeof(buff), _TRUNCATE, str_format, varg);
	return String(buff);
}

//-----------------------------------------------------------------------------
unsigned long int StringTools::Hash(const char* str)
{
	unsigned long int h = 0;
	int s = Len(str);
	for (int i = 0; i < s; i++)
		h += (i + 1) * str[0];
	return h;
}


//-----------------------------------------------------------------------------
const char		String::operator [] (const int i) const
{
	if (i < 0 || i >= Size())
		return 0;
	return m_str[i];
}

//-----------------------------------------------------------------------------
String&			String::operator = (const String& b)
{	return operator=(b.m_str);	}

//-----------------------------------------------------------------------------
String&			String::operator = (const char* str)
{
	if (!str)
	{
		m_str = NULL;
		return *this;
	}

	Alloc(St::Len(str));
	St::Cpy(m_str, str);

	return *this;
}

//-----------------------------------------------------------------------------
String			String::operator + (const String b)
{
	String c = *this;
	c += b.m_str;
	return c;
}

//-----------------------------------------------------------------------------
String			String::Append(const String& b)
{	return *this + b;	}

//-----------------------------------------------------------------------------
String			String::Prepend(const String& str)
{
	String res(str);
	res += m_str;
	return res;
}

//-----------------------------------------------------------------------------
void			String::operator += (const char* str)
{
	const int size = St::Len(m_str) + St::Len(str);
	String tmp_str(size);

	St::Cpy(tmp_str.m_str, m_str);
	St::Cat(tmp_str.m_str, str);

	delete[] m_str;
	m_str = tmp_str.Detach();
}

//-----------------------------------------------------------------------------
void			String::operator += (const String& b)
{	*this += b.m_str;	}

//-----------------------------------------------------------------------------
bool			String::operator == (const char* str) const
{	return St::Cmp(m_str, str);	}

//-----------------------------------------------------------------------------
bool			String::operator == (const String& b) const
{	return *this == b.cStr();	}

//-----------------------------------------------------------------------------
bool			String::operator < (const String& str) const
{	return strcmp(m_str, str.cStr()) < 0;	}

//-----------------------------------------------------------------------------
bool			String::Alloc(const int size)
{
	delete [] m_str;
	m_str = new char[size + 1];
	for (int i = 0; i < size + 1; i++)
		m_str[i] = 0;
	return m_str == NULL;
}

//---------------------------------------------------------------------------
char*			String::Begin() const
{	return &m_str[0];	}

/*
//---------------------------------------------------------------------------
bool			bString::contain(const char* _code)
{
	int code_len = strLen(_code);
	if (len() < code_len)
		return false;

// 	for (int i = 0; i < len() - 1; ++i)
// 	{
// 		if (str[i] == *_code)
// 		{
// 			for ()
// 		}
// 	}
}
*/

//-----------------------------------------------------------------------------
bool			String::CaseCmp(const String& b) const
{	return St::CaseCmp(m_str, b.cStr());	}

//-----------------------------------------------------------------------------
bool			String::Compare(const int pos, const int len, const char* code)
{
	int str_len = Size();
	if (pos >= str_len || pos + len >= str_len)
		return false;
	
	for (int i = 0; i < len; ++i)
	{
		if (m_str[pos + i] != code[i])
			return false;
	}
	return true;
}

//-----------------------------------------------------------------------------
char*			String::Detach()
{
	char* tmp_str = m_str;
	m_str = NULL;
	return tmp_str;
}

//-----------------------------------------------------------------------------
char*			String::End() const
{	return &m_str[Size()];	}

//-----------------------------------------------------------------------------
String			String::Erase(const int pos)
{
	String tmp_str;
	tmp_str.Alloc(pos);

	St::Cpy(tmp_str.m_str, m_str, pos);

	delete[] m_str;
	m_str = tmp_str.Detach();
	return *this;
}

//-----------------------------------------------------------------------------
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

	St::Cpy(tmp_str.m_str, m_str, len_a);
	if (len_b > 0)
	{
		int offset_b = len_a + len;
		St::Cat(tmp_str.m_str, m_str + offset_b);
	}

	delete [] m_str;
	m_str = tmp_str.Detach();
	return *this;
}

//-----------------------------------------------------------------------------
const int		String::FindFirstOf(const char* code)
{
	int str_len = Size();
	for (int i = 0; i < str_len - 1; ++i)
	{
		if (m_str[i] == *code)
		{
			int code_len = St::Len(code);
			for (int j = 0; j < code_len; ++j)
			{
				if (m_str[i + j] != code[j])
					break;
				if (j == code_len - 1)
					return i;
			}
		}
	}
	return -1;
}

//-----------------------------------------------------------------------------
const int		String::FindLastOf(const char* code)
{
	for (int i = Size() - 1; i >= 0; --i)
		if (m_str[i] == *code)
			return i;
	return 0;
}

//-----------------------------------------------------------------------------
String			String::Format(const char* str_format, ...)
{
	va_list varg;
	va_start(varg, str_format);

	char buff[4096];
	memset(buff, 0, sizeof(buff));
	vsnprintf_s(buff, sizeof(buff), _TRUNCATE, str_format, varg);

	va_end(varg);

	return String(buff);
}

//-----------------------------------------------------------------------------
const String	String::FileExtension() const
{
	String p(m_str);
	int n = p.FindLastOf(".") + 1;
	return p.Keep(n, p.Size() - n);
}

//-----------------------------------------------------------------------------
const String	String::FilenameFromPath() const
{
	String buff(m_str);
	int slash = buff.FindLastOf("/") + 1;
	int ext = buff.FindLastOf(".");
	return buff.Keep(slash, ext - slash);
}

//-----------------------------------------------------------------------------
const String	String::Keep(const int pos, const int len)
{
	assert((pos + len) <= Size());

	String tmp_str(len);
	St::Cpy(tmp_str.m_str, m_str + pos, len);
	delete[] m_str;

	return m_str = tmp_str.Detach();
}

//-----------------------------------------------------------------------------
const bool		String::IsEmpty() const
{	return Size() == 0;	}

//-----------------------------------------------------------------------------
const bool		String::IsNull() const
{	return m_str == NULL;	}

//-----------------------------------------------------------------------------
const int		String::Size() const
{	return St::Len(m_str);	}

//-----------------------------------------------------------------------------
void			String::ToLower()
{
	if (m_str)
		for (int i = 0; i < Size(); i++)
			m_str[i] = tolower(m_str[i]);
}

//-----------------------------------------------------------------------------
void			String::ToUpper()
{
	if (m_str)
		for (int i = 0; i < Size(); i++)
			m_str[i] = toupper(m_str[i]);
}

//-----------------------------------------------------------------------------
const String&	String::Unquotify()
{	return *this = Keep(1, Size() - 2);	}
