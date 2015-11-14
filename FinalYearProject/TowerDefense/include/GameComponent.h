#ifndef GAMECOMPONENT_H
#define GAMECOMPONENT_H

#include <string>

class GameComponent {
private:
	std::string componentName;
	int parentObjectID;
public:
	GameComponent(std::string name) { componentName = name; };
	virtual ~GameComponent(){};
	int getObjectID() { return parentObjectID; }
	std::string getName() { return componentName; }
	
	bool operator== (const GameComponent& other) {
		return componentName == other.componentName;
	};
};

#endif