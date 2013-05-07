/*
	Common :: 2011 - 2012
	Baptiste Costa
*/

	#include "pbkdf2.h"
	#include "hmac_sha1.h"
	
using namespace owl;

//---------------------------------------------------------------------------
void			PBKDF2::Hash(uchar* pass, int pass_len, uchar* ssid, int ssid_len, int iterations, int ctr, uchar* hash)
{
	uchar ssid_dgst[36] = {0};
	for (int i = 0; i < ssid_len; i++)
		ssid_dgst[i] = ssid[i];

	ssid_dgst[ssid_len] = (uchar)((ctr / (256 * 256 * 256)) & 0xff);
	ssid_dgst[ssid_len + 1] = (uchar)((ctr / (256 * 256)) & 0xff);
	ssid_dgst[ssid_len + 2] = (uchar)((ctr / 256) & 0xff);
	ssid_dgst[ssid_len + 3] = (uchar)(ctr & 0xff);

	unsigned char digest[20] = {0};
	HMAC_SHA1::Process(pass, pass_len, ssid_dgst, ssid_len + 4, digest);

	for (int j = 0; j < 20; j++)
		hash[j] = digest[j];

	for (int i = 0; i < (iterations - 1); i++)
	{
		HMAC_SHA1::Process(pass, pass_len, digest, 20, ssid_dgst);

		for (int j = 0; j < 20; j++)
			digest[j] = ssid_dgst[j];
		for (int j = 0; j < 20; j++)
			hash[j] = hash[j] ^ ssid_dgst[j];
	}
}

//---------------------------------------------------------------------------
void			PBKDF2::Process(uchar* password, int password_length, uchar* ssid, int ssid_length, uchar* hash)
{
	Hash(password, password_length, ssid, ssid_length, 4096, 1, hash);
	Hash(password, password_length, ssid, ssid_length, 4096, 2, hash + 20);
}
