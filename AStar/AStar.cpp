/* Functions for Pathing Algorithm
 *
 * Created by: Francisco Ruiz
 */

#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <stack>
#include "Node.h"
#include "Astar.h"

using namespace std;

//Possible directions  - 4 sides + 4 diagonals
const int DIR = 8;
//arrays store "East" direction as 0, then clockwise through NE as 7
int dx[DIR]={1, 1, 0, -1, -1, -1, 0, 1};
int dy[DIR]={0, 1, 1, 1, 0, -1, -1, -1};
int m;
int n;

//Function to read rfid tag and find current location
int getPosition(string rfid, int& x, int& y, Node* map)
{
  //Loop through the map and find the rfid tag
  for(int i=0; i<m*n; i++) {
	  //look for the rfid tag that matches the current position within the map
	  // need to account for format of R:rfid#
    if(map[i].getRfid().compare(rfid.substr(2,rfid.length()-2)) ==0) {
    	x= i%n;
    	y = i/m;
    	cout << "current position: "<< x << " " << y << endl;
      return i;
    }
  }

  //If not found, return invalid entry
  return -1;
}

string getRFID(int x, int y, Node* map)
{
	return map[y*n+x].getRfid();
}

bool check(int x_cur, int y_cur, Node* map, string rfid)
{
	return (getRFID(x_cur, y_cur, map).compare(rfid)==0);
}

//update the current x,y position with the next move along the path
void move( int& x, int& y, string path)
{
	//pull off the next move from the string path and convert to int
	x += dx[path[0] - '0'];
	y += dy[path[0] - '0'];
	cout << "new move: " << x << " " << y << endl;
}

//Function to get map from file sent from UI
Node* getMap(int& t_s)
{
	fstream f;
	 Node* hold_node;

	f.open("map_layout.txt", ios::in);
	if(!f.is_open()){
		cout << "could not open file.\n";
	}

	f >> m >> n;	//pull off the dimensions of the map from file

	t_s = m*n;
	hold_node = new Node[t_s];

	//loops through the map and reads values to node
	for(int i = 0; i < t_s; i++)
	{
		f >> hold_node[i].xPos >> hold_node[i].yPos >> hold_node[i].rfid;

		//Sets wall to high priority
		if(hold_node[i].rfid == "w") {
			hold_node[i].priority = 1000000;
		}
	}

	cout << "Map  compiled!" << endl;
	f.close();

	return hold_node;

}

struct Less : public binary_function <Node,Node, bool>
 {
	//Overwriting comparison for priority queue based on Node's priorities
	 bool operator ()(const Node* l, Node* r) const
	 {
		 return l->getPriority() > r->getPriority();
	 }
};

string pathFind(Node* map, int& xStart, int& yStart, int& xFinish, int& yFinish, int t_s)
{
    stack<Node*> possible_nodes; 							// possible alternative nodes
    int* visited_nodes = new int[t_s]; 						// map of closed (tried-out) nodes
	int* dir_map = new int[t_s];							//map of directions
	int* prior_map = new int[t_s];							//map of priorities
    Node* node;
    int x_pos, y_pos, xdx, ydy, temp;

    // initialize the visited node array to zero
    for(int j=0; j < t_s; j++){
    	visited_nodes[j]=0;
    	dir_map[j]=-1;
    	prior_map[j]=1000000;
    }

    // create the start node and push into list of open nodes
    node = new Node(xStart, yStart, 0, 0,-1);
    node->updatePriority(xFinish, yFinish);

    //set the starting points direction to 9 to mark it
    dir_map[yStart*n+xStart]=9;
    prior_map[yStart*n+xStart]=node->getPriority();
    possible_nodes.push(node);

    // A* search
    while(!possible_nodes.empty())
    {
		// get the current node w/ the highest priority
		// from the list of open nodes
		node = possible_nodes.top();
		possible_nodes.pop();

		if(node->getxPos() == xFinish && node->getyPos() == yFinish){
		    return generatePath(dir_map, xFinish, yFinish, xStart, yStart);
		}

		// nodes to go to next (available children), sorted be priority
		priority_queue<Node*, vector<Node*>, Less> node_options;

		//Node's x and y position
		x_pos = node->getxPos();
		y_pos = node->getyPos();

		visited_nodes[(y_pos * n) + x_pos] = 1;		// mark it on the closed nodes map

   //for debugging
	cout << "root: " << x_pos << " " << y_pos << endl;

        // generate moves (child nodes) in all possible directions
        for(int i=0; i < DIR; i++)
        {
            xdx = x_pos + dx[i];
            ydy = y_pos + dy[i];

            //Short-circuit at the edges of the map or if it is a wall
            if (xdx > n-1 || xdx < 0 || ydy < 0 || ydy > m-1 || map[(ydy*n)+xdx].getPriority() == 1000000)
            	continue;

            //Checks to see that node has not been seen before
            if( visited_nodes[(ydy*n)+xdx] != 1 )
            {
                // generate a child node, update its level, priority, and record the direction from the parent
                Node* child = new Node(xdx, ydy, node->getLevel(), node->getPriority(), i);
                child->nextLevel(i);
                child->updatePriority(xFinish, yFinish);

                //push each possible child onto the node's direction queue
                node_options.push(child);
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

					//update the direction map/array
					if(dir_map[(node_options.top()->yPos*n)+node_options.top()->xPos] == -1 ||
							prior_map[(node_options.top()->yPos*n)+node_options.top()->xPos] <= node_options.top()->getPriority()){
						dir_map[(node_options.top()->xPos*m)+node_options.top()->yPos] = node_options.top()->dir;
						prior_map[(node_options.top()->xPos*m)+node_options.top()->yPos] = node_options.top()->getPriority();

						possible_nodes.push(node_options.top());
					}
				}
			}
			//update the direction map/array
			if(dir_map[(top->yPos*n)+top->xPos] == -1 || prior_map[(top->yPos*n)+top->xPos] <= top->getPriority() ){
				dir_map[(top->yPos*n)+top->xPos] = top->dir;
				prior_map[(top->yPos*n)+top->xPos] = top->getPriority();

				possible_nodes.push(top);
			}
		}

	  //garbage collection
	  while(!node_options.empty()) {
		  Node* temp = new Node(node_options.top());
		  node_options.pop();
		  delete temp;
	  }

	  delete node; // garbage collection
    }

	//garbage collection
	while(!possible_nodes.empty()){
		Node* temp = new Node(possible_nodes.top());
		possible_nodes.pop();
		delete temp;
	}

	//start at the end point and follow the directions backwards
    return generatePath(dir_map, xFinish, yFinish, xStart, yStart);
}

string generatePath( int* dir_map, int xEnd, int yEnd, int xStart, int yStart)
{
	// generate the path from finish by following the direction array
	string path="";
	char buffer[200];
	int temp=0;
	stack<string> right_path;
	// Current x and y position in the direction map. Used to check
	// if the start node is the current node.
	int cur_x;
	int cur_y;
	int dir_hold;

	cur_x = xEnd;
	cur_y = yEnd;

	while(cur_x!=xStart || cur_y!=yStart){
		dir_hold = dir_map[(cur_y*n)+cur_x];

		cur_x += dx[dir_hold]*-1;
		cur_y += dy[dir_hold]*-1;

		right_path.push(std::to_string(dir_hold));
	}

	while(!right_path.empty()){
		path += right_path.top();
		right_path.pop();
	}

//for debugging
cout << "path generated: " << path << endl;
	return path;
}
