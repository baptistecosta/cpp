/*
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

	#include "hash_table.h"
	#include "core/string.h"

using namespace owl;

//-----------------------------------------------------------------------------
unsigned long int owl::StringHasher::Process(const String& str)
{	return owl::String::Hash(str.cStr());	}
