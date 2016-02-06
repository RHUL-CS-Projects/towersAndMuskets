#include <StatePlaying.h>
#include <irrlicht/irrlicht.h>
#include <Game.h>
#include <TowerDefenseEngine.h>
#include <sfml/SFML/Audio.hpp>
#include <sfml/SFML/Window.hpp>

using namespace irr;
using namespace core;
using namespace gui;
using namespace scene;
using namespace video;
using namespace sf;

float cameraHeight = 100;
float targetCamHeight = 100;
float camX = 100;
float camZ = 100;
float camRotY = 45;
float camAngleXZ = 1;

ICameraSceneNode* camera;

SoundBuffer bufGunshot1;
Sound sndGunshot1;

StatePlaying::StatePlaying() {
	ISceneManager* smgr = Game::game.getRendMgr()->getSceneManager();
	
	smgr->setShadowColor(video::SColor(80,0,0,0));
	smgr->setAmbientLight(SColorf(0.8f, 0.8f, 0.8f));
	vector3df lightdir = vector3df( 0.8f, -1, 0.8f ).normalize();
	ILightSceneNode* light = smgr->addLightSceneNode(0, -lightdir * 10000, SColorf(1,1,1), 100000);
	light->setLightType(video::ELT_DIRECTIONAL);
	light->getLightData().Direction = lightdir;
	
	
	smgr->addCameraSceneNode(0, vector3df(0, 0, 0), vector3df(0, 0, 0));
	//smgr->addCameraSceneNodeFPS(0, 50, 0.1f);
	camera = smgr->getActiveCamera();
	camera->setPosition(vector3df(camX, cameraHeight, camZ));
	camera->setTarget(vector3df(camX + cos(camRotY), cameraHeight - camAngleXZ, camZ + sin(camRotY)));
	camera->setFarValue(1000.0f);
	camera->setNearValue(1);
	
	matrix4 projMatrix;
	projMatrix.buildProjectionMatrixPerspectiveFovLH(1.250f, 1280.0f/720.0f, 1, 10000.0f);
	camera->setProjectionMatrix(projMatrix);
	
	ITerrainSceneNode* terrain = smgr->addTerrainSceneNode(
		"./res/materials/textures/terrain-heightmap-flat.bmp", 
		0, 
		-1, 
		vector3df(0,0,0),
		vector3df(0,0,0),
		vector3df(2,0.2f,2),
		SColor(255,255,255,255),
		5,
		scene::ETPS_17,
		4
	);
	
	//cout << terrain->getBoundingBox().getCenter().X << ", " << terrain->getBoundingBox().getCenter().Z << endl;
	
	terrain->setMaterialFlag(video::EMF_LIGHTING, true);
	terrain->setMaterialTexture(0, Game::game.getRendMgr()->getDriver()->getTexture("./res/materials/textures/grass-texture2.jpg"));
	//terrain->setMaterialTexture(1, driver->getTexture("./res/materials/textures/grass-texture4.jpg"));
	//terrain->setMaterialType(video::EMT_DETAIL_MAP);
	terrain->scaleTexture(32, 32);
	
	ITriangleSelector* terrainSelector = smgr->createTerrainTriangleSelector(terrain);
	terrain->setTriangleSelector(terrainSelector);
	terrain->setName("MainTerrain");
	

	if (!bufGunshot1.loadFromFile("res/sounds/musketshot.ogg"))
		std::cout << "Sound not loaded" << std::endl;

	sndGunshot1.setBuffer(bufGunshot1);
	sndGunshot1.setRelativeToListener(false);
	sndGunshot1.setAttenuation(0.1f);
	sndGunshot1.setPosition(128, 0, 128);
	
	ObjectManager* objmgr = Game::game.getObjMgr();
	if (objmgr == nullptr)
		std::cout << "fsdljdfs" << std::endl;
	
	// Add soldiers
	for (int i = 0; i < 5; i++) {
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
			
			objmgr->attachComponent(obj1, new RenderComponent(true));
			objmgr->attachComponent(obj1, new FaceDirectionComponent(0, 0.08f));
			objmgr->attachComponent(obj1, new RTSLogicComponent(0, 1, sndGunshot1, 60));
			objmgr->attachComponent(obj1, new SteeringComponent(0.2, 80));
			objmgr->attachComponent(obj1, new HealthComponent(10, 10));
		}
	}
	
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			int obj1 = Game::game.getObjMgr()->createObject();
			objmgr->attachComponent(obj1, new TransformComponent(vector3df(256 + i*15+3,0,128 + j*15+3)));
			
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
			
			objmgr->attachComponent(obj1, new RenderComponent(true));
			objmgr->attachComponent(obj1, new SelectableComponent());
			objmgr->attachComponent(obj1, new FaceDirectionComponent(0, 0.08f));
			objmgr->attachComponent(obj1, new RTSLogicComponent(1, 1, sndGunshot1, 60));
			objmgr->attachComponent(obj1, new SteeringComponent(0.2, 80));
			objmgr->attachComponent(obj1, new HealthComponent(10, 10));
		}
	}
}

void StatePlaying::update() {
    GameState::update();
	
	if (Game::game.getRendMgr()->getDevice()->isWindowActive()) {
		// Camera controls
		if (Keyboard::isKeyPressed(Keyboard::W)) {
			camX += cos(camRotY);
			camZ += sin(camRotY);
		}
		
		if (Keyboard::isKeyPressed(Keyboard::S)) {
			camX -= cos(camRotY);
			camZ -= sin(camRotY);
		}
		
		if (Keyboard::isKeyPressed(Keyboard::A)) {
			camX += cos(camRotY + (90 * (PI/180))); 
			camZ += sin(camRotY + (90 * (PI/180)));
		}
		
		if (Keyboard::isKeyPressed(Keyboard::D)) {
			camX += cos(camRotY - (90 * (PI/180))); 
			camZ += sin(camRotY - (90 * (PI/180)));
		}
		
		if (Keyboard::isKeyPressed(Keyboard::Q)) {
			camRotY += 0.05f;
		}
		
		if (Keyboard::isKeyPressed(Keyboard::E)) {
			camRotY -= 0.05f;
		}
	}
	
	if (EventReceiver::getMouseState()->wheelDelta < 0) {
		EventReceiver::getMouseState()->wheelDelta = 0;
		targetCamHeight += 10;
		targetCamHeight = (targetCamHeight > 120) ? 120 : targetCamHeight;
	}
	
	if (EventReceiver::getMouseState()->wheelDelta > 0) {
		EventReceiver::getMouseState()->wheelDelta = 0;
		targetCamHeight -= 10;
		targetCamHeight = (targetCamHeight < 10) ? 10 : targetCamHeight;
	}
	
	cameraHeight = cameraHeight + (targetCamHeight - cameraHeight) * 0.2f;
	
	camera->setPosition(vector3df(camX, cameraHeight, camZ));
	camera->setTarget(vector3df(camX + cos(camRotY), cameraHeight - sin(camAngleXZ), camZ + sin(camRotY)));
	
	Game::game.getObjMgr()->updateSystems(0);
	
	Listener::setPosition(camX, cameraHeight, camZ);
	vector3df lookvec = (camera->getPosition() - camera->getTarget()).normalize();
	Listener::setDirection(lookvec.X, lookvec.Y, lookvec.Z);
}

void StatePlaying::render ( irr::video::IVideoDriver* driver ) {
    GameState::render ( driver );

	Game::game.getObjMgr()->drawSystems(0);
}

void StatePlaying::transitionIn() {
    GameState::transitionIn();
}

void StatePlaying::transitionOut() {
    GameState::transitionOut();
}
