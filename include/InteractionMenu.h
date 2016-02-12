#ifndef INTERACTIONMENU_H
#define INTERACTIONMENU_H

#include <GuiObserver.h>
#include <list>
#include <GuiElement.h>
#include <irrlicht/irrlicht.h>
#include <sfml/SFML/Audio.hpp>
#include <GameState.h>

class InteractionMenu : public GuiObserver {
private:
	std::list<GuiElement*> guiElements;
	int height;
	sf::Sound* sndRolloverSound;
	sf::Sound* sndClickSound;
	GameState* parentState;
	
public:
	InteractionMenu();
	
	~InteractionMenu() {
		while (guiElements.size() > 0) {
			delete guiElements.front();
			guiElements.pop_front();
		}
	}
	
	void init(int height, GameState* state);
	void update();
	void render(irr::video::IVideoDriver* driver);
	virtual void onNotify(int id, int eventID);
};

#endif