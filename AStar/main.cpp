/***************************************************************
Program that brings it all together

Created by: Francisco Ruiz
***************************************************************/
#include <fstream>
#include "Node.h"

using namespace std;

void main()
{
	fstream f, g;
	int x_size, y_size, map_area, x, y, rfid;
	Node* map;
	Node* curr;

	//reads file sent by UI - path will be changed - downloaded from some source
	f.open("C:/Users/fjr1983/Desktop/Map.txt", ios::in);
	//reads first two values as x- and y-size of map
	f >> x_size >> y_size;
	//allocates space for map dynamically
	map = new Node[x_size*y_size];
	//read nodes to get rfid or walls
	while(!f.eof())
	{
		for(int i=0; i<x_size*y_size; i++)
		{
			curr = map[i];

			f >> x >> y >> rfid;
			//reads values from file into current node
			curr->xPos = x;
			curr->yPos = y;
			curr->rfid = rfid;
			//if rfid is zero (wall), sets priority very high
			if(rfid == 0)
			{
				curr->priority = 500000;
			}
			//increments pointer to next node in sequence

			curr = map[i+1];
		}
	}
}















}






}
