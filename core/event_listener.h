/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef	__EVENT_LISTENER__
#define	__EVENT_LISTENER__

	#include "../core/log.h"

namespace owl {

//!
struct	EventListener
{
virtual	void			onMouseMoveEvent(int x, int y) {}
virtual	void			onMouseWheel(const bool is_forward) {}

		void			onKeyEvent(int glfw_key);

virtual	void			onKeyEvent_Up() {}
virtual	void			onKeyEvent_Down() {}
virtual	void			onKeyEvent_Left() {}
virtual	void			onKeyEvent_Right() {}

virtual	void			onKeyEvent_Kp0() {}
virtual	void			onKeyEvent_Kp1() {}
virtual	void			onKeyEvent_Kp2() {}
virtual	void			onKeyEvent_Kp3() {}
virtual	void			onKeyEvent_Kp4() {}
virtual	void			onKeyEvent_Kp5() {}
virtual	void			onKeyEvent_Kp6() {}
virtual	void			onKeyEvent_Kp7() {}
virtual	void			onKeyEvent_Kp8() {}
virtual	void			onKeyEvent_Kp9() {}

virtual	void			onKeyEvent_O() {}
virtual	void			onKeyEvent_P() {}
};

}		// owl
#endif	// __EVENT_NOTIFIER__
