#include <StatePlaying.h>
#include <irrlicht/irrlicht.h>
#include <Game.h>
#include <sfml/SFML/Audio.hpp>
#include <sfml/SFML/Window.hpp>
#include <TransparentMaterialShader.h>
#include <StateWinLose.h>

using namespace irr;
using namespace core;
using namespace gui;
using namespace scene;
using namespace video;
using namespace sf;


StatePlaying::StatePlaying(std::string mapname) : GameState("StatePlaying") {
	transparentDraw = false;
	transparentUpdate = false;
	ISceneManager* smgr = Game::game.getRendMgr()->getSceneManager();

	IGPUProgrammingServices* gpu = Game::game.getRendMgr()->getDriver()->getGPUProgrammingServices();
	
	if (gpu) {
		TransparentMaterialShader* shader = new TransparentMaterialShader();
		
		s32 newMat = gpu->addHighLevelShaderMaterialFromFiles(
			"./res/materials/shaders/transparentshader.vert", "vertexMain", video::EVST_VS_1_1,
			"./res/materials/shaders/transparentshader.frag", "pixelMain", video::EPST_PS_1_1,
			shader, video::EMT_TRANSPARENT_ADD_COLOR, 0, video::EGSL_CG
		);
		
		s32 newMat2 = gpu->addHighLevelShaderMaterialFromFiles(
			"./res/materials/shaders/transparentshader.vert", "vertexMain", video::EVST_VS_1_1,
			"./res/materials/shaders/transparentshader.frag", "pixelMain", video::EPST_PS_1_1,
			shader, video::EMT_SOLID, 0, video::EGSL_CG
		);
		
		shader->drop();
		
		TransparentMaterialShader::materialID = newMat;
		TransparentMaterialShader::materialIDNonAdd = newMat2;
	}
	
	interactionMenu.init(128, this);
	messageDisplay.init(RenderManager::resPath + "/materials/textures/SerifFont.xml");
	TeamComponent::reset();
	
	loadMap(mapname);
	
	if (!bufGunshot1.loadFromFile("res/sounds/musketshot.ogg"))
		std::cout << "Sound not loaded" << std::endl;

	sndGunshot1.setBuffer(bufGunshot1);
	sndGunshot1.setRelativeToListener(false);
	sndGunshot1.setAttenuation(0.1f);
	sndGunshot1.setPosition(128, 0, 128);
	
	ObjectManager* objmgr = Game::game.getObjMgr();
}

void StatePlaying::loadMap ( std::string mapname ) {
	particleManager.reset();
	Game::game.getRendMgr()->getSceneManager()->clear();
	Game::game.getObjMgr()->clearObjects();
	objectPlacer.init();
	resourceCache.init(150,150,150);
	
	ISceneManager* smgr = Game::game.getRendMgr()->getSceneManager();
	
	Game::game.getObjMgr()->clearObjects();
	smgr->clear();
	
	mapGenerator.generateMap(mapname);
	
	smgr->setShadowColor(video::SColor(80,0,0,0));
	smgr->setAmbientLight(SColorf(0.8f, 0.8f, 0.8f));
	vector3df lightdir = vector3df( 0.8f, -1, 0.8f ).normalize();
	ILightSceneNode* light = smgr->addLightSceneNode(0, -lightdir * 10000, SColorf(1,1,1), 100000);
	light->setLightType(video::ELT_DIRECTIONAL);
	light->getLightData().Direction = lightdir;
	
	camera.addToScene();
	
	this->currentMap = mapname;
	waveController.init();
}

void StatePlaying::reloadMap ( std::string mapname ) {
	shouldReload = true;
	this->currentMap = mapname;
}

void StatePlaying::update() {
	if (!shouldReload) {
		GameState::update();
		
		camera.update();
		waveController.update();
		interactionMenu.update();
		messageDisplay.update();
		objectPlacer.update();
		resourceCache.update();
		particleManager.update();
		
		interactionMenu.setWaveDetails(waveController.getWaveDetails());
		Game::game.getObjMgr()->updateSystems(0);
		
		TeamComponent::updateTeamLists();
		
		if (waveController.getWaveDetails().gameOver) {
			Game::game.pushState(new StateWinLose(waveController.getWaveDetails().victory));
		}
	} else {
		shouldReload = false;
		loadMap(currentMap);
	}
}

void StatePlaying::render ( irr::video::IVideoDriver* driver ) {
    GameState::render ( driver );

	Game::game.getObjMgr()->drawSystems(0);
	interactionMenu.render(driver);
	messageDisplay.render(driver);
}

void StatePlaying::transitionIn() {
    GameState::transitionIn();
}

void StatePlaying::transitionOut() {
    GameState::transitionOut();
}

void StatePlaying::message ( int messageNum, std::string message ) {
	switch (messageNum) {
	case SET_PLACE_OBJECT_TOWER:
		objectPlacer.setObjectType(Tower);
		break;
	case SET_PLACE_OBJECT_TREE:
		objectPlacer.setObjectType(Tree);
		break;
	case SET_PLACE_OBJECT_ROCK:
		objectPlacer.setObjectType(Rock);
		break;
	case SET_PLACE_OBJECT_ENEMY_UNIT:
		objectPlacer.setObjectType(EnemyUnit);
		break;
	case SET_PLACE_OBJECT_PLAYER_UNIT:
		objectPlacer.setObjectType(PlayerUnit);
		break;
	case SET_PLACE_OBJECT_PLAYER_CANNON:
		objectPlacer.setObjectType(PlayerCannon);
		break;
	case SET_PLACE_OBJECT_PLAYER_VILLAGER:
		objectPlacer.setObjectType(PlayerVillager);
		break;
	case SHOW_MESSAGE_GOOD:
		messageDisplay.showMessage(message, SColor(255,0,255,0));
		Game::game.resources.loadSound("goodsound.ogg")->play();
		break;
	case SHOW_MESSAGE_BAD:
		messageDisplay.showMessage(message, SColor(255,255,0,0));
		Game::game.resources.loadSound("badsound.ogg")->play();
		break;
	case SKIP_WAVE_WAIT:
		waveController.skipWait();
		messageDisplay.showMessage("Wave started!", SColor(255,0,255,0));
		Game::game.resources.loadSound("goodsound.ogg")->play();
		break;
	}
}

PlayerResourceCache* StatePlaying::getResourceCache() {
	return &resourceCache;
}

