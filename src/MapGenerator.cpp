#include <MapGenerator.h>
#include <irrlicht/irrlicht.h>
#include <Game.h>
#include <ObjectFactory.h>

using namespace irr;
using namespace scene;
using namespace core;
using namespace video;

void MapGenerator::generateMap() {
	ISceneManager* smgr = Game::game.getRendMgr()->getSceneManager();
	
	addTerrain(smgr);
	
	for (int x = 0; x < 240; x++) {
		for (int y = 0; y < 240; y++) {
			if (rand() % 400 == 0)
				placeTree(vector2df(x*2,y*2), smgr);
		}
	}
}

void MapGenerator::placeTree ( vector2df pos, ISceneManager* smgr ) {
	ITerrainSceneNode* terrain = (ITerrainSceneNode*)smgr->getSceneNodeFromName("MainTerrain");
	int yPos = 0;
	
	if (terrain != nullptr)
		yPos = terrain->getHeight(pos.X, pos.Y);
	
	ObjectFactory::addTree(vector3df(pos.X, yPos, pos.Y));
}

void MapGenerator::addTerrain ( ISceneManager* smgr ) {
	ITerrainSceneNode* terrain = smgr->addTerrainSceneNode(
		"./res/materials/textures/terrain-heightmap-flat.bmp", 
		0, 
		-1, 
		vector3df(0,0,0),
		vector3df(0,0,0),
		vector3df(2,0.2f,2),
		SColor(255,255,255,255),
		1,
		scene::ETPS_17,
		5
	);
	
	terrain->setMaterialFlag(video::EMF_LIGHTING, true);
	terrain->setMaterialTexture(0, Game::game.getRendMgr()->getDriver()->getTexture("./res/materials/textures/grass-texture2.jpg"));
	//terrain->setMaterialTexture(1, driver->getTexture("./res/materials/textures/grass-texture4.jpg"));
	//terrain->setMaterialType(video::EMT_DETAIL_MAP);
	terrain->scaleTexture(32, 32);
	
	ITriangleSelector* terrainSelector = smgr->createTerrainTriangleSelector(terrain);
	terrain->setTriangleSelector(terrainSelector);
	terrain->setName("MainTerrain");
}
