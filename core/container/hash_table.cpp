#include "hash_table.h"

using namespace BCosta::Core::Container;

unsigned long int StringHasher::Process(const String &str)
{ return String::Hash(str.cStr()); }
