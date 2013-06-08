/**
	bEngine :: 2011 - 2013
	Baptiste Costa
*/

#ifndef	__OWL_CORE_BSTRING__
#define	__OWL_CORE_BSTRING__

	#include "containers/vector.h"

namespace owl {

//!
class	String	:	public	SharedObject
{
		Vector<char>		v;

public:

		String();
		String(const String& s);
		String(const char* s);
		explicit String(const int size);
		~String()															{	Destroy();	}

		String&				operator +=	(const char);
		String&				operator += (const String&);
		String&				operator += (const char*);
		String&				operator <<	(const char c)						{	*this += c;	return *this;	}
		String&				operator << (const String& s)					{	*this += s;	return *this;	}
		String&				operator << (const char* s)						{	*this += s; return *this;	}

		String&				operator =	(const String&);
		String&				operator =	(const char*);
		String				operator +	(const char c) const				{	String r = *this; r += c; return r;	}
		String				operator +	(const String& s) const				{	String r = *this; r += s; return r;	}
		String				operator +	(const char* s) const				{	String r = *this; r += s; return r;	}

		char				operator []	(const int i);
		const char			operator []	(const int i) const;

		const bool			operator ==	(const String& s) const				{	return Equals(s);	}
		const bool			operator ==	(const char* s) const				{	return Equals(s);	}

		bool				Allocate(const int size)						{	return v.Allocate(size);	}
		void				Clear();
		void				Destroy()										{	v.Destroy();	}

		char*				Detach();
		const bool			Equals(const String& s) const					{	return Equals(s.cStr());	}
		const bool			Equals(const char* s) const						{	return Equals(this->cStr(), s);	}
		const bool			IEquals(const String& s) const					{	return IEquals(s.cStr());	}
		const bool			IEquals(const char* s) const					{	return IEquals(this->cStr(), s);	}
		String				Extract(const int pos, const int len) const;
		const String		FileExtension() const;
		const String		FilenameFromPath() const;
		const int			FindFirstOf(const char*) const;
		const int			FindFirstOfNot(const char*) const;
		const int			FindLastOf(const char*) const;
		const int			FindLastOfNot(const char*) const;
		void				Keep(const int pos, const int len)				{	*this = Extract(pos, len);	}
		const bool			IsEmpty() const									{	return v.Size() == 0;	}
		const bool			IsNumeric() const;
		const String&		RemoveOccurences(const Vector<String>& occurences);
		const int			Size() const									{	return v.Size();	}
		void				ToLower();
		void				ToUpper();
		void				TrimAll();
		void				TrimSpaces();

		char*				cStr()											{	return v.GetData();	}
		const char*			cStr() const									{	return v.GetData();	}

private:

		template<class HaystackCharacter> void Trim();

public:

static	int					Length(const char*);
static	void				Copy(String& dest, const char* src, const int len);
static	void				Copy(char* dest, const char* src);
static	void				Copy(char* dest, const char* src, const int len);

		// Pointer to the destination array, which should contain a C string,
		// and be large enough to contain the concatenated resulting string.
static	void				Concatenate(char* dest, const char* src);
static	String				Concatenate(const char* a, const char* b);

static	const bool			Equals(const char*, const char*);
static	const bool			IEquals(const char*, const char*);

		// Format specifier: http://www.cplusplus.com/reference/clibrary/cstdio/printf/
static	String				Format(const char* format, ...);
static	String				FormatVarg(const char* format, va_list varg);
static	ulong				Hash(const char* str);

static	bool				IsCharInArray(char needle, const char* haystack);

		// Integer to string.
static	char*				IToS(int n, char* str);
static	String				IToS(int n);
};

//---------------------------------------------------------------------------
template<class HaystackCharacter> void owl::String::Trim()
{
	int start = 0, end = 0, size = Size();
	for (int i = 0; i < size; ++i)
	{
		if (!IsCharInArray(v[i], HaystackCharacter::val))
		{
			start = i;
			break;
		}
	}
	for (int i = size - 1; i >= 0; --i)
	{
		if (!IsCharInArray(v[i], HaystackCharacter::val))
		{
			end = i;
			break;
		}
	}
	int len = end - start + 1;
	Keep(start, len);
}

}		//	owl
#endif	//	__OWL_CORE_BSTRING__
