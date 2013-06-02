/***************************************************************
Program that brings it all together

Created by: Francisco Ruiz
***************************************************************/
#include <cmath>
#include <fstream>
#include <queue>
#include "AStar.h"
#include "Node.h"

using namespace std;

void main()
{
	fstream f, g;
	int x_size, y_size, map_area, x, y, rfid;
	Node start, end;
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
			else
			{
				curr->priority = 1;
			}
			//increments pointer to next node in sequence

			curr = map[i+1];
		}
	}
	f.close();

	g.open("C:/Users/Desktop/Start.txt", ios::in);
	//infinite loop - will research
	while(true)
	{
		//read RFID tag into rfid variable for start position
		curr = map[0];
		//will find current position based on rfid tag and fill start Node
		while (curr->rfid != rfid)
		{
			curr++;
		}

		start.xPos = curr->xPos;
		start.yPos = curr->yPos;
		//read finish coordinates and put in end node
		g >> x >> y;
		end.xPos = x;
		end.yPos = y;
		//calculates distance for every node in map (except walls)
		for(int i=0; i<x_size*y_size; i++)
		{
			if(map[i].priority == 500000)
			{
				map[i].distance = 500000;
			}
			else
			{
				map[i].distance = (sqrt(pow(map[i].xPos - end.xPos, 2) + pow(map[i].yPos - end.yPos, 2)));
			}
		}
		//won't stop until parent node is reached
		while((curr->xPos != end.xPos) && (curr->yPos != end.yPos))
		{

	}















}






}
