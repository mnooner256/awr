/*
 * AStar.h
 *
 *  Created on: Jun 11, 2013
 *      Author: fjr_1983
 */

#ifndef ASTAR_H_
#define ASTAR_H_

extern int getPosition(string rfid, int& x, int& y, Node* map);

extern bool check(int x_cur, int y_cur, Node* map, string rfid);

extern void sendPath(string path, string rfid_path);

extern int getSize();

extern Node* getMap(int t_s);

extern string pathFind(Node*, int& xStart, int& yStart, int& xFinish, int& yFinish, int t_s);

extern string generatePath(int*  dir_map, int m, int n);

#endif
