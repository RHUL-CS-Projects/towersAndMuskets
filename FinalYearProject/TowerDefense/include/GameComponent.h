#ifndef GAMECOMPONENT_H
#define GAMECOMPONENT_H

#include <string>

/**
 * Base component class that all of the game's components
 * inherit from
 */
class GameComponent {
private:
	// Unique name of the component
	std::string componentName;
	
	// ID of the object that the component is attached to
	int parentObjectID;
public:
	
	/**
	 * Initialises the component's name
	 */
	GameComponent(std::string name) { componentName = name; };
	
	virtual ~GameComponent(){};
	
	/**
	 * Returns the ID of the attached object
	 */
	int getObjectID() { return parentObjectID; }
	
	/**
	 * Returns the name of the component
	 */
	std::string getName() { return componentName; }
	
	/**
	 * Overloads the == operator. Tests whether or not two components are the
	 * same type of component based on their names
	 */
	bool operator== (const GameComponent& other) {
		return componentName == other.componentName;
	};
};

#endif