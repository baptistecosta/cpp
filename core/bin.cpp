/**
	(.Y.) Hack (.Y.)
*/
	
	#include "bin.h"
	#include "log.h"

	using namespace owl;

//	Definition of variable var
int				var;

//-----------------------------------------------------------------------------
void			owl::OutputBinary(uchar d, const bool l_end)
{
	char buf[9] = { 0 };
	for (int i = 0; i < 8; ++i)
		buf[i] = d & (l_end ? MSB >> i : LSB << i) ? '1' : '0';
	Log::i(buf);
}

//-----------------------------------------------------------------------------
unsigned long int owl::Rot32l(const uint val, const uint steps)
{	return ((val << steps) | (val >> (32 - steps)));	}