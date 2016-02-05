#ifndef STATEMAINMENU_H
#define STATEMAINMENU_H

#include <GameState.h>
#include <GuiElement.h>
#include <list>

class StateMainMenu : public GameState {
private:
	std::list<GuiElement*> guiElements;
	
public:
	StateMainMenu() : GameState() {
		guiElements.push_back(new GuiElement(10, 10, 160, 30, irr::video::SColor(20,255,0,0)));
		guiElements.push_back(new GuiElement(10, 50, 160, 30, irr::video::SColor(20,255,0,0)));
		guiElements.push_back(new GuiElement(10, 90, 160, 30, irr::video::SColor(20,255,0,0)));
		guiElements.push_back(new GuiElement(10, 130, 160, 30, irr::video::SColor(20,255,0,0)));
	}
	
	virtual void update();
	virtual void render(irr::video::IVideoDriver* driver);
	virtual void transitionIn();
	virtual void transitionOut();
};

#endif