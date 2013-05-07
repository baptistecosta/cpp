#ifndef __COMMON_CRYPTO_SHA_1__
#define __COMMON_CRYPTO_SHA_1__

	#include "typedefs.h"

namespace owl
{

class SHA1
{
static	void			InnerHash(uint* res, uint* w);

public:

static	void			Process(const void* data, const int data_len, unsigned char* hash);
};

}		// owl
#endif	// __COMMON_CRYPTO_SHA_1__