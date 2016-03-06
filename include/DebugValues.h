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
	
	// Whether or not bounding boxes should be drawn
	static bool DRAW_BOUNDING_BOXES;
	
	// Whether or not the FPS counter should be printed into the console
	static bool PRINT_FPS;
	
	// Whether or not the picking texture should be drawn
	static bool DRAW_PICK_BOXES;
};

#endif