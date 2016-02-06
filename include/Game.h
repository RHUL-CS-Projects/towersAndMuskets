#ifndef GAME_H
#define GAME_H

#include <ObjectManager.h>
#include <WorldManager.h>
#include <RenderManager.h>
#include <irrlicht/irrlicht.h>
#include <list>
#include <GameState.h>

class Game {
private:
	ObjectManager objManager;
	RenderManager rendManager;
	
	irr::IrrlichtDevice* device;
	irr::video::IVideoDriver* driver;
	irr::scene::ISceneManager* smgr;
	irr::gui::IGUIEnvironment* guienv;
	
	std::list<GameState*> stateStack;
	
	void updateStates();
	void renderStates();
public:
	Game() {}
	
	static Game game;
	
	void init();
	void run();
	void dispose();
	
	void pushState(GameState* state);
	void popState();
	
	ObjectManager* getObjMgr();
	RenderManager* getRendMgr();
};

#endif