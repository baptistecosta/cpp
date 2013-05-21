/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

	#include <assert.h>
	#include "externs/opengl/inc/glfw.h"
	#include "typedefs.h"
	#include "event_publisher.h"
	#include "keymap.h"

using namespace owl;

//-----------------------------------------------------------------------------
EventPublisher::EventPublisher()
	:	m_lmx(0)
	,	m_lmy(0)
	,	m_lmw(0)
{
	//
}

//-----------------------------------------------------------------------------
void			EventPublisher::OnUpdate()
{
	// Mouse position
	int	x = 0, y = 0;
	glfwGetMousePos(&x, &y);
	if (x != m_lmx || y != m_lmy)
		NotifyMouseMoveEvent(m_lmx = x, m_lmy = y);
	
	// Mouse wheel
	int mw = glfwGetMouseWheel();
	if (mw > m_lmw)
		NotifyMouseWheel(true);
	else if (mw < m_lmw)
		NotifyMouseWheel(false);
	m_lmw = mw;

	// Keys
	if (glfwGetKey(GLFW_KEY_UP) == GLFW_PRESS) NotifyKeyEvent(KeyMap::Up);
	if (glfwGetKey(GLFW_KEY_DOWN) == GLFW_PRESS) NotifyKeyEvent(KeyMap::Down);
	if (glfwGetKey(GLFW_KEY_LEFT) == GLFW_PRESS) NotifyKeyEvent(KeyMap::Left);
	if (glfwGetKey(GLFW_KEY_RIGHT) == GLFW_PRESS) NotifyKeyEvent(KeyMap::Right);

	if (glfwGetKey('A') == GLFW_PRESS) NotifyKeyEvent(KeyMap::A);
	if (glfwGetKey('B') == GLFW_PRESS) NotifyKeyEvent(KeyMap::B);
	if (glfwGetKey('C') == GLFW_PRESS) NotifyKeyEvent(KeyMap::C);
	if (glfwGetKey('D') == GLFW_PRESS) NotifyKeyEvent(KeyMap::D);
	if (glfwGetKey('E') == GLFW_PRESS) NotifyKeyEvent(KeyMap::E);
	if (glfwGetKey('F') == GLFW_PRESS) NotifyKeyEvent(KeyMap::F);
	if (glfwGetKey('G') == GLFW_PRESS) NotifyKeyEvent(KeyMap::G);
	if (glfwGetKey('H') == GLFW_PRESS) NotifyKeyEvent(KeyMap::H);
	if (glfwGetKey('I') == GLFW_PRESS) NotifyKeyEvent(KeyMap::I);
	if (glfwGetKey('J') == GLFW_PRESS) NotifyKeyEvent(KeyMap::J);
	if (glfwGetKey('K') == GLFW_PRESS) NotifyKeyEvent(KeyMap::K);
	if (glfwGetKey('L') == GLFW_PRESS) NotifyKeyEvent(KeyMap::L);
	if (glfwGetKey('M') == GLFW_PRESS) NotifyKeyEvent(KeyMap::M);
	if (glfwGetKey('N') == GLFW_PRESS) NotifyKeyEvent(KeyMap::N);
	if (glfwGetKey('O') == GLFW_PRESS) NotifyKeyEvent(KeyMap::O);
	if (glfwGetKey('P') == GLFW_PRESS) NotifyKeyEvent(KeyMap::P);
	if (glfwGetKey('Q') == GLFW_PRESS) NotifyKeyEvent(KeyMap::Q);
	if (glfwGetKey('R') == GLFW_PRESS) NotifyKeyEvent(KeyMap::R);
	if (glfwGetKey('S') == GLFW_PRESS) NotifyKeyEvent(KeyMap::S);
	if (glfwGetKey('T') == GLFW_PRESS) NotifyKeyEvent(KeyMap::T);
	if (glfwGetKey('U') == GLFW_PRESS) NotifyKeyEvent(KeyMap::U);
	if (glfwGetKey('V') == GLFW_PRESS) NotifyKeyEvent(KeyMap::V);
	if (glfwGetKey('W') == GLFW_PRESS) NotifyKeyEvent(KeyMap::W);
	if (glfwGetKey('X') == GLFW_PRESS) NotifyKeyEvent(KeyMap::X);
	if (glfwGetKey('Y') == GLFW_PRESS) NotifyKeyEvent(KeyMap::Y);
	if (glfwGetKey('Z') == GLFW_PRESS) NotifyKeyEvent(KeyMap::Z);

	if (glfwGetKey(GLFW_KEY_KP_0) == GLFW_PRESS) NotifyKeyEvent(KeyMap::Kp_0);
	if (glfwGetKey(GLFW_KEY_KP_1) == GLFW_PRESS) NotifyKeyEvent(KeyMap::Kp_1);
	if (glfwGetKey(GLFW_KEY_KP_2) == GLFW_PRESS) NotifyKeyEvent(KeyMap::Kp_2);
	if (glfwGetKey(GLFW_KEY_KP_3) == GLFW_PRESS) NotifyKeyEvent(KeyMap::Kp_3);
	if (glfwGetKey(GLFW_KEY_KP_4) == GLFW_PRESS) NotifyKeyEvent(KeyMap::Kp_4);
	if (glfwGetKey(GLFW_KEY_KP_5) == GLFW_PRESS) NotifyKeyEvent(KeyMap::Kp_5);
	if (glfwGetKey(GLFW_KEY_KP_6) == GLFW_PRESS) NotifyKeyEvent(KeyMap::Kp_6);
	if (glfwGetKey(GLFW_KEY_KP_7) == GLFW_PRESS) NotifyKeyEvent(KeyMap::Kp_7);
	if (glfwGetKey(GLFW_KEY_KP_8) == GLFW_PRESS) NotifyKeyEvent(KeyMap::Kp_8);
	if (glfwGetKey(GLFW_KEY_KP_9) == GLFW_PRESS) NotifyKeyEvent(KeyMap::Kp_9);

	if (glfwGetKey(GLFW_KEY_INSERT) == GLFW_PRESS) NotifyKeyEvent(KeyMap::Insert);
	if (glfwGetKey(GLFW_KEY_DEL) == GLFW_PRESS)	NotifyKeyEvent(KeyMap::Del);
	if (glfwGetKey(GLFW_KEY_HOME) == GLFW_PRESS) NotifyKeyEvent(KeyMap::Home);
	if (glfwGetKey(GLFW_KEY_END) == GLFW_PRESS) NotifyKeyEvent(KeyMap::End);
	if (glfwGetKey(GLFW_KEY_PAGEUP) == GLFW_PRESS) NotifyKeyEvent(KeyMap::PageUp);
	if (glfwGetKey(GLFW_KEY_PAGEDOWN) == GLFW_PRESS) NotifyKeyEvent(KeyMap::PageDown);
}

//-----------------------------------------------------------------------------
void			EventPublisher::RegisterListener(EventListener* notifier)
{
	assert(notifier);
	registered_objects.push_back(notifier);
}

//-----------------------------------------------------------------------------
void			EventPublisher::NotifyMouseMoveEvent(const int x, const int y)
{
	for (uint i = 0; i < registered_objects.size(); i++)
	{
		assert(registered_objects[i]);
		registered_objects[i]->onMouseMoveEvent(x, y);
	}
		
}

//-----------------------------------------------------------------------------
void			EventPublisher::NotifyMouseWheel(const bool is_fw)
{
	for (uint i = 0; i < registered_objects.size(); i++)
	{
		assert(registered_objects[i]);
		registered_objects[i]->onMouseWheel(is_fw);
	}
}

//-----------------------------------------------------------------------------
void			EventPublisher::NotifyKeyEvent(int glfw_key)
{
	for (uint i = 0; i < registered_objects.size(); i++)
	{
		assert(registered_objects[i]);
		registered_objects[i]->onKeyEvent(glfw_key);
	}
}

