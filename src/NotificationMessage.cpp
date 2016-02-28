#include <NotificationMessage.h>
#include <Game.h>

using namespace std;
using namespace irr;
using namespace video;
using namespace core;
using namespace gui;

void NotificationMessage::init ( std::string fontpath ) {
	message = "";
	colour = irr::video::SColor(0,0,255,0);
	font = Game::game.getRendMgr()->getGUIEnvironment()->getFont(irr::io::path(fontpath.c_str()));
}

void NotificationMessage::render(IVideoDriver* driver) {
	driver->draw2DRectangle(
		SColor(((alpha > 255) ? 255 : alpha) * 0.5, 0, 0, 0),
		recti(0, 0, driver->getScreenSize().Width, 30)
	);
	
	font->draw(
		stringw(message.c_str()), 
		recti(0, 0, driver->getScreenSize().Width, 30),
		colour,
		true,
		true
	);
}

void NotificationMessage::showMessage ( std::string text, irr::video::SColor col ) {
	alpha = 400;
	message = text;
	colour = col;
}

void NotificationMessage::update() {
	if (alpha > 0)
		alpha -= 2;
	
	colour.setAlpha((alpha > 255) ? 255 : alpha);
}
