/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

	#include "event_listener.h"
	#include "keymap.h"
	
using namespace owl;

//-----------------------------------------------------------------------------
void			EventListener::onKeyEvent(int glfw_key)
{
	switch (glfw_key)
	{
		case KeyMap::Up: onKeyEvent_Up(); break;
		case KeyMap::Down: onKeyEvent_Down(); break;
		case KeyMap::Left: onKeyEvent_Left(); break;
		case KeyMap::Right: onKeyEvent_Right(); break;

		case KeyMap::O: onKeyEvent_O(); break;
		case KeyMap::P: onKeyEvent_P(); break;

		case KeyMap::Kp_0: onKeyEvent_Kp0(); break;
		case KeyMap::Kp_1: onKeyEvent_Kp1(); break;
		case KeyMap::Kp_2: onKeyEvent_Kp2(); break;
		case KeyMap::Kp_3: onKeyEvent_Kp3(); break;
		case KeyMap::Kp_4: onKeyEvent_Kp4(); break;
		case KeyMap::Kp_5: onKeyEvent_Kp5(); break;
		case KeyMap::Kp_6: onKeyEvent_Kp6(); break;
		case KeyMap::Kp_7: onKeyEvent_Kp7(); break;
		case KeyMap::Kp_8: onKeyEvent_Kp8(); break;
		case KeyMap::Kp_9: onKeyEvent_Kp9(); break;
	}
}
