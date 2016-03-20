#include <GuiElementPurchase.h>
#include <Game.h>

using namespace irr;
using namespace core;
using namespace video;

GuiElementPurchase::GuiElementPurchase ( int x, int y, std::string back, std::string front, std::string backlight, int id ) {
	this->id = id;
	
	texBack = Game::game.getRendMgr()->getDriver()->getTexture((RenderManager::resPath + "/materials/textures/" + back).c_str());
	texFront = Game::game.getRendMgr()->getDriver()->getTexture((RenderManager::resPath + "/materials/textures/" + front).c_str());
	texBackLight = Game::game.getRendMgr()->getDriver()->getTexture((RenderManager::resPath + "/materials/textures/" + backlight).c_str());
	
	this->bounds = irr::core::recti(x, y, x + texBack->getSize().Width, y + texBack->getSize().Height);
}

void GuiElementPurchase::notify ( int eventID ) {
    GuiElement::notify ( eventID );
}

void GuiElementPurchase::render ( irr::video::IVideoDriver* driver ) {
   driver->draw2DImage(mouseOver ? texBackLight : texBack, bounds.UpperLeftCorner, recti(0,0,bounds.getWidth(), bounds.getHeight()), 0, SColor(255,255,255,255), true);
   driver->draw2DImage(texFront, bounds.UpperLeftCorner, recti(0,0,bounds.getWidth(), bounds.getHeight()), 0, SColor(255,255,255,255), true);
}

void GuiElementPurchase::update() {
    EventReceiver::SMouseState mouseState = *EventReceiver::getMouseState();

	if (bounds.isPointInside(mouseState.position)) {
		if (mouseState.leftPressed) {
			if (!mouseClicked) {
				notify(mousePressed);
				mouseClicked = true;
			}
		} else {
			if (!mouseOver) {
				notify(mouseOver);
				mouseOver = true;
			}
		}
	} else {
		mouseOver = false;
	}

	if (!mouseState.leftPressed) mouseClicked = false;
}
