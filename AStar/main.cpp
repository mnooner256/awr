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
int read(Serial* SP, OVERLAPPED osReader, char* msg, int toRead)
{
	bool IDnum = true;

	SP->ReadLine(msg,dataLength, osReader, toRead);

	cout << "readline: " << msg << endl;
	cout << "msg length: " << strlen(msg) << endl;

	while(strlen(msg)!= toRead){
		if(strlen(msg) >= toRead){
			cout << "testing first 2" << endl;
			if(msg[0] == 'R' && msg[1] == ':' ) {
				cout << "first part right" << endl;
				for ( int i=2; i<=toRead; i++){
					if( msg[i] < 48 || msg[i] > 70 )
						IDnum = false;
				}
				if(IDnum == false) {
					for( int i=2; i<strlen(msg); i++){
						if( msg[i] < 48 || msg[i] > 70){
							for(int j=i; j<strlen(msg); j++){
								msg[j] = msg[j+1];
							}
							msg[strlen(msg)]='\0';
						}
					}
					cout << "cleaned: " << msg << endl;
				}
			}
			else {
				char* begin = strchr(msg, 'R');
				int j=0;
				for(int i=begin-msg; i<strlen(msg); i++){
					msg[j]=msg[i];
					j++;
				}
				msg[strlen(msg)-(begin-msg)]='\0';
				cout << "found new start " << msg  << endl;
			}

		}
		else if(strlen(SP->buf) >1){
			for( int i=0; i<strlen(SP->buf); i++ ){
				msg[strlen(msg)+1]='\0';
				msg[strlen(msg)]=SP->buf[i];
			}
		}
		else {
			char temp[25];
			cout << "gathering more" << endl;
			if( SP->ReadLine(temp,dataLength, osReader, toRead-strlen(msg))>0)
				cout << "successful read" << endl;
			cout << "temp length: " << strlen(temp) << endl;
			strcat(msg, temp);
		}
	}
	cout << "msg length: " << strlen(msg) << endl;
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
							if(getPosition( rfid, x_start, y_start, map)>=0) {
								x_cur = x_start; y_cur = y_start;
								path = pathFind(map, x_start, y_start, x_end, y_end, total_size);
							}
							else {
								cout << "Error: position not in map." << endl;
								SP->~Serial();
								delete SP;
							    return 0;
							}
						}
						else if(!check(x_cur, y_cur, map, rfid)) {
							cout << "unexpected position. Recalculating...." << endl;
							if( getPosition( rfid, x_cur, y_cur, map) >=0 ) {
								cout << "Current position: " << x_cur << " " << y_cur << endl;
								path = pathFind(map, x_cur, y_cur, x_end, y_end, total_size);
							}
							else {
								cout << "Error: position not in map. 2" << endl;
								//send stop and exit
								SP->~Serial();
								delete SP;
							    return 0;
							}
						}
						else
							path = path.substr(1,path.length()-1);

						fWaitingOnRead = FALSE;
					}
				}
			}
			else
				cout << "communication error 1" << endl;

			//Send instructions as a single char to the robot
			if(SP->IsConnected()) {
				cout << "writing..." << endl;
 				if(!fWaitingOnRead) {
					char array[2] = {path[0], '\0'};
					if(SP->WriteData(array,strlen(array),osReader)){
						cout << "direction: " << array << endl;
						move( x_cur, y_cur, path);
						fWaitingOnRead = TRUE;
					}
				}
			}
			else
				cout << "communication error 2" << endl;

		}

		// At the end, send a stop signal and make output
		if(SP->IsConnected()) {
			if(!fWaitingOnRead) {
				char array[2] = {16, '\0'};
				if(SP->WriteData(array,strlen(array),osReader))
					cout << "Escaped!" << endl;
			}
		}
		else
			cout << "end is reached" << endl;

	}
	else {
		cout<< "Communication error!" << endl;
		SP->~Serial();
	}

	cout << "are you seeing this" << endl;
	delete SP;
    return 0;
}

