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
	if(msg[0] != 'R' && msg[1] != ':') {
		char* begin = strchr(msg, 'R');
		int j=0;
		for(int i=begin-msg; i<strlen(msg); i++){
			msg[j]=msg[i];
			j++;
		}
		msg[strlen(msg)-(begin-msg)]='\0';
	}
	clean(msg);

	if(!RFIDcheck(msg, toRead)) {
		return false;
	}
	return true;
}

//sends a stop signal (anything higher that ASCII 68) to the robot
bool stop(Serial* SP, OVERLAPPED osReader)
{
	bool test = false;

	if(SP->IsConnected()) {
		char array[1] = {'s'};
		if(SP->WriteData(array,strlen(array),osReader))
			test = true;
	}
	return test;
}

bool getEnd(int& x_end, int& y_end)
{
	bool change = false;
	int x, y;
	fstream f;
	f.open("end_location.txt", ios::in);

	//Checks to see if file exists and can be read from
	if(!f.is_open())
		cout << "could not open file.\n";

	//Pull the dimensions of the map from file
	//file format is "y x"
	f >> y >> x;

	//check whether end point has been changed
	if(x != x_end || y != y_end){
		//do not send change for initial case
		if(x_end != -1 && y_end != -1){
			change = true;
		}
		x_end = x;
		y_end = y;
	}
	f.close();
	return change;
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
	BOOL end_change;
	Serial* SP = new Serial("COM5");	//change as needed
	int readResult = 0;
	int x_start,  y_start, x_end, y_end, total_size, x_cur, y_cur;
	string rfid = "";
	char msg[dataLength];

	map = getMap(total_size);
	x_start = y_start = x_end = y_end = x_cur = y_cur = -1;

//	x_end = 4; y_end = 5;
//	x_start = y_start = x_cur = y_cur = -1;

	//perform handshake with the robot before trying to communicate
	if(initComm(SP, osReader)){

		while(  rfid.empty() || !check( x_end , y_end, map, rfid) || x_end == -1 ){
			end_change = FALSE;
			//Check for and read RFID tags
			if(fWaitingOnRead) {
				memset(msg, '\0', strlen(msg));
				if(read(SP, osReader, msg, RFIDlength)) {

	//for debugging
	std::cout << "read: " << msg << std::endl;

					rfid = msg;
					//read from end file. If end point has changed, then update path
					if(getEnd(x_end, y_end)){
						//verify that end points are valid values with RFID tags
						if (!getRFID(x_end, y_end, map).empty()){
							path = pathFind(map, x_cur, y_cur, x_end, y_end, total_size);
						}
						else
							cout << "invalid end point" << endl;

						cout << "new end: " << x_end << " " << y_end << endl;
						end_change = TRUE;
					}
					if( x_start== -1 && y_start == -1 ){
						//read starting point and set initial path
						if(getPosition( rfid, x_start, y_start, map)>=0) {
							x_cur = x_start; y_cur = y_start;
							path = pathFind(map, x_start, y_start, x_end, y_end, total_size);
						}
						else
							 return abort(SP, osReader);
					}
					else if(!check(x_cur, y_cur, map, rfid)) {
						//if unexpected RFID tag is read, then recalculate path
						cout << "unexpected position. Recalculating...." << endl;

						if( getPosition( rfid, x_cur, y_cur, map) >=0 ) {
							path = pathFind(map, x_cur, y_cur, x_end, y_end, total_size);
						}
						else
							return abort(SP, osReader);
					}
					else {
						//update path by removing most recent move
						if(end_change==FALSE){
						  path = path.substr(1,path.length()-1);
						}
					}

					fWaitingOnRead = FALSE;
				}
				else
					cout << "read failed " << endl; //return abort(SP, osReader);
			}

			//Send instructions as a single char to the robot
			if(!fWaitingOnRead) {
				if( path[0] != NULL){
					char array[2] = {path[0], '\0'};
					if(SP->WriteData(array,strlen(array),osReader)) {
						cout << "direction: " << array << endl;
						move(x_cur, y_cur, path, map);
						fWaitingOnRead = TRUE;
					}
				}
				else
					stop( SP, osReader);
			}

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

