#ifndef GAME_H
#define GAME_H

#include <ObjectManager.h>
#include <WorldManager.h>
#include <RenderManager.h>
#include <irrlicht/irrlicht.h>
#include <list>
#include <GameState.h>
#include <GameResources.h>

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
	GameResources resources;
	
	void init();
	void run();
	void dispose();
	
	void pushState(GameState* state);
	void popState();
	GameState* currentState();
	
	ObjectManager* getObjMgr();
	RenderManager* getRendMgr();
};

#endif