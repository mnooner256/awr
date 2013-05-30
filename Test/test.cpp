/*
 * test.cpp
 *
 *  Created on: May 30, 2013
 *      Author: Anna
 */

#include <Windows.h>
#include <iostream>

int main() {
	HANDLE ard;
	//opening an overlapping I/O port
	ard = CreateFile("COM4",GENERIC_READ | GENERIC_WRITE,
			0,0,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,0);

	if(ard == INVALID_HANDLE_VALUE) {
		std::cout << "Could not open new port.";
		return 1;
	}
	else std::cout<< "Success!";

	DWORD dwRead;
	BOOL fWaitingOnRead = FALSE;
	OVERLAPPED osReader = {0};
	const int MAX_BUF_SIZE = 256;
	char buf[MAX_BUF_SIZE];

	// Create the overlapped event. Must be closed before exiting
	// to avoid a handle leak.
	osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (osReader.hEvent == NULL) {
	   // Error creating overlapped event; abort.
		return 1;
	}


	if (!fWaitingOnRead) {
		// Issue read operation.
		if (!ReadFile(ard, buf, MAX_BUF_SIZE, &dwRead, &osReader)) {
			  if (GetLastError() != ERROR_IO_PENDING) {    // read not delayed?
				 // Error in communications; report it.
				  std::cout << "Could not read from port.";
				  return 1;
			  }
			  else
				 fWaitingOnRead = TRUE;
		 }
		 else {
		  // read completed immediately
			 std::cout << "Successfully read from port.";
			 for(int i=0; i<MAX_BUF_SIZE; i++) {
				 std::cout << buf[i];
			 }
		}
	}
	return 0;
}

