#ifndef RTSCAMERA_H
#define RTSCAMERA_H

#include <irrlicht/irrlicht.h>
#include <Game.h>

class RTSCamera {
private:
	float cameraHeight;
	float targetCamHeight;
	float targetGroundCamHeight;
	float camX;
	float camZ;
	float targetCamX;
	float targetCamZ;
	float camRotY;
	float targetCamRotY;
	float camAngleXZ;
	float lookTarget;
	
	bool groundCam;
	bool tabPressed;
	irr::scene::ICameraSceneNode* camera;

public:
	RTSCamera();

	void addToScene();
	void update();
};

#endif