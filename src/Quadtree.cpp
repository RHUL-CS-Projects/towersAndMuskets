#include <Quadtree.h>
#include <iostream>
#include <RenderManager.h>
#include <irrlicht/irrlicht.h>

Quadtree::Quadtree ( int level, irr::core::rectf bounds ) {
	this->level = level;
	this->bounds = bounds;
	
	for (int i = 0; i < 4; i++) {
		nodes[i] = nullptr;
	}
}

/**
 * Clears all nodes and objects from the quadtree
 */
void Quadtree::clear() {
	// Clear objects
	objects.clear();

	// Clear nodes
	for (int i = 0; i < 4; i++) {
		if (nodes[i] != nullptr) {
			nodes[i]->clear();
			nodes[i] = nullptr;
		}
	}
}

/**
 * Splits the current quadtree into 4 subnodes
 */
void Quadtree::split() {
	// Calculate width and height of subnodes
	float width = bounds.getWidth() / 2;
	float height = bounds.getHeight() / 2;
	float x = bounds.UpperLeftCorner.X;
	float y = bounds.UpperLeftCorner.Y;
	
	// Initialise the subnodes
	nodes[0] = new Quadtree(level+1, irr::core::rectf(x + width, y, x + width + width, y + height));
	nodes[1] = new Quadtree(level+1, irr::core::rectf(x, y, x + width, y + height));
	nodes[2] = new Quadtree(level+1, irr::core::rectf(x, y + height, x + width, y + height + height));
	nodes[3] = new Quadtree(level+1, irr::core::rectf(x + width, y + height, x + width + width, y + height + height));
}

/**
 * Check which node the current object should be placed in
 */
int Quadtree::getIndex (ObjectData obj) {
	int index = -1;
	float midX = bounds.getCenter().X;	
	float midY = bounds.getCenter().Y;
	
	irr::core::vector3df pos = obj.pos;
	float rad = obj.rad;
	
	// Treat circular bounds of object as rectangle
	float topLeftX = pos.X - rad;
	float topLeftY = pos.Z - rad;
	float width = rad*2;
	float height = rad*2;
	
	// Check if circular bounds of object lie within bounds of each subnode
	// Top quadrant
	bool topHalf = (topLeftY < midY && topLeftY + height < midY);
	
	// Bottom quadrant
	bool bottomHalf = (topLeftY > midY);
	
	// Left quadrant
	if (topLeftX < midX && topLeftX + width < midX) {
		if (topHalf)
			index = 1;
		else if (bottomHalf)
			index = 2;
	} 
	// Right quadrant
	else if (topLeftX > midX) {
		if (topHalf)
			index = 0;
		else if (bottomHalf)
			index = 3;
	}
	
	return index;
}

/**
 * Insert an object into the quadtree, splitting the current node into
 * 4 subnodes if the object capacity is exceeded
 */
void Quadtree::insert ( int id, irr::core::vector3df pos, float rad ) {
	ObjectData obj;
	obj.id = id;
	obj.pos = pos;
	obj.rad = rad;

	if (nodes[0] != nullptr) {
		int index = getIndex(obj);
		
		if (index != -1) {
			nodes[index]->insert(id, pos, rad);
			
			return;
		}
	}
	
	objects.push_back(obj);	
	
	if (objects.size() > maxObjects && level < maxLevels) {
		if (nodes[0] == nullptr)
			split();
		
		int i = 0;
		while (i < objects.size()) {
			ObjectData current = objects.at(i);
			int index = getIndex(current);
			
			if (index != -1) {
				nodes[index]->insert(current.id, current.pos, current.rad);
				objects.erase(objects.begin() + i);
			} else {
				i++;
			}
		}
	}
}

/**
 * Returns a list of all game objects that a given object may potentially collide with
 */
std::list< int > Quadtree::getObjects ( std::list< int >& returnObjects, int id, irr::core::vector3df pos, float rad ) {
	ObjectData obj;
	obj.id = id;
	obj.pos = pos;
	obj.rad = rad;
	
	int index = getIndex(obj);
	if (index != -1 && nodes[0] != nullptr) {
		nodes[index]->getObjects(returnObjects, id, pos, rad);
	}

	for (ObjectData o : objects) {
		returnObjects.push_back(o.id);
	}
	
	return returnObjects;
}

/**
 * Recursively draw the quadtree
 */
void Quadtree::draw() {
	
	irr::video::SMaterial m;
	m.Lighting = false;
	m.Thickness = 1.0f;
	RenderManager::renderManager.getDriver()->setMaterial(m);
	RenderManager::renderManager.getDriver()->setTransform(irr::video::ETS_WORLD, irr::core::IdentityMatrix);
	
	irr::video::SColorHSL col(100 * (1.0-(1.0/maxLevels*level)), 100, 50);
	irr::video::SColorf colRGBf;
	col.toRGB(colRGBf);
	irr::video::SColor colRGB = colRGBf.toSColor();
	
	RenderManager::renderManager.getDriver()->draw3DLine(
		irr::core::vector3df(bounds.UpperLeftCorner.X, 1+level/6.0, bounds.UpperLeftCorner.Y),
		irr::core::vector3df(bounds.UpperLeftCorner.X + bounds.getWidth(), 1+level/6.0, bounds.UpperLeftCorner.Y),
		colRGB);
	
	RenderManager::renderManager.getDriver()->draw3DLine(
		irr::core::vector3df(bounds.UpperLeftCorner.X, 1+level/6.0, bounds.UpperLeftCorner.Y),
		irr::core::vector3df(bounds.UpperLeftCorner.X, 1+level/6.0, bounds.UpperLeftCorner.Y + bounds.getHeight()),
		colRGB);
	
	RenderManager::renderManager.getDriver()->draw3DLine(
		irr::core::vector3df(bounds.LowerRightCorner.X, 1+level/6.0, bounds.LowerRightCorner.Y),
		irr::core::vector3df(bounds.LowerRightCorner.X - bounds.getWidth(), 1+level/6.0, bounds.LowerRightCorner.Y),
		colRGB);
	
	RenderManager::renderManager.getDriver()->draw3DLine(
		irr::core::vector3df(bounds.LowerRightCorner.X, 1+level/6.0, bounds.LowerRightCorner.Y),
		irr::core::vector3df(bounds.LowerRightCorner.X, 1+level/6.0, bounds.LowerRightCorner.Y - bounds.getHeight()),
		colRGB);
	
	if (nodes[0] != nullptr) {
		nodes[0]->draw();
		nodes[1]->draw();
		nodes[2]->draw();
		nodes[3]->draw();
	}
}








