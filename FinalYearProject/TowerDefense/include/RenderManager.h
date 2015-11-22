#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include <irrlicht/irrlicht.h>
#include <string>

class RenderManager {
private:
	irr::IrrlichtDevice* device;
	irr::video::IVideoDriver* driver;
	irr::scene::ISceneManager* smgr;
	irr::gui::IGUIEnvironment* guienv;
	
public:
	static RenderManager renderManager;
	static std::string const resPath;
	
	RenderManager();
	~RenderManager();
	void init(const wchar_t* caption);
	
	irr::IrrlichtDevice* getDevice();
	irr::video::IVideoDriver* getDriver();
	irr::scene::ISceneManager* getSceneManager();
	irr::gui::IGUIEnvironment* getGUIEnvironment();
};

#endif