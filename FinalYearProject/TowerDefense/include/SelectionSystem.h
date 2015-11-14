#ifndef SELECTIONSYSTEM_H
#define SELECTIONSYSTEM_H

#include <ComponentSystem.h>
#include <irrlicht/irrlicht.h>

class SelectionSystem : public ComponentSystem {	
private:
	bool dragging = false;
	irr::core::vector2d<int> mousePos;
	irr::core::vector2d<int> clickPos;
	
public:
	SelectionSystem() : ComponentSystem("SelectionSystem") {}
	void update(float timestep);
	void draw(float timestep);
};


#endif