/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef	__AUTO_ARRAY__
#define	__AUTO_ARRAY__

	#include "typedefs.h"

namespace owl {

//!
template <class T>	class	AutoArray
{
public:
		enum
		{	DefCapacity	=	16	};

private:
		
		T*					m_data;
		int					m_size,
							m_capacity;

public:
		//---------------------------------------------------------------------
		AutoArray()
			:	m_data(0)
			,	m_size(0)
			,	m_capacity(DefCapacity)
		{
			alloc(m_capacity);
		}

		AutoArray(const int size)
			:	m_data(0)
			,	m_size(size)
			,	m_capacity(size > DefCapacity ? size + DefCapacity : DefCapacity)
		{
			alloc(m_capacity);
		}

		//---------------------------------------------------------------------
		~AutoArray()
		{
			destroy();
		}

public:
		//---------------------------------------------------------------------
		T&					operator []	(int index)
		{	return m_data[index];	}
		const T&			operator []	(int index) const
		{	return m_data[index];	}


private:
		//---------------------------------------------------------------------
		bool				alloc(int count)
		{
			delete [] m_data;
			if (m_data = new T[count])
			{
				for (int i = 0; i < count; i++)
					m_data[i] = 0;
				return true;
			}
			return false;
		}

		//---------------------------------------------------------------------
		void				destroy()
		{
			clear();
			delete [] m_data;
			m_data = 0;
			m_size = 0;
		}

public:
		//---------------------------------------------------------------------
		void				add(T item)
 		{
			if (m_size == m_capacity)
				increaseCapacity();
			m_data[m_size++] = item;
 		}

		//---------------------------------------------------------------------
		void				remove()
		{
			int i = --m_size;
			if (!isNull(i))
				delete m_data[i];
			m_data[i] = 0;
		}

		//---------------------------------------------------------------------
		void				increaseCapacity(int size = DefCapacity)
		{
			// Create a temporary array with the proper size.
			size += m_size;
			T* tmp = new T[size];
			if (!tmp)
				return;

			// Copy data to the temp array.
			for (int i = 0; i < size; i++)
				tmp[i] = i < m_size ? m_data[i] : 0;

			m_capacity = size;		// Refresh capacity.
			clear();
			delete [] m_data;		// Delete old pointers' array data.
			m_data = tmp;			// Point to the new array.
			tmp = 0;
		}

		//---------------------------------------------------------------------
		const int			size() const
		{	return m_size;	}
		const int			capacity() const
		{	return m_capacity;	}


		//---------------------------------------------------------------------
		const bool			isNull() const
		{	return m_data == 0;	}
		const bool			isNull(int i) const
		{
			if (isNull() || i >= m_capacity)
				return true;
			return m_data[i] == 0;
		}

		//---------------------------------------------------------------------
		void				clear()
		{
			for (int i = 0; i < size(); i++)
			{
				delete m_data[i];
				m_data[i] = 0;
			}
			m_size = 0;
			m_capacity = DefCapacity;
		}
};

}		//	owl
#endif	//	__B_AUTO_ARRAY__
