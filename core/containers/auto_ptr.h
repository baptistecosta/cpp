/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __AUTO_PTR__
#define __AUTO_PTR__

	#include "defines.h"

namespace owl {

//!
template <class T>	class	AutoPtr
{
private:

		T*					p;

public:
	
		//---------------------------------------------------------------------
		AutoPtr(T* _p = 0) : p(_p)	{}
		~AutoPtr()
		{
			delete p;
			p = 0;
		}
		
		//---------------------------------------------------------------------
		operator T*	() const
		{	return p;	}
		T*					operator ->	() const
		{	return p;	}
		T&					operator *	() const
		{	return *p;	}
		T&					operator []	(uint n) const
		{	return p[n];	}
		T*					operator =	(T*	_p)
		{
			if (p != _p)
			{
				delete p;
				p = _p;
				_p = 0;
			}
			return p;
		}
		bool				operator ==	(const T* _p) const
		{	return p == _p;	}
		bool				operator !=	(const T* _p) const
		{	return p != _p;	}
		
		//---------------------------------------------------------------------
		T*					cPtr()
		{	return p;	}
		T*					Detach()
		{
			T* d = p;
			p = 0;
			return d;
		}
		const bool			IsNull() const
		{	return p == 0;	}
		const bool			IsValid() const
		{	return p != 0;	}
};

}		// owl
#endif	// __AUTO_PTR__