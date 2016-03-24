#ifndef GUIELEMENTPURCHASE_H
#define GUIELEMENTPURCHASE_H

#include <GuiElement.h>

struct PurchaseDetails {
	int wood;
	int stone;
	int gold;
	std::string name;
};

class GuiElementPurchase : public GuiElement {
private:
	irr::video::ITexture* texBack, *texFront, *texBackLight;
	PurchaseDetails purchase;
	int x, y;
	
public:
	GuiElementPurchase ( int x, int y, std::string back, std::string front, std::string backlight, PurchaseDetails purchase, int id=0);
	
	virtual void update();
	virtual void render(irr::video::IVideoDriver* driver);
	virtual void notify(int eventID);
};

#endif