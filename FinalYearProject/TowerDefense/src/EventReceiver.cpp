#include <EventReceiver.h>
#include <RenderManager.h>
#include <iostream>
#include <DebugValues.h>

EventReceiver::SMouseState EventReceiver::MouseState;

using namespace irr;
using namespace core;
using namespace gui;

bool EventReceiver::OnEvent ( const SEvent& event ) {
	if (event.EventType == EEVENT_TYPE::EET_GUI_EVENT) {
		int id = event.GUIEvent.Caller->getID();
		IGUIEnvironment* guiEnv = Context.device->getGUIEnvironment();
		
		switch (event.GUIEvent.EventType){
			case EGUI_EVENT_TYPE::EGET_BUTTON_CLICKED:
				
				switch (id) {
					case BUTTON_ID_QUADTREE:
						DebugValues::DRAW_QUADTREE = !DebugValues::DRAW_QUADTREE;
						Context.txtQuadtree->setText((DebugValues::DRAW_QUADTREE) ? L"ON" : L"OFF");
						break;
					case BUTTON_ID_PATHS:
						DebugValues::DRAW_PATHS = !DebugValues::DRAW_PATHS;
						Context.txtPaths->setText((DebugValues::DRAW_PATHS) ? L"ON" : L"OFF");
						break;
					case BUTTON_ID_STEER:
						DebugValues::DRAW_STEER = !DebugValues::DRAW_STEER;
						Context.txtSteer->setText((DebugValues::DRAW_STEER) ? L"ON" : L"OFF");
						break;
					case BUTTON_ID_STENCIL:
						DebugValues::STENCIL_ENABLED = !DebugValues::STENCIL_ENABLED;
						Context.txtStencil->setText((DebugValues::STENCIL_ENABLED) ? L"ON" : L"OFF");
						RenderManager::renderManager.getDriver()->disableFeature(video::EVDF_STENCIL_BUFFER);
						break;
					case BUTTON_ID_GRIDWORLD:
						DebugValues::DRAW_GRIDWORLD = !DebugValues::DRAW_GRIDWORLD;
						Context.txtGridWorld->setText((DebugValues::DRAW_GRIDWORLD) ? L"ON" : L"OFF");
						break;
				}
				
				break;
			default:
				break;
		}
	}
	
	if (event.EventType == irr::EET_MOUSE_INPUT_EVENT) {
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


