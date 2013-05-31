/*
 * test.cpp
 *
 *  Created on: May 30, 2013
 *      Author: Anna
 */

#include <Windows.h>
#include <iostream>

int main()
{
	HANDLE serialArd;
	DWORD dwRead;
	BOOL fWaitingOnRead = FALSE;
	OVERLAPPED osReader = {0};
	const int MAX_BUF_SIZE = 256;
	char buf[MAX_BUF_SIZE];
	DCB dcb = {0};

	//opening an overlapping I/O port
	serialArd = CreateFile("COM4",GENERIC_READ | GENERIC_WRITE,
			0,0,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,0);

	if(serialArd == INVALID_HANDLE_VALUE) {
		std::cout << "Could not open new port.\n";
		return 1;
	}
	else {
	    dcb.DCBlength = sizeof(dcb);
	    if (!::GetCommState(serialArd, &dcb)) {
	    	std::cout << "Could not get communication state.\n";
	    	return 1;
	    }
	    dcb.BaudRate    = 9600;
	    dcb.ByteSize    = 8;
	    dcb.Parity      = NOPARITY;
	    dcb.StopBits    = ONESTOPBIT;

	    if (!::SetCommState (serialArd,&dcb)) {
	    	std::cout << "Could not set communication state.\n";
	    	return 1;
	    }


		// Create the overlapped event. Must be closed before exiting
		// to avoid a handle leak.
		osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

		if (osReader.hEvent == NULL) {
		   // Error creating overlapped event; abort.
			CloseHandle(serialArd);
			return 1;
		}

		while (!fWaitingOnRead) {
			// Issue read operation.
			if (!ReadFile(serialArd, buf, MAX_BUF_SIZE-1, &dwRead, &osReader)) {
				  if (GetLastError() != ERROR_IO_PENDING) {    // read not delayed?
					 // Error in communications; report it.
					  std::cout << "Could not read from port.\n";
					  CloseHandle(serialArd);
					  return 1;
				  }
				  else
					 fWaitingOnRead = TRUE;
			 }
			 else {
			  // read successful
//				 for(int i=0; i<MAX_BUF_SIZE; i++) {
//					 std::cout << buf[i];
//				 }

					 std::cout <<buf;

			}
		}
	}

	CloseHandle(serialArd);
	return 0;
}

