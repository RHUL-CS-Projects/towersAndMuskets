#ifndef NODEPATH_H
#define NODEPATH_H

#include <irrlicht/irrlicht.h>
#include <vector>

class NodePath {
private:
	std::vector<irr::core::vector3df> waypoints;
	int currentNode = -1;
	
public:
	irr::core::vector3df getCurrentNode();
	void addNode(irr::core::vector3df position);
	void addNodeToNew(irr::core::vector3df position);
	void nextNode();
	void resetPath();
	bool ended();
	std::vector<irr::core::vector3df> getWaypoints();
};


#endif