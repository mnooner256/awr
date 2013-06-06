/* Functions for Pathing Algorithm
 *
 * Created by: Francisco Ruiz
 */

#include <fstream>
#include <string>
#include "Node.h"

using namespace std;

int getSize()
{
	fstream f;
	int x_size, y_size, tot_size;

	f.open("c:/Users/Francisco/Desktop/map_layout.txt", ios::in);

	//reads sizes from first line in file
	f >> x_size >>y_size;

	tot_size = x_size * y_size;

	return tot_size;
}

//Function to get map from file sent from UI
Node* getMap(int t_s)
{
	fstream f;
	int x_size, y_size;
	Node* map;
	string dummy;

	f.open("c:/Users/Francisco/Desktop/map_layout.txt", ios::in);

	map = new Node[t_s];

	//loops through the map and reads values to node
	for(int i = 0; i < t_s; i++)
	{
		f >> map[i].xPos >> map[i].yPos >> map[i].rfid >> dummy;
		//will check for garbage and collect if necessary
		while(dummy != '\n')
		{
			f >> dummy;
		}
	}

	return map;
}

//overloads < to compare node priorities
bool operator<(const Node &a, const Node &b)
{
  return a.getPriority() > b.getPriority();
}



string pathFind(int& xStart, int& yStart, int& xFinish, int& yFinish )
{
    priority_queue<Node*> open_queue; // list of open (not-yet-tried) nodes
    priority_queue<Node*> closed_queue; // list of tried nodes

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
                sprintf(buffer, "%i,%s", buffer, ((j+DIR/2)%DIR));
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
        for(i=0;i<DIR;i++)
        {
            xdx = x+dx[i];
            ydy = y+dy[i];

            if(!(xdx<0 || xdx>n-1 || ydy<0 || ydy>m-1 || map[xdx][ydy]==1
                || closed_nodes_map[xdx][ydy]==1))
            {
                // generate a child node
                m0=new Node( xdx, ydy, node->getLevel(),
                             node->getPriority());
                m0->nextLevel(i, DIR);
                m0->updatePriority(xFinish, yFinish);

                // if it is not in the open list then add into that
                if(open_nodes_map[xdx][ydy]==0)
                {
                    open_nodes_map[xdx][ydy]=m0->getPriority();
                    open_queue.push(m0);
                    // mark its parent node direction
                    dir_map[xdx][ydy]=(i+DIR/2)%DIR;
                }
                else if(open_nodes_map[xdx][ydy]>m0->getPriority())
                {
                    // update the priority info
                    open_nodes_map[xdx][ydy]=m0->getPriority();
                    // update the parent direction info
                    dir_map[xdx][ydy]=(i+DIR/2)%DIR;

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











