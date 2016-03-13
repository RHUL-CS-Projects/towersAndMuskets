#ifndef TOWERCOMPONENT_H
#define TOWERCOMPONENT_H

#include <GameComponent.h>
#include <irrlicht/irrlicht.h>

class TowerComponent : public GameComponent {
public:
	TowerComponent() : GameComponent("TowerComponent") {
		doorOffset = irr::core::vector3df(0,0,-1.2f);
	}
	
	int platformHeight = 20;
	irr::core::vector3df doorOffset;
	int freeSpace[4] = {-1, -1, -1, -1};
	
	int flashNum = 0;
	int flashTimer = 30;
	bool flashOn = false;
};

#endif