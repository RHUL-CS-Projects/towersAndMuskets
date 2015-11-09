#include <ObjectManager.h>
#include <iostream>
#include <algorithm>

// int /*ObjectManager::*/objectID = 0;
// int /*ObjectManager::*/objectCount = 0;
// 
// unordered_map<int, list<GameComponent*>>* /*ObjectManager::*/objectComponents = new unordered_map<int, list<GameComponent*>>();
// unordered_map<string, list<int>> /*ObjectManager::*/componentObjects;
// list<ComponentSystem> /*ObjectManager::*/systems;


ObjectManager::ObjectManager() {
	objectID = 0;
	objectCount = 0;

	objectComponents = new unordered_map<int, list<GameComponent*>>();
}

ObjectManager::~ObjectManager() {
	delete objectComponents;
}


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
	objectComponents->insert(make_pair(newObjID, list<GameComponent*>()));
	
	objectID++;
	objectCount++;
	
	return newObjID;
}

/**
 * Remove all components from a given object and then remove the object
 * id from the object map.
 */
void ObjectManager::destroyObject ( int id ) {
	if (!objectExists(id)) {
		cerr << "Destroy Object Failed: Object with ID=" << id << " does not exist!" << endl;
		return;
	}
	
	list<GameComponent*> components = getObjectComponents(id);
	
	for (GameComponent* gc : components) {
		detachComponent(id, gc->getName());
	}
	
	objectComponents->erase(id);
	objectCount--;
}

/**
 * Check if a given object has a specific component attached to it.
 */
bool ObjectManager::objectHasComponent ( int id, string componentName ) {
	if (!objectExists(id)) {
		cerr << "Object Has Component Failed: Object with ID=" << id << " does not exist!" << endl;
		return false;
	}
	
	list<GameComponent*> components = getObjectComponents(id);
	
	for (GameComponent* gc : components) {
		if (gc->getName() == componentName)
			return true;
	}
	
	return false;
}

/**
 * Get a specific component attached to a given object id.
 */
GameComponent* ObjectManager::getObjectComponent( int id, string componentName ) {
	if (!objectExists(id)) {
		cerr << "Get Object Component Failed: Object with ID=" << id << " does not exist!" << endl;
		return nullptr;
	}
	
	list<GameComponent*> components = objectComponents->at(id);
	
	for (GameComponent* gc : components) {
		if (gc->getName() == componentName) {
			return gc;
		}
	}
	
	return nullptr;
}

/**
 * Get a list of all components attached to a game object.
 */
list< GameComponent* > ObjectManager::getObjectComponents ( int id ) {
	if (objectExists(id)) {
		return objectComponents->at(id);
	}
	
	cerr << "Get Object Components Failed: Object with ID=" << id << " does not exist!" << endl;
	return list<GameComponent*>();
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
void ObjectManager::attachComponent ( int id, GameComponent* component ) {
	if (!objectExists(id)) {
		cerr << "Attach Component Failed: Object with ID=" << id << " does not exist!" << endl;
		return;
	}
	
	list<GameComponent*>* components = &objectComponents->at(id);
	components->push_back(component);
	
	//objectComponents.insert(make_pair(id, components));
	
	if (componentObjects.count(component->getName()) == 0) {
		componentObjects.insert(make_pair(component->getName(), list<int>()));
	}
	
	(&componentObjects.at(component->getName()))->push_back(id);
}

/**
 * Remove a component a given object.
 */
void ObjectManager::detachComponent ( int id, string componentName ) {
	if (!objectExists(id)) {
		cerr << "Detach Component Failed: Object with ID=" << id << " does not exist!" << endl;
		return;
	}
	
	list<int>* objectIDs = &componentObjects.at(componentName);
	objectIDs->remove(id);
	
	list<GameComponent*>* components = &objectComponents->at(id);
	
	for (GameComponent* gc : *components) {
		if (gc->getName() == componentName) {
			components->remove(gc);
			break;
		}
	}
}

/**
 * Update all component systems.
 */
void ObjectManager::updateSystems ( float timestep ) {
	// system::update(); etc.
}

/**
 * Check if there is an object with the given id currently
 * in the game.
 */
bool ObjectManager::objectExists ( int id ) {
	return objectComponents->find(id) != objectComponents->end();
}


/**
 * Print information about a game object's components for debugging purposes.
 */
void ObjectManager::printGameObject ( int id ) {
	if (!objectExists(id)) {
		cerr << "Print Object Failed: Object with ID=" << id << " does not exist!" << endl;
		return;
	}
	
	list<GameComponent*> components = getObjectComponents(id);
	if (components.size() == 0) {
		cout << "Object ID=" << id << " has NO components" << endl;
		return;
	}
	
	cout << "Object ID=" << id << " has components:" << endl;
	
	for (GameComponent* gc : components) {
		cout << gc->getName() << endl;
	}
	
	cout << endl;
}

/**
 * Print the IDs of all game objects that have a specific component attached.
 */
void ObjectManager::printObjectsWithComponent ( string componentName ) {
	list<int> ids = getObjectsWithComponent(componentName);
	if (ids.size() == 0) {
		cout << "NO components have " << componentName << " attached" << endl;
		return;
	}
	
	cout << "Object IDs with " << componentName << " attached:" << endl;
	for (int i : ids) {
		cout << i << ", ";
	}
	
	cout << endl << endl;
}









