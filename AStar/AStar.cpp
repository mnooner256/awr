/* Functions for Pathing Algorithm
 *
 * Created by: Francisco Ruiz
 */

#include <fstream>
#include "Node.h"

using namespace std;

//Function to get map from file sent from UI
void getMap(Node map[])
{
	fstream f;
	int x_size, y_size, total_size;

	f.open("c:/Users/Francisco/Desktop/map_layout.txt", ios::in);

	//reads sizes from first line in file
	f >> x_size >>y_size;

	total_size = x_size * y_size;

	//loops through the map and reads values to node
	for(int i = 0; i < total_size; i++)
	{

	}
}






}

