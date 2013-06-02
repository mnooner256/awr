/***************************************************************
Node Class for Map

Created by: Francisco Ruiz

Needs to "hard-code" map data - only things that change should be start and end
***************************************************************/

#ifndef NODE_H
#define NODE_H

#include <fstream>

using namespace std;

class Node
{
private:
	int xPos, yPos;		//holds x- and y- coordinates
	double distance;	//distance to target
	int priority;		//smaller means better - super high is a wall or elevator
	long int rfid;		//holds RFID data
	Node* parent;		//holds parent node
public:
	//default constructor
	Node()
	{
		xPos = -1;
		yPos = -1;
		distance = -1;
		priority = -1;
		rfid = -1;
		parent = NULL;
	}
	//constructor from UI data
	Node(int x_p, int y_p, int d, int p, int r)
	{
		xPos = x_p;
		yPos = y_p;
		distance = d;
		priority = p;
		rfid = r;
		parent = NULL;
	}
	//method to compare nodes
	int compare(Node a, Node b)
	{
		int diff;

		diff = a.distance - b.distance;

		if(diff >= 0)
			return 1;
		else
			return 2;
	}
	//method to see if nodes are in same position
	bool match(Node a, Node b)
	{
		if(a.xPos == b.xPos || a.yPos == b.yPos)
			return true;
		else
			return false;
	}
}

class compareNode
{
public:
	bool operator()(Node& a, Node& b)
	{
		if((a.priority < b.priority) && (a.distance < b.distance))
			return true;
		return false;
	}
};

#endif
