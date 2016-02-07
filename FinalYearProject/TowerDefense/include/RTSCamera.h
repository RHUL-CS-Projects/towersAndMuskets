#ifndef RTSCAMERA_H
#define RTSCAMERA_H

#include <irrlicht/irrlicht.h>
#include <Game.h>

class RTSCamera {
private:
	float cameraHeight;
	float targetCamHeight;
	float camX;
	float camZ;
	float camRotY;
	float camAngleXZ;
	irr::scene::ICameraSceneNode* camera;

public:
	RTSCamera();

	void addToScene();
	void update();
};

#endif