/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __VECTOR__
#define __VECTOR__

	#include <assert.h>
	#include "shared_object.h"
	#include "core/traits.h"

namespace owl {

//!
template <class T>	struct AutoVectorPolicy
{
static	void			onInit(T* data, int size)
		{
			for (int i = 0; i < size; ++i)
				data[i] = 0;
		}

static	void			onPush(T p) {}
static	void			onPull(T p)
		{	delete p;	}

static	void			onClear(T* data, int size)
		{
			for (int i = 0; i < size; ++i)
			{
				assert(data[i]);
				delete data[i];
			}
		}
};

//!
template <class T>	struct SharedVectorPolicy
{
	static void			onInit(T* data, int size)
	{
		for (int i = 0; i < size; ++i)
			data[i] = 0;
	}

	static void			onPush(T p)
	{	assert(p); p->IncRef();	}

	static void			onPull(T p)
	{	assert(p); p->DecRef();	}

	static void			onClear(T* data, int size)
	{
		for (int i = 0; i < size; ++i)
		{
			assert(data[i]);
			data[i]->decRef();
		}
	}
};

//!
template <class T>	struct StandardVectorPolicy
{
static	void			onInit(T* data, int size) {}
static	void			onPush(T p) {}
static	void			onPull(T p) {}
static	void			onClear(T* data, int size) {}
};

//!
template
<
	class T
,	template<class> class LifetimePolicy	=	StandardVectorPolicy
>
class Vector
{
public:

		enum
		{	DefCapacity	=	16	};

		const bool		is_t_ptr;

protected:
		
		T*				m_data;
		int				m_size,
						m_capacity;

public:

		Vector()
			: m_data(0)
			, m_size(0)
			, m_capacity(DefCapacity)
			, is_t_ptr(IsPointer<T>::val)
		{
			Alloc(m_capacity);
		}

		Vector(const int size)
			: m_data(0)
			, m_size(size)
			, m_capacity(size > DefCapacity ? size + DefCapacity : DefCapacity)
			, is_t_ptr(IsPointer<T>::val)
		{
			Alloc(m_capacity);
		}

		~Vector()
		{
			Clear();
		}

		T&				operator []	(int index)
		{	return m_data[index];	}
		const T&		operator []	(int index) const
		{	return m_data[index];	}

		Vector<T>&		operator << (const T &o)
		{
			Push(o);
			return *this;
		}

private:

		bool			Alloc(int count)
		{
			delete [] m_data;
			m_data = new T[count];
			if (m_data)
			{
				for (int i = 0; i < m_capacity; ++i)
					m_data[i] = 0;
				LifetimePolicy<T>::onInit(m_data, count);
				return true;
			}
			return false;
		}

public:

		bool			Realloc(int size)
		{
			m_size = size;
			m_capacity = size > DefCapacity ? size + DefCapacity : DefCapacity;
			return Alloc(m_capacity);
		}

		bool			Push(T item)
		{
			if (m_size == m_capacity)
				if (!IncreaseCapacity())
					return false;

			m_data[m_size++] = item;
			LifetimePolicy<T>::onPush(item);

			return true;
		}

		void			Pull()
		{
			LifetimePolicy<T>::onPull(m_data[m_size - 1]);
			--m_size;
		}

		bool			IncreaseCapacity(int increase_size = DefCapacity)
		{
			// Create a temporary array with the proper size.
			int new_capacity = m_size + increase_size;
			T* tmp = new T[new_capacity];
			if (!tmp)
				return false;

			LifetimePolicy<T>::onInit(tmp, new_capacity);

			for (int i = 0; i < m_size ; i++)
				tmp[i] = m_data[i];
			
			// Refresh capacity.
			m_capacity = new_capacity;

			delete [] m_data;
			m_data = tmp;
			tmp = 0;
			
			return true;
		}

		T*				GetData() const
		{	return m_data;	}
		T				GetData(int i) const
		{	assert(i >= 0 && i < m_size); return m_data[i];	}

		const int		Size() const
		{	return m_size;	}
		const int		GetCapacity() const
		{	return m_capacity;	}
		
		const bool		IsNull() const
		{	return m_data == 0;	}

		void			Clear()
		{
			LifetimePolicy<T>::onClear(m_data, m_size);
			delete [] m_data;
			m_data = 0;
			m_size = 0;
			m_capacity = DefCapacity;
		}
};

//!
template <class T>	struct AutoVector	{	typedef	Vector<T*, AutoVectorPolicy>	type;	};
template <class T>	struct SharedVector	{	typedef	Vector<T*, SharedVectorPolicy>	type;	};

}		// owl
#endif	// __VECTOR__
