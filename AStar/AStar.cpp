/****************************************************************
A* Pathfinding Algorithm for Automated Wheelchair

Created by: Francisco Ruiz

*****************************************************************/
#include <vector>
#include "Node.h"

using namespace std;

int size = 25;		//placeholder for actual value -- will compute later
vector<int> solution_path;
vector<Node> open_list;
vector<Node> closed_list;
Node goal_node;
Node start_node;


void AStar()
{
	//Put node_start on the OPEN list
	open_list.push_back(start_node);

	//while the OPEN list is not empty
	 while (open_list.count > 0)
	 {
	  //Get the node off the open list
	  //with the lowest f and call it node_current
	  Node node_current = open_list.pop();

	  //if node_current is the same state as node_goal we
	  //have found the solution;
	  //break from the while loop;
	  if (node_current.match(node_current, goal_node))
	  {












