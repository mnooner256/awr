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

//Possible directions  - 4 sides + 4 diagonals
const int DIR = 8;
//arrays store "East" direction as 0, then clockwise through NE as 7
int dx[DIR]={1, 1, 0, -1, -1, -1, 0, 1};
int dy[DIR]={0, 1, 1, 1, 0, -1, -1, -1};

//Function to read rfid tag and find current location
int getCurrent(char rfid[], Node* map, int total_size)
{
  string tag;

  tag = rfid;

  //Loop through the map and find the rfid tag
  for(int i=0; i<total_size; i++)
  {
    if(map[i].getRfid() == tag)
    {
      return i;
    }
  }

  //If not found, return invalid entry
  return -1;
}

void sendPath(string path, string rfid_path)
{
  string rfid;
  int size = path.size();
  char* send_path;

  send_path = new char[size];

  //transfers string to character array
  for(int i=0; i < size; i++)
  {
    send_path[i] = path[i];
  }

  //infinite loop (until broken)
  while(true)
  {
    for(int j=0; j<size; j++)
    {
      //send path[i] to robot

      //receive rfid from robot

      if(rfid_path == rfid)
      {
        continue;
      }
      //breaks out of loop for restart if rfid value is unexpected
      else
        break;
    }
  }
}

int getSize()
{
	fstream f;
	int x_size, y_size, tot_size;

	f.open("map_layout.txt", ios::in);
	if(!f.is_open()){
		cout << "could not open file.\n";
	}

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

	f.open("map_layout.txt", ios::in);
	if(!f.is_open()){
		cout << "could not open file.\n";
	}

	m = new Node[t_s];

	//Need to get rid of size of map on top of file
	f >> temp;
	f >> temp;

	//loops through the map and reads values to node
	for(int i = 0; i < t_s; i++)
	{
		f >> m[i].xPos >> m[i].yPos >> m[i].rfid;

		//Sets wall to high priority
		if(m[i].rfid == "w")
		{
			m[i].priority = 1000000;
		}
		//cout << m[i].xPos << m[i].yPos << endl << m[i].rfid << endl;
	}

	cout << "Map  compiled!" << endl;

	f.close();

	return m;

}

struct Less : public binary_function <Node,Node, bool>
 {
	//Overwriting comparison for priority queue based on Node's priorities
	 bool operator ()(const Node* l, Node* r) const
	 {
		 return l->getPriority() < r->getPriority();
	 }
};

string pathFind(Node* map, int& xStart, int& yStart, int& xFinish, int& yFinish, int t_s)
{
    queue<Node*> possible_nodes; 							// possible alternative nodes
    int* visited_nodes = new int[t_s]; 						// map of closed (tried-out) nodes
	int* dir_map = new int[t_s];							//map of directions

    fstream f;
    Node* node;
    int x_pos, y_pos, xdx, ydy, temp, m , n;

    f.open("map_layout.txt", ios::in);
    if(!f.is_open())
    	cout << "could not open file.\n";

	f >> m >> n;	//pull off the dimensions of the map from file
cout << m;
    // initialize the visited node array to zero
    for(int j=0; j < t_s; j++){
    	visited_nodes[j]=0;
    	dir_map[j]=-1;
    }

    // create the start node and push into list of open nodes
    node = new Node(xStart, yStart, 0, 0,-1);
    node->updatePriority(xFinish, yFinish);

    //set the starting points direction to 9 to mark it
    dir_map[xStart*m+yStart]=9;
    possible_nodes.push(node);

    // A* search
    while(!possible_nodes.empty())
    {
    	cout << "queue size: " << possible_nodes.size()  << endl;

		// get the current node w/ the highest priority
		// from the list of open nodes
		node = possible_nodes.front();
		possible_nodes.pop();

		priority_queue<Node*, vector<Node*>, Less> node_options;  // nodes to go to next, sorted be priority

		//Node's x and y position
		x_pos = node->getxPos();
		y_pos = node->getyPos();

		visited_nodes[(x_pos * m) + y_pos] = 1;		// mark it on the closed nodes map

   //for debugging
        cout << "root: " << x_pos << " " << y_pos << endl;

        // generate moves (child nodes) in all possible directions
        for(int i=0; i < DIR; i++)
        {
        	cout <<  "checking direction " << i << endl;
            xdx = x_pos + dx[i];
            ydy = y_pos + dy[i];

            //Short-circuit at the edges of the map or if it is a wall
            if (xdx > n-1 || xdx < 0 || ydy < 0 || ydy > m-1 || map[(ydy*n)+xdx].getPriority() == 1000000)
            	continue;

            //Checks to see that node has not been seen before
            if( visited_nodes[(xdx * m) + ydy] != 1 )
            {
                // generate a child node, update its level, priority, and record the direction from the parent
                Node* child = new Node(xdx, ydy, node->getLevel(), node->getPriority(), i);
                child->nextLevel(i);
                child->updatePriority(xFinish, yFinish);

                //push each possible child onto the node's direction queue
                node_options.push(child);

       //for debugging purposes
                cout << "position: " << xdx << " " << ydy
                	 << " - priority, level, direction: " << child->priority
                	 << " ; " << child->level << " ; " << child->dir << endl;
            }
        }

		// after having considered each of the possible directions, pull the top two children off the queue
		// and check for a tie, otherwise push the child with the lowest priority (top) onto the possible nodes queue
		if(!node_options.empty())
		{
			Node* top = new Node(node_options.top());

			if(node_options.size() >= 2){
				node_options.pop();

				if(top->getPriority() == node_options.top()->getPriority()){
					possible_nodes.push(top);
					possible_nodes.push(node_options.top());

					//update the direction map/array
					dir_map[(node_options.top()->xPos*m)+node_options.top()->yPos] = node_options.top()->dir;
				}
			}
			//update the direction map/array
			dir_map[(top->xPos*m)+top->yPos] = top->dir;

			possible_nodes.push(top);
		}

	  //garbage collection
	  while(!node_options.empty()) {
		  Node* temp = new Node(node_options.top());
		  node_options.pop();
		  delete temp;
	  }

	  delete node; // garbage collection
    }

    cout << "visited: " << endl;
    for (int i=0; i<m; i++) {
    	for(int j=0; j<n; j++)
    		cout << visited_nodes[(i*m)+j] << " ";
    	cout << endl;
    }

    cout << "directions: " << endl;
    for (int i=0; i<m; i++) {
    	for(int j=0; j<n; j++)
    		cout << dir_map[(i*m)+j] << " ";
    	cout << endl;
    }
	f.close();

	//garbage collection
	while(!possible_nodes.empty()){
		Node* temp = new Node(possible_nodes.front());
		possible_nodes.pop();
		delete temp;
	}

	//
    return generatePath(dir_map, m, n);
}

string generatePath( int* dir_map, int m, int n)
{
	// generate the path from finish by following the direction array
	string path="";
	char buffer[200];
	int temp=0;

	//Frank: can you look at this? Wasn't quite sure what you were aiming for for output here.
	//I got an output, but it's not very good...
	for (int i=0; i<m; i++) {
	    for(int j=0; j<n; j++){
	    	temp = dir_map[(i * m) + j];
	    	//if(temp < 8 && temp > 0)
	    		sprintf_s(buffer+((j * n) + i), 200-((j * n) + i), "%i,%s", temp);
	    }
	}
	path = buffer;

//for debugging
	cout << "path generated:" << path << endl;
	return path;
}
