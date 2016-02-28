#ifndef NOTIFICATIONMESSAGE_H
#define NOTIFICATIONMESSAGE_H

#include <string>
#include <irrlicht/irrlicht.h>

class NotificationMessage {
private:
	std::string message;
	irr::video::SColor colour;
	irr::gui::IGUIFont* font;
	
	float alpha = 0;
	
public:
	NotificationMessage() {}
	
	void init(std::string fontpath);
	void showMessage(std::string text, irr::video::SColor col);
	void update();
	void render(irr::video::IVideoDriver* driver);
};

#endif