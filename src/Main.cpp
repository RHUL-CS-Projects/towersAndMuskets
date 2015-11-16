#include <iostream>
#include <string>
#include <irrlicht/irrlicht.h>
#include <sfml/SFML/Window.hpp>

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <TowerDefenseEngine.h>

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
using namespace sf;

Server server;
Client client;

void signalExit(int s) {
	server.stopServer();
	client.disconnect();
}

void basicGraphics() {
	RenderManager::renderManager.init(L"Tower Defense");
	
	IrrlichtDevice* device = RenderManager::renderManager.getDevice();
	IVideoDriver* driver = RenderManager::renderManager.getDriver();
	ISceneManager* smgr = RenderManager::renderManager.getSceneManager();
	IGUIEnvironment* guienv = RenderManager::renderManager.getGUIEnvironment();
	
	smgr->setShadowColor(video::SColor(80,0,0,0));
	smgr->setAmbientLight(SColorf(0.8f, 0.8f, 0.8f));
	vector3df lightdir = vector3df( 0.8f, -1, 0.8f ).normalize();
	ILightSceneNode* light = smgr->addLightSceneNode(0, -lightdir * 10000, SColorf(1,1,1), 100000);
	light->setLightType(video::ELT_DIRECTIONAL);
	light->getLightData().Direction = lightdir;
	
	float cameraHeight = 100;
	float camX = -50;
	float camZ = -50;
	float camRotY = 45;
	float camAngleXZ = 1;
	smgr->addCameraSceneNode(0, vector3df(0, 0, 0), vector3df(0, 0, 0));
	//smgr->addCameraSceneNodeFPS(0, 50, 0.1f);
	ICameraSceneNode* camera = smgr->getActiveCamera();
	camera->setPosition(vector3df(camX, cameraHeight, camZ));
	camera->setTarget(vector3df(camX + cos(camRotY), cameraHeight - camAngleXZ, camZ + sin(camRotY)));
	camera->setFarValue(10000.0f);
	camera->setNearValue(1);
	
	matrix4 projMatrix;
	projMatrix.buildProjectionMatrixPerspectiveFovLH(1.250f, 1280.0f/720.0f, 1, 10000.0f);
	camera->setProjectionMatrix(projMatrix);
	
	ITerrainSceneNode* terrain = smgr->addTerrainSceneNode(
		"./res/materials/textures/terrain-heightmap-flat.bmp", 
		0, 
		-1, 
		vector3df(-128*4,0,-128*4),
		vector3df(0,0,0),
		vector3df(4,0.5f,4),
		SColor(255,255,255,255),
		5,
		scene::ETPS_17,
		4
  		);
	
	terrain->setMaterialFlag(video::EMF_LIGHTING, true);
	terrain->setMaterialTexture(0, driver->getTexture("./res/materials/textures/grass-texture.jpg"));
	terrain->setMaterialTexture(1, driver->getTexture("./res/materials/textures/grass-texture.jpg"));
	terrain->setMaterialType(video::EMT_DETAIL_MAP);
	terrain->scaleTexture(250, 250);
// 	terrain->overrideLODDistance(0, 10000);
// 	terrain->overrideLODDistance(1, 11000);
// 	terrain->overrideLODDistance(2, 12000);
// 	terrain->overrideLODDistance(3, 13000);
// 	terrain->overrideLODDistance(4, 14000);
	
// 	device->getCursorControl()->setVisible(false);
	
	// Add soldiers
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			int obj1 = ObjectManager::manager.createObject();
			ObjectManager::manager.attachComponent(obj1, new TransformComponent(vector3df(i*15,0,j*15)));
			ObjectManager::manager.attachComponent(obj1, new AnimatedMeshComponent("humantest.x", "ManTexture.png"));
			ObjectManager::manager.attachComponent(obj1, new RenderComponent(true));
			ObjectManager::manager.attachComponent(obj1, new SelectableComponent());
		}
// 		ObjectManager::manager.printGameObjects();
	}
	
	
	while (device->run()) {
		// Camera controls
		
		if (Keyboard::isKeyPressed(Keyboard::W)) {
			camX += cos(camRotY);
			camZ += sin(camRotY);
		}
		
		if (Keyboard::isKeyPressed(Keyboard::S)) {
			camX -= cos(camRotY);
			camZ -= sin(camRotY);
		}
		
		if (Keyboard::isKeyPressed(Keyboard::A)) {
			camX += cos(camRotY + (90 * (PI/180))); 
			camZ += sin(camRotY + (90 * (PI/180)));
		}
		
		if (Keyboard::isKeyPressed(Keyboard::D)) {
			camX += cos(camRotY - (90 * (PI/180))); 
			camZ += sin(camRotY - (90 * (PI/180)));
		}
		
		if (Keyboard::isKeyPressed(Keyboard::Q)) {
			camRotY += 0.05f;
		}
		
		if (Keyboard::isKeyPressed(Keyboard::E)) {
			camRotY -= 0.05f;
		}
		
		camera->setPosition(vector3df(camX, cameraHeight, camZ));
		camera->setTarget(vector3df(camX + cos(camRotY), cameraHeight - sin(camAngleXZ), camZ + sin(camRotY)));
		
// 		ObjectManager::manager.getObjectComponent<TransformComponent>(obj1, "TransformComponent")->worldPosition.Z -= 0.1f;
		ObjectManager::manager.updateSystems(0);
	}
	
	device->drop();
}

int main() {
	struct sigaction sigIntHandler;
	sigIntHandler.sa_handler = signalExit;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;
	
	sigaction(SIGINT, &sigIntHandler, NULL);
	
	/*enet_uint16 port;
	string input;
	
	cout << "Host? y/n: ";
	cin >> input;
	
	if (input[0] == 'y') {
		server.startServer();
	} else {
		cout << "Connect to ip: ";
		cin >> input;
		
		client.connectToServer(input);
	}*/
	basicGraphics();
	
	return 0;
}







