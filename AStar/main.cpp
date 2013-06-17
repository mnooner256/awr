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
#include "Node.h"
#include "Astar.h"

using namespace std;

Node* map;
string path;

// A-star algorithm.
// The route returned is a string of directional digits.

int main()
{
	int x_start,  y_start, x_end, y_end, total_size;

	total_size = getSize();
	map = getMap(total_size);

	//start at (0,0) end at (4,0)
	x_start = y_start = 0;
	x_end = 3;
	y_end = 3;

	//magically read position from RFID sensor

	path = pathFind(map, x_start, y_start, x_end, y_end, total_size);

	//findStart(char[])
	//sendPath(string)
	//--->verify RFID
	//------->error checking

    return 0;
}
