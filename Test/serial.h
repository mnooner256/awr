/*
 * serial.h
 *
 *  Created on: Jun 4, 2013
 *      Author: Anna
 */

#ifndef SERIAL_H_
#define SERIAL_H_

#include <Windows.h>

class Serial
{
    private:
        HANDLE serialArd;	//Serial comm handler
        bool connected;		//Connection status
        COMSTAT status;		//Get various information about the connection
        DWORD errors;		//Keep track of last error

    public:
        //Initialize Serial communication with the given COM port
        Serial(char *portName);
        //Close the connection
        ~Serial();
        //Read data in a buffer, if nbChar is greater than the
        //maximum number of bytes available, it will return only the
        //bytes available. The function return -1 when nothing could
        //be read, the number of bytes actually read.
        int ReadData(char *buffer, unsigned int nbChar, OVERLAPPED osReader);
        //Writes data from a buffer through the Serial connection
        //return true on success.
        bool WriteData(char *buffer, unsigned int nbChar, OVERLAPPED osReader);
        //Check if we are actually connected
        bool IsConnected();


};

#endif /* SERIAL_H_ */
