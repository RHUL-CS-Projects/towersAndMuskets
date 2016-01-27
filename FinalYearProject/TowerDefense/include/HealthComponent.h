#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H

#include <GameComponent.h>

class HealthComponent : public GameComponent {
private:

public:
	HealthComponent (int max, int hp) : GameComponent("HealthComponent") {
		maxHealth = max;
		health = hp;
	}
	
	int maxHealth;
	int health;
};

#endif