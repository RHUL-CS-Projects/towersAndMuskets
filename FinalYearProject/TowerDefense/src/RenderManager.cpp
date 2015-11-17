#include <iostream>

#include <RenderManager.h>
#include <EventReceiver.h>

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

RenderManager RenderManager::renderManager;
std::string const RenderManager::resPath = "./res";

RenderManager::RenderManager() {

}

RenderManager::~RenderManager() {
	device->drop();
}

/**
 * Initialise the Render Manager and get pointers to the various systmes
 * used by Irrlicht
 */
void RenderManager::init ( const wchar_t* caption ) {
	EventReceiver* eventReceiver = new EventReceiver();
	device = createDevice(video::EDT_OPENGL, dimension2d<u32>(1280, 720), 32, false, false, true, eventReceiver);
	
	if (!device) {
		cerr << "An error occurred when creating the display device :(" << endl; 
		return;
	}
	
	device->setWindowCaption(caption);
	
	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();
}

/**
 * Returns a pointer to the manager's IrrlichtDevice
 */
IrrlichtDevice* RenderManager::getDevice() {
	return device;
}

/**
 * Returns a pointer to the manager's IVideoDriver
 */
IVideoDriver* RenderManager::getDriver() {
	return driver;
}

/**
 * Returns a pointer to the manager's IGUIEnvironment
 */
IGUIEnvironment* RenderManager::getGUIEnvironment() {
	return guienv;
}

/**
 * Returns a pointer to the manager's ISceneManager
 */
ISceneManager* RenderManager::getSceneManager() {
	return smgr;
}

