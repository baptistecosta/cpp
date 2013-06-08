/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __ARRAY__
#define __ARRAY__

	#include "defines.h"
	#include "typedefs.h"

namespace owl {

//!
template<class T>	class	Array
{
protected:

		T*					cells;
		int					size;

public:

		Array(int _size = 0)
		{
			cells = 0;
			if (_size > 0)
			{
				Alloc(_size);
				size = _size;
			}
		}

		~Array()
		{	Clear();	}

		T&					operator []	(int index)
		{	return cells[index];	}
		const T&			operator []	(int index) const
		{	return cells[index];	}

		bool				Alloc(int _size)
		{
			delete [] cells;
			return (cells = new T[_size]) ? true : false;
		}

		void				Clear()
		{
			delete [] cells;
			cells = 0;
		}

		void				Resize(int _size)
		{
			T* temp = new T[_size];
			if (!temp)
				return;

			int min = _size < size ? _size : size;
			
			for (int i = 0; i < min; i++)
				temp[i] = cells[i];

			size = _size;

			if (cells != 0)
				delete [] cells;

			cells = temp;
		}

		void				Insert(T item, int index)
		{
			for (int i = size - 1; i > index; i--)
				cells[i] = cells[i - 1];
			cells[index] = item;
		}

		void				Remove(int index)
		{
			for (int i = index + 1; i < size; i++)
				cells[i - 1] = cells[i];
		}

		const int			Size() const
		{	return size;	}
};

}		// owl
#endif	// __ARRAY__
