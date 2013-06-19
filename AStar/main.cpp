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
#include "Node.h"
#include "Astar.h"
#include "serial.h"
#include <Windows.h>


using namespace std;

Node* map;
string path;
const int dataLength = 64;

BOOL initComm(Serial* SP, OVERLAPPED osReader)
{
	//Check whether the Serial Port connected successfully
	if (SP->IsConnected()) {
		std::cout <<"Com port connected\n";

		//Create event handle for oberlapping
		osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		if (osReader.hEvent == NULL) {
			return FALSE; 	// Error creating event; abort.
		}

		char temp[100] = "";
		if(SP->ReadLine(temp,dataLength, osReader, 1)){
			if(temp[0]=='H') {
				if(SP->WriteData(temp,strlen(temp),osReader)){
					std::cout<< "Connected to the Arduino.\n";
					return TRUE;
				}
				else std::cout << "Could not send ACK.\n";
			}
			else std::cout << "Did not receive proper msg: " << temp <<std::endl;
		}
	}
	return FALSE;
}

int main()
{
	BOOL fWaitingOnRead = TRUE;
	OVERLAPPED osReader = {0};
	Serial* SP = new Serial("COM5");	//change as needed
	int readResult = 0;
	int x_start,  y_start, x_end, y_end, total_size, x_cur, y_cur;
	string rfid;

	map = getMap(total_size);

	x_end = 6; y_end = 6; x_start = y_start = 0;
	x_cur = 0; y_cur = 0;

	//perform handshake with the robot before trying to communicate
	if(initComm(SP, osReader)){

		while( x_cur != x_end || y_cur != y_end ){
			//Check for and read in RFID tags
			if(SP->IsConnected()) {

				if(fWaitingOnRead) {
					char msg[dataLength] = "";
					if(SP->ReadLine(msg,dataLength, osReader, 14)>0) {

		//for debugging
		std::cout << "read: " << msg << std::endl;

						rfid = msg;
						if( path.empty() ){
							getPosition( rfid, x_start, y_start, map);
							x_cur = x_start; y_cur = y_start;
							path = pathFind(map, x_start, y_start, x_end, y_end, total_size);
						}
						else if(!check(x_cur, y_cur, map, rfid)) {
							cout << "unexpected position. Recalculating...." << endl;
							getPosition( rfid, x_cur, y_cur, map);
							path = pathFind(map, x_start, y_start, x_end, y_end, total_size);
						}
						else
							path = path.substr(1,path.length()-1);

						fWaitingOnRead = FALSE;
					}
				}
			}

			//Send instructions as a single char to the robot
			if(SP->IsConnected()) {
				if(!fWaitingOnRead) {
					char array[2] = {path[0], '\0'};
					if(SP->WriteData(array,strlen(array),osReader)){
						cout << "direction: " << array << endl;
						move( x_cur, y_cur, path);
						fWaitingOnRead = TRUE;
					}
				}
			}
		}

		if(SP->IsConnected()) {
			if(!fWaitingOnRead) {
				char array[2] = {16, '\0'};
				if(SP->WriteData(array,strlen(array),osReader))
					cout << "Escaped!" << endl;
			}
			else
				cout << "end is reached" << endl;
		}

	}
	else
		cout<< "Communication error!" << endl;

	delete SP;
    return 0;
}

