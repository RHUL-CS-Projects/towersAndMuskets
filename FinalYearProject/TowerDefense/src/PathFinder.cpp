#include <PathFinder.h>
#include <queue>
#include <algorithm>
#include <iostream>
#include <RenderManager.h>
#include <Game.h>
#include <StatePlaying.h>

using namespace irr;
using namespace scene;
using namespace core;
using namespace std;

bool pathFound;

/**
* Comparison operator for Nodes
*/
bool operator<(const PathFindNode& left, const PathFindNode& right) {
	return left.f < right.f;
}

/**
* Equality operator for Nodes
*/
bool operator==(const PathFindNode& left, const PathFindNode& right) {
	return (left.x == right.x && left.y == right.y);
}

template <typename T> bool PComp(const T * const & a, const T * const & b)
{
   return *a < *b;
}

NodePath PathFinder::findPath ( vector3df start, vector3df end ) {
	int startX = (int)floor(start.X/worldManager->gridSize);
	int startY = (int)floor(start.Z/worldManager->gridSize);
	
	int endX = (int)floor(end.X/worldManager->gridSize);
	int endY = (int)floor(end.Z/worldManager->gridSize);
	
	pathFound = false;
	NodePath path = findPath(startX, startY, endX, endY);
	
	end.Y = ((ITerrainSceneNode*)Game::game.getRendMgr()->getSceneManager()->getSceneNodeFromName("MainTerrain"))->getHeight(end.X, end.Z);
	
	if (pathFound) {
		path.addNodeFront(start);
		path.addNode(end);
	} else {
		((StatePlaying*)Game::game.currentState())->message(SHOW_MESSAGE_BAD, "Cannot move here");
	}
	
	return path;
}


NodePath PathFinder::findPath ( int startX, int startY, int endX, int endY ) {
	NodePath path;
	int gridWidth = worldManager->gridWidth;
	int gridHeight = worldManager->gridHeight;
	
	vector<PathFindNode*> openList;
	vector<PathFindNode*> closedList;
	PathFindNode nodes[gridWidth][gridHeight];
	
	// Initialise start node
	nodes[startX][startY].h = calcHValue(startX, startY, endX, endY);
	nodes[startX][startY].f = nodes[startX][startY].h;
	nodes[startX][startY].g = 0;
	nodes[startX][startY].parent = nullptr;
	nodes[startX][startY].x = startX;
	nodes[startX][startY].y = startY;
	
	// Initliase end node
	nodes[endX][endY].f = 0;
	nodes[endX][endY].g = 0;
	nodes[endX][endY].h = 0;
	nodes[endX][endY].parent = nullptr;
	nodes[endX][endY].x = endX;
	nodes[endX][endY].y = endY;
	
	int currentX = startX;
	int currentY = startY;
	openList.push_back(&nodes[startX][startY]);
	
	int indexOfLowestF = 0;
	bool foundPath = false;
	
	while (openList.size() > 0) {
		indexOfLowestF = indexOfSmallestF(openList);
		PathFindNode* current = openList.at(indexOfLowestF);
		currentX = current->x;
		currentY = current->y;
		
		if (currentX == endX && currentY == endY) {
			foundPath = true;
			break;
		}
		
		openList.erase(openList.begin() + indexOfLowestF);
		closedList.push_back(current);
		current->closed = true;
		
		// Iterate over neighbours
		for (int xx = currentX-1; xx <= currentX+1; xx++) {
			for (int yy = currentY-1; yy <= currentY+1; yy++) {
				if (xx < 0 || xx >= worldManager->gridWidth || 
					yy < 0 || yy >= worldManager->gridHeight)
					continue;
				
				if (xx == currentX && yy == currentY) continue;
				if (!worldManager->checkPassableGridCoords(xx,yy)) continue;
				//if (xx != currentX && yy != currentY) continue;
				
				if (nodes[xx][yy].closed) continue;
				nodes[xx][yy].x = xx;
				nodes[xx][yy].y = yy;
				
				int tempG = current->g + ((xx == currentX || yy == currentY) ? 10 : 14);
				
				if (!nodes[xx][yy].open) {
					openList.push_back(&nodes[xx][yy]);
					nodes[xx][yy].open = true;
				} else if (tempG >= nodes[xx][yy].g || nodes[xx][yy].g < 0) {
					continue;
				}
				
				nodes[xx][yy].g = tempG;
				nodes[xx][yy].parent = current;
				nodes[xx][yy].h = calcHValue(xx,yy,endX,endY);
				nodes[xx][yy].f = nodes[xx][yy].g + nodes[xx][yy].h;
			}
		}
	}
	
	if (foundPath) {
		PathFindNode* current = &nodes[currentX][currentY];
		while (currentX != startX || currentY != startY) {
			if (currentX != endX || currentY != endY) {
				float worldX = currentX * worldManager->gridSize + worldManager->gridSize/2;
				float worldZ = currentY * worldManager->gridSize + worldManager->gridSize/2;
				int yPos = ((ITerrainSceneNode*)Game::game.getRendMgr()->getSceneManager()->getSceneNodeFromName("MainTerrain"))->getHeight(worldX, worldZ);
				path.addNodeFront(vector3df(worldX, yPos, worldZ));
			}
			current = current->parent;
			currentX = current->x;
			currentY = current->y;
		}
		pathFound = true;
	}
	//path.addNodeFront(vector3df(startX * worldManager->gridSize + worldManager->gridSize/2, 0, startY * worldManager->gridSize + worldManager->gridSize/2));
	
	return path;
}


int PathFinder::costFromNode ( int x, int y, int prevX, int prevY ) {
	if (prevX == x && prevY == y) 
		return 0;  // No movement
	else if (prevX == x || prevY == y) 
		return 10; // Horizontal/vertical move
	else
		return 14; // Diagonal move
}


int PathFinder::calcFValue ( int gVal, int hVal ) {
	return gVal + hVal;
}


int PathFinder::calcGValue ( int x, int y, int prevX, int prevY ) {
	return costFromNode(x, y, prevX, prevY);
}


int PathFinder::calcHValue ( int fromX, int fromY, int endX, int endY ) {
	return manhattanDistance(fromX, fromY, endX, endY);
}


int PathFinder::manhattanDistance ( int fromX, int fromY, int endX, int endY ) {
	return abs(fromX - endX) + abs(fromY - endY);
}


PathFindNode PathFinder::makeNode ( PathFindNode parent, int x, int y, int f, int g, int h ) {
	PathFindNode newNode;
	return newNode;
}

int PathFinder::indexOfSmallestF ( std::vector< PathFindNode* > nodes ) {
	int f = -1;
	int index = 0;
	for (int i = 0; i < nodes.size(); i++) {
		if (f < 0 || nodes.at(i)->f < f) {
			f = nodes.at(i)->f;
			index = i;
		}
	}
	return index;
}

/*vector< PathFindNode* > PathFinder::getNeighbours ( PathFindNode nodes[][], int x, int y ) {
	vector<PathFindNode*> neighbours;
	
	
}*/


/*int PathFinder::containsNode ( PathFindNode* node, std::vector< PathFindNode* > nodes ) {

}
*/


/*std::vector< PathFindNode* > PathFinder::getNeighboursJPS ( PathFindNode* current, int endX, int endY ) {

}*/


/*PathFindNode* PathFinder::jump ( int x, int y, int fromX, int fromY, int endX, int endY ) {

}*/












