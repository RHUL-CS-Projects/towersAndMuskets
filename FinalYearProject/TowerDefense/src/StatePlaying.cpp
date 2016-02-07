#include <StatePlaying.h>
#include <irrlicht/irrlicht.h>
#include <Game.h>
#include <sfml/SFML/Audio.hpp>
#include <sfml/SFML/Window.hpp>

using namespace irr;
using namespace core;
using namespace gui;
using namespace scene;
using namespace video;
using namespace sf;


SoundBuffer bufGunshot1;
Sound sndGunshot1;

RTSCamera camera;
InteractionMenu interactionMenu;
MapGenerator mapGenerator;
ObjectPlacer objectPlacer;

StatePlaying::StatePlaying() {
	ISceneManager* smgr = Game::game.getRendMgr()->getSceneManager();
	
	smgr->setShadowColor(video::SColor(80,0,0,0));
	smgr->setAmbientLight(SColorf(0.8f, 0.8f, 0.8f));
	vector3df lightdir = vector3df( 0.8f, -1, 0.8f ).normalize();
	ILightSceneNode* light = smgr->addLightSceneNode(0, -lightdir * 10000, SColorf(1,1,1), 100000);
	light->setLightType(video::ELT_DIRECTIONAL);
	light->getLightData().Direction = lightdir;
	
	camera.addToScene();
	interactionMenu.init(128, this);
	mapGenerator.generateMap();

	if (!bufGunshot1.loadFromFile("res/sounds/musketshot.ogg"))
		std::cout << "Sound not loaded" << std::endl;

	sndGunshot1.setBuffer(bufGunshot1);
	sndGunshot1.setRelativeToListener(false);
	sndGunshot1.setAttenuation(0.1f);
	sndGunshot1.setPosition(128, 0, 128);
	
	ObjectManager* objmgr = Game::game.getObjMgr();
	
	// Add soldiers
	/*for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			int obj1 = objmgr->createObject();
			objmgr->attachComponent(obj1, new TransformComponent(vector3df(16 + i*15+3,0,128 + j*15+3)));
			
			objmgr->attachComponent(obj1, new AnimatedMeshComponent("humantest.x", "ManTexture.png", vector3df(0,0,0)));
			
			AnimatorComponent* animComp = new AnimatorComponent();
			animComp->addAnimation("IDLE", 0, 62, 30);
			animComp->addAnimation("WALK", 63, 142, 90);
			animComp->addAnimation("TAKEAIM", 143, 153, 20);
			animComp->addAnimation("AIM", 154, 156, 30);
			animComp->addAnimation("SHOOT", 157, 165, 30);
			animComp->addAnimation("RELOAD", 166, 275, 20);
			animComp->addAnimation("REST", 276, 287, 30);
			animComp->addAnimation("DEATH1", 288, 313, 20);
			
			objmgr->attachComponent(obj1, animComp);
			
			objmgr->attachComponent(obj1, new SelectableComponent());
			objmgr->attachComponent(obj1, new RenderComponent(true));
			objmgr->attachComponent(obj1, new FaceDirectionComponent(0, 0.08f));
			objmgr->attachComponent(obj1, new RTSLogicComponent(0, 1, sndGunshot1, 60));
			objmgr->attachComponent(obj1, new SteeringComponent(0.2, 80));
			objmgr->attachComponent(obj1, new HealthComponent(10, 10));
		}
	}
	
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			int obj1 = objmgr->createObject();
			objmgr->attachComponent(obj1, new TransformComponent(vector3df(256 + i*15+3,0,256 + j*15+3)));
			
			objmgr->attachComponent(obj1, new AnimatedMeshComponent("humantest.x", "ManTexture2.png", vector3df(0,0,0)));
			
			AnimatorComponent* animComp = new AnimatorComponent();
			animComp->addAnimation("IDLE", 0, 62, 30);
			animComp->addAnimation("WALK", 63, 142, 90);
			animComp->addAnimation("TAKEAIM", 143, 153, 20);
			animComp->addAnimation("AIM", 154, 156, 30);
			animComp->addAnimation("SHOOT", 157, 165, 30);
			animComp->addAnimation("RELOAD", 166, 275, 20);
			animComp->addAnimation("REST", 276, 287, 30);
			animComp->addAnimation("DEATH1", 288, 313, 20);
			
			objmgr->attachComponent(obj1, animComp);
			
			//objmgr->attachComponent(obj1, new SelectableComponent());
			objmgr->attachComponent(obj1, new RenderComponent(true));
			objmgr->attachComponent(obj1, new FaceDirectionComponent(0, 0.08f));
			objmgr->attachComponent(obj1, new RTSLogicComponent(1, 1, sndGunshot1, 60));
			objmgr->attachComponent(obj1, new SteeringComponent(0.2, 80));
			objmgr->attachComponent(obj1, new HealthComponent(10, 10));
		}
	}*/
}

void StatePlaying::update() {
    GameState::update();
	
	camera.update();
	interactionMenu.update();
	objectPlacer.update();
	Game::game.getObjMgr()->updateSystems(0);
}

void StatePlaying::render ( irr::video::IVideoDriver* driver ) {
    GameState::render ( driver );

	Game::game.getObjMgr()->drawSystems(0);
	interactionMenu.render(driver);
}

void StatePlaying::transitionIn() {
    GameState::transitionIn();
}

void StatePlaying::transitionOut() {
    GameState::transitionOut();
}

void StatePlaying::message ( int messageNum ) {
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
	}
}
