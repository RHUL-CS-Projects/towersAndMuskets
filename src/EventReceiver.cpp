#include <EventReceiver.h>

EventReceiver::SMouseState EventReceiver::MouseState;

bool EventReceiver::OnEvent ( const irr::SEvent& event ) {
	if (event.EventType == irr::EET_MOUSE_INPUT_EVENT) {
		switch (event.MouseInput.Event) {
			case irr::EMIE_LMOUSE_PRESSED_DOWN:
				EventReceiver::MouseState.leftPressed = true;
				break;
			case irr::EMIE_LMOUSE_LEFT_UP:
				EventReceiver::MouseState.leftPressed = false;
				break;
			case irr::EMIE_RMOUSE_PRESSED_DOWN:
				EventReceiver::MouseState.rightPressed = true;
				break;
			case irr::EMIE_RMOUSE_LEFT_UP:
				EventReceiver::MouseState.rightPressed = false;
				break;
			case irr::EMIE_MOUSE_WHEEL:
				EventReceiver::MouseState.wheelDelta = event.MouseInput.Wheel;
				break;
			case irr::EMIE_MOUSE_MOVED:
				EventReceiver::MouseState.position.X = event.MouseInput.X;
				EventReceiver::MouseState.position.Y = event.MouseInput.Y;
				break;
			default:
				break;
		}
	}
}

	
EventReceiver::SMouseState* EventReceiver::getMouseState() {
	return &EventReceiver::MouseState;
}
