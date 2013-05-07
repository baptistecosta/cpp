	#include "hmac_sha1.h"
	#include "sha1.h"

using namespace owl;

#define	MAX_MSG_LEN	4096

//---------------------------------------------------------------------------
void			HMAC_SHA1::Process(uchar* key, int key_len, uchar* data, int data_len, uchar* digest)
{
	static const int BLOCK_SIZE = 64;
	
	uchar k0[BLOCK_SIZE]  = {0};

	// Step 1
	if (key_len != BLOCK_SIZE)
	{
		// Step 2
		if (key_len > BLOCK_SIZE)
		{
			SHA1::Process(key, key_len, digest);
			for (int i = 0; i < 20; i++)
				k0[i] = digest[i];
		}
		// Step 3
		else if (key_len < BLOCK_SIZE)
			for (int i = 0; i < key_len; i++)
				k0[i] = key[i];
	}
	else
		for (int i = 0; i < BLOCK_SIZE; i++)
			k0[i] = key[i];

	// Step 4
	uchar k0_xor_ipad[BLOCK_SIZE]  = {0};
	uchar ipad = 0x36;
	for (int i = 0; i < BLOCK_SIZE; i++)
		k0_xor_ipad[i] = k0[i] ^ ipad;

	// Step 5
	uchar step_5_data[MAX_MSG_LEN + 128] = {0};
	for (int i = 0; i < BLOCK_SIZE; i++)
		step_5_data[i] = k0_xor_ipad[i];

	for (int i = 0; i < data_len; i++)
		step_5_data[i + BLOCK_SIZE] = data[i];

	// Step 6
	SHA1::Process(step_5_data, data_len + BLOCK_SIZE, digest);

	// Step 7
	uchar step_7_data[BLOCK_SIZE] = {0};
	uchar opad = 0x5c;
	for (int i = 0; i < BLOCK_SIZE; i++)
		step_7_data[i] = k0[i] ^ opad;

	// Step 8
	uchar step_8_data[BLOCK_SIZE + 20] = {0};
	for (int i = 0; i < BLOCK_SIZE; i++)
		step_8_data[i] = step_7_data[i];

	for (int i = 0; i < 20; i++)
		step_8_data[i + BLOCK_SIZE] = digest[i];

	// Step 9
	SHA1::Process(step_8_data, BLOCK_SIZE + 20, digest);
}
