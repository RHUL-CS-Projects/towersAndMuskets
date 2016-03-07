#include <Quadtree.h>
#include <iostream>
#include <RenderManager.h>
#include <irrlicht/irrlicht.h>
#include <Game.h>

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

std::list<int> Quadtree::getObjectsInRange(std::list<int>& returnObjects, irr::core::vector3df pos, float distance) {
	distance *= 2;
	irr::core::rectf rect(pos.X - distance/2, pos.Z - distance/2, pos.X + distance/2, pos.Z + distance/2);
	
	return getObjectsInBox(returnObjects, rect);
}


std::list<int> Quadtree::getObjectsInBox(std::list<int>& returnObjects, irr::core::rectf box) {
	if (!bounds.isRectCollided(box)) {
		return returnObjects;
	}
	
	for (ObjectData o : objects) {
		if (box.isPointInside(irr::core::vector2df(o.pos.X, o.pos.Z)))
			returnObjects.push_back(o.id);
	}
	
	if (nodes[0] == nullptr)
		return returnObjects;
	
	nodes[0]->getObjectsInBox(returnObjects, box);
	nodes[1]->getObjectsInBox(returnObjects, box);
	nodes[2]->getObjectsInBox(returnObjects, box);
	nodes[3]->getObjectsInBox(returnObjects, box);
	
	return returnObjects;
}

/**
 * Recursively draw the quadtree
 */
void Quadtree::draw() {
	
	irr::video::SMaterial m;
	m.Lighting = false;
	m.Thickness = 1.0f;
	Game::game.getRendMgr()->getDriver()->setMaterial(m);
	Game::game.getRendMgr()->getDriver()->setTransform(irr::video::ETS_WORLD, irr::core::IdentityMatrix);
	
	irr::video::SColorHSL col(100 * (1.0-(1.0/maxLevels*level)), 100, 50);
	irr::video::SColorf colRGBf;
	col.toRGB(colRGBf);
	irr::video::SColor colRGB = colRGBf.toSColor();
	
	Game::game.getRendMgr()->getDriver()->draw3DLine(
		irr::core::vector3df(bounds.UpperLeftCorner.X, 1+level/6.0, bounds.UpperLeftCorner.Y),
		irr::core::vector3df(bounds.UpperLeftCorner.X + bounds.getWidth(), 1+level/6.0, bounds.UpperLeftCorner.Y),
		colRGB);
	
	Game::game.getRendMgr()->getDriver()->draw3DLine(
		irr::core::vector3df(bounds.UpperLeftCorner.X, 1+level/6.0, bounds.UpperLeftCorner.Y),
		irr::core::vector3df(bounds.UpperLeftCorner.X, 1+level/6.0, bounds.UpperLeftCorner.Y + bounds.getHeight()),
		colRGB);
	
	Game::game.getRendMgr()->getDriver()->draw3DLine(
		irr::core::vector3df(bounds.LowerRightCorner.X, 1+level/6.0, bounds.LowerRightCorner.Y),
		irr::core::vector3df(bounds.LowerRightCorner.X - bounds.getWidth(), 1+level/6.0, bounds.LowerRightCorner.Y),
		colRGB);
	
	Game::game.getRendMgr()->getDriver()->draw3DLine(
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

int Quadtree::getNearest ( int id, irr::core::vector3df pos ) {
	float x = pos.X;
	float y = pos.Z;
	
	BestNode best;
	best.distance = -1;
	best.object.id = -1;
	
	best = nearest(x, y, id, best, this);
	
	return best.object.id;
}

Quadtree::BestNode Quadtree::nearest ( float x, float y, int id, Quadtree::BestNode best, Quadtree* node ) {
	float x1 = node->bounds.UpperLeftCorner.X;
	float y1 = node->bounds.UpperLeftCorner.Y;
	float x2 = node->bounds.LowerRightCorner.X;
	float y2 = node->bounds.LowerRightCorner.Y;

	if (x < x1 - best.distance || x > x2 + best.distance || y < y1 - best.distance || y > y2 + best.distance) {
		return best;
	}
	
	for (ObjectData o : node->objects) {
		if (o.id == id) continue;
		
		float dx = o.pos.X - x;
		dx *= dx;
		
		float dy = o.pos.Z - y;
		dy *= dy;
		
		float d = dx + dy;
		
		if (d < best.distance || best.distance < 0) {
			best.distance = d;
			best.object = o;
		}
	}
	
	Quadtree** children = node->nodes;
	int rl = (2*x > x1 + x2) ? 1 : 0;
	int bt = (2*y > y1 + y2) ? 1 : 0;
	
	if (children[bt*2+rl]) best = nearest(x, y, id, best, children[bt*2+rl]);
	if (children[bt*2+(1-rl)]) best = nearest(x, y, id, best, children[bt*2+(1-rl)]);
	if (children[(1-bt)*2+rl]) best = nearest(x, y, id, best, children[(1-bt)*2+rl]);
	if (children[(1-bt)*2+(1-rl)]) best = nearest(x, y, id, best, children[(1-bt)*2+(1-rl)]);
	
	return best;
}






