/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __SHARED_PTR__
#define __SHARED_PTR__

	#include "typedefs.h"
	#include "shared_object.h"

namespace owl {

//!
template
<
	class T
,	template<class> class RefCountingPolicy	=	StandardRefCountPolicy
>
class SharedPtr
{
		T*				ptr;

public:
		SharedPtr()							:	ptr(0)			{}
		SharedPtr(T* ptr)					:	ptr(0)			{	init(ptr);	}
		SharedPtr(const SharedPtr& shptr)	:	ptr(0)			{	init(const_cast<T*>(shptr.Raw()));	}

		~SharedPtr()
		{	RefCountingPolicy<T*>::Decrement(ptr);	}

private:
		void			init(T* _ptr)
		{
			if (ptr = _ptr)
				RefCountingPolicy<T*>::Increment(ptr);
		}

public:
		//!	Operators
		T*				operator -> ()							{	return ptr;	}
		const T*		operator -> () const					{	return ptr;	}
		T&				operator *	()							{	return *ptr;	}
		SharedPtr&		operator =	(const SharedPtr& shptr)	{	return *this = const_cast<T*>(shptr.Raw());	}
		SharedPtr&		operator =	(T* _ptr)
		{
			SharedDataTools<T*>::Copy(_ptr, ptr);
			return *this;
		}
		
		//!	Get raw pointer.
		T*				Raw()									{	return ptr;	}
		const T*		Raw() const								{	return ptr;	}
		//!	Is pointer non null.
		const bool		isValid() const							{	return ptr != 0;	}
		//!	Is pointer null.
		const bool		isNull() const							{	return ptr == 0;	}
};

}		// owl
#endif	//__SHARED_PTR__
