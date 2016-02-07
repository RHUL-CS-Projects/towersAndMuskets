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
	camRotY = 45;
	camAngleXZ = 1;
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
	
	Listener::setPosition(camX, cameraHeight, camZ);
	vector3df lookvec = (camera->getPosition() - camera->getTarget()).normalize();
	Listener::setDirection(lookvec.X, lookvec.Y, lookvec.Z);
}
