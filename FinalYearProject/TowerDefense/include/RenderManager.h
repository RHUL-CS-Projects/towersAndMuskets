#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include <irrlicht/irrlicht.h>
#include <string>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class RenderManager {
private:
	IrrlichtDevice* device;
	IVideoDriver* driver;
	ISceneManager* smgr;
	IGUIEnvironment* guienv;
	
public:
	static RenderManager renderManager;
	static std::string const resPath;
	
	RenderManager();
	~RenderManager();
	void init(const wchar_t* caption);
	
	IrrlichtDevice* getDevice();
	IVideoDriver* getDriver();
	ISceneManager* getSceneManager();
	IGUIEnvironment* getGUIEnvironment();
};

#endif