/*
 * test.cpp
 *
 *  Created on: May 30, 2013
 *      Author: Anna
 */

#include <iostream>
#include "serial.h"
#include <Windows.h>
#include <string>

const int dataLength = 256;

BOOL write(Serial* SP, OVERLAPPED osReader, char* msg)
{
	char send[dataLength] = "";
	BOOL writeResult = FALSE;

	if(strlen(msg) <= 1){
		std::cout <<"What would you like to send? ";
		scanf( send);
		writeResult = SP->WriteData(send,strlen(send),osReader);
	}
	else {
		writeResult = SP->WriteData(msg,strlen(msg),osReader);
	}
	if(!writeResult) {
		std::cout <<"Could not write to serial port.\n";
	}
	return writeResult;
}

char* read(Serial* SP, OVERLAPPED osReader, char* msg)
{
	int readResult = 0;
	char buf[dataLength] = "";

	readResult = SP->ReadData(buf,dataLength, osReader);

	if( strstr(buf,"Hello") != NULL){
		write(SP,osReader,"Hello");
		readResult = SP->ReadData(buf,dataLength, osReader);
		std::string str(buf);
		int index = str.find_first_of("0123456789ABCDEF");

		if(index <= str.length() ) {
			tag.append( str.substr(index,str.find_first_not_of("0123456789ABCDEF")) );

			if(tag.length()>= 12) {
				std::cout << tag << std::endl;
				tag.clear();

			}
		}
	}
	if(readResult >=0) return buf;
	return buf;
}

int main()
{
	BOOL fWaitingOnRead = TRUE;
	char buf[dataLength] = "";

    OVERLAPPED osReader = {0};
	std::string tag;

	Serial* SP = new Serial("COM4");

	if (SP->IsConnected())
		printf("We're connected\n");

    // Create the overlapped event. Must be closed before exiting
	// to avoid a handle leak.
	osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (osReader.hEvent == NULL) {
	   // Error creating overlapped event; abort.
		return 1;
	}

	while(SP->IsConnected())
	{
		char msg[dataLength] = "";

		if(fWaitingOnRead) {
			if(read(SP, osReader,msg)) {
				fWaitingOnRead = FALSE;
			}
		}
		if(!fWaitingOnRead) {
			if(!write(SP, osReader,""))
				fWaitingOnRead = TRUE;
		}
	}

	return 0;
}
