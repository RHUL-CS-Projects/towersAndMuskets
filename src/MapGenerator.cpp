#include <MapGenerator.h>
#include <irrlicht/irrlicht.h>
#include <Game.h>
#include <ObjectFactory.h>
#include <NoiseGenerator.h>
#include <time.h>
#include <unistd.h>

using namespace irr;
using namespace scene;
using namespace core;
using namespace video;
using namespace io;

void MapGenerator::generateMap(std::string mapname) {
	ISceneManager* smgr = Game::game.getRendMgr()->getSceneManager();

	addTerrain(smgr);
	loadMap(mapname);
	
	/*for (int x = 0; x < 240; x++) {
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
	}*/
	
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

void MapGenerator::loadMap ( std::string mapname ) {
	std::string filename = RenderManager::resPath + "/materials/textures/" + mapname + ".png";
	IImage* mapTexture = Game::game.getRendMgr()->getDriver()->createImageFromFile(path(filename.c_str()));
	
	if (mapTexture != nullptr) {
		std::cout << "Map '" << mapname << "' loaded successfully" << std::endl; 
		float gridSize = Game::game.getObjMgr()->worldManager->gridSize;
		ISceneManager* smgr = Game::game.getRendMgr()->getSceneManager();
		
		int objCount = 0;
		int counter = 0;
		
		for (int x = 0; x < mapTexture->getDimension().Width; x++) {
			for (int y = 0; y < mapTexture->getDimension().Height; y++) {
				u32 data = mapTexture->getPixel(x,y).color;
				
				if (data == SColor(255,0,255,0).color) {
					placeTree(vector2df(x * gridSize + gridSize/2, y * gridSize + gridSize/2), smgr);
				}
				
				if (data == SColor(255,200,200,200).color) {
					placeRock(vector2df(x * gridSize, y * gridSize), smgr);
				}
				
				if (data == SColor(255,255,255,0).color) {
					placeGold(vector2df(x * gridSize, y * gridSize), smgr);
				}
			}
		}
		
		mapTexture->drop();
	} else {
		std::cout << "Map '" << mapname << "' could not be loaded at path " << filename << std::endl; 
	}
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
	
	ObjectFactory::addRock(vector3df(floor(pos.X), floor(yPos), floor(pos.Y)));
}

void MapGenerator::placeGold ( vector2df pos, ISceneManager* smgr ) {
	ITerrainSceneNode* terrain = (ITerrainSceneNode*)smgr->getSceneNodeFromName("MainTerrain");
	int yPos = 0;
	
	if (terrain != nullptr)
		yPos = terrain->getHeight(pos.X, pos.Y);
	
	ObjectFactory::addGold(vector3df(pos.X, yPos, pos.Y));
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
