#ifndef GROUPPATHMOVER_H
#define GROUPPATHMOVER_H

#include <vector>
#include <irrlicht/irrlicht.h>

class GroupPathMover {
private:
	std::vector<int> objects;
	irr::core::vector3df destination;
	
public:	
	GroupPathMover() {}
	
	void requestPath(int id, irr::core::vector3df pos);
	void startCalc();
	static void calculatePaths(std::vector<int> objs, irr::core::vector3df dest);
};

#endif