/*
 * test.cpp
 *
 *  Created on: May 30, 2013
 *      Author: Anna
 */

#include <iostream>
#include "serial.h"
#include <Windows.h>

int main()
{
	BOOL fWaitingOnRead = TRUE;
	BOOL writeResult = FALSE;
	const int dataLength = 256;
	int readResult = 0;
	char buf[dataLength] = "";

	Serial* SP = new Serial("COM4");

	if (SP->IsConnected())
		printf("We're connected\n");

    OVERLAPPED osReader = {0};
    // Create the overlapped event. Must be closed before exiting
	// to avoid a handle leak.
	osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (osReader.hEvent == NULL) {
	   // Error creating overlapped event; abort.
		return 1;
	}

	while(SP->IsConnected())
	{
		char buf[dataLength] = "";

		if(fWaitingOnRead) {
			readResult = SP->ReadData(buf,dataLength, osReader);
			Sleep(50);			//Wait for communication to initialize
			//printf("Bytes read: (-1 means no data available) %i\n",readResult);
			std::cout <<buf;

			if(readResult >= 12 ) fWaitingOnRead = FALSE;
		}
		if(!fWaitingOnRead) {
			std::cout <<"What would you like to send? ";
			std::cin >> buf[0];
			writeResult = SP->WriteData(buf,1,osReader);
			if(!writeResult) {
				std::cout <<"Could not write to serial port.\n";
			}
			else fWaitingOnRead = TRUE;
		}
	}

	return 0;
}
