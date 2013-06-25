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
		std::cout <<"Com port connected." << endl;

		//Create event handle for oberlapping
		osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		if (osReader.hEvent == NULL) {
			return FALSE; 	// Error creating event; abort.
		}

		char temp[100] = "";
		if(SP->ReadLine(temp,dataLength, osReader, 1)){
			if(temp[0]=='H') {
				if(SP->WriteData(temp,strlen(temp),osReader)){
					std::cout<< "Connected to the Arduino." << endl;
					return TRUE;
				}
				else std::cout << "Could not send ACK." << endl;
			}
			else std::cout << "Did not receive proper msg: " << temp <<std::endl;
		}
	}
	return FALSE;
}
int read(Serial* SP, OVERLAPPED osReader, char* msg, int toRead){

	int length = SP->ReadLine(msg,dataLength, osReader, toRead);
	//msg[length+1]='\0';

	cout << "readline: " << msg << endl;
	if(length > toRead){
		if(msg[0] != 'R' || msg[1] != ':') {
			length = SP->ReadLine(msg,dataLength, osReader, toRead);
		}
		else {
			for( int i=2; i<strlen(msg); i++){
				if( (int)msg[i] < 30 ||(int)msg[i] > 39){
					for(int j=i; j<strlen(msg)-2; j++){
						msg[j] = msg[j+1];
						cout << "cleaning: " << msg << endl;
					}
					msg[14]='\0';
					cout << "cleaned: " << msg << endl;

				}
			}
		}
	}

	cout << "length: " << strlen(msg) << endl;
	return strlen(msg);
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

	x_end = 5; y_end = 5; x_start = y_start = 0;
	x_cur = -1; y_cur = -1;

	//perform handshake with the robot before trying to communicate
	if(initComm(SP, osReader)){

		while( x_cur != x_end || y_cur != y_end ){

			//Check for and read in RFID tags
			if(SP->IsConnected()) {

				if(fWaitingOnRead) {

					char msg[dataLength] = "";
					if(read(SP, osReader, msg, 14) == 14) {


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
				    		cout << "Current position: " << x_cur << " " << y_cur << endl;
							path = pathFind(map, x_cur, y_cur, x_end, y_end, total_size);
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

		// At the end, send a stop signal and make output
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

