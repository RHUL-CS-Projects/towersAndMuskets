#include <RTSCamera.h>
#include <sfml/SFML/Window.hpp>
#include <EventReceiver.h>
#include <sfml/SFML/Audio.hpp>

using namespace sf;
using namespace irr;
using namespace core;
using namespace scene;

RTSCamera::RTSCamera() {
	cameraHeight = 100;
	targetCamHeight = 100;
	camX = 100;
	camZ = 100;
	targetCamX = 100;
	targetCamZ = 100;
	camRotY = 45;
	targetCamRotY = 45;
	camAngleXZ = 1.2f;
}

void RTSCamera::addToScene() {
	ISceneManager* smgr = Game::game.getRendMgr()->getSceneManager();
	
	smgr->addCameraSceneNode(0, vector3df(0, 0, 0), vector3df(0, 0, 0));
	camera = smgr->getActiveCamera();
	camera->setPosition(vector3df(camX, cameraHeight, camZ));
	camera->setTarget(vector3df(camX + cos(camRotY), cameraHeight - camAngleXZ, camZ + sin(camRotY)));
	camera->setFarValue(1000.0f);
	camera->setNearValue(1);
	
	matrix4 projMatrix;
	projMatrix.buildProjectionMatrixPerspectiveFovLH(1.250f, 1280.0f/720.0f, 1, 10000.0f);
	camera->setProjectionMatrix(projMatrix);
}

void RTSCamera::update() {
	if (Game::game.getRendMgr()->getDevice()->isWindowActive()) {
		// Camera controls
		if (Keyboard::isKeyPressed(Keyboard::W)) {
			targetCamX += cos(camRotY) * 2;
			targetCamZ += sin(camRotY) * 2;
		}
		
		if (Keyboard::isKeyPressed(Keyboard::S)) {
			targetCamX -= cos(camRotY) * 2;
			targetCamZ -= sin(camRotY) * 2;
		}
		
		if (Keyboard::isKeyPressed(Keyboard::A)) {
			targetCamX += cos(camRotY + (90 * (PI/180))) * 2; 
			targetCamZ += sin(camRotY + (90 * (PI/180))) * 2;
		}
		
		if (Keyboard::isKeyPressed(Keyboard::D)) {
			targetCamX += cos(camRotY - (90 * (PI/180))) * 2; 
			targetCamZ += sin(camRotY - (90 * (PI/180))) * 2;
		}
		
		if (Keyboard::isKeyPressed(Keyboard::Q)) {
			targetCamRotY += 0.05f;
		}
		
		if (Keyboard::isKeyPressed(Keyboard::E)) {
			targetCamRotY -= 0.05f;
		}
	}
	
	if (EventReceiver::getMouseState()->wheelDelta < 0) {
		EventReceiver::getMouseState()->wheelDelta = 0;
		targetCamHeight += 10;
		targetCamHeight = (targetCamHeight > 200) ? 200 : targetCamHeight;
	}
	
	if (EventReceiver::getMouseState()->wheelDelta > 0) {
		EventReceiver::getMouseState()->wheelDelta = 0;
		targetCamHeight -= 10;
		targetCamHeight = (targetCamHeight < 10) ? 10 : targetCamHeight;
	}
	
	cameraHeight = cameraHeight + (targetCamHeight - cameraHeight) * 0.2f;
	camRotY = camRotY + (targetCamRotY - camRotY) * 0.2f;
	camX = camX + (targetCamX - camX) * 0.2f;
	camZ = camZ + (targetCamZ - camZ) * 0.2f;
	
	camera->setPosition(vector3df(camX, cameraHeight, camZ));
	camera->setTarget(vector3df(camX + cos(camRotY), cameraHeight - camAngleXZ, camZ + sin(camRotY)));
	
	Listener::setPosition(camX, cameraHeight, camZ);
	vector3df lookvec = (camera->getPosition() - camera->getTarget()).normalize();
	Listener::setDirection(lookvec.X, lookvec.Y, lookvec.Z);
}



