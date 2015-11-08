#include <unordered_map>
#include <list>
#include <string>
#include <GameComponent.h>
#include <ComponentSystem.h>

using namespace std;

class ObjectManager {
private:
	static unordered_map<int, list<GameComponent>> objectComponents;
	static unordered_map<string, list<int>> componentObjects;
	static list<ComponentSystem> systems;
	static int objectID;
	static int objectCount;

public:
	static int createObject();
	static void destroyObject(int id);
	static list<int> getObjectsWithComponent(string componentName);
	static list<GameComponent> getObjectComponents(int id);
	static GameComponent getObjectComponent(int id, string componentName);
	static bool objectHasComponent(int id, string componentName);
	
	static void attachComponent(int id, GameComponent component);
	static void detachComponent(int id, string componentName);
	
	static int getObjectCount();
	
	static void updateSystems(float timestep);
};