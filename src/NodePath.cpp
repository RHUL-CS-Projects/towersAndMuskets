#include <NodePath.h>

/**
 * Add a new node to the path
 */
void NodePath::addNode(irr::core::vector3df position) {
	if (waypoints.size() == 0)
		currentNode = 0;
	
	waypoints.push_back(position);
}

/**
 * Reset the path and then add the new node
 */
void NodePath::addNodeToNew ( irr::core::vector3df position ) {
	resetPath();
	addNode(position);
}

/**
 * Get the path's current node
 */
irr::core::vector3df NodePath::getCurrentNode() {
	return (currentNode < 0) ? irr::core::vector3df(0,0,0) : waypoints[currentNode];
}

/**
 * Remove all waypoints and reset the current node counter
 */
void NodePath::resetPath() {
	currentNode = -1;
	waypoints.clear();
}

/**
 * Increment the current node counter
 */
void NodePath::nextNode() {
	currentNode++;
}

/**
 * 
 */
bool NodePath::ended() {
	return currentNode >= waypoints.size() || currentNode < 0;
}

/**
 * Returns the vector of all waypoints
 */
std::vector< irr::core::vector3df > NodePath::getWaypoints() {
	return waypoints;
}

