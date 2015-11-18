#ifndef EVENTRECEIVER_H
#define EVENTRECEIVER_H

#include <irrlicht/irrlicht.h>

// Code adapted from http://irrlicht.sourceforge.net/docu/example019.html

class EventReceiver : public irr::IEventReceiver {
public:
	struct SMouseState
	{
		irr::core::position2di position;
		bool leftPressed;
		bool rightPressed;
		float wheelDelta; 
	};
	
	static EventReceiver::SMouseState MouseState;
	
	virtual bool OnEvent(const irr::SEvent& event);
	
	static EventReceiver::SMouseState* getMouseState();
};




#endif