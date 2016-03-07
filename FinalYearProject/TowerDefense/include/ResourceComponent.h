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
	
	int gatherSlots[8] = {-1, -1, -1, -1, -1, -1, -1, -1};
};

#endif