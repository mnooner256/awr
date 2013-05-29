//Author:       Anna Shafer
//Description:  Receives and stores a 12 hexadecimal digit RFID tag number.
//              Note:Uses virtual serial port for RFID input serial stream and uses USB for serial
//              output to moniter.

#include <Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial id20(3,2); // virtual serial port(RX,T
char i;

void setup()
{
  Serial.begin(9600);
  id20.begin(9600);
}

void loop(){
  char tagString[13] = {NULL};  
  
  if(id20.available() ) {      
    for( int index = 0; index < 12; index++ ) {  //once serial input is detected, loop until all 12 digits are collected
      i = id20.read();                           // receive character from ID20
      int a=i;                                   // retrieve character's ascii value
      if(a>=48 && a<=70){                        //check whether it is a valid hex value
        tagString[index] = i;
      }
      else index--;                              //if it is not a hex digit, then do not count towards the 12 digits in ID
    }
  }
    
  //for debugging purposes
  if(tagString != NULL) {
    Serial.println(tagString);                  // send ID# to serial monitor
  }
  
}

int main(void)
{
  init();
  setup();

  while(true) {
    loop();
  }
}
