/*
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

	#include "hash_table.h"
	#include "core/string.h"

using namespace owl;

//-----------------------------------------------------------------------------
unsigned long int StringHasher::process(const String& str)
{	return StringTools::Hash(str.cStr());	}
