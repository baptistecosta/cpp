/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __BITVECTOR__
#define __BITVECTOR__

	#include "defines.h"

// OPTIMIZING SMALL DATA ITEMS.
// Meant to condense bit values (or booleans) into an array so that no space is wasted.

//!
class BitVector
{
protected:

	ulong*				data;
	uint				size;

public:

	BitVector(uint _size)
	{
		data = null_ptr;
		size = 0;
		Resize(_size);
	}

	~BitVector()
	{
		if (data != 0)
			delete[]data;
		data = null_ptr;
	}

	bool				operator [] (uint index)
	{
		uint cell = index / 32;
		uint bit = index % 32;
		return (data[cell] & (1 << bit)) >> bit;
	}

	void				Resize(uint _size)
	{
		if (_size % 32 == 0)
			_size = _size / 32;
		else
			_size = (_size / 32) + 1;

		uint* temp = new uint[_size];
		if (temp == 0)
			return;

		uint min;
		if (_size < size)
			min = _size;
		else
			min = size;

		for (uint i = 0; i < min; i++)
			temp[i] = data[i];
		size = _size;

		if (data != 0)
			delete[]data;
		data = temp;
	}

	void				Set(uint index, bool b)
	{
		uint cell = index / 32;
		uint bit = index % 32;

		if (b)
			data[cell] = (data[cell] | (1 << bit));
		else
			data[cell] = (data[cell] & (~(1 << bit)));
	}

	void				ClearAll()
	{
		for(uint i = 0; i < size; i++)
			data[i] = 0;
	}

	void				SetAll()
	{
		for(uint i = 0; i < size; i++)
			data[i] = 0xFFFFFFFF;
	}
};

#endif // __BITVECTOR__