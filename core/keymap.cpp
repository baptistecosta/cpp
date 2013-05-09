/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

	#include <glfw.h>
	#include "keymap.h"

using namespace owl;

//-----------------------------------------------------------------------------
const bool			KeyPress::leftCtrl()
{	return glfwGetKey(GLFW_KEY_LCTRL) == GLFW_PRESS;	}
const bool			KeyPress::rightCtrl()
{	return glfwGetKey(GLFW_KEY_RCTRL) == GLFW_PRESS;	}
const bool			KeyPress::leftShift()
{	return glfwGetKey(GLFW_KEY_LSHIFT) == GLFW_PRESS;	}
const bool			KeyPress::rightShift()
{	return glfwGetKey(GLFW_KEY_RSHIFT) == GLFW_PRESS;	}
