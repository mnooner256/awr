/*
 A* Pathfinding Algorithm for REU

 Created by: Francisco Ruiz
 */

#ifndef ASTAR_H
#define ASTAR_H

#include <fstream>
#include "Node.h"

using namespace std;

class Path
{
private:
	Node* path;
	Node* curr;
	int start_x, start_y, end_x, end_y;
public:
	//default constructor
	Path(int size)
	{
		path = new int[size];
		curr = path;
		start_x = start_y = end_x = end_y = -1;
	}
	//actual pathfinding algorithm
	int* aStar()
	{

	}
};
#endif
