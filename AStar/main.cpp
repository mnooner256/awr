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

using namespace std;

Node* map;
string path;

// A-star algorithm.
// The route returned is a string of direction digits.

int main()
{
	int x_start, y_start, x_end, y_end, total_size;

	total_size = getSize();
	map = getMap(total_size);

	x_start = y_start = 0;
	x_end = y_end = 3;

	//magically read position from RFID sensor
	path = pathFind(x_start, y_start, x_end, y_end);

	cout << path;

	//magically convey directions to robot

    return(0);
}
