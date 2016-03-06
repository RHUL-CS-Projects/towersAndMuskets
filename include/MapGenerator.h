#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H

#include <irrlicht/irrlicht.h>
#include <string>

class MapGenerator {
private:
	void addTerrain(irr::scene::ISceneManager* smgr);
	void placeTree(irr::core::vector2df pos, irr::scene::ISceneManager* smgr);
	void placeRock(irr::core::vector2df pos, irr::scene::ISceneManager* smgr);
	void placeGold(irr::core::vector2df pos, irr::scene::ISceneManager* smgr);
	void loadMap(std::string mapname);
public:
	void generateMap(std::string mapname);
};

#endif