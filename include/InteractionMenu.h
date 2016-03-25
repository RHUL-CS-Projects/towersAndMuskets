#ifndef INTERACTIONMENU_H
#define INTERACTIONMENU_H

#include <GuiObserver.h>
#include <list>
#include <GuiElement.h>
#include <irrlicht/irrlicht.h>
#include <sfml/SFML/Audio.hpp>
#include <GameState.h>
#include <map>
#include <GuiElementPurchase.h>
#include <WaveController.h>

class InteractionMenu : public GuiObserver {
private:
	enum MENU_BUTTONS { btnGatherer=0, btnUnit, btnCannon, btnTower, btnReveille, btnMenu, btnSkip };
	
	std::list<GuiElement*> guiElements;
	int height;
	int mouseOverID;
	double alpha = 0;
	
	sf::Sound* sndRolloverSound;
	sf::Sound* sndClickSound;
	GameState* parentState;
	irr::gui::IGUIFont* font;
	
	std::map<int, PurchaseDetails> purchases;
	PurchaseDetails currentPurchase;
	bool showPurchaseTooltip = false;
	
	irr::video::ITexture* texRes, *texPurchase;
	irr::video::ITexture* texHUD, *texBarFront, *texBarBack;
	
	WaveDetails waveDetails;
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
	void setWaveDetails(WaveDetails details);
	virtual void onNotify(int id, int eventID);
};

#endif