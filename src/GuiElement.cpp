#include <GuiElement.h>
#include <Game.h>

using namespace std;
using namespace irr;
using namespace core;
using namespace video;
using namespace gui;

GuiElement::GuiElement ( int x, int y, int width, int height, std::string text, std::string fontpath, SColor col, int id ) {
	bounds = recti(vector2di(x,y), vector2di(x + width, y + height));
	colour = col;
	fontColour = SColor(255,0,0,0);
	this->text = text;
	this->fontpath = fontpath;
	font = Game::game.getRendMgr()->getGUIEnvironment()->getFont(irr::io::path(fontpath.c_str()));
	this->id = id;
}

void GuiElement::update() {
	EventReceiver::SMouseState mouseState = *EventReceiver::getMouseState();
	
	if (bounds.isPointInside(mouseState.position)) {
		if (mouseState.leftPressed) {
			//colour.setAlpha(255);
			fontColour.setAlpha(255);
			
			if (!mouseClicked) {
				notify(mousePressed);
				mouseClicked = true;
			}
		} else {
			//colour.setAlpha(100);
			fontColour.setAlpha(255);
			
			if (!mouseOver) {
				notify(mouseOver);
				mouseOver = true;
			}
		}
	} else {
		//colour.setAlpha(50);
		fontColour.setAlpha(150);
		mouseOver = false;
	}
	
	if (!mouseState.leftPressed) mouseClicked = false;
}

void GuiElement::render ( IVideoDriver* driver ) {
	driver->draw2DRectangle(colour, bounds);
	
	if (font != nullptr) {
		stringw irrText(text.c_str());
		font->draw(irrText, bounds, fontColour, true, true);
	}
}

void GuiElement::notify(int eventID) {
	GuiSubject::notify(id, eventID);
}
