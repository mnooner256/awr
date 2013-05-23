#include <Arduino.h>
#include <SoftwareSerial\SoftwareSerial.h>

SoftwareSerial id20(3,2); // virtual serial port
char i;

void setup()
{
	 Serial.begin(9600);
	 id20.begin(9600);
}

void loop(){

	if(id20.available()) {
	 i = id20.read(); // receive character from ID20
	 Serial.print(i); // send character to serial monitor
	 Serial.print(" ");
	}

//  char tagString[13];
//  int index = 0;
//  boolean reading = false;
//
//  while(Serial.available()){
//
//	Serial.println("Tag present");
//    int readByte = Serial.read(); //read next available byte
//
//    if(readByte == 2) reading = true; //beginning of tag
//    if(readByte == 3) reading = false; //end of tag
//
//    if(reading && readByte != 2 && readByte != 10 && readByte != 13){
//      //store the tag
//      tagString[index] = readByte;
//      index ++;
//    }
//    Serial.println(tagString); //read out any unknown tag
//  }

  //Serial.println("No tag present");
}

int main(void)
{
  init();
  setup();

  while(true) {
    loop();
  }
}
