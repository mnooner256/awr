/*
 * test.cpp
 *
 *  Created on: May 30, 2013
 *      Author: Anna
 */

#include <Windows.h>
#include <iostream>

using namespace std;

int main() {
	HANDLE ard;
	ard = CreateFile("COM4",GENERIC_READ | GENERIC_WRITE,
			0,0,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,0);

	if(ard == INVALID_HANDLE_VALUE) {
		cout << "Could not open new port.";
		return 1;
	}
	else cout<< "Success!";


	return 0;
}
