<<<<<<< HEAD
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
		printf("We're connected");

	while(SP->IsConnected())
	{
		readResult = SP->ReadData(buf,dataLength);
		printf("Bytes read: (-1 means no data available) %i\n",readResult);

		std::string test(buf);

		printf("%s",buf);
		test = "";

		Sleep(500);
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

=======
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
	BOOL writeResult = FALSE;

	writeResult = SP->WriteData(msg,strlen(msg),osReader);

	if(!writeResult)
		std::cout <<"Could not write to serial port.\n";

	return writeResult;
}

BOOL read(Serial* SP, OVERLAPPED osReader, char* msg, int toRead)
{
	int readResult = 0;
	BOOL finished = FALSE;

	//blocking function call - should read until all is read off of the serial buffer
	//and the expected array size (toRead) is reached
	readResult = SP->ReadLine(msg,dataLength, osReader, toRead);

	if(readResult >=0){
		finished = TRUE;
	}
	return finished;
}

BOOL initComm(Serial* SP, OVERLAPPED osReader)
{
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
	Serial* SP = new Serial("COM5");	//change as needed
	char msg[dataLength] = "";

	//Check whether the Serial Port connected successfully
	if (SP->IsConnected()) {
		std::cout <<"Com port connected\n";

		//Create event handle for oberlapping
		osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		if (osReader.hEvent == NULL) {
			return 1; 	// Error creating event; abort.
		}

		//perform handshake with the robot before trying to communicate
		if(initComm(SP, osReader)){

			while(SP->IsConnected())
			{
				//Check for and read in RFID tags
				if(fWaitingOnRead) {
					if(read(SP, osReader,msg, 14)) {
						std::cout << msg << std::endl;
						fWaitingOnRead = FALSE;
					}
				}
				//Send instructions as a single char to the robot
				if(!fWaitingOnRead) {
					std::cout <<"What is your command? ";
					std::cin >> msg;
					if(SP->WriteData(msg,strlen(msg),osReader))
						fWaitingOnRead = TRUE;
				}
			}
		}
	}
	return 0;
}
>>>>>>> remotes/origin/Michael
