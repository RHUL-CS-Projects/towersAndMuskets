#ifndef NODEPATH_H
#define NODEPATH_H

#include <irrlicht/irrlicht.h>
#include <vector>

/**
 * Represents a path made up of a list of 3D positions
 * in the world. Also remembers the current position on the
 * path for testing whether or not an object has reached
 * the end of it
 */
class NodePath {
private:
	// List of 3D waypoints
	std::vector<irr::core::vector3df> waypoints;
	
	// The current nodal position along the path
	int currentNode = -1;
	
public:
	NodePath() {};
	
	/**
	 * Returns the current node on the path
	 */
	irr::core::vector3df getCurrentNode();
	
	/**
	 * Adds a waypoint to the path
	 */
	void addNode(irr::core::vector3df position);
	
	/**
	 * Adds a waypoint to the beginning of the path
	 */
	void addNodeFront(irr::core::vector3df position);
	
	/**
	 * Resets the path and then adds the new waypoint
	 */
	void addNodeToNew(irr::core::vector3df position);
	
	/**
	 * Increments the node position counter
	 */
	void nextNode();
	
	/**
	 * Removes all waypoints from the path and resets the position counter
	 */
	void resetPath();
	
	/**
	 * Returns whether or not the path has ended
	 */
	bool ended();
	
	/**
	 * Returns the list of all waypoints
	 */
	std::vector<irr::core::vector3df> getWaypoints();
	
	/**
	 * Returns whether or not the path is currently at its final node
	 */
	bool finalNode();
};


#endif