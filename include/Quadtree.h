#ifndef QUADTREE_H
#define QUADTREE_H

#include <vector>
#include <list>
#include <irrlicht/irrlicht.h>

/**
 * A quadtree data structure. Holds pointers to 4 child nodes which
 * are populated when too many objects are contained within this node
 */
class Quadtree {
private:
	/**
	 * Stores some basic information about an object that
	 * is being added to the quadtree
	 */
	struct ObjectData {
		int id;
		irr::core::vector3df pos;
		float rad;
	};
	
	// The max number of objects that can be in any node before splitting
	int maxObjects = 1;
	
	// The max depth to split to
	int maxLevels = 6;
	
	// The depth of this node
	int level;
	
	// List of objects stored in the current node
	std::vector<ObjectData> objects;
	
	// The 4 child nodes
	Quadtree* nodes[4];
	
	// The bounding rectangle of this node
	irr::core::rectf bounds;

	/**
	 * Splits the current node into its 4 children and moves objects
	 * down to their depths
	 */
	void split();
	
	/**
	 * Checks which child an object should be contained within
	 * by checking its collision box with the node's bounding rectangle
	 */
	int getIndex(ObjectData obj);
public:	
	
	/**
	 * Initialises the level and bounding rectangle of the node
	 */
	Quadtree(int level, irr::core::rectf bounds);
	
	/**
	 * Clears all objects recursively from this node and its children
	 */
	void clear();
	
	/**
	 * Inserts an object with a position and radius into the quadtree
	 */
	void insert(int id, irr::core::vector3df pos, float rad);
	
	/**
	 * Takes an object ID, position and radius and uses it to retrieve a list of
	 * all objects that are encountered when recursing down to the depth where
	 * this object should be stored
	 */
	std::list<int> getObjects(std::list<int>& returnObjects, int id, irr::core::vector3df pos, float rad);
	
	/**
	 * Draws the tree and its child nodes to the screen for debugging
	 */
	void draw();
};

#endif