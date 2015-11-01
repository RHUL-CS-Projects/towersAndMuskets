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

int main() {
	/*struct sigaction sigIntHandler;
	sigIntHandler.sa_handler = signalExit;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;
	
	sigaction(SIGINT, &sigIntHandler, NULL);
	
	enet_uint16 port;
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
	
	IrrlichtDevice* device = createDevice(video::EDT_OPENGL, dimension2d<u32>(1280, 720), 16, false, false, false, 0);
	
	if (!device)
		return 1;
	
	device->setWindowCaption(L"Tower Defense");
	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();
	
	guienv->addStaticText(L"Hello world!", rect<s32>(10,10,260,22), true);
	guienv->addButton(rect<s32>(10,40,140,80),0, -1, L"Click me");
	
	IAnimatedMesh* mesh = smgr->getMesh("./res/models/humantest.obj");
	if (!mesh) {
		cout << "Mesh not found!" << endl;
	}
	
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			IMeshSceneNode* node = smgr->addMeshSceneNode(mesh);
			if (node) {
				node->setMaterialFlag(video::EMF_LIGHTING, false);
				//node->setMD2Animation(scene::EMAT_STAND);
				node->setMaterialTexture(0, driver->getTexture("./res/materials/textures/ManTexture.png"));
				node->setPosition(vector3df(i*5,0,j*10));
			}
		}
	}
	
	//smgr->addCameraSceneNode(0, vector3df(-8, 10, 0), vector3df(0, 5, 0));
	smgr->addCameraSceneNodeFPS(0, 50, 0.01f);
	smgr->getActiveCamera()->setPosition(vector3df(-8, 10, 0));
	smgr->getActiveCamera()->setTarget(vector3df(0, 5, 0));
	
	while (device->run()) {
		driver->beginScene(true, true, SColor(255,100,101,140));
		
		smgr->drawAll();
		guienv->drawAll();
		
		driver->endScene();
	}
	
	device->drop();
	
	return 0;
}
























