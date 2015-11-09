#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <unordered_map>
#include <list>
#include <string>
#include <GameComponent.h>
#include <ComponentSystem.h>

using namespace std;

class ObjectManager {
private:
	unordered_map<int, list<GameComponent*>>* objectComponents;
	unordered_map<string, list<int>> componentObjects;
	list<ComponentSystem> systems;
	int objectID;
	int objectCount;

public:
	ObjectManager();
	~ObjectManager();
	
	int createObject();
	void destroyObject(int id);
	list<int> getObjectsWithComponent(string componentName);
	list<GameComponent*> getObjectComponents(int id);
	GameComponent* getObjectComponent(int id, string componentName);
	bool objectHasComponent(int id, string componentName);
	bool objectExists(int id);
	
	void attachComponent(int id, GameComponent* component);
	void detachComponent(int id, string componentName);
	
	int getObjectCount();
	
	void updateSystems(float timestep);
	void printGameObject(int id);
	void printObjectsWithComponent(string componentName);
};

#endif