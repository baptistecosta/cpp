/*
	bEngine :: 2011 - 2013
	Baptiste Costa
*/

#ifndef	__OWL_CRYPTO_PTK__
#define	__OWL_CRYPTO_PTK__

	#include "defines.h"

namespace owl {

//!
struct	PTK
{
static	void			Process(__i uchar psk[32], __i uchar amac[6], __i uchar smac[6], __i uchar anounce[32], __i uchar snounce[32], __o uchar ptk[64]);
};

}		//	owl
#endif	//	__OWL_CRYPTO_PTK__
