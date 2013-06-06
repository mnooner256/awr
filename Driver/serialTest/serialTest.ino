
#include <SoftwareSerial.h>

SoftwareSerial id20(3,4); // virtual serial port(RX,TX)
char i;
unsigned long isrTime =0;
unsigned long last_isrTime =0;

void RFID_ISR(){
  isrTime = millis();
  //check to see if increment() was called in the last 250 milliseconds
  int test = isrTime - last_isrTime;
  Serial.println(test);
  
  if ( test > .005)
  {
    char tagString[13] = {NULL};  
    
    Serial.println("about to read");  
    
    if(id20.available() ) {    
      for( int index = 0; index < 12; index++ ) {  //once serial input is detected, loop until all 12 digits are collected
        i = id20.read();                           // receive character from ID20
        if((int)i>=48 && (int)i<=70){                        //check whether it is a valid hex value
          tagString[index] = i;
        }
        else index--;                              //if it is not a hex digit, then do not count towards the 12 digits in ID
      }
    }
      
    //for debugging purposes
    if(tagString != NULL) {
      Serial.println(tagString);           // send ID# to serial monitor
    }
  }
  Serial.println("false alarm");  
      
    delay(250);
    Serial.println("resetting...");  
      
    last_isrTime = isrTime;
}
void setup()
{
   attachInterrupt(0, RFID_ISR, HIGH);
     
   Serial.begin(9600);
   id20.begin(9600); 
}
void loop()
{
  //for debugging purposes
  Serial.println("Test");

}
