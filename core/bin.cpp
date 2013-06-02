/**
	(.Y.) Hack (.Y.)
*/
	
	#include "bin.h"
	#include "log.h"

	using namespace owl;

//	Definition of variable var
int				var;

//---------------------------------------------------------------------------
bool			owl::IsLittleEndian()	{	return host32_order.value == Order32_LittleEndian;	}
bool			owl::IsBigEndian()		{	return host32_order.value == Order32_BigEndian;	}
bool			owl::IsPDPEndian()		{	return host32_order.value == Order32_PDPEndian;	}

//---------------------------------------------------------------------------
void			owl::OutputBinary(uchar d, const bool l_end)
{
	char buf[9] = { 0 };
	for (int i = 0; i < 8; ++i)
		buf[i] = d & (l_end ? MSB >> i : LSB << i) ? '1' : '0';
	Log::i(buf);
}

//---------------------------------------------------------------------------
unsigned long int owl::Rot32l(const uint val, const uint steps)
{	return ((val << steps) | (val >> (32 - steps)));	}

//---------------------------------------------------------------------------
void			owl::SwapEndian(uchar* in, uchar* out, const int len)
{
	for (int i = 0; i < len; i++)
		out[i] = in[len - i - 1];
}
