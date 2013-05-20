/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __RESOURCE__
#define __RESOURCE__

	#include "core/containers/shared_object.h"
	#include "core/string.h"

namespace owl {

//!
class	Resource	:	public	SharedObject
{
protected:

		String			m_guid;

public:

		Resource(const String& guid);

const	String&			getGuid() const			{	return	m_guid;	}
};

}		// owl
#endif	// __RESOURCE__
