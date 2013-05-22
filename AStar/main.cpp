/***************************************************************
Program that brings it all together

Created by: Francisco Ruiz
***************************************************************/
#include <fstream>
#include "Node.h"

using namespace std;

void main()
{
	fstream f;
	int x_size, y_size, map_area;

	//reads file sent by UI - path will be changed
	f.open("C:/Users/fjr1983/Desktop/Map.txt", ios::in);
	//reads first two values as x- and y-size of map
	f >> x_size >> y_size;






}
