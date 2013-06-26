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
const int dataLength = 64;	//Serial data buffer length
const int RFIDlength = 14;  //RFID tag string length

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

void clean ( char* msg )
{
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

bool RFIDcheck ( char* msg, int toRead )
{
	bool IDnum = true;
	if(strlen(msg) != toRead){
		IDnum = false;
	}
	if(msg[0] == 'R' && msg[1] == ':' ) {
		for ( int i=2; i<strlen(msg); i++){
			if( msg[i] < 48 || msg[i] > 70 )
				IDnum = false;
		}
	}
	return IDnum;
}

// Reads in RFID tag strings (R:123456789ABC~) and cleans out any
// noise accumulated in the serial line.
bool read(Serial* SP, OVERLAPPED osReader, char* msg, int toRead)
{
	SP->ReadLine(msg,dataLength, osReader, toRead);

	cout << "readline: " << msg << endl;
	cout << "msg length: " << strlen(msg) << endl;

	if(msg[0] != 'R' && msg[1] != ':') {
		char* begin = strchr(msg, 'R');
		int j=0;
		for(int i=begin-msg; i<strlen(msg); i++){
			msg[j]=msg[i];
			j++;
		}
		msg[strlen(msg)-(begin-msg)]='\0';
		cout << "found new start " << msg  << endl;
	}
	clean(msg);

	if(!RFIDcheck(msg, toRead)) {
		return false;
	}

	cout << "msg length: " << strlen(msg) << endl;
	return true;
}

bool stop(Serial* SP, OVERLAPPED osReader)
{
	if(SP->IsConnected()) {
		char array[2] = {'s', '\0'};
		if(SP->WriteData(array,strlen(array),osReader)>0)
			return true;
	}
	return false;
}

//send stop and exit
int abort(Serial* SP, OVERLAPPED osReader)
{
	cout << "Aborting" << endl;
	stop(SP, osReader);
	SP->~Serial();
	delete SP;
	return 0;
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

	x_end = 4; y_end = 5;
	x_start = y_start = 0;
	x_cur = y_cur = -1;

	//perform handshake with the robot before trying to communicate
	if(initComm(SP, osReader)){

		while( x_cur != x_end || y_cur != y_end ){

			//Check for and read RFID tags
			if(SP->IsConnected()) {
				if(fWaitingOnRead) {
					char msg[dataLength] = "";
					if(read(SP, osReader, msg, RFIDlength)) {

		//for debugging
		std::cout << "read: " << msg << std::endl;

						rfid = msg;
						if( path.empty() ){
							if(getPosition( rfid, x_start, y_start, map)>=0) {
								x_cur = x_start; y_cur = y_start;
								path = pathFind(map, x_start, y_start, x_end, y_end, total_size);
							}
							else
								 return abort(SP, osReader);
						}
						else if(!check(x_cur, y_cur, map, rfid)) {
							cout << "unexpected position. Recalculating...." << endl;
							if( getPosition( rfid, x_cur, y_cur, map) >=0 ) {
								cout << "Current position: " << x_cur << " " << y_cur << endl;
								path = pathFind(map, x_cur, y_cur, x_end, y_end, total_size);
							}
							else
							    return abort(SP, osReader);
						}
						else {
							//update path by removing most recent move
							path = path.substr(1,path.length()-1);
						}

						fWaitingOnRead = FALSE;
					}
					else
						return abort(SP, osReader);
				}
			}
			else
			    return abort(SP, osReader);

			//Send instructions as a single char to the robot
			if(SP->IsConnected()) {
				if(!fWaitingOnRead) {
					char array[2] = {path[0], '\0'};
					if(SP->WriteData(array,strlen(array),osReader)) {
						cout << "direction: " << array << endl;
						move(x_cur, y_cur, path);
						fWaitingOnRead = TRUE;
					}
				}
			}
			else
			    return abort(SP, osReader);

		}

		// At the end, send a stop signal and make output
		if(!stop( SP, osReader))
			cout << "Error: could not send stop" << endl;
		else
			cout << "end is reached" << endl;

	}
	else
		cout<< "Communication error!" << endl;

    return abort(SP, osReader);
}

