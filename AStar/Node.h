/***************************************************************
Node Class for Map

Created by: Francisco Ruiz, Anna Shafer

Needs to "hard-code" map data - only things that change should be start and end
***************************************************************/
#ifndef NODE_H
#define NODE_H

using namespace std;

class Node
{
public:

    //current position
    int xPos;
    int yPos;
    //total distance already traveled to reach the node
    int level;
    //priority = level + remaining distance estimate (smaller is higher)
    int priority;
    //rfid code
    string rfid;
    //direction of the parent node
    int dir;

    //Default constructor
    Node()
	{
    	level = 0;
    	xPos = yPos = priority = -1;
		rfid = "";
		dir = -1;
	}

	//Constructor which initializaes all values but the rfid tag
    Node(int xp, int yp, int d, int p, int pardir)
    {
    	xPos = xp;
    	yPos = yp;
    	level = d;
    	priority = p;
    	rfid = "";
		dir = pardir;
    }

	//Copy constructor
    Node(Node* n)
    {
    	xPos = n->xPos;
    	yPos = n->yPos;
    	level = n->level;
    	priority = n->priority;
    	rfid = n->rfid;
		dir=n->dir;
    }

	//Getters for various elements
    int getxPos() const
    {
    	return xPos;
    }

    int getyPos() const
    {
    	return yPos;
    }

    int getLevel() const
    {
    	return level;
    }

    int getPriority() const
    {
    	return priority;
    }

    string getRfid() const
    {
    	return rfid;
    }

	//Updates priority dependent on destination
    void updatePriority(const int & xDest, const int & yDest)
    {
        priority = level + estimate(xDest, yDest);
    }

	//Updates level based on direction (passed as i)
    void nextLevel(int& i) 
    {
    	//If direction is cardinal, level is 10.
		//Otherwise, level is 20 (longer for diagonal). --> Should the level be 14? (ten times square root of 2)
       // level += (i % 2 == 0?10 : 20);
    	level += 10;
    }

    // Estimation function for the remaining distance to the goal.
    int estimate(const int & xDest, const int & yDest) const
    {
        int xd, yd, d;

        xd = xDest - xPos;
        yd = yDest - yPos;

        //Distance Formula
        d = static_cast<int>(sqrt(xd * xd + yd * yd)*10);

        return d;
    }
};

#endif
