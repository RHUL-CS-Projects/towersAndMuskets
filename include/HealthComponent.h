#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H

#include <GameComponent.h>
#include <irrlicht/irrlicht.h>

class HealthComponent : public GameComponent {
private:

public:
	HealthComponent (int max, int hp) : GameComponent("HealthComponent") {
		maxHealth = max;
		health = hp;
		billboardNode = nullptr;
		barTexture = nullptr;
	}
	
	irr::scene::IBillboardSceneNode* billboardNode;
	irr::video::ITexture* barTexture;
	
	int maxHealth;
	int health;
	
	float alpha = 0;
};

#endif