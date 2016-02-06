#include <WorldManager.h>
#include <RenderManager.h>
#include <Game.h>

using namespace irr;
using namespace core;
using namespace video;
using namespace gui;

/**
 * Set the value on the grid at an x and y position
 */
void WorldManager::setGridXY ( int x, int y, short val ) {
	grid[x + y * gridWidth] = val;
}

/**
 * Get the value stored in the grid at an x and y position
 */
int WorldManager::getGridXY ( int x, int y ) {
	return grid[x + y * gridWidth];
}

/**
 * Set the region of the grid that intersects a given rectangle
 * to be either passable or impassable
 */
void WorldManager::setPassable ( rectf bounds, bool passable ) {
	int topLeftX = (int)floor(bounds.UpperLeftCorner.X/gridSize);
	int topLeftY = (int)floor(bounds.UpperLeftCorner.Y/gridSize);
	
	int bottomRightX = (int)floor(bounds.LowerRightCorner.X/gridSize);
	int bottomRightY = (int)floor(bounds.LowerRightCorner.Y/gridSize);
	
	topLeftX = clamp<int>(topLeftX, 0, gridWidth-1);
	topLeftY = clamp<int>(topLeftY, 0, gridHeight-1);
	bottomRightX = clamp<int>(bottomRightX, 0, gridWidth-1);
	bottomRightY = clamp<int>(bottomRightY, 0, gridHeight-1);
	
	for (int x = topLeftX; x <= bottomRightX; x++) {
		for (int y = topLeftY; y <= bottomRightY; y++) {
			setGridXY(x, y, passable ? 0 : 1);
		}
	}
}

/**
 * Check if a circular region is intersecting the world.
 * This simplified the circle down to a rectangular area and uses the
 * other checkColliding method
 */ 
bool WorldManager::checkColliding ( vector3df worldPosition, float rad ) {
	return checkColliding(rectf(worldPosition.X - rad, worldPosition.Z - rad, worldPosition.X + rad, worldPosition.Z + rad));
}

/**
 * Check if a rectangular area is intersecting the world
 */
bool WorldManager::checkColliding ( rectf bounds ) {
	int topLeftX = (int)floor(bounds.UpperLeftCorner.X/gridSize);
	int topLeftY = (int)floor(bounds.UpperLeftCorner.Y/gridSize);
	
	int bottomRightX = (int)floor(bounds.LowerRightCorner.X/gridSize);
	int bottomRightY = (int)floor(bounds.LowerRightCorner.Y/gridSize);
	
	topLeftX = clamp<int>(topLeftX, 0, gridWidth-1);
	topLeftY = clamp<int>(topLeftY, 0, gridHeight-1);
	bottomRightX = clamp<int>(bottomRightX, 0, gridWidth-1);
	bottomRightY = clamp<int>(bottomRightY, 0, gridHeight-1);
	
	for (int x = topLeftX; x <= bottomRightX; x++) {
		for (int y = topLeftY; y <= bottomRightY; y++) {
			if (getGridXY(x,y) > 0)
				return true;
		}
	}
	
	return false;
}

/**
 * Check if a position on the map is empty or impassable
 */
bool WorldManager::checkPassable ( vector3df worldPosition ) {
	int flooredX = (int)floor(worldPosition.X/gridSize);
	int flooredY = (int)floor(worldPosition.Z/gridSize);
	
	return getGridXY(flooredX, flooredY) == 0;
}

/**
 * Check if a position on the map is empty or impassable using grid coordinates
 */
bool WorldManager::checkPassableGridCoords ( int x, int y ) {
	return getGridXY(x,y) == 0;
}


/**
 * Draw the world grid as an overlay
 */
void WorldManager::draw ( float timestep ) {
	SMaterial m;
	m.Lighting = false;
	m.Thickness = 2.0f;
	m.setFlag(video::EMF_BLEND_OPERATION, true);
	Game::game.getRendMgr()->getDriver()->setMaterial(m);
	Game::game.getRendMgr()->getDriver()->setTransform(video::ETS_WORLD, IdentityMatrix);

	float offset = 0.1;
	
	for (int x = 0; x < gridWidth; x++) {
		for (int y = 0; y < gridHeight; y++) {
			SColor col(30,255,255,255);
			
			if (getGridXY(x,y) > 0) {
				col.setGreen(0);
				col.setBlue(0);
				col.setAlpha(255);
			}
			
			Game::game.getRendMgr()->getDriver()->draw3DLine(
			irr::core::vector3df(x*gridSize+offset, 0.1, y*gridSize+offset),
			irr::core::vector3df(x*gridSize+gridSize-offset, 0.1, y*gridSize+offset),
			col);

			Game::game.getRendMgr()->getDriver()->draw3DLine(
			irr::core::vector3df(x*gridSize+offset, 0.1, y*gridSize+offset),
			irr::core::vector3df(x*gridSize+offset, 0.1, y*gridSize+gridSize-offset),
			col);
			
			Game::game.getRendMgr()->getDriver()->draw3DLine(
			irr::core::vector3df(x*gridSize+offset, 0.1, y*gridSize+gridSize-offset),
			irr::core::vector3df(x*gridSize+gridSize-offset, 0.1, y*gridSize+gridSize-offset),
			col);
			
			Game::game.getRendMgr()->getDriver()->draw3DLine(
			irr::core::vector3df(x*gridSize+gridSize-offset, 0.1, y*gridSize+offset),
			irr::core::vector3df(x*gridSize+gridSize-offset, 0.1, y*gridSize+gridSize-offset),
			col);
		}
	}
}

/**
 * Clean up any resources declared by this world manager
 */
void WorldManager::dropWorld() {
	delete grid;
}


