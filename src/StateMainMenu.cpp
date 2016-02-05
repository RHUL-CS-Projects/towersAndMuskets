#include <StateMainMenu.h>
#include <irrlicht/irrlicht.h>

void StateMainMenu::update() {
    GameState::update();
	
	for (GuiElement* e : guiElements) {
		e->update();
	}
}

void StateMainMenu::render ( irr::video::IVideoDriver* driver ) {
    GameState::render ( driver );
	
	for (GuiElement* e : guiElements) {
		e->render(driver);
	}
}

void StateMainMenu::transitionIn() {
    GameState::transitionIn();
}

void StateMainMenu::transitionOut() {
    GameState::transitionOut();
}

