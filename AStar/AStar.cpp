/* Functions for Pathing Algorithm
 *
 * Created by: Francisco Ruiz
 */

#include <fstream>
#include <string>
#include "Node.h"

using namespace std;

//Function to get map from file sent from UI
void getMap(Node map[])
{
	fstream f;
	int x_size, y_size, total_size;
	string dummy;

	f.open("c:/Users/Francisco/Desktop/map_layout.txt", ios::in);

	//reads sizes from first line in file
	f >> x_size >>y_size;

	total_size = x_size * y_size;

	//loops through the map and reads values to node
	for(int i = 0; i < total_size; i++)
	{
		f >> map[i].xPos >> map[i].yPos >> map[i].rfid >> dummy;
		//will check for garbage and collect if necessary
		while(dummy != '\n')
		{
			f >> dummy;
		}
	}
}

	}
}






}

