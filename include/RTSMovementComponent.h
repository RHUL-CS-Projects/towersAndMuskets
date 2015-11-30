#ifndef RTSMOVEMENTCOMPONENT_H
#define RTSMOVEMENTCOMPONENT_H

#include <GameComponent.h>

/**
 * Component to allow an object's path to be set by right-clicking
 * on the world map
 */
class RTSMovementComponent : public GameComponent {
private:

public:
	RTSMovementComponent() : GameComponent("RTSMovementComponent") {}
};

#endif