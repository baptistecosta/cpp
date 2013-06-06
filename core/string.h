/**
	Common	::	2011 - 2013
	@author		Baptiste Costa
*/

#ifndef __STRING__
#define __STRING__

	#include <stdarg.h>
	#include "containers/vector.h"

namespace owl {

class	String;

//!
typedef	struct	StringTools
{
		enum
		{	NullChar	=	0	};

		//! Numeration system base
		enum	NumSysBase
		{
			Binary		=	2,
			Decimal		=	10,
			Hexadecimal	=	16
		};

		//!	Integer to string max length
		enum	IToS
		{	MaxLen	=	64	};

static	int					Len(const char* cstr);
static	char*				Cpy(char* dest, const char* src);
static	char*				Cpy(char* dest, const char* src, const int length);
static	char*				Cat(char* dest, const char* src);
static	bool				Cmp(const char* a, const char* b);
static	bool				CaseCmp(const char* a, const char* b);
static	char*				IToS(int val, char* buf_start_addr);
static	String				IToS(int n);

		//! Format specifier: http://www.cplusplus.com/reference/clibrary/cstdio/printf/
static	String				Format(const char* str_format, va_list varg);

static	unsigned long int	Hash(const char* str);
} St;

//!
class	String
{
private:

		char*				ptr;

public:
		//---------------------------------------------------------------------
		String()						: ptr(0)		{}
		String(const String& b)			: ptr(0)		{	operator = (b);	}
		String(const char* str)			: ptr(0)		{	operator = (str);	}
		explicit String(const int size)	: ptr(0)		{	Alloc(size);	}

		~String()										{	delete [] ptr;	}

		//---------------------------------------------------------------------
		const char			operator [] (const int i) const;

		String&				operator =	(const char*);
		String&				operator =	(const String&);
		String				operator +	(const char c);
		String				operator +	(const char*);
		String				operator +	(const String);

		void				operator +=	(const char);
		void				operator +=	(const char*);
		void				operator +=	(const String&);
		bool				operator == (const char*) const;
		bool				operator == (const String&) const;

		bool				operator <	(const String&) const;

		//---------------------------------------------------------------------
		bool				Alloc(const int size);
		String				Append(const String&);
		char*				Begin() const;
		bool				CaseCmp(const String&) const;
		bool				Compare(const int pos, const int len, const char* code);
		char*				cStr()						{	return ptr;	};
		const char*			cStr() const				{	return ptr;	};
//		bool				contain(const char* _code);
		// The class won't be responsible anymore of the object pointed by str.
		// The string object won't be deleted when the destructor is called.
		char*				Detach();
		char*				End() const;
		String				Erase(const int pos);
		String				Erase(const int pos, int size);
		const int			FindFirstOf(const char*);
		const int			FindFirstOfNot(const char*);
		const int			FindLastOf(const char*);
		const int			FindLastOfNot(const char*);
		
		const String		FileExtension() const;
		const String		FilenameFromPath() const;
		String				Extract(const int pos, const int size);
		void				Keep(const int pos, const int size);
		const bool			IsEmpty() const;
		const bool			IsNull() const;
		const bool			IsNumeric() const;
		String				Prepend(const String&);
		const String&		RemoveOccurences(const Vector<String>&);
		const int			Size() const;
		void				ToLower();
		void				ToUpper();
		bool				IsCharInArray(char needle, const char* haystack);
		void				TrimAll();
		void				TrimSpaces();
		const String&		Unquotify();

		// Format specifier: http://www.cplusplus.com/reference/clibrary/cstdio/printf/
static	String				Format(const char* str_format, ...);

private:
		template<class HaystackCharacter> void Trim();

};

//---------------------------------------------------------------------------
template<class HaystackCharacter> void owl::String::Trim()
{
	int start = 0, end = 0, size = Size();
	for (int i = 0; i < size; ++i)
	{
		if (!IsCharInArray(ptr[i], HaystackCharacter::val))
		{
			start = i;
			break;
		}
	}
	for (int i = size - 1; i >= 0; --i)
	{
		if (!IsCharInArray(ptr[i], HaystackCharacter::val))
		{
			end = i;
			break;
		}
	}
	int len = end - start + 1;
	Keep(start, len);
}

}		// owl
#endif	// __STRING__

