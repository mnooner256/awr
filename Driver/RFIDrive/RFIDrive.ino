#include <SoftwareSerial.h>

SoftwareSerial id20(3,4); // virtual serial port(RX,TX) for RFID reader
char tag[100];
int dir = 6;
char* hand_var = "H~"; //Serial comm handshake variable
int hand_flag = 0; //flag for successful handshake/setup

void setup()
{
  //set pins 5-8 to output for motors
  for(int i=5; i<=8; i++)
     pinMode(i, OUTPUT);
     
  //initialize the serial communication baudrates
  Serial.begin(9600);
  id20.begin(9600);
  //initialize the LED for confirmation
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  
  //perform handshake with PC via serial and then light LED
  Serial.flush();
  Serial.print(hand_var);
  char confirm[2];
  Serial.readBytes(confirm,1);
  if(confirm[0] == 'H') {
    digitalWrite(13, HIGH);
    hand_flag = 1; //if handshake was successful, set flag
  }
}

void loop()
{
  //begin program only if serial communication setup was successful
  if(hand_flag ==  1){
    scan_rfid();
    read_from_controller();
  }
}

void read_from_controller(){
  if(Serial.available() >0){
    //read in the new direction command and convert to int
    long temp = Serial.read();
    int var = temp-'0';
    
      dir = var - dir;
      
      if(abs(dir) >4){
        //new direction relative to previous direction
        if(dir<0){
          drive_motors(dir+8);
        }
        else
          drive_motors(dir-8);
      }
      else
        drive_motors(dir);

      
    //reset direction to the command that was received  
    dir = var;
    Serial.flush();
  }
}

void scan_rfid() {
  int bytes = 0;
  if (id20.available() > 0) {
    
    tag[0] ='R';
    tag[1] =':';
    bytes = id20.readBytesUntil(12, tag+2, sizeof(tag));
    tag[15]='~';
   // for ( int i = 0; i < bytes+2; i++) {
      Serial.print(tag);
    //}
   // Serial.print('\n');
    Serial.flush();
    
    //Read off the closing two bytes constant of the RFID
    id20.readBytes(tag,sizeof(tag));
  }
}
