#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <WorldManager.h>
#include <NodePath.h>
#include <irrlicht/irrlicht.h>
#include <vector>

/**
 * Node struct for storing F, G and H costs, position in the world
 * and parent node
 */
struct PathFindNode {
	PathFindNode* parent;
	int x;
	int y;
	int f;
	int g;
	int h;
};

/**
 * Uses the grid from the World Manager to find a path between
 * two coordinates using the A* algorithm.
 */
class PathFinder {
private:
	// The world manager to pathfind through
	WorldManager* worldManager;
	
	/**
	 * Use the cost function to calculate a node's G cost from
	 * a given node
	 */
	int calcGValue(int x, int y, int prevX, int prevY);
	
	/**
	 * Use the heuristic function to estimate the distance from a node
	 * to the goal
	 */
	int calcHValue(int fromX, int fromY, int endX, int endY);
	
	/**
	 * Calculate a node's F value by summing its G and H values
	 */
	int calcFValue(int gVal, int hVal);
	
	/**
	 * Returns either 10 or 14 depending on whether a horizontal, vertical or
	 * diagonal move was made to reach this node
	 */
	int costFromNode(int x, int y, int prevX, int prevY);
	
	/**
	 * Uses the manhattan distance to estimate the distance from a node
	 * to the goal (abs(startX-endx) + abs(startY-endY))
	 */
	int manhattanDistance(int fromX, int fromY, int endX, int endY);
	
	/**
	 * Constructs a node from parameters
	 */
	PathFindNode* makeNode(PathFindNode* parent, int x, int y, int f, int g, int h);
	
	/**
	 * Returns a NodePath from a start position to an end position
	 */
	NodePath findPath(int startX, int startY, int endX, int endY);
	
	/**
	 * Returns a vector of neighbouring nodes to a given node
	 */
	std::vector<PathFindNode*> getNeighbours(PathFindNode* current, int endX, int endY);
	
	int indexOfSmallestF(std::vector<PathFindNode*> nodes);
	
	int containsNode(PathFindNode* node, std::vector<PathFindNode*> nodes);
public:
	
	/**
	 * Stores the pointer to the World Manager
	 */
	PathFinder(WorldManager* world) {
		worldManager = world;
	}
	
	/**
	 * Returns a NodePath of waypoints from start to end. Start and end are
	 * expressed as vector3df. This method uses the simplified findPath method
	 * by expressing the vector3df values as integer coordinates
	 */
	NodePath findPath(irr::core::vector3df start, irr::core::vector3df end);
};


#endif