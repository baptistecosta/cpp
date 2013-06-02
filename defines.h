/**
	Common	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __OWL_DEFINES__
#define __OWL_DEFINES__

#include <limits.h>

#ifndef	__i
#define	__i
#endif

#ifndef	__o
#define	__o
#endif

#define	null_ptr	0

namespace owl {


typedef		unsigned long				ulong;
typedef		unsigned int				uint;
typedef		unsigned long int			ulint;
typedef		unsigned long long			ullong;
typedef		unsigned short				ushort;
typedef		unsigned char				uchar;

typedef		char						i8;
typedef		unsigned char				ui8;
typedef		short						i16;
typedef		unsigned short				ui16;
typedef		int							i32;
typedef		unsigned int				ui32;
#if			ULONG_MAX == 0xffffffffUL
	typedef	long long					i64;
	typedef	unsigned long long			ui64;
#elif		ULONG_MAX == 0xffffffffffffffffull
	typedef	long						i64;
	typedef	unsigned long				ui64;
#else
	#error	OWL: "defines.h", ULONG_MAX != 0xffffffffUL and ULONG_MAX != 0xffffffffffffffffull
#endif

}		//	owl
#endif	//	__OWL_DEFINES__
