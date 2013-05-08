/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef	__TRAITS__
#define	__TRAITS__

namespace owl {

//! IsPointer traits
template <typename T>	struct IsPointer		{	static const bool val = false;	};
template <typename T>	struct IsPointer<T*>	{	static const bool val = true;	};

//! TEST
template <int N>		struct TemplateInt			{	static const int val = N;	};

}		// owl
#endif	// __TRAITS__