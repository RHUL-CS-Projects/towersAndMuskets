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
	targetGroundCamHeight = 8;
	camX = 100;
	camZ = 100;
	targetCamX = 100;
	targetCamZ = 100;
	camRotY = 45;
	targetCamRotY = 45;
	camAngleXZ = 1.2f;
	groundCam = false;
	tabPressed = false;
	lookTarget = 0;
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
	double speedMult = 1;
	
	if (Game::game.getRendMgr()->getDevice()->isWindowActive()) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)) {
			if (!tabPressed) {
				tabPressed = true;
				groundCam = !groundCam;
				Game::game.getRendMgr()->getSceneManager()->getSceneNodeFromName("SkyDome")->setVisible(groundCam);
			}
		} else {
			tabPressed = false;
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) speedMult = 0.25;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) speedMult = 0.05;
		
		if (groundCam) speedMult *= 0.5;
		
		// Camera controls
		if (Keyboard::isKeyPressed(Keyboard::W)) {
			targetCamX += cos(camRotY) * 2 * speedMult;
			targetCamZ += sin(camRotY) * 2 * speedMult;
		}
		
		if (Keyboard::isKeyPressed(Keyboard::S)) {
			targetCamX -= cos(camRotY) * 2 * speedMult;
			targetCamZ -= sin(camRotY) * 2 * speedMult;
		}
		
		if (Keyboard::isKeyPressed(Keyboard::A)) {
			targetCamX += cos(camRotY + (90 * (PI/180))) * 2 * speedMult; 
			targetCamZ += sin(camRotY + (90 * (PI/180))) * 2 * speedMult;
		}
		
		if (Keyboard::isKeyPressed(Keyboard::D)) {
			targetCamX += cos(camRotY - (90 * (PI/180))) * 2 * speedMult; 
			targetCamZ += sin(camRotY - (90 * (PI/180))) * 2 * speedMult;
		}
		
		if (Keyboard::isKeyPressed(Keyboard::Q)) {
			targetCamRotY += 0.05f  * speedMult;
		}
		
		if (Keyboard::isKeyPressed(Keyboard::E)) {
			targetCamRotY -= 0.05f  * speedMult;
		}
	}
	
	if (!groundCam) {
		if (EventReceiver::getMouseState()->wheelDelta < 0) {
			EventReceiver::getMouseState()->wheelDelta = 0;
			targetCamHeight += 10 * speedMult;
			targetCamHeight = (targetCamHeight > 200) ? 200 : targetCamHeight;
		}
		
		if (EventReceiver::getMouseState()->wheelDelta > 0) {
			EventReceiver::getMouseState()->wheelDelta = 0;
			targetCamHeight -= 10 * speedMult;
			targetCamHeight = (targetCamHeight < 60) ? 60 : targetCamHeight;
		}
	}
	
	double targetHeight = groundCam ? targetGroundCamHeight : targetCamHeight;
	cameraHeight = cameraHeight + (targetHeight - cameraHeight) * 0.2f;
	camRotY = camRotY + (targetCamRotY - camRotY) * 0.2f;
	camX = camX + (targetCamX - camX) * 0.2f;
	camZ = camZ + (targetCamZ - camZ) * 0.2f;
	
	ITerrainSceneNode* terrain = ((ITerrainSceneNode*)Game::game.getRendMgr()->getSceneManager()->getSceneNodeFromName("MainTerrain"));
	aabbox3df bounds = terrain->getBoundingBox();
	double onTerX = camX;
	double onTerY = camZ;
	onTerX = (onTerX > bounds.MaxEdge.X-1) ? bounds.MaxEdge.X-1 : onTerX;
	onTerY = (onTerY > bounds.MaxEdge.Z-1) ? bounds.MaxEdge.Z-1 : onTerY;
	onTerX = (onTerX < bounds.MinEdge.X) ? bounds.MinEdge.X : onTerX;
	onTerY = (onTerY < bounds.MinEdge.Y) ? bounds.MinEdge.Z : onTerY;
	double terY = terrain->getHeight(onTerX, onTerY);
	
	camera->setPosition(vector3df(camX, terY + cameraHeight, camZ));
	
	if (groundCam)
		lookTarget = terY+cameraHeight-0.4;
	else
		lookTarget = (terY + cameraHeight) - camAngleXZ;
		
	camera->setTarget(vector3df(camX + cos(camRotY), lookTarget, camZ + sin(camRotY)));
	
	Listener::setPosition(camX, cameraHeight, camZ);
	vector3df lookvec = (camera->getPosition() - camera->getTarget()).normalize();
	Listener::setDirection(lookvec.X, lookvec.Y, lookvec.Z);
}



