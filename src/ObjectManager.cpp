#include <ObjectManager.h>
#include <iostream>
#include <algorithm>

int ObjectManager::objectID = 0;
int ObjectManager::objectCount = 0;

unordered_map<int, list<GameComponent>> ObjectManager::objectComponents;
unordered_map<string, list<int>> ObjectManager::componentObjects;
list<ComponentSystem> ObjectManager::systems;


/**
 * Get the total number of objects currently in the game.
 */
int ObjectManager::getObjectCount() {
	return objectCount;
}

/**
 * Return a new GameObject with a unique id and update the object counter.
 */
int ObjectManager::createObject() {
	int newObjID = objectID;
	
	objectID++;
	objectCount++;
	
	return newObjID;
}

/**
 * Remove all components from a given object and then remove the object
 * id from the object map.
 */
void ObjectManager::destroyObject ( int id ) {
	list<GameComponent> components = getObjectComponents(id);
	
	for (GameComponent gc : components) {
		detachComponent(id, gc.getName());
	}
	
	objectComponents.erase(id);
	objectCount--;
}

/**
 * Check if a given object has a specific component attached to it.
 */
bool ObjectManager::objectHasComponent ( int id, string componentName ) {
	list<GameComponent> components = getObjectComponents(id);
	
	for (GameComponent gc : components) {
		if (gc.getName() == componentName)
			return true;
	}
	
	return false;
}

/**
 * Get a specific component attached to a given object id.
 */
GameComponent ObjectManager::getObjectComponent ( int id, string componentName ) {
	list<GameComponent> components = getObjectComponents(id);
	
	for (GameComponent gc : components) {
		if (gc.getName() == componentName)
			return gc;
	}
	
	return GameComponent();
}

/**
 * Get a list of all components attached to a game object.
 */
list< GameComponent > ObjectManager::getObjectComponents ( int id ) {
	return ((list<GameComponent>)objectComponents.at(id));
}

/**
 * Get a list of all game objects with a specific component attached.
 */
list< int > ObjectManager::getObjectsWithComponent ( string componentName ) {
	return ((list<int>)componentObjects.at(componentName));
}


/**
 * Attach a component to a game object.
 */
void ObjectManager::attachComponent ( int id, GameComponent component ) {
	list<GameComponent> components = getObjectComponents(id);
	components.push_back(component);
	
	objectComponents.insert(make_pair(id, components));
	
	if (componentObjects.count(component.getName()) == 0) {
		componentObjects.insert(make_pair(component.getName(), list<int>()));
	}
	
	((list<int>)componentObjects.at(component.getName())).push_back(id);
}

/**
 * Remove a component a given object.
 */
void ObjectManager::detachComponent ( int id, string componentName ) {
	list<int> objectIDs = ((list<int>)componentObjects.at(componentName));
	objectIDs.remove(id);
}

/**
 * Update all component systems.
 */
void ObjectManager::updateSystems ( float timestep ) {
	// system::update(); etc.
}



