#include <MapGenerator.h>
#include <irrlicht/irrlicht.h>
#include <Game.h>
#include <ObjectFactory.h>
#include <NoiseGenerator.h>
#include <time.h>

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
	
	for (int x = 0; x < 120; x++) {
		for (int y = 0; y < 120; y++) {
			if (rand() % 1000 == 0)
				placeRock(vector2df(x*4,y*4), smgr);
		}
	}
	
	/*int mapWidth = Game::game.getObjMgr()->worldManager->gridWidth;
	int mapHeight = Game::game.getObjMgr()->worldManager->gridHeight;
	float gridSize = Game::game.getObjMgr()->worldManager->gridSize;
	
	srand(time(NULL));
	NoiseGenerator gen;
	std::cout << "Generating noise map... ";
	gen.generatePerlinNoise(rand(), 480, 480);
	std::cout << "Done" << std::endl;
	
	std::cout << "Building map... ";
	for (int x = 0; x < mapWidth; x++) {
		for (int y = 0; y < mapHeight; y++) {
			float val = gen.getNoiseAt(x,y);

			if (val < 0.5f)
				placeTree(vector2df(x * gridSize, y * gridSize), smgr);
		}
	}
	std::cout << "Done" << std::endl;*/
}

void MapGenerator::placeTree ( vector2df pos, ISceneManager* smgr ) {
	ITerrainSceneNode* terrain = (ITerrainSceneNode*)smgr->getSceneNodeFromName("MainTerrain");
	int yPos = 0;
	
	if (terrain != nullptr)
		yPos = terrain->getHeight(pos.X, pos.Y);
	
	if (Game::game.getObjMgr()->worldManager->checkPassable(vector3df(pos.X, yPos, pos.Y))) {
		ObjectFactory::addTree(vector3df(pos.X, yPos, pos.Y));
		Game::game.getObjMgr()->worldManager->setPassable(rectf(pos.X-0.5f, pos.Y-0.5f, pos.X+0.5f, pos.Y+0.5f), false);
	}
}

void MapGenerator::placeRock ( vector2df pos, ISceneManager* smgr ) {
	ITerrainSceneNode* terrain = (ITerrainSceneNode*)smgr->getSceneNodeFromName("MainTerrain");
	int yPos = 0;
	
	if (terrain != nullptr)
		yPos = terrain->getHeight(pos.X, pos.Y);
	
	ObjectFactory::addRock(vector3df(pos.X, yPos, pos.Y));
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
