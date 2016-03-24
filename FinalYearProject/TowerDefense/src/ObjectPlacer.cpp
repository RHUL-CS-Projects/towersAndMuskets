#include <ObjectPlacer.h>
#include <irrlicht/irrlicht.h>
#include <EventReceiver.h>
#include <Game.h>
#include <ObjectFactory.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

void ObjectPlacer::init() {
	sceneNode = nullptr;
}

void ObjectPlacer::update() {
	//Get mouse position on terrain
	ISceneCollisionManager* colmgr = Game::game.getRendMgr()->getSceneManager()->getSceneCollisionManager();
	line3df ray = colmgr->getRayFromScreenCoordinates(EventReceiver::getMouseState()->position);
	
	triangle3df triangle;
	ISceneNode* node;
	
	// Check if the user clicked on terrain or object
	colmgr->getCollisionPoint(ray, Game::game.getRendMgr()->getSceneManager()->getSceneNodeFromName("MainTerrain")->getTriangleSelector(), terrainPoint, triangle, node);
	
	if (sceneNode == nullptr) {
		setModelData("rock.x", "RockTexture.png");
	}
	
	WorldManager* world = Game::game.getObjMgr()->worldManager;
	float gridSize = world->gridSize;
	
	if (snap) {
		terrainPoint.X = round(terrainPoint.X / gridSize) * gridSize;
		terrainPoint.Z = round(terrainPoint.Z / gridSize) * gridSize;
	}
	
	sceneNode->setPosition(terrainPoint);
	sceneNode->setRotation(vector3df(-90, 0, 0));
	
	if (EventReceiver::getMouseState()->leftPressed && !mousePressed && currentType != nullObject &&
		EventReceiver::getMouseState()->position.Y < Game::game.getRendMgr()->getDriver()->getScreenSize().Height - 128) {
		mousePressed = true;
		place();
	} 
	
	if (!EventReceiver::getMouseState()->leftPressed) {
		mousePressed = false;
	}
	
	if (EventReceiver::getMouseState()->rightPressed) {
		//setObjectType(nullObject);
	}
}

void ObjectPlacer::setObjectType ( OBJECT_TYPES type ) {
	currentType = type;
	std::vector<std::string> textureNames;
	snap = false;
	
	switch (currentType) {
	case Tower:
		snap = true;
		colWidth = 2; colHeight = 2;
		setModelData("tower2.x", "TowerTexture.png");
		sceneNode->setVisible(true);
		addObject = &ObjectFactory::addTower;
		break;
	case Tree:
		colWidth = 0.5f; colHeight = 0.5f;
		setModelData("tree.x", "TreeTexture.png");
		sceneNode->setVisible(true);
		addObject = &ObjectFactory::addTree;
		break;
	case Rock:
		colWidth = 1; colHeight = 1;
		setModelData("rock.x", "RockTexture.png");
		sceneNode->setVisible(true);
		addObject = &ObjectFactory::addRock;
		break;
	case EnemyUnit:
		colWidth = 0; colHeight = 0;
		setModelData("humanteststatic.x", "ManTexture2.png");
		sceneNode->setVisible(true);
		addObject = &ObjectFactory::addEnemyUnit;
		break;
	case PlayerUnit:
		colWidth = 0; colHeight = 0;
		setModelData("humanteststatic.x", "ManTexture.png");
		sceneNode->setVisible(true);
		addObject = &ObjectFactory::addPlayerUnit;
		break;
	case PlayerCannon:
		colWidth = 0; colHeight = 0;
		
		textureNames.push_back("ManTexture.png");
		textureNames.push_back("CannonTexture.png");
		
		setModelData("cannonstatic.x", textureNames);
		sceneNode->setVisible(true);
		addObject = &ObjectFactory::addPlayerCannon;
		break;
	case PlayerVillager:
		colWidth = 0; colHeight = 0;
		
		textureNames.push_back("VillagerTexture.png");
		
		setModelData("humanteststatic.x", "VillagerTexture.png");
		sceneNode->setVisible(true);
		addObject = &ObjectFactory::addPlayerVillager;
		break;
	case nullObject:
		sceneNode->setVisible(false);
		break;
	}
}

bool ObjectPlacer::checkPlaceable ( vector3df pos ) {
	if (colHeight == 0 || colWidth == 0)
		return true;
	
	WorldManager* world = Game::game.getObjMgr()->worldManager;
	float gridSize = world->gridSize;
	
	return !world->checkColliding(rectf(pos.X - (colWidth * gridSize) / 2.0f, pos.Z - (colHeight * gridSize) / 2.0f, pos.X + (colWidth * gridSize) / 2.0f, pos.Z + (colHeight * gridSize) / 2.0f));
}

void ObjectPlacer::render ( video::IVideoDriver* driver ) {
	if (currentType != nullObject) {
		SColor color(255,255,255,255);
		
		if (placeable)
			color = SColor(255,255,0,0);
	}
}

void ObjectPlacer::setModelData ( std::string meshName, std::string textureName ) {
	std::vector<std::string> textures;
	textures.push_back(textureName);
	setModelData(meshName, textures);
}

void ObjectPlacer::setModelData ( std::string meshName, std::vector<std::string> textureNames ) {
	std::string meshPath = RenderManager::resPath + "/models/" + meshName;
	IMesh* mesh = Game::game.getRendMgr()->getSceneManager()->getMesh(meshPath.c_str());

	if (sceneNode != nullptr)
		sceneNode->remove();
	
	sceneNode = Game::game.getRendMgr()->getSceneManager()->addMeshSceneNode(mesh, 0, -1, terrainPoint, vector3df(-90, 0, 0));
	
	sceneNode->setMaterialFlag(video::EMF_LIGHTING, true);
	sceneNode->setMaterialFlag(video::EMF_GOURAUD_SHADING, true);
	sceneNode->setMaterialFlag(video::EMF_WIREFRAME, true);
	
	if (textureNames.size() == 1) {
		std::string texturePath = RenderManager::resPath + "/materials/textures/" + textureNames[0];
		ITexture* texture = Game::game.getRendMgr()->getDriver()->getTexture(texturePath.c_str());
		sceneNode->setMaterialTexture(0, texture);
	} else {
		int layer = 0;
		for (std::string texture : textureNames) {
			std::string texturePath = RenderManager::resPath + "/materials/textures/" + textureNames[layer];
			
			sceneNode->getMaterial(layer).setTexture(0, Game::game.getRendMgr()->getDriver()->getTexture(texturePath.c_str()));
			layer++;
		}
	}
	
	sceneNode->getMaterial(0).SpecularColor = SColor(0,0,0,0);
	sceneNode->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	
	sceneNode->setVisible(false);
}

void ObjectPlacer::place() {
	if (checkPlaceable(terrainPoint)) {
		WorldManager* world = Game::game.getObjMgr()->worldManager;
		float gridSize = world->gridSize;
		
		if (colWidth != 0 && colHeight != 0)
			world->setPassable(rectf(terrainPoint.X - (colWidth * gridSize) / 2.0f, 
									 terrainPoint.Z - (colHeight * gridSize) / 2.0f, 
									 terrainPoint.X + (colWidth * gridSize) / 2.0f, 
									 terrainPoint.Z + (colHeight * gridSize) / 2.0f), false);
		
		addObject(terrainPoint);
		setObjectType(nullObject);
	}
}





