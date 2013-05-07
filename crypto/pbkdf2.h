/*
	Common :: 2011 - 2012
	Baptiste Costa
*/

#ifndef	__CRYPTO_PBKDF2__
#define	__CRYPTO_PBKDF2__

	#include "typedefs.h"

namespace owl {

//!
class	PBKDF2
{
static	void			Hash(uchar* pass, int passlen, uchar* ssid, int ssid_len, int iterations, int ctr, uchar* digest);

public:

static	void			Process(uchar* password, int password_length, uchar* ssid, int ssid_length, uchar* hash);
};

}		//	owl
#endif	//	__CRYPTO_PBKDF2__
