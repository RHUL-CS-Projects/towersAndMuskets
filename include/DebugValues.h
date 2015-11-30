#ifndef DEBUGVALUES_H
#define DEBUGVALUES_H

/**
 * Stores a number of debug flags
 */
class DebugValues {
public:
	// Whether or not the collision quadtree should be drawn
	static bool DRAW_QUADTREE;
	
	// Whether or not paths should be drawn
	static bool DRAW_PATHS;
	
	// Whether or not steering forces should be drawn
	static bool DRAW_STEER;
	
	// Whether or not the world grid should be drawn
	static bool DRAW_GRIDWORLD;
	
	// Whether the stencil buffer should be enabled for Irrlicht's stencil shadows
	static bool STENCIL_ENABLED;
};

#endif