#include <PathFinder.h>
#include <queue>
#include <algorithm>
#include <iostream>

using namespace irr;
using namespace core;
using namespace std;

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
	
	return findPath(startX, startY, endX, endY);
}


NodePath PathFinder::findPath ( int startX, int startY, int endX, int endY ) {
	vector<PathFindNode*> openList;
	vector<PathFindNode*> closedList;
	vector<PathFindNode*> neighbours;
	
	int startG = 0;
	int startH = calcHValue(startX, startY, endX, endY);
	PathFindNode* start = makeNode(nullptr, startX, startY, calcFValue(startG, startH), startG, startH);
	PathFindNode* current = start;
	
	PathFindNode* end = makeNode(nullptr, endX, endY, 0, 0, 0);

	openList.push_back(start);
	
	while (openList.size() > 0) {
		//vector<PathFindNode*>::iterator itOpen = min_element(openList.begin(), openList.end(), PComp<PathFindNode>);
		int smallestFIndex = indexOfSmallestF(openList);
		
		//current = openList.at(itOpen - openList.begin());
		//openList.erase(itOpen);
		
		current = openList.at(smallestFIndex);
		openList.erase(openList.begin() + smallestFIndex);
		
		closedList.push_back(current);
		neighbours = getNeighbours(current, endX, endY);
		
		std::cout << current->x << ", " << current->y << ", " << current->f << ",      " << end->x << ", " << end->y <<
			",      " << openList.size() << ", " << closedList.size() << std::endl;
		
		if (*current == *end) {
			// End reached
			end->parent = current->parent;
			break;
		}
		
		for (PathFindNode* n : neighbours) {
			// Node is already in the closed list
			//if (find(closedList.begin(), closedList.end(), n) != closedList.end())
			if (containsNode(n, closedList) > -1)
				continue;
			
			//itOpen = find(openList.begin(), openList.end(), n);
			
			// Check if node is not already in open list
			//if (itOpen == openList.end()) {
			int inOpen = containsNode(n, openList);
			
			if (inOpen < 0) {
				// Add to open list
				openList.push_back(n);
				continue;
			}
			
			//PathFindNode* fromOpen = openList.at(itOpen - openList.begin());
			PathFindNode* fromOpen = openList.at(inOpen);
			
			if (n->g >= fromOpen->g)
				continue;	// New path to this node is not shorter
			
			fromOpen->g = n->g;
			fromOpen->parent = current;
			fromOpen->f = calcFValue(fromOpen->g, fromOpen->h);
		}
	}
	
	NodePath path;

	PathFindNode* node = end;
	 while (!(*node == *start)) {
		std::cout << "here " << node->x << ", " << node->y << ",      " << node->parent->x << ", " << node->parent->y << std::endl;
		path.addNodeFront(vector3df((node->x + 0.5f)*worldManager->gridSize, 0, (node->y + 0.5f)*worldManager->gridSize));
		node = node->parent;
	}
	path.addNodeFront(vector3df((start->x + 0.5f)*worldManager->gridSize, 0, (start->y + 0.5f)*worldManager->gridSize));
	
	std::cout << "size " << path.getWaypoints().size() << std::endl;
	
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


PathFindNode* PathFinder::makeNode ( PathFindNode* parent, int x, int y, int f, int g, int h ) {
	PathFindNode* node = new PathFindNode();
	
	node->parent = parent;
	node->x = x;
	node->y = y;
	node->f = f;
	node->g = g;
	node->h = h;
		
	return node;
}


vector< PathFindNode* > PathFinder::getNeighbours ( PathFindNode* current, int endX, int endY ) {
	vector<PathFindNode*> nodes;
	int x = current->x;
	int y = current->y;

	
	for (int xx = x-1; xx <= x+1; xx++) {
		for (int yy = y-1; yy <= y+1; yy++) {
			// Check within grid
			if (xx < 0 || yy < 0 || xx >= worldManager->gridWidth || yy >= worldManager->gridHeight)
				continue;
			
			if (xx != x && yy < y && !worldManager->checkPassableGrixCoords(x, y-1))
				continue;
			
			if (xx != x && yy > y && !worldManager->checkPassableGrixCoords(x, y+1))
				continue;
			
			if (yy != y && xx < x && !worldManager->checkPassableGrixCoords(x-1, y))
				continue;
			
			
			if (yy != y && xx > x && !worldManager->checkPassableGrixCoords(x+1, y))
				continue;
			
			// Check passable
			if (!worldManager->checkPassableGrixCoords(xx, yy))
				continue;
			
			// Check not current node
			if (xx == x && yy == y)
				continue;
			
			int gVal = current->g + calcGValue(xx, yy, x, y);
			int hVal = calcHValue(xx, yy, endX, endY);
			nodes.push_back(makeNode(current, xx, yy, calcFValue(gVal, hVal), gVal, hVal));
					
		}
	}
	
	return nodes;
}


int PathFinder::containsNode ( PathFindNode* node, std::vector< PathFindNode* > nodes ) {
	for (int i = 0; i < nodes.size(); i++) {
		if (*nodes.at(i) == *node)
			return i;
	}
	
	return -1;
}


int PathFinder::indexOfSmallestF ( std::vector< PathFindNode* > nodes ) {
	int f = -1;
	int index = 0;
	
	for (int i = 0; i < nodes.size(); i++) {
		if (f == -1 || nodes.at(i)->f < f) {
			f = nodes.at(i)->f;
			index = i;
		}
	}
	
	return index;
}


