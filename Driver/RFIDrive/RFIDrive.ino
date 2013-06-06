#include <SoftwareSerial.h>

SoftwareSerial id20(3,4); // virtual serial port(RX,TX)
int pin = 13;
volatile int state = LOW;
char tag[100];
char dir;
char confirm;
void setup()
{
  Serial.begin(9600);
  id20.begin(9600);
  
  Serial.println("H");
  confirm = Serial.read();
  if(confirm == "H") {
    digitalWrite(13, HIGH);
  }
}

void loop()
{
  scan_rfid();
  read_from_controller();
}

void read_from_controller(){
  if(Serial.available() >0){
    dir = Serial.read();
    
    if(dir != NULL) {
      Serial.println(dir);
      drive_motors();
    }
    dir = NULL;
  }
}

void drive_motors() {
}

void scan_rfid() {
  int bytes = 0;
  if (id20.available() > 0) {
    bytes = id20.readBytesUntil(13, tag, sizeof(tag));
    
    Serial.print("RFID:");
    for ( int i = 1; i < bytes; i++) {
      Serial.print(tag[i]);
    }
    Serial.print('\n');
    Serial.flush();
    
    //Read off the closing two bytes constant of the RFID
    id20.readBytes(tag,sizeof(tag));
  }
}
