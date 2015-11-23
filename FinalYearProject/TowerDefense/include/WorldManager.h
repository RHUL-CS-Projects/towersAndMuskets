#ifndef WORLDMANAGER_H
#define WORLDMANAGER_H

#include <irrlicht/irrlicht.h>

class WorldManager {
private:
	short *grid;
	int gridWidth;
	int gridHeight;
	float gridSize;
	
	irr::core::recti worldBounds;
	
	void setGridXY(int x, int y, short val);
	int getGridXY(int x, int y);
public:
	WorldManager(float gridSize, irr::core::recti worldBounds) {
		this->gridSize = gridSize;
		this->worldBounds = worldBounds;
		
		gridWidth = (int)ceil(worldBounds.getWidth() / gridSize);
		gridHeight = (int)ceil(worldBounds.getHeight() / gridSize);
		
		grid = new short[gridWidth * gridHeight]();
		
	}
	
	void setPassable(irr::core::rectf bounds, bool passable);
	bool checkPassable(irr::core::vector3df worldPosition);
	bool checkColliding(irr::core::rectf bounds);
	bool checkColliding(irr::core::vector3df worldPosition, float rad);
	
	void draw(float timestep);
	
	void dropWorld(); // Delete grid
};

#endif