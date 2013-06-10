#include <SoftwareSerial.h>

SoftwareSerial id20(3,4); // virtual serial port(RX,TX)
int pin = 13;
volatile int state = LOW;
char tag[100];
char dir;
char* hand_var = "H\n";
int hand_flag = 0;

void setup()
{
  Serial.begin(9600);
  id20.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  
  Serial.flush();
  Serial.print(hand_var);
  char confirm[2];
  Serial.readBytes(confirm,1);
  if(confirm[0] == 'H') {
    digitalWrite(13, HIGH);
    hand_flag = 1;
  }
}

void loop()
{
  if(hand_flag ==  1){
    scan_rfid();
    read_from_controller();
  }
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
    
    Serial.print("R:");
    for ( int i = 1; i < bytes; i++) {
      Serial.print(tag[i]);
    }
    Serial.print('\n');
    Serial.flush();
    
    //Read off the closing two bytes constant of the RFID
    id20.readBytes(tag,sizeof(tag));
  }
}
