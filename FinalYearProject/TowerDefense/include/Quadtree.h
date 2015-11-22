#ifndef QUADTREE_H
#define QUADTREE_H

#include <vector>
#include <list>
#include <irrlicht/irrlicht.h>

class Quadtree {
private:
	struct ObjectData {
		int id;
		irr::core::vector3df pos;
		float rad;
	};
	
	int maxObjects = 1;
	int maxLevels = 6;
	int level;
	
	std::vector<ObjectData> objects;
	Quadtree* nodes[4];
	
	irr::core::rectf bounds;

	void split();
	int getIndex(ObjectData obj);
public:	
	Quadtree(int level, irr::core::rectf bounds);
	void clear();
	void insert(int id, irr::core::vector3df pos, float rad);
	std::list<int> getObjects(std::list<int>& returnObjects, int id, irr::core::vector3df pos, float rad);
	void draw();
};

#endif