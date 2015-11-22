#ifndef EVENTRECEIVER_H
#define EVENTRECEIVER_H

#include <irrlicht/irrlicht.h>

// Code adapted from http://irrlicht.sourceforge.net/docu/example019.html

struct SAppContext
{
    irr::IrrlichtDevice *device;
	irr::gui::IGUIStaticText* txtQuadtree;
	irr::gui::IGUIStaticText* txtPaths;
};

enum GUI_IDS {
	BUTTON_ID_QUADTREE,
	BUTTON_ID_PATHS,
	LABEL_ID_QUADTREE,
	LABEL_ID_PATHS
};

class EventReceiver : public irr::IEventReceiver {
private:
	SAppContext& Context;
	
public:
	EventReceiver(SAppContext& context) : Context(context) {}
	
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