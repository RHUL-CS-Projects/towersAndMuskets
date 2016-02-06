#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <iostream>
#include <unordered_map>
#include <list>
#include <string>

#include <GameComponent.h>
#include <ComponentSystem.h>
#include <WorldManager.h>

/**
 * The game's object manager. Has methods for creating and destroying game
 * objects, attaching/detaching their components, retrieving lists of
 * objects with specific components, testing whether or not an object has
 * a component, and retrieving a full list of an object's components
 */
class ObjectManager {
private:
	// Maps object IDs to lists of game components
	std::unordered_map<int, std::list<GameComponent*>>* objectComponents;
	
	// Maps components back to the list of objects that have them attached
	std::unordered_map<std::string, std::list<int>> componentObjects;
	
	// List of systems to be updated
	std::list<ComponentSystem*> systems;
	
	// Current object ID
	int objectID;
	
	// Number of objects in the game
	int objectCount;

public:
	// Static reference to the game's object manager
	//static ObjectManager manager;
	
	// Reference to the world manager for drawing the world grid
	WorldManager* worldManager;
	
	ObjectManager();
	~ObjectManager();
	
	/**
	 * Creates a new object and returns its unique ID
	 */
	int createObject();
	
	/**
	 * Destroys an object with a given ID
	 */
	void destroyObject(int id);
	
	/**
	 * Retrieves a list of all objects witha specific component attached
	 */
	std::list<int> getObjectsWithComponent(std::string componentName);
	
	/**
	 * Retrieves a list of all components attached to an object
	 */
	std::list<GameComponent*> getObjectComponents(int id);
	
	/**
	* Get a specific component attached to a given object id, and casts it to
	* be of the correct component type
	*/
	template <class T>
	T* getObjectComponent( int id, std::string componentName ) {
		if (!objectExists(id)) {
			std::cout << "Get Object Component Failed: Object with ID=" << id << " does not exist!" << std::endl;
			return nullptr;
		}
		
		std::list<GameComponent*> components = objectComponents->at(id);
		
		for (GameComponent* gc : components) {
			if (gc->getName() == componentName) {
				T* t = dynamic_cast<T*>(gc);
				if (t == nullptr) 
					continue;
				else
					return t;
			}
		}

		return nullptr;
	}
	
	/**
	 * Checks whether or not an object has a particular component attached
	 */
	bool objectHasComponent(int id, std::string componentName);
	
	/**
	 * Checks if an object with a particular ID exists in the system
	 */
	bool objectExists(int id);
	
	/**
	 * Attaches a component to a game object
	 */
	void attachComponent(int id, GameComponent* component);
	
	/**
	 * Removes a component from a game object
	 */
	void detachComponent(int id, std::string componentName);
	
	/**
	 * Returns the total number of objects currently in the game
	 */
	int getObjectCount();
	
	/**
	 * Updates all systems. Takes a delta time between updates which
	 * is currently unused
	 */
	void updateSystems(float timestep);
	
	/**
	 * Calls the draw method for each system
	 */
	void drawSystems(float timestep);
	
	/**
	 * Prints information about a game object to the terminal
	 */
	void printGameObject(int id);
	
	/**
	 * Prints information about all game objects to the terminal
	 */
	void printGameObjects();
	
	/**
	 * Prints a list of all objects with a specific component to the terminal
	 */
	void printObjectsWithComponent(std::string componentName);
};

#endif