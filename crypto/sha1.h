#ifndef __COMMON_CRYPTO_SHA_1__
#define __COMMON_CRYPTO_SHA_1__

	#include "defines.h"
	#include "hash.h"

namespace owl
{

class SHA1	:	public	Hash
{
static	void			InnerHash(uint* res, uint* w);

public:

static	const int		iterations;

static	void			Process(const void* data, const int data_len, unsigned char* hash);
};

}		// owl
#endif	// __COMMON_CRYPTO_SHA_1__