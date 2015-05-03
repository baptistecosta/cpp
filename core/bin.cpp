#include "bin.h"

using namespace BCosta::Core;

bool BCosta::Core::IsLittleEndian()
{ return host32_order.value == Order32_LittleEndian; }

bool BCosta::Core::IsBigEndian()
{ return host32_order.value == Order32_BigEndian; }

bool BCosta::Core::IsPDPEndian()
{ return host32_order.value == Order32_PDPEndian; }

void BCosta::Core::OutputBinary(uchar d, const bool l_end)
{
    char buf[9] = {0};
    for (int i = 0; i < 8; ++i) {
        buf[i] = d & (l_end ? MSB >> i : LSB << i) ? '1' : '0';
    }
}

unsigned long int BCosta::Core::Rot32l(const unsigned int val, const unsigned int steps)
{ return ((val << steps) | (val >> (32 - steps))); }

void BCosta::Core::SwapEndian(unsigned char *in, unsigned char *out, const int len)
{
    for (int i = 0; i < len; i++) {
        out[i] = in[len - i - 1];
    }
}
