#ifndef __CRYPTO_HMAC_SHA_1__
#define __CRYPTO_HMAC_SHA_1__

	#include "typedefs.h"

namespace owl
{

struct HMAC_SHA1
{
static	void			Process(uchar *key, int key_len, uchar* data, int data_len, uchar* digest);
};

}		// owl
#endif	// __CRYPTO_HMAC_SHA_1__