/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __CONTAINERS_VECTOR__
#define __CONTAINERS_VECTOR__

	#include <string.h>
	#include <assert.h>
	#include "core/traits.h"
	#include "defines.h"
	#include "shared_object.h"

namespace owl {

//!
template <class T>	struct AutoVectorPolicy
{
static	void			OnInit(T* data, int size)
		{
			for (int i = 0; i < size; ++i)
				data[i] = 0;
		}
static	void			OnCopy(T*, int)	{}
static	void			OnClear(T* data, int size)
		{
			for (int i = 0; i < size; ++i)
				delete data[i];
		}
static	void			OnPush(T)		{}
static	void			OnPull(T p)		{	delete p;	}
};

//!
template <class T>	struct SharedVectorPolicy
{
static	void			OnInit(T* data, int size)
		{
			for (int i = 0; i < size; ++i)
				data[i] = 0;
		}
static	void			OnCopy(T* data, int size)
		{
			for (int i = 0; i < size; ++i)
				data[i]->IncRef();
		}
static	void			OnClear(T* data, int size)
		{
			for (int i = 0; i < size; ++i)
				data[i]->DecRef();
		}
static	void			OnPush(T p)		{	p->IncRef();	}
static	void			OnPull(T p)		{	p->DecRef();	}
};

//!
template <class T>	struct StandardVectorPolicy
{
static	void			OnInit(T*, int) {}
static	void			OnCopy(T*, int) {}
static	void			OnClear(T*, int) {}
static	void			OnPush(T) {}
static	void			OnPull(T) {}
};

//!
template
<
	class T,
	template<class> class LifetimePolicy	=	StandardVectorPolicy
>
class Vector
{
public:

		enum
		{	DefCapacity	=	16	};

		const bool		is_t_ptr;

protected:
		
		T*				data;
		int				size, capacity;

public:

		Vector()
			: is_t_ptr(IsPointer<T>::val)
			, data(0)
			, size(0)
			, capacity(DefCapacity)
		{
			Alloc(capacity);
		}

		Vector(const int _size)
			: is_t_ptr(IsPointer<T>::val)
			, data(0)
			, size(_size)
			, capacity(_size > DefCapacity ? _size + DefCapacity : DefCapacity)
		{
			Alloc(capacity);
		}

		Vector(const Vector& v)
			: is_t_ptr(IsPointer<T>::val)
			, data(v.data)
			, size(v.size)
			, capacity(v.capacity)
		{
			LifetimePolicy<T>::OnCopy(data, size);
		}

		~Vector()
		{	Clear();	}

		Vector&			operator = (const Vector& v)
		{
			Clear();
			data = v.data;
			size = v.size;
			capacity = v.capacity;
			LifetimePolicy<T>::OnCopy(data, size);
			return *this;
		}
		T&				operator []	(int index)
		{	return data[index];	}
		const T&		operator []	(int index) const
		{	return data[index];	}

		Vector<T>&		operator << (const T &o)
		{
			Push(o);
			return *this;
		}

private:

		bool			Alloc(int count)
		{
			delete [] data;
			data = new T[count];
			if (data)
			{
				memset(data, 0, count);
				LifetimePolicy<T>::OnInit(data, count);
				return true;
			}
			return false;
		}

public:

		T*				Detach()
		{
			T* t = data;
			data = 0;
			return t;
		}

		bool			Realloc(int _size)
		{
			size = _size;
			capacity = _size > DefCapacity ? _size + DefCapacity : DefCapacity;
			return Alloc(capacity);
		}

		bool			Push(T item)
		{
			if (size == capacity)
				if (!IncreaseCapacity())
					return false;

			data[size++] = item;
			LifetimePolicy<T>::OnPush(item);

			return true;
		}

		void			Pull()
		{
			LifetimePolicy<T>::OnPull(data[size - 1]);
			--size;
		}

		bool			IncreaseCapacity(int increase_size = DefCapacity)
		{
			// Create a temporary array with the proper size.
			int new_capacity = size + increase_size;
			T* tmp = new T[new_capacity];
			if (!tmp)
				return false;

			LifetimePolicy<T>::OnInit(tmp, new_capacity);

			for (int i = 0; i < size ; i++)
				tmp[i] = data[i];
			
			// Refresh capacity.
			capacity = new_capacity;

			delete [] data;
			data = tmp;
			tmp = 0;
			
			return true;
		}

		T*				GetData() const
		{	return data;	}
		T				GetData(int i) const
		{	assert(i >= 0 && i < size); return data[i];	}

		const int		Size() const
		{	return size;	}
		const int		GetCapacity() const
		{	return capacity;	}
		
		const bool		IsNull() const
		{	return data == 0;	}

		void			Clear()
		{
			LifetimePolicy<T>::OnClear(data, size);
			delete [] data;
			data = 0;
			size = 0;
			capacity = DefCapacity;
		}
};

//!
template <class T>	struct AutoVector	{	typedef	Vector<T*, AutoVectorPolicy>	type;	};
template <class T>	struct SharedVector	{	typedef	Vector<T*, SharedVectorPolicy>	type;	};

}		// owl
#endif	// __CONTAINERS_VECTOR__
