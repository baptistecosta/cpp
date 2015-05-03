#ifndef BCOSTA_CORE_BIN_H
#define BCOSTA_CORE_BIN_H

#include <limits.h>
#include "../defines.h"

namespace BCosta {
namespace Core {

#if CHAR_BIT != 8
#error "OWL ERROR: Unsupported char size!"
#endif

enum
{
    Order32_LittleEndian = 0x03020100ul,
    Order32_BigEndian = 0x00010203ul,
    Order32_PDPEndian = 0x01000302ul,
};

static const union
{
    ui8 bytes[4];
    ui32 value;

} host32_order = {{0, 1, 2, 3}};

bool IsLittleEndian();

bool IsBigEndian();

bool IsPDPEndian();

enum
{
    LSB = 0x01, // Least significant bit
    MSB = 0x80  // Most significant bit
};

enum Flags
{
    Flag_0 = 1 << 0,   // 0000 0000 0000 0001	==	0x1
    Flag_1 = 1 << 1,   // 0000 0000 0000 0010	==	0x2
    Flag_2 = 1 << 2,   // 0000 0000 0000 0100	==	0x4
    Flag_3 = 1 << 3,   // 0000 0000 0000 1000	==	0x8
    Flag_4 = 1 << 4,   // 0000 0000 0001 0000	==	0x10
    Flag_5 = 1 << 5,   // 0000 0000 0010 0000	==	0x20
    Flag_6 = 1 << 6,   // 0000 0000 0100 0000	==	0x40
    Flag_7 = 1 << 7,   // 0000 0000 1000 0000	==	0x80

    Flag_8 = 1 << 8,   // 0000 0001 0000 0000 ==	0x100
    Flag_9 = 1 << 9,   // 0000 0010 0000 0000 ==	0x200
    Flag_10 = 1 << 10, // 0000 0100 0000 0000 ==	0x400
    Flag_11 = 1 << 11, // 0000 1000 0000 0000 ==	0x800
    Flag_12 = 1 << 12, // 0001 0000 0000 0000 ==	0x1000
    Flag_13 = 1 << 13, // 0010 0000 0000 0000 ==	0x2000
    Flag_14 = 1 << 14, // 0100 0000 0000 0000 ==	0x4000
    Flag_15 = 1 << 15, // 1000 0000 0000 0000 ==	0x8000

    Flag_16 = 1 << 16, // etc...
    Flag_17 = 1 << 17,
    Flag_18 = 1 << 18,
    Flag_19 = 1 << 19,
    Flag_20 = 1 << 20,
    Flag_21 = 1 << 21,
    Flag_22 = 1 << 22,
    Flag_23 = 1 << 23,

    Flag_24 = 1 << 24,
    Flag_25 = 1 << 25,
    Flag_26 = 1 << 26,
    Flag_27 = 1 << 27,
    Flag_28 = 1 << 28,
    Flag_29 = 1 << 29,
    Flag_30 = 1 << 30,
    Flag_31 = 1 << 31
};

extern void OutputBinary(unsigned char d, const bool l_end = true);

ulint Rot32l(const uint val, const uint steps);

void SwapEndian(uchar *in, uchar *out, const int len);

}
}
#endif	// BCOSTA_CORE_BIN_H
