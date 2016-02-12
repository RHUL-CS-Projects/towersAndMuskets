#ifndef OBJECTPLACER_H
#define OBJECTPLACER_H

#include <irrlicht/irrlicht.h>
#include <string>

enum OBJECT_TYPES {
	nullObject,
	Tower,
	Rock,
	Tree,
	EnemyUnit,
	PlayerUnit
};

class ObjectPlacer {
	irr::scene::ISceneNode* sceneNode;
	OBJECT_TYPES currentType = nullObject;
	
	float colWidth = 0;
	float colHeight = 0;
	
	bool mousePressed = false;
	bool placeable = false;
	irr::core::vector3df terrainPoint;
	
	// Pointer to a function in the object factory
	int (*addObject)(irr::core::vector3df);
	
	bool checkPlaceable(irr::core::vector3df pos);
	void setModelData(std::string meshName, std::string textureName);
	void place();
public:
	void init();
	void update();
	void render(irr::video::IVideoDriver* driver);
	void setObjectType(OBJECT_TYPES type);
};

#endif