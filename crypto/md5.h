#ifndef __OWL_CRYPTO_MD5__
#define __OWL_CRYPTO_MD5__

	#include "defines.h"

namespace owl {

// This package supports both compile-time and run-time determination of CPU
// byte order.  If ARCH_IS_BIG_ENDIAN is defined as 0, the code will be
// compiled to run only on little-endian CPUs; if ARCH_IS_BIG_ENDIAN is
// defined as non-zero, the code will be compiled to run only on big-endian
// CPUs; if ARCH_IS_BIG_ENDIAN is not defined, the code will be compiled to
// run on either big- or little-endian CPUs, but will run slightly less
// efficiently on either one than if ARCH_IS_BIG_ENDIAN is defined.

//!
class	MD5
{
		uchar			buf[64];	// Accumulate block
		uint			count[2],	// Message length in bits, lsw first
						abcd[4];	// Digest buffer

		void			Compute(const uchar* data);

public:

		MD5();
		// Append a string to the message
		void			Append(const uchar* data, int nbytes);
		// Finish the message and return the digest
		void			Finish(uchar digest[16]);

static	void			Process(__i const void* data, __i const int data_len, __o uchar* hash);
};

}		//	owl
#endif	//	__OWL_CRYPTO_MD5__
