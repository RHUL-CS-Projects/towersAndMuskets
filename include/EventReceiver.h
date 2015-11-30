#ifndef EVENTRECEIVER_H
#define EVENTRECEIVER_H

#include <irrlicht/irrlicht.h>

// Code adapted from http://irrlicht.sourceforge.net/docu/example019.html

/**
 * Struct for storing information about the current context. Holds
 * pointers to various GUI elements so they can be modified during
 * GUI events
 */
struct SAppContext
{
    irr::IrrlichtDevice *device;
	irr::gui::IGUIStaticText* txtQuadtree;
	irr::gui::IGUIStaticText* txtPaths;
	irr::gui::IGUIStaticText* txtSteer;
	irr::gui::IGUIStaticText* txtStencil;
	irr::gui::IGUIStaticText* txtGridWorld;
};

/**
 * IDs for GUI elements
 */
enum GUI_IDS {
	BUTTON_ID_QUADTREE,
	BUTTON_ID_PATHS,
	BUTTON_ID_STEER,
	BUTTON_ID_STENCIL,
	BUTTON_ID_GRIDWORLD
};

/**
 * Extends Irrlicht's IEventReceiver class to catch mouse and GUI
 * events
 */
class EventReceiver : public irr::IEventReceiver {
private:
	// Pointers to some of the GUI elements
	SAppContext& Context;
	
public:
	/**
	 * Initialises the context
	 */
	EventReceiver(SAppContext& context) : Context(context) {}
	
	/**
	 * Stores information about the state of the mouse; which buttons
	 * are pressed and its position on the screen
	 */
	struct SMouseState
	{
		irr::core::position2di position;
		bool leftPressed;
		bool rightPressed;
		float wheelDelta; 
	};
	
	// The current state of the mouse
	static EventReceiver::SMouseState MouseState;
	
	/**
	 * Called by Irrlicht's event dispatcher to handle GUI
	 * and input events
	 */
	virtual bool OnEvent(const irr::SEvent& event);
	
	/**
	 * Returns the current mouse state struct
	 */
	static EventReceiver::SMouseState* getMouseState();
};


#endif