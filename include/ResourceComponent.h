#ifndef RESOURCECOMPONENT_H
#define RESOURCECOMPONENT_H

#include <GameComponent.h>
#include<Enums.h>

class ResourceComponent : public GameComponent {
private:
public:
	ResourceComponent (RESOURCE_TYPE resType) : GameComponent("ResourceComponent") {
		this->resType = resType;
	}
	
	RESOURCE_TYPE resType;
	
	int gatherSlots[4] = {-1, -1, -1, -1};
	int maxGatherers = 4;
	
	int flashNum = 0;
	int flashTimer = 30;
	bool flashOn = false;
};

#endif