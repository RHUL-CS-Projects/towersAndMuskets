#ifndef COMPONENTSYSTEM_H
#define COMPONENTSYSTEM_H

#include <string>

/**
 * Base class for the game's systems. Stores the name of the system
 * and has update and draw methods for systems to implement
 */
class ComponentSystem {
private:
	// The system's name
	std::string systemName;

public:
	/**
	 * Initialises the name of the system
	 */
	ComponentSystem(std::string name) { systemName = name; }
	~ComponentSystem();
	
	/**
	 * Returns the name of this system
	 */
	std::string getName() { return systemName; }
	
	/**
	 * Update method for systems to perform actions on their associated
	 * components
	 */
	virtual void update(float timestep) {};
	
	/**
	 * Draw method for rendering any debug information to the screen
	 */
	virtual void draw(float timestep) {};
};

#endif