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
		std::cout <<"What is your command? ";
		scanf(send);
		writeResult = SP->WriteData(send,strlen(send),osReader);
	}
	else {
		writeResult = SP->WriteData(msg,strlen(msg),osReader);
	}
	if(!writeResult) {
		std::cout <<"Could not write to serial port.\n";
	}
	else {
		writeResult = TRUE;
	}
	return writeResult;
}

BOOL read(Serial* SP, OVERLAPPED osReader, char* msg, int toRead)
{
	int readResult = 0;
	BOOL finished = FALSE;

	//blocking function call - should read until all is read off of the serial buffer
	readResult = SP->ReadLine(msg,dataLength, osReader, toRead);

	if(readResult >=0){
		finished = TRUE;
	}
	return finished;
}

BOOL initComm(Serial* SP, OVERLAPPED osReader) {
	char temp[100] = "";
	if(read(SP,osReader,temp, 1)){
		if(temp[0]=='H'){
			if(write(SP, osReader,temp)){
				std::cout<< "Connected to the Arduino.\n";
				return TRUE;
			}
			else std::cout << "Could not send ACK.\n";
		}
		else std::cout << "Did not receive proper msg: " << temp <<std::endl;
	}
	return FALSE;
}

int main()
{
	BOOL fWaitingOnRead = TRUE;
    OVERLAPPED osReader = {0};
	Serial* SP = new Serial("COM4");
	char msg[dataLength] = "";

	if (SP->IsConnected())
		std::cout <<"Com port connected\n";

	osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (osReader.hEvent == NULL) {
		return 1; 	// Error creating event; abort.
	}

	if(initComm(SP, osReader)){

		while(SP->IsConnected())
		{
			if(fWaitingOnRead) {
				if(read(SP, osReader,msg, 14)) {
					for(int i=0; i<strlen(msg); i++)
						std::cout << msg[i];
					std::cout << std::endl;

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
