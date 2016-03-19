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
	ITerrainSceneNode* terrain = (ITerrainSceneNode*)Game::game.getRendMgr()->getSceneManager()->getSceneNodeFromName("MainTerrain");
	Game::game.getObjMgr()->worldManager->setTerrainData(terrain);
	Game::game.getObjMgr()->worldManager->clear();
	loadMap(mapname);
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
					placeRock(vector2df(x * gridSize + gridSize/2, y * gridSize + gridSize/2), smgr);
				}
				
				if (data == SColor(255,255,255,0).color) {
					placeGold(vector2df(x * gridSize + gridSize/2, y * gridSize + gridSize/2), smgr);
				}
				
				if (data == SColor(255,255,0,0).color) {
					placeVillager(vector2df(x * gridSize + gridSize/2, y * gridSize + gridSize/2), smgr);
				}
				
				if (data == SColor(255,255,255,255).color) {
					placeTownhall(vector2df(x * gridSize + gridSize/2, y * gridSize + gridSize/2 + 2), smgr);
				}
				
				if (data == SColor(255,0,0,255).color) {
					placeSpawn(vector2df(x * gridSize + gridSize/2, y * gridSize + gridSize/2 + 2), smgr);
				}
			}
		}
		
		// Build terrain texture
		/*ITerrainSceneNode* terrain = (ITerrainSceneNode*)Game::game.getRendMgr()->getSceneManager()->getSceneNodeFromName("MainTerrain");
		
		float gridW = Game::game.getObjMgr()->worldManager->gridWidth;
		float gridH = Game::game.getObjMgr()->worldManager->gridHeight;
		float scale = 16;
		float texWidth = gridW * gridSize * scale;
		float texHeight = gridH * gridSize * scale;
		
		IVideoDriver* driver = Game::game.getRendMgr()->getDriver();
		ITexture* texTerrain = driver->addRenderTargetTexture(dimension2du(texWidth, texHeight));
		ITexture* texGrass1 = driver->getTexture("./res/materials/textures/grass-texture2.jpg");
		ITexture* texGrass2 = driver->getTexture("./res/materials/textures/grass-texture5.png");
		driver->setRenderTarget(texTerrain);
		
		float tileSize = texWidth / 32;
		
		for (int x = 0; x < 32; x++) {
			for (int y = 0; y < 32; y++) {
				driver->draw2DImage(
					texGrass1, 
					recti(tileSize*x, tileSize*y, tileSize*x + texWidth/32, tileSize*y + texHeight/32),
					recti(0, 0, texGrass1->getSize().Width, texGrass1->getSize().Height)
				);
			}
		}
		
		for (int x = 0; x < gridW; x++) {
			for (int y = 0; y < gridH; y++) {
				int dx = gridW - x - 1;
				int dy = y;
				
				if (Game::game.getObjMgr()->worldManager->checkPassableGridCoords(x,y)) {
					driver->draw2DImage(
						texGrass1, 
						recti((dx * gridSize)*scale, (dy * gridSize)*scale, (dx * gridSize + gridSize)*scale, (dy * gridSize + gridSize)*scale),
						recti(0, 0, texGrass1->getSize().Width, texGrass1->getSize().Height)
					);
				} else {
					driver->draw2DImage(
						texGrass2, 
						recti(
							(dx * gridSize)*scale - (gridSize*scale*0.5f), 
							(dy * gridSize)*scale - (gridSize*scale*0.5f), 
							(dx * gridSize + gridSize)*scale + (gridSize*scale*0.5f), 
							(dy * gridSize + gridSize)*scale  + (gridSize*scale*0.5f)
						),
						recti(0, 0, texGrass2->getSize().Width, texGrass2->getSize().Height), 0, 0, true
					);
				}
			}
		}
		
		driver->setRenderTarget(0);
		terrain->setMaterialTexture(0, texTerrain);
		terrain->scaleTexture(1, 1);*/
		
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

