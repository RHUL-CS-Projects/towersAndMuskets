#ifndef GUIELEMENT_H
#define GUIELEMENT_H

#include <irrlicht/irrlicht.h>
#include <EventReceiver.h>
#include <iostream>

class GuiElement {
protected:
	irr::core::recti bounds;
	irr::video::SColor colour;
	
public:
	GuiElement(int x, int y, int width, int height, irr::video::SColor col) {
		bounds = irr::core::recti(irr::core::vector2di(x,y), irr::core::vector2di(x + width, y + height));
		colour = col;
	}
	
	virtual void update() {
		EventReceiver::SMouseState mouseState = *EventReceiver::getMouseState();
		
		if (bounds.isPointInside(mouseState.position)) {
			if (mouseState.leftPressed)
				colour.setAlpha(255);
			else
				colour.setAlpha(100);
		} else {
			colour.setAlpha(50);
		}
	}
	
	virtual void render(irr::video::IVideoDriver* driver) {
		driver->draw2DRectangle(colour, bounds);
	}
};

#endif