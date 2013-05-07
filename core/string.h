/**
	Common	::	2011 - 2013
	@author		Baptiste Costa
*/

#ifndef __STRING__
#define __STRING__

	#include <stdarg.h>

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

		char*				m_str;

public:
		//---------------------------------------------------------------------
		String()						: m_str(0)		{}
		String(const String& b)			: m_str(0)		{	operator = (b);	}
		String(const char* str)			: m_str(0)		{	operator = (str);	}
		explicit String(const int size)	: m_str(0)		{	Alloc(size);	}

		~String()										{	delete [] m_str;	}

		//---------------------------------------------------------------------
const	char				operator [] (const int i) const;

		String&				operator =	(const String& b);
		String&				operator =	(const char* str);
		String				operator +	(const String b);

		void				operator +=	(const char* str);
		void				operator +=	(const String& b);
		bool				operator == (const char* str) const;
		bool				operator == (const String& str) const;

		bool				operator <	(const String& str) const;

		//---------------------------------------------------------------------
		bool				Alloc(const int size);
		String				Append(const String& b);
		char*				Begin() const;
		bool				CaseCmp(const String& b) const;
		bool				Compare(const int pos, const int len, const char* code);
		char*				cStr()												{	return m_str;	};
const	char*				cStr() const										{	return m_str;	};
//		bool				contain(const char* _code);
		// The class won't be responsible anymore of the object pointed by str.
		// The string object won't be deleted when the destructor is called.
		char*				Detach();
		char*				End() const;
		String				Erase(const int pos);
		String				Erase(const int pos, int size);
		const int			FindFirstOf(const char* code);
		const int			FindLastOf(const char* code);
		
const	String				FileExtension() const;
const	String				FilenameFromPath() const;
const	String				Keep(const int pos, const int size);
const	bool				IsEmpty() const;
const	bool				IsNull() const;
		String				Prepend(const String& str);
const	int					Size() const;
		void				ToLower();
		void				ToUpper();
const	String&				Unquotify();

		// Format specifier: http://www.cplusplus.com/reference/clibrary/cstdio/printf/
static	String				Format(const char* str_format, ...);
};

}		// owl
#endif	// __STRING__

