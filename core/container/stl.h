/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __STL_CONTAINERS__
#define __STL_CONTAINERS__

	#include <map>
	#include <vector>

using namespace std;

//!
struct	Stl
{
	template <typename T> static void DeleteVector(vector<T*>& container)
	{
		for (int i = 0; i < (int)container.size(); i++)
			delete container[i];
		container.clear();
	}

	// Delete a std::map foo that contains pointers at foo.second.
	// If there are no pointers stroring in the map container, simply call foo.clear().
	template <typename T1, typename T2> static void	DeleteMap( map<T1, T2*>& container )
	{
		map<T1, T2*>::const_iterator it;
		for (it = container.begin(); it != container.end(); ++it)
			delete it->second;
		container.clear();
	}
};

#endif //__STL_CONTAINERS__
