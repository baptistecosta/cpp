/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __ARRAY__
#define __ARRAY__

	#include "typedefs.h"

namespace owl {

//!
template<class T>	class	Array
{
protected:

		T*					m_cells;
		int					m_size;

public:

		Array(int size = 0)
		{
			m_cells = NULL;
			if (size > 0)
			{
				alloc(size);
				m_size = size;
			}
		}

		~Array()
		{	
			clear();
		}

		T&					operator []	(int index)
		{	return m_cells[index];	}
const	T&					operator []	(int index) const
		{	return m_cells[index];	}

		bool				alloc(int size)
		{
			delete [] m_cells;
			return (m_cells = new T[size]) ? true : false;
		}

		void				clear()
		{
			delete [] m_cells;
			m_cells = NULL;
		}

		void				resize(int size)
		{
			T* temp = new T[size];
			if (!temp)
				return;

			int min = size < m_size ? size : m_size;
			
			for (int i = 0; i < min; i++)
				temp[i] = m_cells[i];

			m_size = size;

			if (m_cells != 0)
				delete [] m_cells;

			m_cells = temp;
		}

		void				insert(T item, int index)
		{
			for (int i = m_size - 1; i > index; i--)
				m_cells[i] = m_cells[i - 1];
			m_cells[index] = item;
		}

		void				remove(int index)
		{
			for (int i = index + 1; i < m_size; i++)
				m_cells[i - 1] = m_cells[i];
		}

const	int					size() const
		{	return m_size;	}
};

}		// owl
#endif	// __ARRAY__
