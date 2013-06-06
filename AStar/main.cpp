/*A* Pathing Algorithm for Automated Wheelchair

 Created by: Francisco Ruiz
 */

#include <fstream>
#include <iostream>
#include <iomanip>
#include <queue>
#include <string>
#include <math.h>
#include <ctime>
#include "AStar.cpp"
#include "Node.h"

using namespace std;

//Directions
const int DIR = 8;
int m, n, total_size;
int dir_map[n][m]; 			// map of directions
int closed_nodes_map[total_size]; 	// map of closed (tried-out) nodes
int open_nodes_map[total_size]; 	// map of open (not-yet-tried) nodes
int dx[DIR]={1, 1, 0, -1, -1, -1, 0, 1};
int dy[DIR]={0, 1, 1, 1, 0, -1, -1, -1};
Node* map;
string path;

// A-star algorithm.
// The route returned is a string of direction digits.

int main()
{
	int x_start, y_start, x_end, y_end;

	total_size = getSize();
	map = getMap(total_size);

	//magically read position from RFID sensor
	path = pathFind(x_start, y_start, x_end, y_end);

	//magically convey directions to robot

    return(0);
}
