/*Author:    Anna Shafer
Description: Controls and directs the Rover with received instructions (a single char).
             f=forward, r=turn right 90 deg, t=turn right 45? deg(for diagonal),
             l=turn left 90 deg, k=turn left 45 deg, b=reverse/backwards*/
             
/*Current notes:
 - Check out encoders for turning?
 - Check out onboard compass for correction (and turning?)
 -- Integrate with RFID for navigation output
*/

#include <SoftwareSerial.h>

SoftwareSerial id20(3,4); // virtual serial port(RX,TX)
char i;
volatile int flag =LOW;              // flag variable for RFID interrupt

int E1 = 6; //M1 Speed Control
int E2 = 5; //M2 Speed Control
int M1 = 8; //M1 Direction Control
int M2 = 7; //M2 Direction Control


void setup()
{
//   for(int i=5; i<=8; i++)
//     pinMode(i, OUTPUT);
   
   
   attachInterrupt(0, RFID_ISR, CHANGE);
     
   Serial.begin(9600);
   id20.begin(9600);
   
   Serial.println("Setup Done!");
}
void loop()
{
  //for debugging purposes
  Serial.println(flag);
  
  if(flag == HIGH) {
    test();
  }
  
}

void RFID_ISR(){
  digitalWrite(13, HIGH);
  flag =HIGH;
}

void test() {
  Serial.println("Test");
  digitalWrite(2, LOW);
}

void RFID() {
  char tagString[18] = {'R','F','I','D',':'};
  Serial.write("Hello World");
  Serial.flush();
  delay(100);
  if(id20.available() ) {      
    for( int index = 4; index < 12; index++ ) {  //once serial input is detected, loop until all 12 digits are collected
      i = id20.read();                           // receive character from ID20
      if((int)i>=48 && (int)i<=70){                        //check whether it is a valid hex value
        tagString[index] = i;
      }
      else index--;                              //if it is not a hex digit, then do not count towards the 12 digits in ID
    }
  }
  tagString[17] = '\0';
  //if(tagString != NULL) {
    Serial.println(tagString);           // send ID# to serial monitor
 // }
  
  //Reset pin so we can read a new card
  digitalWrite(2, LOW);
  
  delay(100);
  
  flag = 0;
}
void drive()
{
  int leftspeed = 255; //255 is maximum speed 
   int rightspeed = 255;
//   switch(val) // Perform an action depending on the command
//   {
//     case 'f'://Move Forward
//       forward (leftspeed,rightspeed);
//     break;
//     case 'l'://Turn Left
//       left (leftspeed,rightspeed);
//     break;
//     case 'r'://Turn Right
//       right (leftspeed,rightspeed);
//     break;
//     case 'k'://Move Diagonally Left
//       dleft (leftspeed,rightspeed);
//     break;
//     case 't'://Move Diagonally Right
//       dright (leftspeed,rightspeed);
//     break;
//     case 'b'://Move Backwards
//       reverse (leftspeed,rightspeed);
//     break;
//     default:
//     stop();
//     break;
//   } 
}

void stop(void) //Stop
{
 digitalWrite(E1,LOW);
 digitalWrite(E2,LOW);
}
void forward(char a,char b)
{
 analogWrite (E1,a);
 digitalWrite(M1,LOW);
 analogWrite (E2,b);
 digitalWrite(M2,LOW);
}
void reverse (char a,char b)
{
 analogWrite (E1,a);
 digitalWrite(M1,HIGH);
 analogWrite (E2,b);
 digitalWrite(M2,HIGH);
}
void left (char a,char b)
{
 analogWrite (E1,a);
 digitalWrite(M1,HIGH);
 analogWrite (E2,b);
 digitalWrite(M2,LOW);
}
void right (char a,char b)
{
 analogWrite (E1,a);
 digitalWrite(M1,LOW);
 analogWrite (E2,b);
 digitalWrite(M2,HIGH);
}
void dleft (char a,char b)
{
 analogWrite (E1,a);
 digitalWrite(M1,HIGH);
 analogWrite (E2,b);
 digitalWrite(M2,LOW);
}
void dright (char a,char b)
{
 analogWrite (E1,a);
 digitalWrite(M1,LOW);
 analogWrite (E2,b);
 digitalWrite(M2,HIGH);
}
