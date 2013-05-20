/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __SHARED_OBJECT__
#define __SHARED_OBJECT__

	#include "typedefs.h"

namespace owl {

//!	\brief	Base class for reference counted object used by shared containers.
class	SharedObject
{
	int				ref_ct;

public:

	SharedObject()	:	ref_ct(0) {}
	virtual ~SharedObject()	{}

	int				GetRef()			{	return ref_ct;	}
	int				IncRef()			{	return ++ref_ct;	}
	void			DecRef()
	{
		if (--ref_ct == 0)
			delete this;
	}

	virtual	void	Log() const;
};

//!	\brief	Standard reference counting policy for shared containers.
template <class T>	struct StandardRefCountPolicy
{
	static void		Increment(T p)		{	p->IncRef();	}
	static void		Decrement(T p)		{	p->DecRef();	}
};

//!
template
<
	class T
,	template<class> class RefCountPolicy = StandardRefCountPolicy
>
struct	SharedDataTools 
{
	//!	Copy a SharedObject pointer value in an other, automatically incrementing pointed object's reference count if it exists.
	//!	The object pointed by the crushed ReferencedObject pointer will be automatically deleted if his reference count fall to 0.
	static void		Copy(T src, T& dest)
	{
		if (src != 0)
			RefCountPolicy<T>::Increment(src);
		if (dest != 0)
			RefCountPolicy<T>::Decrement(dest);
		dest = src;
	}

	//!	Delete a shared object if non null and his reference count count fall to 0.
//	template<class T> static void safeDelete(T* data)
//	{
//		if (data != null_ptr)
//			data->decrementRefCount();
//	}
};

}		// owl
#endif	// __SHARED_OBJECT__