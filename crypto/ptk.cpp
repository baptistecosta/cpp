/*
	bEngine :: 2011 - 2013
	Baptiste Costa
*/

	#include <string.h>
	#include "ptk.h"
	#include "hmac.h"
	#include "sha1.h"
	
	using namespace owl;

//---------------------------------------------------------------------------
void			PTK::Process(__i uchar psk[32], __i uchar amac[6], __i uchar smac[6], __i uchar anounce[32], __i uchar snounce[32], __o uchar ptk[64])
{
	static const int	MAC_ADDR_LEN = 6,
						NOUNCE_LEN = 32,
						MACS_NOUNCES_PKT_LEN = (2 * MAC_ADDR_LEN) + (2 * NOUNCE_LEN);

	uchar data[MACS_NOUNCES_PKT_LEN] = {0};
	if (memcmp(amac, smac, MAC_ADDR_LEN) < 0)
	{
		memcpy(data, amac, MAC_ADDR_LEN);
		memcpy(data + MAC_ADDR_LEN,	smac, MAC_ADDR_LEN);
	}
	else
	{
		memcpy(data, smac, MAC_ADDR_LEN);
		memcpy(data + MAC_ADDR_LEN,	amac, MAC_ADDR_LEN);
	}
	if (memcmp(anounce, snounce, NOUNCE_LEN) < 0)
	{
		memcpy(data + (2 * MAC_ADDR_LEN), anounce, NOUNCE_LEN);
		memcpy(data + (2 * MAC_ADDR_LEN) + NOUNCE_LEN,	snounce, NOUNCE_LEN);
	}
	else
	{
		memcpy(data + (2 * MAC_ADDR_LEN), snounce, NOUNCE_LEN);
		memcpy(data + (2 * MAC_ADDR_LEN) + NOUNCE_LEN,	anounce, NOUNCE_LEN);
	}

	uchar pke[100] = {0};
	memcpy(pke, "Pairwise key expansion", 22);
	memcpy(pke + 23, data, MACS_NOUNCES_PKT_LEN);

	uchar _ptk[80] = {0};
	for (int i = 0; i < 4; ++i)
	{
		pke[99] = i;
		HMAC<SHA1>::Process(psk, 32, pke, 100, _ptk + i * 20);
	}
	memcpy(ptk, _ptk, 64);
}
