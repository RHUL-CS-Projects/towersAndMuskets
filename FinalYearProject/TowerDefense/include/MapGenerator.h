#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H

#include <irrlicht/irrlicht.h>

class MapGenerator {
private:
	void addTerrain(irr::scene::ISceneManager* smgr);
	void placeTree(irr::core::vector2df pos, irr::scene::ISceneManager* smgr);
	void placeRock(irr::core::vector2df pos, irr::scene::ISceneManager* smgr);
public:
	void generateMap();
};

#endif