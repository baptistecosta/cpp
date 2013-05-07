/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __FILE_SYSTEM__
#define __FILE_SYSTEM__
	
	#include <stdio.h>

namespace owl {

class	String;

//!
class	FileSystem
{
private:

		FILE*					f;

public:

		FileSystem() : f(NULL)	{}
		~FileSystem();

const	bool					Open(const String& path);
const	bool					Word(String& w) const;
const	bool					Scan(String& line) const;
const	String					Read();
const	int						Size() const;
		void					Close();

inline	FILE*					GetFile() const				{	return f;	}
};

}		// owl
#endif	// __FILE_SYSTEM__
