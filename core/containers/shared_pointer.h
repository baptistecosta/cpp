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
		T*				m_ptr;

public:
		SharedPtr()							:	m_ptr(NULL)				{}
		SharedPtr(T* ptr)					:	m_ptr(NULL)				{	init(ptr);	}
		SharedPtr(const SharedPtr& shptr)	:	m_ptr(shptr.cPtr())		{	init(shptr.cPtr());	}

		~SharedPtr()
		{
			RefCountingPolicy<T*>::Decrement(m_ptr);
		}

private:
		void			init(T* ptr)
		{
			if (m_ptr = ptr)
				RefCountingPolicy<T*>::Increment(m_ptr);
		}

public:
		//!	Operators
		T*				operator -> ()									{	return m_ptr;	}
		T&				operator *	()									{	return *m_ptr;	}
		SharedPtr&		operator =	(const SharedPtr& shptr)			{	return *this = shptr.cPtr();	}
		SharedPtr&		operator =	(T* ptr)
		{
			SharedDataTools<T*>::Copy(ptr, m_ptr);
			return *this;
		}
		
		//!	Get raw pointer.
		T*				cPtr()											{	return m_ptr;	}
const	T*				cPtr() const									{	return m_ptr;	}
		//!	Is pointer non null.
const	bool			isValid() const									{	return m_ptr != NULL;	}
		//!	Is pointer null.
const	bool			isNull() const									{	return m_ptr == NULL;	}
};

}		// owl
#endif	//__SHARED_PTR__
