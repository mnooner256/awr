/* Functions for Pathing Algorithm
 *
 * Created by: Francisco Ruiz
 */

#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include "Node.h"
#include "Astar.h"

using namespace std;

//Directions
const int DIR = 8;
//Both arrays start in "East" direction
int dx[DIR]={1, 1, 0, -1, -1, -1, 0, 1};
int dy[DIR]={0, 1, 1, 1, 0, -1, -1, -1};

int getSize()
{
	fstream f;
	int x_size, y_size, tot_size;

	f.open("c:/Users/fjr_1983/Desktop/map_layout.txt", ios::in);

	//reads sizes from first line in map file
	f >> x_size >> y_size;

	tot_size = x_size * y_size;

	f.close();

	return tot_size;
}

//Function to get map from file sent from UI
Node* getMap(int t_s)
{
	fstream f;
	Node* m;
	char temp;

	f.open("c:/Users/fjr_1983/Desktop/map_layout.txt", ios::in);

	m = new Node[t_s];

	//Need to get rid of size on top of file

	f >> temp;
	f >> temp;

	//loops through the map and reads values to node
	for(int i = 0; i < t_s; i++)
	{
		f >> m[i].xPos >> m[i].yPos >> m[i].rfid;

		//Sets wall to high priority
		if(m[i].rfid == "0")
		{
			m[i].priority = 1000000;
		}

		cout << m[i].xPos << m[i].yPos << endl << m[i].rfid << endl;
	}

	cout << "Map  compiled!" << endl;

	f.close();

	return m;
}

string pathFind(int& xStart, int& yStart, int& xFinish, int& yFinish, int t_s)
{
	priority_queue<Node*> open_queue; 	// list of open (not-yet-tried) nodes
    priority_queue<Node*> closed_queue; 		// vector of tried nodes
    fstream f;
    Node* node;
    Node* child;
    int x_pos, y_pos, xdx, ydy, temp, m , n;
    int* closed_nodes_map = new int[t_s]; 	// map of closed (tried-out) nodes
    int* open_nodes_map = new int[t_s]; 	// map of open (not-yet-tried) nodes
	int* dir_map = new int[t_s];			//map of directions
    char buffer[200];

	f.open("c:/Users/fjr_1983/Desktop/map_layout.txt", ios::in);

	f >> m >> n;

    // initialize the node maps to zero
    for(int j=0; j < t_s; j++)
    {
    	closed_nodes_map[j]=0;
        open_nodes_map[j]=0;
    }

    // create the start node and push into list of open nodes
    node = new Node(xStart, yStart, 0, 0);
    node->updatePriority(xFinish, yFinish);
    open_queue.push(node);
    open_nodes_map[(xStart * m) + yStart] = node->getPriority(); // mark it on the open nodes map

    // A* search
    while(!open_queue.empty())
    {
        // get the current node w/ the highest priority
        // from the list of open nodes
        node = open_queue.top();
        open_queue.pop();

        //Node's x and y position
        x_pos = node->getxPos();
        y_pos = node->getyPos();

        // mark it on the closed nodes map
        closed_nodes_map[(x_pos * m) + y_pos + 1] = 1;

        // quit searching when the goal state is reached
        if(x_pos == xFinish && y_pos == yFinish)
        {
            // generate the path from finish to start
            // by following the directions
            string path="";

            while(!(x_pos == xStart && y_pos == yStart))
            {
                temp = dir_map[(x_pos * m) + y_pos + 1];
                sprintf_s(buffer, "%i,%s", buffer, temp % DIR);
                x_pos += dx[temp];
                y_pos += dy[temp];				
            }

            path = buffer;

            // garbage collection

            while(!open_queue.empty())
            {
            	open_queue.pop();
            }

            return path;
        }

        // generate moves (child nodes) in all possible directions
        for(int i=0; i < DIR; i++)
        {
            xdx = x_pos + dx[i];
            ydy = y_pos + dy[i];

            //Short-circuit at the edges of the map
            if (xdx > n-1 || xdx < 0 || ydy < 0 || ydy > m-1)
            	continue;

            //Checks to see that node has not been run before
            else if(open_nodes_map[(xdx * m) + ydy + 1] != 0 && closed_nodes_map[(xdx * m) + ydy + 1] != 1)
            {
                // generate a child node
                child = new Node(xdx, ydy, node->getLevel(), node->getPriority());				
                child->nextLevel(i);
                child->updatePriority(xFinish, yFinish);

                // if it is not in the open list then add into that
                if(open_nodes_map[(xdx * m) + ydy + 1] == 0)
                {
                    open_nodes_map[(xdx * m) + ydy + 1] = child->getPriority();
                    open_queue.push(child);

                    // mark its parent node direction
                    dir_map[(xdx * 8) + ydy] = i;
                }

                else if(open_nodes_map[(xdx * m) + ydy + 1] > child->getPriority())
                {
                    // update the priority info					
                    open_nodes_map[(xdx * m) + ydy + 1] = child->getPriority();

					// update the parent direction info
                    dir_map[(xdx * m) + ydy + 1] = i;

                    // replace the node
                    // by emptying one list to the other one
                    // except the node to be replaced will be ignored
                    // and the new node will be pushed in instead
                    while(open_queue.top()->getxPos() != xdx && open_queue.top()->getyPos() != ydy)
                    {
						//Vector pointer error here during runtime
                        closed_queue.push(open_queue.top());
                        open_queue.pop();
                    }

                    open_queue.pop(); // remove the wanted node
                }
                else
                {
                	delete child; // garbage collection
                }
            }
        }
        delete node; // garbage collection
    }

	f.close();

    return ""; // no route found	
}
