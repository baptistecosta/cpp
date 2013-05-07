/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/
	
	#include "shared_object.h"
	#include "core/log.h"

using namespace owl;

//---------------------------------------------------------------------------
void			SharedObject::Log() const
{	Log::i("%d reference(s) count.", ref_ct);	}
