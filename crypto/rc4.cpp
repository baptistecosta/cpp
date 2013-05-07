	#include <stdio.h>
	#include "rc4.h"

//---------------------------------------------------------------------------
RC4::RC4()
{
	for (int i = 0; i < ARRAY_LENGTH; i++)
		s[i] = i;

	rc4_i = rc4_j = 0;
}

//---------------------------------------------------------------------------
void			RC4::KSA(unsigned char* key, int key_len)
{
	for (rc4_i = 0; rc4_i < ARRAY_LENGTH; rc4_i++)
	{
		rc4_j = (rc4_j + s[rc4_i] + key[rc4_i % key_len]) % ARRAY_LENGTH;
		Swap(&s[rc4_i], &s[rc4_j]);
	}
	rc4_i = rc4_j = 0;
}

//---------------------------------------------------------------------------
char			RC4::PRGA()
{
	rc4_i = (rc4_i + 1) % ARRAY_LENGTH;
	rc4_j = (rc4_j + s[rc4_i]) % ARRAY_LENGTH;
	Swap(&s[rc4_i], & s[rc4_j]);
	return s[(s[rc4_i] + s[rc4_j]) % ARRAY_LENGTH];
}

//---------------------------------------------------------------------------
void			RC4::Swap(unsigned char* a, unsigned char* b)
{
	char c = *a;
	*a = *b;
	*b = c;
}