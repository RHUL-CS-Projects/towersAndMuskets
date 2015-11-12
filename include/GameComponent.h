#ifndef GAMECOMPONENT_H
#define GAMECOMPONENT_H

#include <string>

using namespace std;

class GameComponent {
private:
	string componentName;
	int parentObjectID;
public:
	GameComponent(string name) { componentName = name; };
	virtual ~GameComponent(){};
	int getObjectID() { return parentObjectID; }
	string getName() { return componentName; }
	
	bool operator== (const GameComponent& other) {
		return componentName == other.componentName;
	};
};

#endif