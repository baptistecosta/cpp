/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __SHADOW_MAP_FBO__
#define __SHADOW_MAP_FBO__

	#include "platform\defines.h"

namespace owl {

//!
class	ShadowMapFBO
{
private:

		uint					m_fbo_id,
								m_texture_id;

public:

		ShadowMapFBO			();
		~ShadowMapFBO			();

		bool					init(uint window_w, uint window_h);
		void					bindForWriting();
		void					bindForReading(uint texture_unit);
};

}		//	owl
#endif	//	__SHADOW_MAP_FBO__
