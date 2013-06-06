/*
 * test.cpp
 *
 *  Created on: May 30, 2013
 *      Author: Anna
 */

#include <iostream>
#include "serial.h"
#include <Windows.h>

const int dataLength = 256;

BOOL write(Serial* SP, OVERLAPPED osReader, char* msg)
{
	char send[dataLength] = "";
	BOOL writeResult = FALSE;

	if(strlen(msg) < 1){
		std::cout <<"What would you like to send? ";
		scanf(send);
		writeResult = SP->WriteData(send,strlen(send),osReader);
	}
	else {
		writeResult = SP->WriteData(msg,strlen(msg),osReader);
	}
	if(!writeResult) {
		std::cout <<"Could not write to serial port.\n";
		return writeResult;
	}else {
		return writeResult = TRUE;
	}
}

BOOL read(Serial* SP, OVERLAPPED osReader, char* msg)
{
	int readResult = 0;

	readResult = SP->ReadData(msg,dataLength, osReader);
	char* ptr = strstr(msg,"RFID:");
	if( ptr != NULL){

	}
	if(readResult >=0) return TRUE;
	return FALSE;
}

BOOL initComm(Serial* SP, OVERLAPPED osReader, char* msg) {
	if(read(SP,osReader,msg)){
		if(strcmp(msg,"H\n")==0){
			if(write(SP, osReader,"H\n")){
				std::cout<< "Connected to the Arduino." << std::endl;
				return TRUE;
			}
		}
		else std::cout << "did not receive proper msg" << std::endl;
	}
	return FALSE;
}

int main()
{
	BOOL fWaitingOnRead = TRUE;

    OVERLAPPED osReader = {0};

	Serial* SP = new Serial("COM4");

	if (SP->IsConnected())
		printf("We're connected\n");

	osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (osReader.hEvent == NULL) {
	   // Error creating event; abort.
		return 1;
	}

	char msg[dataLength] = "";
	while(!initComm(SP, osReader, msg)){

		while(SP->IsConnected())
		{
			if(fWaitingOnRead) {
				if(read(SP, osReader,msg)) {
					std::cout << msg;
					fWaitingOnRead = FALSE;
				}
			}
			if(!fWaitingOnRead) {
				if(write(SP, osReader, ""))
					fWaitingOnRead = TRUE;
			}

		}
	}

	return 0;
}
