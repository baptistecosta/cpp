#ifndef __CORE_BIN__
#define __CORE_BIN__

	#include "typedefs.h"

namespace owl
{

enum
{
	LSB		=	0x01,	// Least significant bit
	MSB		=	0x80	// Most significant bit
};

enum	Flags
{
	Flag_0	=	1 << 0,	//	0000 0001	==	0x1
	Flag_1	=	1 << 1,	//	0000 0010	==	0x2
	Flag_2	=	1 << 2,	//	0000 0100	==	0x4
	Flag_3	=	1 << 3,	//	0000 1000	==	0x8
	Flag_4	=	1 << 4,	//	0001 0000	==	0x10
	Flag_5	=	1 << 5,	//	0010 0000	==	0x20
	Flag_6	=	1 << 6,	//	0100 0000	==	0x40
	Flag_7	=	1 << 7	//	1000 0000	==	0x80
};

//	Declaration of variable var
extern int		var;

extern void		OutputBinary(unsigned char d, const bool l_end = true);

ulint			Rot32l(const uint val, const uint steps);

}		//	owl
#endif	//	__CORE_BIN__