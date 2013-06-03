/*
	engine :: 2011 - 2013
	Baptiste Costa
*/

#ifndef	__OWL_NETWORK_UTILS__
#define	__OWL_NETWORK_UTILS__

	#include "core/bin.h"
	#include "core/log.h"

namespace owl {

//!
struct	NetworkUtils
{
	//-----------------------------------------------------------------------
	template<typename T> static T FromRawFrame(const ui8* a)
	{
		T r = 0;
		const int s = sizeof(T);
		if (s == 1)
			memcpy(&r, a, s);
		else
		{
			if (IsLittleEndian())
			{
				ui8 t[s], le[s];
				memcpy(t, a, s);
				SwapEndian(t, le, s);
				memcpy(&r, le, s);
			}
			else if (IsBigEndian())
				memcpy(&r, a, s);
			else
				Log::e("Endianess unknown!");
		}
		return r;
	}
};

}		//	owl
#endif	//	__OWL_NETWORK_UTILS__
