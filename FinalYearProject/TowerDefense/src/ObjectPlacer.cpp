#include <ObjectPlacer.h>
#include <irrlicht/irrlicht.h>
#include <EventReceiver.h>
#include <Game.h>
#include <ObjectFactory.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

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
		setObjectType(nullObject);
	}
}

void ObjectPlacer::setObjectType ( OBJECT_TYPES type ) {
	currentType = type;
	
	switch (currentType) {
	case Tower:
		colWidth = 4; colHeight = 4;
		setModelData("tower2.x", "TowerTexture.png");
		sceneNode->setVisible(true);
		addObject = &ObjectFactory::addTower;
		break;
	case Tree:
		colWidth = 0; colHeight = 0;
		setModelData("tree.x", "TreeTexture.png");
		sceneNode->setVisible(true);
		addObject = &ObjectFactory::addTree;
		break;
	case Rock:
		colWidth = 0; colHeight = 0;
		setModelData("rock.x", "RockTexture.png");
		sceneNode->setVisible(true);
		addObject = &ObjectFactory::addRock;
		break;
	case EnemyUnit:
		colWidth = 0; colHeight = 0;
		setModelData("humantest.x", "ManTexture2.png");
		sceneNode->setVisible(true);
		addObject = &ObjectFactory::addEnemyUnit;
		break;
	case PlayerUnit:
		colWidth = 0; colHeight = 0;
		setModelData("humantest.x", "ManTexture.png");
		sceneNode->setVisible(true);
		addObject = &ObjectFactory::addPlayerUnit;
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
	
	return !world->checkColliding(rectf(pos.X - colWidth / 2.0f, pos.Z - colHeight / 2.0f, pos.X + colWidth / 2.0f, pos.Z + colHeight / 2.0f));
}

void ObjectPlacer::render ( video::IVideoDriver* driver ) {
	if (currentType != nullObject) {
		SColor color(255,255,255,255);
		
		if (placeable)
			color = SColor(255,255,0,0);
	}
}

void ObjectPlacer::setModelData ( std::string meshName, std::string textureName ) {
	std::string meshPath = RenderManager::resPath + "/models/" + meshName;
	std::string texturePath = RenderManager::resPath + "/materials/textures/" + textureName;
	
	IMesh* mesh = Game::game.getRendMgr()->getSceneManager()->getMesh(meshPath.c_str());
	ITexture* texture = Game::game.getRendMgr()->getDriver()->getTexture(texturePath.c_str());
	
	if (sceneNode != nullptr)
		sceneNode->remove();
	
	sceneNode = Game::game.getRendMgr()->getSceneManager()->addMeshSceneNode(mesh, 0, -1, terrainPoint, vector3df(-90, 0, 0));
	
	sceneNode->setMaterialFlag(video::EMF_LIGHTING, true);
	sceneNode->setMaterialFlag(video::EMF_GOURAUD_SHADING, true);
	sceneNode->setMaterialFlag(video::EMF_WIREFRAME, true);
	
	sceneNode->setMaterialTexture(0, texture);
	sceneNode->getMaterial(0).SpecularColor = SColor(0,0,0,0);
	sceneNode->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	
	sceneNode->setVisible(false);
}

void ObjectPlacer::place() {
	if (checkPlaceable(terrainPoint)) {
		WorldManager* world = Game::game.getObjMgr()->worldManager;
	
		if (colWidth != 0 && colHeight != 0)
			world->setPassable(rectf(terrainPoint.X - colWidth / 2, terrainPoint.Z - colHeight / 2, terrainPoint.X + colWidth / 2, terrainPoint.Z + colHeight / 2), false);
		
		addObject(terrainPoint);
	}
}