void MapGenerator::placeTownhall ( vector2df pos, ISceneManager* smgr ) {
	ITerrainSceneNode* terrain = (ITerrainSceneNode*)smgr->getSceneNodeFromName("MainTerrain");
	int yPos = 0;
	
	if (terrain != nullptr)
		yPos = terrain->getHeight(pos.X, pos.Y);
	
	if (Game::game.getObjMgr()->worldManager->checkPassable(vector3df(pos.X, yPos, pos.Y))) {
		ObjectFactory::addTownhall(vector3df(pos.X, yPos, pos.Y));
		double gridSize = Game::game.getObjMgr()->worldManager->gridSize;
		Game::game.getObjMgr()->worldManager->setPassable(rectf(pos.X-(1.5f * gridSize), pos.Y-(2.5f * gridSize), pos.X+(1.5f * gridSize), pos.Y+(0.5f * gridSize)), false);
	}
}

void MapGenerator::placeSpawn( vector2df pos, ISceneManager* smgr ) {
	ITerrainSceneNode* terrain = (ITerrainSceneNode*)smgr->getSceneNodeFromName("MainTerrain");
	int yPos = 0;
	
	if (terrain != nullptr)
		yPos = terrain->getHeight(pos.X, pos.Y);
	
	if (Game::game.getObjMgr()->worldManager->checkPassable(vector3df(pos.X, yPos, pos.Y))) {
		ObjectFactory::addSpawnLocation(vector3df(pos.X, yPos, pos.Y));
		double gridSize = Game::game.getObjMgr()->worldManager->gridSize;
	}
}

void MapGenerator::placeRock ( vector2df pos, ISceneManager* smgr ) {
	ITerrainSceneNode* terrain = (ITerrainSceneNode*)smgr->getSceneNodeFromName("MainTerrain");
	int yPos = 0;
	
	if (terrain != nullptr)
		yPos = terrain->getHeight(pos.X, pos.Y);
	
	if (Game::game.getObjMgr()->worldManager->checkPassable(vector3df(pos.X, yPos, pos.Y))) {
		ObjectFactory::addRock(vector3df(pos.X, yPos, pos.Y));
		Game::game.getObjMgr()->worldManager->setPassable(rectf(pos.X-0.5f, pos.Y-0.5f, pos.X+0.5f, pos.Y+0.5f), false);
	}
}

void MapGenerator::placeGold ( vector2df pos, ISceneManager* smgr ) {
	ITerrainSceneNode* terrain = (ITerrainSceneNode*)smgr->getSceneNodeFromName("MainTerrain");
	int yPos = 0;
	
	if (terrain != nullptr)
		yPos = terrain->getHeight(pos.X, pos.Y);
	
	if (Game::game.getObjMgr()->worldManager->checkPassable(vector3df(pos.X, yPos, pos.Y))) {
		ObjectFactory::addGold(vector3df(pos.X, yPos, pos.Y));
		Game::game.getObjMgr()->worldManager->setPassable(rectf(pos.X-0.5f, pos.Y-0.5f, pos.X+0.5f, pos.Y+0.5f), false);
	}
}

void MapGenerator::placeVillager ( vector2df pos, ISceneManager* smgr ) {
	ITerrainSceneNode* terrain = (ITerrainSceneNode*)smgr->getSceneNodeFromName("MainTerrain");
	int yPos = 0;
	
	if (terrain != nullptr)
		yPos = terrain->getHeight(pos.X, pos.Y);
	
	if (Game::game.getObjMgr()->worldManager->checkPassable(vector3df(pos.X, yPos, pos.Y))) {
		ObjectFactory::addPlayerVillager(vector3df(pos.X, yPos, pos.Y));
	}
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
		1
	);
	
	terrain->setMaterialFlag(video::EMF_LIGHTING, true);
	terrain->setMaterialTexture(0, Game::game.getRendMgr()->getDriver()->getTexture("./res/materials/textures/grass-texture2.jpg"));
	terrain->scaleTexture(32, 32);
	
	ITriangleSelector* terrainSelector = smgr->createTerrainTriangleSelector(terrain);
	terrain->setTriangleSelector(terrainSelector);
	terrain->setName("MainTerrain");
}
