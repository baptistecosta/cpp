	#include "sha1.h"
	#include "core/bin.h"

	using namespace owl;

//---------------------------------------------------------------------------
void			SHA1::InnerHash(uint* res, uint* w)
{
	uint a = res[0];
	uint b = res[1];
	uint c = res[2];
	uint d = res[3];
	uint e = res[4];

#define sha1macro(func, val) \
{ \
	const uint t = Rot32l(a, 5) + (func) + e + val + w[round]; \
	e = d; \
	d = c; \
	c = Rot32l(b, 30); \
	b = a; \
	a = t; \
}

	int round = 0;
	while (round < 16)
	{
		sha1macro((b & c) | (~b & d), 0x5a827999)
			++round;
	}
	while (round < 20)
	{
		w[round] = Rot32l((w[round - 3] ^ w[round - 8] ^ w[round - 14] ^ w[round - 16]), 1);
		sha1macro((b & c) | (~b & d), 0x5a827999)
			++round;
	}
	while (round < 40)
	{
		w[round] = Rot32l((w[round - 3] ^ w[round - 8] ^ w[round - 14] ^ w[round - 16]), 1);
		sha1macro(b ^ c ^ d, 0x6ed9eba1)
			++round;
	}
	while (round < 60)
	{
		w[round] = Rot32l((w[round - 3] ^ w[round - 8] ^ w[round - 14] ^ w[round - 16]), 1);
		sha1macro((b & c) | (b & d) | (c & d), 0x8f1bbcdc)
			++round;
	}
	while (round < 80)
	{
		w[round] = Rot32l((w[round - 3] ^ w[round - 8] ^ w[round - 14] ^ w[round - 16]), 1);
		sha1macro(b ^ c ^ d, 0xca62c1d6)
			++round;
	}
#undef sha1macro

	res[0] += a;
	res[1] += b;
	res[2] += c;
	res[3] += d;
	res[4] += e;
}

//---------------------------------------------------------------------------
void			SHA1::Process(const void* data, const int data_len, unsigned char* hash)
{
	// Init the result array
	uint res[5] = { 0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476, 0xc3d2e1f0 };

	// Cast the void src pointer to be the byte array we can work with
	const unsigned char* sarray = (const unsigned char*)data;

	// The reusable round buffer
	uint w[80];

	// Loop through all complete 64byte blocks.
	const int full_blocks_end = data_len - 64;
	int cur_block_end;
	int cur_block = 0;

	while (cur_block <= full_blocks_end)
	{
		cur_block_end = cur_block + 64;

		// Init the round buffer with the 64 byte block data
		for (int round_pos = 0; cur_block < cur_block_end; cur_block += 4)
		{
			// This line will swap endian on big endian and keep endian on little endian
			w[round_pos++] =
				(uint) sarray[cur_block + 3]
			|	(((uint) sarray[cur_block + 2]) << 8)
			|	(((uint) sarray[cur_block + 1]) << 16)
			|	(((uint) sarray[cur_block]) << 24);
		}
		InnerHash(res, w);
	}

	// Handle the last and not full 64 byte block if existing
	cur_block_end = data_len - cur_block;

	for (int pos = 16; --pos >= 0;)
		w[pos] = 0;

	int last_block_bytes = 0;
	for (;last_block_bytes < cur_block_end; ++last_block_bytes)
	{
		w[last_block_bytes >> 2] |= (uint) sarray[last_block_bytes + cur_block] << ((3 - (last_block_bytes & 3)) << 3);
	}
	w[last_block_bytes >> 2] |= 0x80 << ((3 - (last_block_bytes & 3)) << 3);

	if (cur_block_end >= 56)
	{
		InnerHash(res, w);
		for (int pos = 16; --pos >= 0;)
			w[pos] = 0;
	}
	w[15] = data_len << 3;

	InnerHash(res, w);

	// Store hash in result pointer, and make sure we get in in the correct order on both endian models
	for (int hash_byte = 20; --hash_byte >= 0;)
	{
		hash[hash_byte] = (res[hash_byte >> 2] >> (((3 - hash_byte) & 0x3) << 3)) & 0xff;
	}
}
