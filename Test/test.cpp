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

	const int dataLength = 256;
	int readResult = 0;
	char buf[dataLength] = "";

	// application reads from the specified serial port and reports the collected data
	printf("Welcome to the serial test app!\n\n");

	Serial* SP = new Serial("COM4");    // adjust as needed

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

		readResult = SP->ReadData(buf,dataLength, osReader);
		Sleep(50);			//Wait for communication to initialize
		//printf("Bytes read: (-1 means no data available) %i\n",readResult);
		std::cout <<buf;
	}
	return 0;
}

//DWORD dwRead;
//BOOL fWaitingOnRead = FALSE;
//OVERLAPPED osReader = {0};

//	// Create the overlapped event. Must be closed before exiting
//	// to avoid a handle leak.
//	osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
//
//	if (osReader.hEvent == NULL) {
//	   // Error creating overlapped event; abort.
//		CloseHandle(serialArd);
//		return 1;
//	}
//
//	while (!fWaitingOnRead) {
//		//for (char c : buf){ c = NULL; }
//
//		// Issue read operation.
//		if (!ReadFile(serialArd, buf, MAX_BUF_SIZE-1, &dwRead, &osReader)) {
//			  if (GetLastError() != ERROR_IO_PENDING) {    // read not delayed?
//				 // Error in communications; report it.
//				  std::cout << "Could not read from port.\n";
//				  CloseHandle(serialArd);
//				  return 1;
//			  }
//			  else
//				 fWaitingOnRead = TRUE;
//		 }
//		 else {
//		  // read successful
//			std::cout << buf;
//		}

