/*Author:       Anna Shafer
Description:  Receives and stores a 12 hexadecimal digit RFID tag number.
              Note:Uses virtual serial port for RFID input serial stream and uses USB for serial
              output to moniter. */

void RFID() {
  //char tagString[13] = {NULL};  
  String tagString = NULL;
  
  if(id20.available() ) {      
    for( int index = 0; index < 12; index++ ) {  //once serial input is detected, loop until all 12 digits are collected
      i = id20.read();                           // receive character from ID20
      if((int)i>=48 && (int)i<=70){                        //check whether it is a valid hex value
        //tagString[index] = i;
        tagString += i;
      }
      else index--;                              //if it is not a hex digit, then do not count towards the 12 digits in ID
    }
  }
    
  //for debugging purposes
  if(tagString != NULL) {
    Serial.println(tagString);           // send ID# to serial monitor
    Serial.println(tagString);           // send ID# to serial monitor
    Serial.println(tagString);           // send ID# to serial monitor
  }
  //delay(250);
  flag =0;
}
