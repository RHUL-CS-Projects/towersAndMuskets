#include <iostream>
#include <algorithm>

#include <irrlicht/irrlicht.h>
#include <ObjectManager.h>
#include <GameComponent.h>

#include <ComponentSystem.h>
#include <RenderSystem.h>
#include <CameraSystem.h>
#include <SelectionSystem.h>
#include <PathMovementSystem.h>
#include <FaceDirectionSystem.h>
#include <RTSMovementSystem.h>
#include <SteeringSystem.h>
#include <DebugValues.h>

ObjectManager ObjectManager::manager;

using namespace std;

ObjectManager::ObjectManager() {
	objectID = 0;
	objectCount = 0;

	objectComponents = new unordered_map<int, std::list<GameComponent*>>();
	
	// Create and add game systems
	systems.push_back(new RenderSystem());
	systems.push_back(new CameraSystem());
	systems.push_back(new SelectionSystem());
	systems.push_back(new SteeringSystem());
	systems.push_back(new FaceDirectionSystem());
	systems.push_back(new RTSMovementSystem());
	
	worldManager = new WorldManager(8, irr::core::recti(0,0,480,480));
	
	worldManager->setPassable(irr::core::rectf(300, 300, 360, 400), false);
	worldManager->setPassable(irr::core::rectf(30, 50, 80, 70), false);
	worldManager->setPassable(irr::core::rectf(200, 300, 250, 320), false);
}

ObjectManager::~ObjectManager() {
	delete objectComponents;
	worldManager->dropWorld();
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
	objectComponents->insert(make_pair(newObjID, std::list<GameComponent*>()));
	
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
	
	std::list<GameComponent*> components = getObjectComponents(id);
	
	for (GameComponent* gc : components) {
		detachComponent(id, gc->getName());
	}
	
	objectComponents->erase(id);
	objectCount--;
}


/**
 * Check if a given object has a specific component attached to it.
 */
bool ObjectManager::objectHasComponent ( int id, std::string componentName ) {
	if (!objectExists(id)) {
		cerr << "Object Has Component Failed: Object with ID=" << id << " does not exist!" << endl;
		return false;
	}
	
	std::list<GameComponent*> components = getObjectComponents(id);
	
	for (GameComponent* gc : components) {
		if (gc->getName() == componentName)
			return true;
	}
	
	return false;
}


/**
 * Get a list of all components attached to a game object.
 */
std::list< GameComponent* > ObjectManager::getObjectComponents ( int id ) {
	if (objectExists(id)) {
		return objectComponents->at(id);
	}
	
	cerr << "Get Object Components Failed: Object with ID=" << id << " does not exist!" << endl;
	return std::list<GameComponent*>();
}


/**
 * Get a list of all game objects with a specific component attached.
 */
std::list< int > ObjectManager::getObjectsWithComponent ( std::string componentName ) {
	if (!(componentObjects.find(componentName) != componentObjects.end()))
		return std::list<int>();
	else
		return ((std::list<int>)componentObjects.at(componentName));
}


/**
 * Attach a component to a game object.
 */
void ObjectManager::attachComponent ( int id, GameComponent* component ) {
	if (!objectExists(id)) {
		cerr << "Attach Component Failed: Object with ID=" << id << " does not exist!" << endl;
		return;
	}
	
	std::list<GameComponent*>* components = &objectComponents->at(id);
	components->push_back(component);
	
	//objectComponents.insert(make_pair(id, components));
	
	if (componentObjects.count(component->getName()) == 0) {
		componentObjects.insert(make_pair(component->getName(), std::list<int>()));
	}
	
	(&componentObjects.at(component->getName()))->push_back(id);
}


/**
 * Remove a component a given object.
 */
void ObjectManager::detachComponent ( int id, std::string componentName ) {
	if (!objectExists(id)) {
		cerr << "Detach Component Failed: Object with ID=" << id << " does not exist!" << endl;
		return;
	}
	
	std::list<int>* objectIDs = &componentObjects.at(componentName);
	objectIDs->remove(id);
	
	std::list<GameComponent*>* components = &objectComponents->at(id);
	
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
	
	for (ComponentSystem* system : systems) {
		system->update(timestep);
	}
}


/**
 * Render all component systems.
 */
void ObjectManager::drawSystems ( float timestep ) {
	RenderManager::renderManager.getDriver()->beginScene(true, true, irr::video::SColor(255,159,200,214));
	
	for (ComponentSystem* system : systems) {
		system->draw(timestep);
	}
	
	RenderManager::renderManager.getGUIEnvironment()->drawAll();
	
	if (DebugValues::DRAW_GRIDWORLD)
		worldManager->draw(timestep);
	
	RenderManager::renderManager.getDriver()->endScene();
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
	
	std::list<GameComponent*> components = getObjectComponents(id);
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
 * Print information about all game objects
 */
void ObjectManager::printGameObjects() {

	cout << endl << "ALL GAME OBJECTS:" << endl;
	for (unordered_map<int, std::list<GameComponent*>>::iterator it = objectComponents->begin(); it != objectComponents->end(); ++it) {
		printGameObject(it->first);
	}
}


/**
 * Print the IDs of all game objects that have a specific component attached.
 */
void ObjectManager::printObjectsWithComponent ( std::string componentName ) {
	std::list<int> ids = getObjectsWithComponent(componentName);
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









