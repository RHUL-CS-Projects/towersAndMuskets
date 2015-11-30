#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include <irrlicht/irrlicht.h>
#include <string>

/**
 * Initialises Irrlicht for rendering and holds references to the
 * various subsystems that may be required by other parts of the game
 */
class RenderManager {
private:
	// The different systems used by Irrlicht
	irr::IrrlichtDevice* device;
	irr::video::IVideoDriver* driver;
	irr::scene::ISceneManager* smgr;
	irr::gui::IGUIEnvironment* guienv;
	
public:
	// Static reference to the renderManager
	static RenderManager renderManager;
	
	// The top level path to the resource folder
	static std::string const resPath;
	
	RenderManager();
	~RenderManager();
	
	/**
	 * Initialises Irrlicht and sets the window caption
	 */
	void init(const wchar_t* caption);
	
	/**
	 * Retrieves the Irrlicht Device
	 */
	irr::IrrlichtDevice* getDevice();
	
	/**
	 * Retrieves the Irrlicht Driver
	 */
	irr::video::IVideoDriver* getDriver();
	
	/**
	 * Retrieves the Irrlicht Scene Manager
	 */
	irr::scene::ISceneManager* getSceneManager();
	
	/**
	 * Retrieves the Irrlicht GUI Environment
	 */
	irr::gui::IGUIEnvironment* getGUIEnvironment();
};

#endif