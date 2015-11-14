#ifndef COMPONENTSYSTEM_H
#define COMPONENTSYSTEM_H

#include <string>

class ComponentSystem {
private:
	std::string componentName;

public:
	ComponentSystem(std::string name) { componentName = name; }
	~ComponentSystem();
	std::string getName() { return componentName; }
	virtual void update(float timestep) {};
	virtual void draw(float timestep) {};
};

#endif