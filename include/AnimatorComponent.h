#ifndef ANIMATORCOMPONENT_H
#define ANIMATORCOMPONENT_H

#include <GameComponent.h>
#include <string>
#include <map>
#include <irrlicht/irrlicht.h>

/**
 * Struct for storing data about a single animation, including
 * its name, the begin and end frames and a playback speed.
 */
struct AnimationData {
	std::string name;
	int begin;
	int end;
	int speed;
};

/**
 * Component for holding a number of different animations
 */
class AnimatorComponent : public GameComponent {
private:
	std::map<std::string, AnimationData> animations;
public:
	AnimatorComponent() : GameComponent("AnimatorComponent") {}
	
	// Holds the name of the current animation
	std::string currentAnimation = "";
	
	/**
	 * Adds an animation to the component. Takes a name, the begin and
	 * end frames, and the playback speed
	 */
	void addAnimation(std::string name, int begin, int end, int speed) {
		AnimationData data;
		data.name = name;
		data.begin = begin;
		data.end = end;
		data.speed = speed;
		
		animations.insert(std::pair<std::string, AnimationData>(name, data));
	}
	
	/**
	 * Sets the component's current animation. Takes as argument a reference to 
	 * an animated scene node and sets the frame loop region and speed based on
	 * the name of the animation to use
	 */
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




