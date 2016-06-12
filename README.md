#Towers and Muskets
<strong>Programmed by Jon Mortiboy</strong><br /><br />
This is a tower defence game mixing elements of real-time strategy created over the course of 
6 months as my final year project on the computer science course at Royal Holloway University of London.
It was written under Ubuntu in C++ using KDevelop4 for code editing. The code is cross-platform and can be
built for both Linux and Windows (possibly MacOS as well, although this is untested). 

#Features
The game uses an engine that was written from the ground up by myself, utilising the Irrlicht
library for grapihcs rendering and SFML for input and audio. The main technical features of the engine
consist of:<br />

- Pathfinding using the A* algorithm on a 2D grid<br />
- AI behaviours controlled by pushdown automata<br />
- Support for menu systems using state machines<br />
- Path following using steering behaviours<br />
- Basic collision avoidance for units<br />
- Mouse picking using an off-screen image rendered using shaders<br />
- Loading game worlds encoded as image files<br />
- Collision optimisation using spacial partitioning (quadtrees)<br />
- Units moving into formation when arriving as a group<br />

#Build/compilation instructions
(To be written)

#Directory Structure

/src			-	Source files (.cpp)<br />
/include		-	Header files (.h)<br />
/lib			-	32bit libraries<br />
/lib64			-	64bit libraries<br />
/res			- 	Game assets, models, textures<br />

# Gameplay Instructions

The objective of 'Tower Defence' is to survive 5 waves of enemies by building towers and
creating musketeers and cannons. You can also create gatherers which are used to collect
the necessary resources. Left-click and drag to select multiple units, or left-click on
individual units to select them. When gatherers are selected, right-click on trees, gold
or stone deposits to order them to gather there. Military units can be ordered to attack
specific enemies by right-clicking on them, however they will attack nearby enemies
automatically. Musketeers can be garissoned inside towers to gain attack and range bonuses
by first selecting them, then right-clicking on a tower. Up to 4 units can be garissoned
in a tower at any given time.

Blue flags along the far edge of the map represent enemy spawn locations. New flags will
appear with each wave as the enemy spawn counters increase.

The game is won when you have survived 5 waves. The game is lost if all of your units are
killed.

#Controls

W,A,S,D 	- Pan camera<br />
Q,E			- Rotate camera left/right<br />
Mousewheel	- Zoom in/out<br />
TAB			- Toggle ground camera<br />

<strong>Hints</strong>

- Units cannot be bought during waves<br />
- There is a timer that counts up between waves during which no enemies spawn. Use this time
to build your defence and collect resources.<br />
- The 'reveille' option, represented by the flag icon, turns all of your gatherers into 
musketeers at the cost of all of your resources. This essentially ends the game (you will have
no resources so no way to create more gatherers to collect any), however it allows you to make
a final stand if you have become overwhelmed.<br />
- The 'reveille' option can also be used in the final wave to benefit, as there is no need for
your resources after it has begun

