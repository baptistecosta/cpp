/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef	__EVENT_PUBLISHER__
#define	__EVENT_PUBLISHER__

	#include <vector>
	#include "event_listener.h"

namespace owl {

//!
class	EventPublisher
{
		std::vector<EventListener*>	registered_objects;

		// Last mouse position
		int							m_lmx,
									m_lmy;
		// Last mouse wheel
		int							m_lmw;

public:

		EventPublisher();

		void						OnUpdate();
		void						RegisterListener(EventListener* notifier);

		void						NotifyMouseMoveEvent(const int x, const int y);
		void						NotifyMouseWheel(const bool is_fw);
		void						NotifyMouseWheel();
		void						NotifyKeyEvent(int glfw_key);
};

}		// owl
#endif	// __EVENT_PUBLISHER__