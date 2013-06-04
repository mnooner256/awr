/*
 * serial.cpp
 *
 *  Created on: Jun 4, 2013
 *      Author: Anna
 */

#include "serial.h"
#include <Windows.h>
#include <iostream>

Serial::Serial(char *portName)
{
    //We're not yet connected
    this->connected = false;
    this->errors = NULL;

    //Try to connect to the given port through CreateFile
    this->serialArd = CreateFile(portName,
            GENERIC_READ | GENERIC_WRITE,
            0, NULL, OPEN_EXISTING,
            FILE_FLAG_OVERLAPPED, 0);

    //Check if the connection was successful
    if(this->serialArd==INVALID_HANDLE_VALUE) {
        //If not success full display an Error
        if(GetLastError()==ERROR_FILE_NOT_FOUND){
            //Print Error if neccessary
            std::cout << "ERROR: Handle was not attached.\n";
        }
        else {
            std::cout << "ERROR!!!\n";
        }
    }
    else {
        //If connected we try to set the comm parameters
        DCB dcbSerialParams = {0};

        //Try to get the current
        if (!GetCommState(this->serialArd, &dcbSerialParams)) {
            //If impossible, show an error
        	std::cout << "Could not get current communication status.\n";
	    }
        else {
            //Define serial connection parameters for the arduino board
            dcbSerialParams.BaudRate  = 9600;
            dcbSerialParams.ByteSize  = 8;
            dcbSerialParams.StopBits  = ONESTOPBIT;
            dcbSerialParams.Parity	  = NOPARITY;

             //Set the parameters and check for their proper application
             if(!SetCommState(serialArd, &dcbSerialParams)) {
            	 if (!::SetCommState (serialArd,&dcbSerialParams)) {
					std::cout << "Could not set communication status variables.\n";
				 }
             }
             else
             {
                 //If everything went fine we're connected
                 this->connected = true;
                 //We wait 2s as the arduino board will be reseting
                 //Sleep(ARDUINO_WAIT_TIME);
             }
        }
    }

}

Serial::~Serial()
{
    //Check if we are connected before trying to disconnect
    if(this->connected) {
        //We're no longer connected
        this->connected = false;

        CloseHandle(serialArd);
    }
}

int Serial::ReadData(char *buffer, unsigned int nbChar, OVERLAPPED osReader)
{
    //Number of bytes we'll have read
    DWORD bytesRead;
    //Number of bytes we'll really ask to read
    unsigned int toRead;

    //Use the ClearCommError function to get status info on the Serial port
    ClearCommError(this->serialArd, &this->errors, &this->status);

    //Check if there is something to read
    if(this->status.cbInQue>0)
    {
        //If there is we check if there is enough data to read the required number
        //of characters, if not we'll read only the available characters to prevent
        //locking of the application.
        if(this->status.cbInQue>nbChar) {
            toRead = nbChar;
        }
        else {
            toRead = this->status.cbInQue;
        }

        //Try to read the require number of chars, and return the number of read bytes on success
        if(ReadFile(this->serialArd, buffer, toRead, &bytesRead, &osReader) && bytesRead != 0) {
			return bytesRead;
        }

    }

    //If nothing has been read, or that an error was detected return -1
    return -1;
}


bool Serial::WriteData(char *buffer, unsigned int nbChar, OVERLAPPED osReader)
{
    DWORD bytesSend;

    //Try to write the contents of the buffer onto the Serial port
    if(!WriteFile(this->serialArd, (void *)buffer, nbChar, &bytesSend, 0)) {
        //In case it don't work get comm error and return false
        ClearCommError(this->serialArd, &this->errors, &this->status);
        return false;
    }
    else
        return true;
}

bool Serial::IsConnected()
{
    return this->connected;
}

