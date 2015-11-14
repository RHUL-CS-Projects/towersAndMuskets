#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <iostream>
#include <unordered_map>
#include <list>
#include <string>

#include <GameComponent.h>
#include <ComponentSystem.h>

class ObjectManager {
private:
	std::unordered_map<int, std::list<GameComponent*>>* objectComponents;
	std::unordered_map<std::string, std::list<int>> componentObjects;
	std::list<ComponentSystem*> systems;
	int objectID;
	int objectCount;

public:
	static ObjectManager manager;
	
	ObjectManager();
	~ObjectManager();
	
	int createObject();
	void destroyObject(int id);
	std::list<int> getObjectsWithComponent(std::string componentName);
	std::list<GameComponent*> getObjectComponents(int id);
	
	/**
	* Get a specific component attached to a given object id.
	*/
	template <class T>
	T* getObjectComponent( int id, std::string componentName ) {
		if (!objectExists(id)) {
			std::cerr << "Get Object Component Failed: Object with ID=" << id << " does not exist!" << std::endl;
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
	
	bool objectHasComponent(int id, std::string componentName);
	bool objectExists(int id);
	
	void attachComponent(int id, GameComponent* component);
	void detachComponent(int id, std::string componentName);
	
	int getObjectCount();
	
	void updateSystems(float timestep);
	void printGameObject(int id);
	void printGameObjects();
	void printObjectsWithComponent(std::string componentName);
};

#endif