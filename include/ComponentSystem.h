#ifndef COMPONENTSYSTEM_H
#define COMPONENTSYSTEM_H

#include <string>

using namespace std;

class ComponentSystem {
private:
	string componentName;

public:
	ComponentSystem(string name) { componentName = name; }
	string getName() { return componentName; }
	void update(float timestep);
};

#endif