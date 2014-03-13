#include <SoftwareSerial.h>

SoftwareSerial id20(3,4); // virtual serial port(RX,TX) for RFID reader
char tag[100];
int dir = 6;          //assuming starting facing map north
char* hand_var = "H~"; //Serial comm handshake variable
int hand_flag = 0; //flag for successful handshake/setup
//calibrated to account for right motor faster than left
int leftspeed = 100;
int rightspeed = 90;//138;

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
    Serial.flush();
    
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
    int newdir = temp-'0';
    
    //find the new direction relative to previous direction
    dir = newdir - dir;
      
    if(abs(dir) >4){
        if(dir<0){
          drive_motors(dir+8);
        }
        else
          drive_motors(dir-8);
     }
     else
        drive_motors(dir);

    //reset direction to the command that was received  
    dir = newdir;
    Serial.flush();
  }
}

void scan_rfid() {
  int bytes = 0;
  //check whether the RFID reader has read anything into the serial buffer
  if (id20.available() > 0) {
    
    //read in the tag and store it in the format R:123456789ABC~
    tag[0] ='R';
    tag[1] =':';
    bytes = id20.readBytesUntil(12, tag+2, sizeof(tag));
    tag[15]='~';
    //then send it bach through the serial line
    Serial.print(tag);
    Serial.flush();
    
    //Read off the closing two bytes constant of the RFID to throw away
    id20.readBytes(tag,sizeof(tag));
  }
}
