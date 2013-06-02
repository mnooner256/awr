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
    // current position
    int xPos;
    int yPos;
    // total distance already travelled to reach the node
    int level;
    // priority = level + remaining distance estimate (smaller is higher)
    int priority;
public:
    Node(int xp, int yp, int d, int p)
    {
    	xPos=xp;
    	yPos=yp;
    	level=d;
    	priority=p;
    }

    int getxPos()
    {
    	return xPos;
    }

    int getyPos()
    {
    	return yPos;
    }

    int getLevel()
    {
    	return level;
    }

    int getPriority()
    {
    	return priority;
    }

    void updatePriority(const int & xDest, const int & yDest)
    {
        priority = level + estimate(xDest, yDest) * 10;
    }

    // give better priority to going straight instead of diagonally
    void nextLevel(const int & i) // i: direction
    {
        level+=(dir==8?(i%2==0?10:14):10);
    }

    // Estimation function for the remaining distance to the goal.
    const int & estimate(const int & xDest, const int & yDest) const
    {
        static int xd, yd, d;

        xd = xDest-xPos;
        yd = yDest-yPos;

        //Distance Formula
        d = static_cast<int>(sqrt(xd * xd + yd * yd));

        return d;
    }
};

#endif
