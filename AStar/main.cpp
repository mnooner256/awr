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

using namespace std;
//Random Numbers - tired of trying to fstream - will finish later
const int n = 25;
const int m = 25;
int map[n][m];
int dir_map[n][m]; 			// map of directions
const int dir = 8; 					// number of possible directions

static int dx[dir]={1, 1, 0, -1, -1, -1, 0, 1};
static int dy[dir]={0, 1, 1, 1, 0, -1, -1, -1};

#include "Node.h"

// Determine priority (in the priority queue)
bool operator<(const Node &a, const Node &b)
{
  return a.getPriority() > b.getPriority();
}

// A-star algorithm.
// The route returned is a string of direction digits.
string pathFind( const int &  xStart, const int & yStart,
                 const int & xFinish, const int & yFinish )
{
    priority_queue<Node*> open_queue; // list of open (not-yet-tried) nodes
    priority_queue<Node*> closed_queue; // list of tried nodes
    int closed_nodes_map[n][m]; 	// map of closed (tried-out) nodes
    int open_nodes_map[n][m]; 	// map of open (not-yet-tried) nodes
    Node* node;
    static Node* m0;
    static int i, y, j, x, xdx, ydy;
    static char c;
    char buffer[200];

    // intiliaze the node maps to zero
    for(int y=0; y < m; y++)
    {
        for(int x=0; x < n; x++)
        {
            closed_nodes_map[x][y]=0;
            open_nodes_map[x][y]=0;
        }
    }

    // create the start node and push into list of open nodes
    node = new Node(xStart, yStart, 0, 0);
    node->updatePriority(xFinish, yFinish);
    open_queue.push(node);
    open_nodes_map[xStart][yStart] = node->getPriority(); // mark it on the open nodes map

    // A* search
    while(!open_queue.empty())
    {
        // get the current node w/ the highest priority
        // from the list of open nodes
        node = open_queue.top();
        open_queue.pop();

        //Node's x and y position
        x = node->getxPos();
        y = node->getyPos();

        // mark it on the closed nodes map
        closed_nodes_map[x][y]=1;

        // quit searching when the goal state is reached
        if(x == xFinish && y == yFinish)
        {
            // generate the path from finish to start
            // by following the directions
            string path="";
            while(!(x == xStart && y == yStart))
            {
                j=dir_map[x][y];
                sprintf(buffer, "%i,%s", buffer, ((j+dir/2)%dir));
                x+=dx[j];
                y+=dy[j];
            }
            path=buffer;
            // garbage collection
            delete node;
            // empty the leftover nodes
            while(!open_queue.empty()) open_queue.pop();
            return path;
        }

        // generate moves (child nodes) in all possible directions
        for(i=0;i<dir;i++)
        {
            xdx = x+dx[i];
            ydy = y+dy[i];

            if(!(xdx<0 || xdx>n-1 || ydy<0 || ydy>m-1 || map[xdx][ydy]==1
                || closed_nodes_map[xdx][ydy]==1))
            {
                // generate a child node
                m0=new Node( xdx, ydy, node->getLevel(),
                             node->getPriority());
                m0->nextLevel(i);
                m0->updatePriority(xFinish, yFinish);

                // if it is not in the open list then add into that
                if(open_nodes_map[xdx][ydy]==0)
                {
                    open_nodes_map[xdx][ydy]=m0->getPriority();
                    open_queue.push(m0);
                    // mark its parent node direction
                    dir_map[xdx][ydy]=(i+dir/2)%dir;
                }
                else if(open_nodes_map[xdx][ydy]>m0->getPriority())
                {
                    // update the priority info
                    open_nodes_map[xdx][ydy]=m0->getPriority();
                    // update the parent direction info
                    dir_map[xdx][ydy]=(i+dir/2)%dir;

                    // replace the node
                    // by emptying one pq to the other one
                    // except the node to be replaced will be ignored
                    // and the new node will be pushed in instead
                    while(!(open_queue.top()->getxPos()==xdx &&
                           open_queue.top()->getyPos()==ydy))
                    {
                        closed_queue.push(open_queue.top());
                        open_queue.pop();
                    }
                    open_queue.pop(); // remove the wanted node
                }
                else
                {
                	delete m0; // garbage collection
                }
            }
        }
        delete node; // garbage collection
    }
    return ""; // no route found
}

int main()
{
    // create empty map
    for(int y=0;y<m;y++)
    {
        for(int x=0;x<n;x++) map[x][y]=0;
    }

    // fill out the map matrix with a '+' pattern
    for(int x=n/8;x<n*7/8;x++)
    {
        map[x][m/2]=1;
    }
    for(int y=m/8;y<m*7/8;y++)
    {
        map[n/2][y]=1;
    }

    int xA, yA, xB, yB;
    fstream f;
    f.open("C:/Users/Francisco/Desktop/End.txt", ios::in);

    //random start and end (for now)
    xA = 1;
    yA = 1;
    xB = 4;
    yB = 3;
    //finds path
    string route = pathFind(xA, yA, xB, yB);

    //checks for empty route
    if(route == "")
    {
    	cout << "An empty route generated!" << endl;
    }

    cout << route;

    return(0);
}
