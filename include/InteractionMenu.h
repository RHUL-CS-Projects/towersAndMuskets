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
	double progressPercent = 1;
	sf::Sound* sndRolloverSound;
	sf::Sound* sndClickSound;
	GameState* parentState;
	irr::gui::IGUIFont* font;
	
	irr::video::ITexture* texRes;
	irr::video::ITexture* texHUD, *texBarFront, *texBarBack;
	
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
	void setResourcesDisplay(int stone, int gold, int wood);
	void setProgress(double percent);
	virtual void onNotify(int id, int eventID);
};

#endif