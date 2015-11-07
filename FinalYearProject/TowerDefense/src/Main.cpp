#include <iostream>
#include <Server.h>
#include <Client.h>
#include <string>
#include <irrlicht/irrlicht.h>

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

Server server;
Client client;

void signalExit(int s) {
	server.stopServer();
	client.disconnect();
}

void basicGraphics() {
	IrrlichtDevice* device = createDevice(video::EDT_OPENGL, dimension2d<u32>(1280, 720), 32, false, true);
	
	if (!device)
		return;
	
	device->setWindowCaption(L"Tower Defense");
	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();
	
	guienv->addStaticText(L"Hello world!", rect<s32>(10,10,260,22), true);
	guienv->addButton(rect<s32>(10,40,140,80),0, -1, L"Click me");
	
	IAnimatedMesh* mesh = smgr->getMesh("./res/models/humantest.x");
	if (!mesh) {
		cout << "Mesh not found!" << endl;
	}
	
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			
		}
	}
	
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			IAnimatedMeshSceneNode* animnode = smgr->addAnimatedMeshSceneNode(mesh);
			if (animnode) {
				animnode->setMaterialFlag(video::EMF_LIGHTING, true);
				animnode->setMaterialFlag(video::EMF_GOURAUD_SHADING, true);

				animnode->setMaterialTexture(0, driver->getTexture("./res/materials/textures/ManTexture.png"));
				animnode->setPosition(vector3df(i*10,0,j*15));
				animnode->setRotation(vector3df(0, rand()%360,0));
				
				animnode->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
				
				animnode->addShadowVolumeSceneNode();
				
				if (rand()%2 == 1) {
					animnode->setFrameLoop(1,61);
					animnode->setCurrentFrame(1+rand()%(62-1));
				} else {
					animnode->setFrameLoop(62,142);
					animnode->setCurrentFrame(62+rand()%(142-62));
				}
				//animnode->setFrameLoop(63,142);
				animnode->setAnimationSpeed(60);
			}
		}
	}
	
	smgr->setShadowColor(video::SColor(80,0,0,0));
	smgr->setAmbientLight(SColorf(0.8f, 0.8f, 0.8f));
	vector3df lightdir = vector3df( 0.8f, -1, 0.8f ).normalize();
	ILightSceneNode* light = smgr->addLightSceneNode(0, -lightdir * 10000, SColorf(1,1,1), 100000);
	light->setLightType(video::ELT_DIRECTIONAL);
	light->getLightData().Direction = lightdir;
	
	//smgr->addCameraSceneNode(0, vector3df(-8, 10, 0), vector3df(0, 5, 0));
	smgr->addCameraSceneNodeFPS(0, 50, 0.01f);
	smgr->getActiveCamera()->setPosition(vector3df(-8, 10, 0));
	smgr->getActiveCamera()->setTarget(vector3df(20, 5,20));
	smgr->getActiveCamera()->setFarValue(10000.0f);
	
	ITerrainSceneNode* terrain = smgr->addTerrainSceneNode(
		"./res/materials/textures/terrain-heightmap-flat.bmp", 
		0, 
		-1, 
		vector3df(-200,0,-200),
		vector3df(0,0,0),
		vector3df(20,4,20),
		SColor(255,255,255,255),
		5,
		scene::ETPS_17,
		4
  		);
	
	terrain->setMaterialFlag(video::EMF_LIGHTING, true);
	terrain->setMaterialTexture(0, driver->getTexture("./res/materials/textures/grass-texture.jpg"));
	terrain->setMaterialTexture(1, driver->getTexture("./res/materials/textures/grass-texture.jpg"));
	terrain->setMaterialType(video::EMT_DETAIL_MAP);
	terrain->scaleTexture(500, 500);
	
	while (device->run()) {
		driver->beginScene(true, true, SColor(0,0,0,0));
		
		smgr->drawAll();
		//guienv->drawAll();
		
		driver->endScene();
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

























