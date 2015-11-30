#ifndef SELECTIONSYSTEM_H
#define SELECTIONSYSTEM_H

#include <ComponentSystem.h>
#include <irrlicht/irrlicht.h>

/**
 * System for selecting a game object when a box is drawn over the screen
 */
class SelectionSystem : public ComponentSystem {	
private:
	// Whether or not the mouse is being dragged
	bool dragging = false;
	
	// The current position of the mouse
	irr::core::vector2d<int> mousePos;
	
	// The position where the mouse was initially clicked
	irr::core::vector2d<int> clickPos;
	
public:
	SelectionSystem() : ComponentSystem("SelectionSystem") {}
	
	/**
	 * Checks if a box was dragged on the screen and selects objects within
	 * the box
	 */
	void update(float timestep);
	
	/**
	 * Draw the 2D selection box to the screen
	 */
	void draw(float timestep);
	
	/**
	 * Checks whether or not objects lie within the bounds of the 2D rectangle
	 * drawn on screen and toggles their selection
	 */
	void selectObjects();
	
	/**
	 * Transforms a 3D point to 2D screen coordinates using the camera's
	 * view and projection matrices
	 */
	irr::core::vector2di worldToScreen(irr::core::vector3df point);
};


#endif