#include <Game.h>
#include <irrlicht/irrlicht.h>
#include <EventReceiver.h>
#include <chrono>
#include <sfml/SFML/Window.hpp>
#include <StateMainMenu.h>

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
using namespace chrono;
using namespace sf;

Game Game::game;

void Game::init() {
	rendManager.init(L"Tower Defense");
	
	device = rendManager.getDevice();
	driver = rendManager.getDriver();
	smgr = rendManager.getSceneManager();
	guienv = rendManager.getGUIEnvironment();
	
	SAppContext context;
    context.device = device;
	
	EventReceiver* eventReceiver = new EventReceiver(context);
	device->setEventReceiver(eventReceiver);
	
	pushState(new StateMainMenu());
}

void Game::run() {

	Clock gameClock;
	double ticksPerSecond = 60;
	double tickTime = 1000.0 / ticksPerSecond * 1000.0;
	double maxFrameSkip = 10;
	double nextTick = gameClock.getElapsedTime().asMicroseconds();
	double currentTime;
	int loops = 0;
	
	int tickCounter = 0;
	int frameCounter = 0;
	double updateTime = gameClock.getElapsedTime().asMicroseconds();
	
	while (device->run()) {
		currentTime = gameClock.getElapsedTime().asMicroseconds();
		
		loops = 0;
		while (currentTime >= nextTick && loops < maxFrameSkip) {
			
			// Update game
			updateStates();
			
			nextTick += tickTime;
			loops++;
			tickCounter++;
		}
		
		//objManager.drawSystems(0);
		
		// Render game
		renderStates();
		frameCounter++;

		if (currentTime - updateTime >= 1000000.0) {
			cout << "Ticks: " << tickCounter << ", Frames: " << frameCounter << endl;
			
			std::wstring labelText;
			labelText += L"TICKS: ";
			labelText += to_wstring(tickCounter);
			labelText += L", FRAMES: ";
			labelText += to_wstring(frameCounter);;
			
			/*lblTickFrame->setText(labelText.c_str());*/
			
			frameCounter = 0;
			tickCounter = 0;
			updateTime += 1000000.0;//currentTime - ((currentTime - updateTime) - 1000);
		}
	}
}

void Game::dispose() {
	resources.freeSounds();
	device->drop();
}

void Game::updateStates() {
	for (GameState* state : stateStack) {
		state->update();
		
		if (!state->transparentUpdate)
			break;
	}
}

void Game::renderStates() {
	driver->beginScene(true, true, irr::video::SColor(255,0,0,0));
	
	int bottom = 0;
	for (int i = 0; i < stateStack.size(); i++) {
		if (stateStack.at(i)->transparentDraw)
			bottom++;
		else
			break;
	}
	
	for (int i = bottom; i >= 0; i--) {
		stateStack.at(i)->render(driver);
	}
	
	driver->endScene();
}

void Game::pushState ( GameState* state ) {
	stateStack.insert(stateStack.begin(), state);
}

void Game::popState() {
	GameState* current = currentState();
	stateStack.erase(stateStack.begin());
	delete current;
}

void Game::popStates ( int num ) {
	for (int i = 0; i < num; i++) {
		popState();
	}
}

ObjectManager* Game::getObjMgr() {
	return &objManager;
}

RenderManager* Game::getRendMgr() {
	return &rendManager;
}

GameState* Game::currentState() {
	return stateStack.front();
}
