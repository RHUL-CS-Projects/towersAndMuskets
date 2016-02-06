#ifndef GUIELEMENT_H
#define GUIELEMENT_H

#include <irrlicht/irrlicht.h>
#include <EventReceiver.h>
#include <iostream>
#include <string>
#include <GuiSubject.h>

class GuiElement : public GuiSubject {
protected:
	irr::core::recti bounds;
	irr::video::SColor colour;
	irr::video::SColor fontColour;
	irr::gui::IGUIFont* font;
	std::string text;
	std::string fontpath;
	int id;
	bool mouseClicked = false;
	bool mouseOver = false;
	
public:
	GuiElement(int x, int y, int width, int height, std::string text, std::string fontpath, irr::video::SColor col, int id = 0);
	
	virtual void update();
	virtual void render(irr::video::IVideoDriver* driver);
	virtual void notify(int eventID);
};

#endif