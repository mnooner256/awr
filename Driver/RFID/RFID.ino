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
  char tagString[13] = {0};  //Initialize ID tag string to zero
  
  if(id20.available() ) {
    for( int index = 0; index < 12; index++) {
    i = id20.read(); // receive character from ID20
    tagString[index] = i;
    }
  }
    
  if(tagString != 0) {
    Serial.println(tagString); // send ID# to serial monitor
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
