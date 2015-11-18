#ifndef ANIMATORCOMPONENT_H
#define ANIMATORCOMPONENT_H

#include <GameComponent.h>
#include <string>
#include <map>
#include <irrlicht/irrlicht.h>

struct AnimationData {
	std::string name;
	int begin;
	int end;
	int speed;
};

class AnimatorComponent : public GameComponent {
private:
	std::map<std::string, AnimationData> animations;
public:
	AnimatorComponent() : GameComponent("AnimatorComponent") {}
	
	std::string currentAnimation = "";
	
	void addAnimation(std::string name, int begin, int end, int speed) {
		AnimationData data;
		data.name = name;
		data.begin = begin;
		data.end = end;
		data.speed = speed;
		
		animations.insert(std::pair<std::string, AnimationData>(name, data));
	}
	
	void setAnimation(std::string name, irr::scene::IAnimatedMeshSceneNode* sceneNode) {
		if (animations.find(name) != animations.end() && currentAnimation != name) {
			currentAnimation = name;
			
			AnimationData data = animations.at(name);
			sceneNode->setFrameLoop(data.begin, data.end);
			sceneNode->setAnimationSpeed(data.speed);
		}
	}
};

#endif




