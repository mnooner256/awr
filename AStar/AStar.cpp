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

//Put node_start on the OPEN list
open_list.push_back(start_node);

 //while the OPEN list is not empty
 while (OPEN.Count>0)
 {
  //Get the node off the open list
  //with the lowest f and call it node_current
  Node node_current = OPEN.pop ();

  //if node_current is the same state as node_goal we
  //have found the solution;
  //break from the while loop;
  if (node_current.isMatch (node_goal))
  {
    node_goal.parentNode = node_current.parentNode ;
    break;
  }

  //Generate each state node_successor that can come after node_current
  ArrayList successors = node_current.GetSuccessors ();

  //for each node_successor or node_current
  foreach (Node node_successor in successors)
  {
    //Set the cost of node_successor to be the cost of node_current plus
    //the cost to get to node_successor from node_current
    //--> already set while we were getting successors

    //find node_successor on the OPEN list
    int oFound = OPEN.IndexOf (node_successor);

    //if node_successor is on the OPEN list but the existing one is as good
    //or better then discard this successor and continue

    if (oFound>0)
    {
      Node existing_node = OPEN.NodeAt (oFound);
      if (existing_node.CompareTo (node_current) <= 0)
        continue;
    }


    //find node_successor on the CLOSED list
    int cFound = CLOSED.IndexOf (node_successor);

    //if node_successor is on the CLOSED list
    //but the existing one is as good
    //or better then discard this successor and continue;
    if (cFound>0)
    {
      Node existing_node = CLOSED.NodeAt (cFound);
      if (existing_node.CompareTo (node_current) <= 0 )
        continue;
    }

    //Remove occurences of node_successor from OPEN and CLOSED
    if (oFound!=-1)
       OPEN.RemoveAt (oFound);
    if (cFound!=-1)
       CLOSED.RemoveAt (cFound);

    //Set the parent of node_successor to node_current;
    //--> already set while we were getting successors

    //Set h to be the estimated distance to node_goal
    //(Using heuristic function)
    //--> already set while we were getting successors

    //Add node_successor to the OPEN list
    OPEN.push (node_successor);

   }
   //Add node_current to the CLOSED list
   CLOSED.push (node_current);
 }













