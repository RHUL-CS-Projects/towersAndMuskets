#include <EventReceiver.h>
#include <RenderManager.h>
#include <iostream>
#include <DebugValues.h>
#include <Game.h>
#include <list>
#include <ResourceComponent.h>

EventReceiver::SMouseState EventReceiver::MouseState;

using namespace irr;
using namespace core;
using namespace gui;
using namespace scene;
using namespace video;

int EventReceiver::hoverID;
irr::video::ITexture* EventReceiver::renderTarget;
std::list<CollisionCube> EventReceiver::cubes;
irr::video::IImage* EventReceiver::renderImage;

bool EventReceiver::OnEvent ( const SEvent& event ) {
	
	if (event.EventType == irr::EET_MOUSE_INPUT_EVENT) {
		ResourceComponent* rescomp;
		
		switch (event.MouseInput.Event) {
			case EMIE_LMOUSE_PRESSED_DOWN:
				EventReceiver::MouseState.leftPressed = true;
				break;
			case EMIE_LMOUSE_LEFT_UP:
				EventReceiver::MouseState.leftPressed = false;
				break;
			case EMIE_RMOUSE_PRESSED_DOWN:
				EventReceiver::MouseState.rightPressed = true;
				break;
			case EMIE_RMOUSE_LEFT_UP:
				EventReceiver::MouseState.rightPressed = false;
				break;
			case EMIE_MOUSE_WHEEL:
				EventReceiver::MouseState.wheelDelta = event.MouseInput.Wheel;
				break;
			case EMIE_MOUSE_MOVED:
				EventReceiver::MouseState.position.X = event.MouseInput.X;
				EventReceiver::MouseState.position.Y = event.MouseInput.Y;
				break;
			default:
				break;
		}
	}
	
	return false;
}

	
EventReceiver::SMouseState* EventReceiver::getMouseState() {
	return &EventReceiver::MouseState;
}

void EventReceiver::renderCollisionBoxes() {
	Game::game.getRendMgr()->getSceneManager();
	
	if (renderTarget == nullptr) {
		if (Game::game.getRendMgr()->getDriver()->queryFeature(video::EVDF_RENDER_TO_TARGET)) {
			renderTarget = Game::game.getRendMgr()->getDriver()->addRenderTargetTexture(Game::game.getRendMgr()->getDriver()->getScreenSize());
		} else {
			std::cout << "Video card does not support rendering to textures!" << std::endl;
		}
	}
	
	for (CollisionCube n : cubes) {
		if (n.cubeNode != nullptr) {
			aabbox3df box = n.parentNode->getTransformedBoundingBox();
			vector3df centre = box.getCenter();
			vector3df size = box.getExtent();
			
			n.cubeNode->setPosition(centre);
			n.cubeNode->setScale(size);
			
			n.parentPrevVisibility = n.parentNode->isVisible();
			n.cubeNode->setVisible(n.parentPrevVisibility);
			n.parentNode->setVisible(false);
		} else {
			std::cout << "Cube was null" << std::endl;
		}
	}
	
	// Hide terrain
	ISceneNode* terrain = Game::game.getRendMgr()->getSceneManager()->getSceneNodeFromName("MainTerrain");
	if (terrain != nullptr)
		terrain->setVisible(false);	
	
	Game::game.getRendMgr()->getDriver()->setRenderTarget(renderTarget, true, true, SColor(255,255,255,255));
	Game::game.getRendMgr()->getSceneManager()->drawAll();
	Game::game.getRendMgr()->getDriver()->setRenderTarget(0, true, true, 0);
	
	if (terrain != nullptr)
		terrain->setVisible(true);
	
	for (CollisionCube n : cubes) {
		if (n.cubeNode != nullptr) {
			n.cubeNode->setVisible(false);
			n.parentNode->setVisible(n.parentPrevVisibility);
		} else {
			std::cout << "Cube was null" << std::endl;
		}
	}
}

int EventReceiver::getHoverObject() {
	if (renderTarget == nullptr) return -1;
	
	hoverID = -1;
	
	if (renderImage != nullptr)
		renderImage->drop();
	
	renderImage = Game::game.getRendMgr()->getDriver()->createImageFromData(renderTarget->getColorFormat(), renderTarget->getSize(), renderTarget->lock());
	//renderTarget->unlock();
	
	SColor pixel;
	pixel = renderImage->getPixel(MouseState.position.X, MouseState.position.Y);
	
	if (pixel.color != SColor(255,255,255,255).color)
		hoverID = pixel.color;
	
	return hoverID;
}






