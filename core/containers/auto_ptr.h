/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __AUTO_PTR__
#define __AUTO_PTR__

	#include "typedefs.h"

namespace owl {

//!
template <class T>	class	AutoPtr
{
private:

		T*					p;

public:
	
		//---------------------------------------------------------------------
		AutoPtr(T* _p = NULL)	:	p(_p)	{}
		~AutoPtr()
		{
			delete p;
			p = NULL;
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
				_p = NULL;
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
		T*					detach()
		{
			T* d = p;
			p = NULL;
			return d;
		}
		bool				isNull() const
		{	return p == NULL;	}
		bool				isValid() const
		{	return p != NULL;	}
};

}		// owl
#endif	// __AUTO_PTR__